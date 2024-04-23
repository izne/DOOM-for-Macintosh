#ifndef __MPW_VERSION__
	#if defined(powerc) || defined (__powerc)
		/* Pre-compiled Mac header */
		#include <DGMacHeadersPPC>
	#else
		#include <DGMacHeaders68K>
	#endif
#else
	#include <Dialogs.h>
	#include <Files.h>
	#include <Memory.h>
	#include <SegLoad.h>
	#include <StandardFile.h>
	#include <Strings.h>
#endif

#include "LionDoom.h"
#include <Lion.h>
#include <ctype.h>

#include "doomdef.h"
#include "DoomResources.h"

#include <stdlib.h>

#include "W_WAD.PROTO.H"

#define MAXWADFILES     8
extern FSSpec			gWadFiles[MAXWADFILES];
extern int				gNumWads;

//===============
//   TYPES
//===============


typedef struct
{
	char		identification[4];		// should be IWAD
	int			numlumps;
	int			infotableofs;
} wadinfo_t;


typedef struct
{
	int			filepos;
	int			size;
	char		name[8];
} filelump_t;


//=============
// GLOBALS
//=============

lumpinfo_t		*lumpinfo = NULL;		// location of each lump on disk
int				numlumps = 0;

void			**lumpcache = NULL;

extern short	gAppVRefNum;
extern long		gAppDirId;

extern DialogPtr	gStatusDialog;

extern void W_CloseWadFiles (void);
extern void CloseStatusDialog (void);
extern void StatusDialog (tLong total, tLong current);
extern void DrawStatusDialog (Boolean forUpdate);
extern void CopyPStr (void *src, void *dst);
extern void RestoreScreenDepth (void);
extern void BlackScreen(void);

//===================

/*
 *
 *	Functions.
 *
 */

#undef __NO_USE_ASM

#ifdef __BIG_ENDIAN__
	#if defined(powerc) || defined (__powerc) || defined __NO_USE_ASM
	
	unsigned long SwapLONG (unsigned long x)
	{
		return (x << 24) | ((x << 8) & 0xFF0000L) | ((x >> 8) & 0xFF00L) | (x >> 24);
	}
	
	short SwapSHORT (unsigned short x)
	{
		return ((x << 8) | (x >> 8));
	}
	
	#endif	// defined(powerc) || defined (__powerc) || defined __NO_USE_ASM
#endif 	// __BIG_ENDIAN__

void strupr (char *s)
{
	short		count = 0;
	
	while (*s)
	{
		*s = toupper(*s);
		s++;
		if (++count >= 8)
			break;
	}
}

void ExtractFileBase (char *path, char *dest)
{
	char	*src;
	int		length;

	src = path + strlen(path) - 1;

//
// back up until a \ or the start
//
	while (src != path && *(src-1) != '\\' && *(src-1) != '/')
		src--;

//
// copy up to eight characters
//
	memset (dest,0,8);
	length = 0;
	while (*src && *src != '.')
	{
		if (++length == 9)
			I_Error ("Filename base of %s >8 chars",path);
		*dest++ = toupper((int)*src++);
	}
}

/*
============================================================================

						LUMP BASED ROUTINES

============================================================================
*/

/*
====================
=
= W_AddFile
=
= All files are optional, but at least one file must be found
= Files with a .wad extension are wadlink files with multiple lumps
= Other files are single lumps with the base filename for the lump name
=
= If filename starts with a tilde, the file is handled specialy to
= allow map reloads
====================
*/

// the reload feature is a fragile hack...
int		reloadlump = 0;
char	*reloadname = NULL;

void W_CloseWadFiles (void)
{
	int i;
	
	for (i = 0; i < gNumWads; i++)
	{
		FSClose (lumpinfo->handle);
	}
}

void W_AddFile (int fileIndex)
{
	wadinfo_t		header;
	lumpinfo_t		*lump_p;
	unsigned		i;
	int				length;
	int				startlump;
	filelump_t		*fileinfo, singleinfo;
	int				storehandle;
	
	Ptr			*tempHndl;
	Str255			tempName;
	FSSpec			fileSpec;
	short			fileRefNum, err;
	long			count, fileEOF;
	Boolean			alreadyLooked;
	StandardFileReply	reply;
	
//
// open the file and add to directory
//	

/* ·· what is reloading all about?
	if (filename[0] == '~')
	{
		filename++;
		reloadname = filename;
		reloadlump = numlumps;
	}
*/
	alreadyLooked = FALSE;
	
	fileSpec = gWadFiles[fileIndex];
StartOver :
	CopyPStr(fileSpec.name, tempName);
	p2cstr(tempName);
	
	err = FSpOpenDF(&fileSpec, fsRdWrPerm, &fileRefNum);
	if (err != noErr)
	{
	CheckForFile0 :
		if (fileIndex == 0 && !alreadyLooked)
		{
			ParamText("\pThe WAD file could not be found. Would you like to look for it?", "\p", "\p", "\p");

			if (Alert(600, NULL) == 1)
			{
				BlackScreen();
				
				alreadyLooked = TRUE;
				
				StandardGetFile(NULL, -1, NULL, &reply);
				
				BlackScreen();
				
				if (reply.sfGood)
				{
					fileSpec = reply.sfFile;
					goto StartOver;
				}
				else
					goto BailNow;
			}
			else
			{
			BailNow :
				I_Shutdown();
				RestoreScreenDepth();
				ExitToShell();
			}
		}
		else
			return;
	}
	
	err = GetEOF(fileRefNum, &fileEOF);
	if (err != noErr)
	{
	ErrForFile0 :
		if (fileIndex == 0)
		{
			ParamText("\pA file access error occurred while loading the Doom II WAD file. Sorry.", "\p", "\p", "\p");
			Alert(rAlertErrGeneral, NULL);
			I_Shutdown();
			ExitToShell();
		}
		else
			return;
	}
	
	startlump = numlumps;
	
	length = strlen((char *)tempName) - 3;
	strupr((char *)tempName);
	
	if (strcmp((char *)tempName + length, "WAD") != 0)
	{
		// single lump file
		
		fileinfo = &singleinfo;
		singleinfo.filepos = 0;
		singleinfo.size = fileEOF;
		ExtractFileBase((char *)tempName, singleinfo.name);
		numlumps++;
	}
	else 
	{
		// WAD file
	
		count = sizeof(header);
		
		err = FSRead(fileRefNum, &count, &header);
		if (err != noErr)
			goto ErrForFile0;

		if (strncmp(header.identification, "IWAD", 4))
		{
			if (strncmp(header.identification, "PWAD", 4))
				I_Error ("WAD file `%s' doesn't have IWAD or PWAD id.", (char *)tempName);
			modifiedgame = true;		// homebrew levels
		}
		
		header.numlumps = LONG(header.numlumps);
		header.infotableofs = LONG(header.infotableofs);
		length = header.numlumps * sizeof(filelump_t);
		
		tempHndl = NewHandle( length );
		// New in 1.0.1
		if ((tempHndl == NULL) || (MemError()))
		{
			I_Error("You do not have enough memory allocated to load all the .WAD files.");
		}
		
		HLock( tempHndl );
		
		fileinfo = (filelump_t *) *tempHndl;
		
		err = SetFPos(fileRefNum, fsFromStart, header.infotableofs);
		if (err != noErr)
		{
			DisposeHandle( tempHndl );
			err = FSClose( fileRefNum );
			goto ErrForFile0;
		}
		
		count = length;
		
		err = FSRead(fileRefNum, &count, fileinfo);
		if (err != noErr)
		{
			DisposeHandle( tempHndl );
			err = FSClose( fileRefNum );
			goto ErrForFile0;
		}
		
		HUnlock( tempHndl );
		
		numlumps += header.numlumps;
	}
	
	if (reloadname)
		err = FSClose(fileRefNum);
	
//
// Fill in lumpinfo
//
	SetPtrSize((Ptr)lumpinfo, numlumps * sizeof(lumpinfo_t));
	
	if (MemError() != noErr)
	{
		void	*tempPtr;
		
		tempPtr = NewPtr( numlumps * sizeof(lumpinfo_t) );
		if ((tempPtr == NULL) || (MemError()))
			I_Error ("Couldn't realloc lumpinfo. Try allocating more memory for DOOM.");
		else
		{
			BlockMoveData(lumpinfo, tempPtr, GetPtrSize((Ptr)lumpinfo));
			DisposePtr((Ptr)lumpinfo);
			lumpinfo = tempPtr;
		}
	}
	
	fileinfo = (filelump_t *) *tempHndl;
	
	// Get pointer to global array since indexing may be too
	// large and is slower anyway.
	
	lump_p = &lumpinfo[startlump];
	
	storehandle = reloadname ? -1 : fileRefNum;
	
	for (i = startlump; i < numlumps; i++, lump_p++, fileinfo++)
	{
		lump_p->handle = storehandle;
		lump_p->position = LONG(fileinfo->filepos);
		lump_p->size = LONG(fileinfo->size);
		*((int *)lump_p->name) = 0L;
		*((int *)&lump_p->name[4]) = 0L;
		strncpy(lump_p->name, fileinfo->name, 8);
	}
	
	DisposeHandle( tempHndl );
}



/*
====================
=
= W_Reload
=
= Flushes any of the reloadable lumps in memory and reloads the directory
====================
*/

void W_Reload (void)
{
	wadinfo_t		header;
	int				lumpcount, length;
	long			count;
	lumpinfo_t		*lump_p;
	unsigned		i;
	short			err, fileRefNum;
	FSSpec			fileSpec;
	char			tempName[30];
	filelump_t		*fileinfo;
	Handle			tempHndl;
	
	if (!reloadname)
		return;
	
	strcpy((char *)tempName, reloadname);
	c2pstr((char *)tempName);

	err = FSMakeFSSpec(gAppVRefNum, gAppDirId, (unsigned char *)tempName, &fileSpec);
	if (err != noErr)
		I_Error ("W_Reload: couldn't open %s", reloadname);
	
	err = FSpOpenDF(&fileSpec, fsRdWrPerm, &fileRefNum);
	if (err != noErr)
		I_Error ("W_Reload: couldn't open %s", reloadname);
	
	count = sizeof(header);
	
	err = FSRead(fileRefNum, &count, &header);
	if (err != noErr)
		I_Error ("W_Reload: couldn't read from %s", reloadname);
		
	lumpcount = LONG(header.numlumps);
	header.infotableofs = LONG(header.infotableofs);
	
	length = lumpcount * sizeof(filelump_t);
	
	err = SetFPos(fileRefNum, fsFromStart, header.infotableofs);
	if (err != noErr)
		I_Error ("W_Reload: couldn't seek in %s", reloadname);
	
	tempHndl = NewHandle( length );
	if ((tempHndl == NULL) || (MemError()))
		I_Error("W_Reload: couldn't allocate %ld bytes of memory.", length);
	HLock( tempHndl );
	
	count = length;
	
	err = FSRead(fileRefNum, &count, *tempHndl);
	if (err != noErr)
		I_Error("W_Reload: couldn't read in %s", reloadname);

//
// Fill in lumpinfo
//
	lump_p = &lumpinfo[reloadlump];
	fileinfo = (filelump_t *) *tempHndl;
	
	for (i = reloadlump; i < (reloadlump + lumpcount); i++, lump_p++, fileinfo++)
	{
		if (lumpcache[i])
			Z_Free (lumpcache[i]);
		lump_p->position = LONG(fileinfo->filepos);
		lump_p->size = LONG(fileinfo->size);
	}
	
	(void) FSClose(fileRefNum);
	HUnlock( tempHndl );
	DisposeHandle( tempHndl );
}



/*
====================
=
= W_InitMultipleFiles
=
= Pass a null terminated list of files to use.
=
= All files are optional, but at least one file must be found
=
= Files with a .wad extension are idlink files with multiple lumps
=
= Other files are single lumps with the base filename for the lump name
=
= Lump names can appear multiple times. The name searcher looks backwards,
= so a later file can override an earlier one.
=
====================
*/

void W_InitMultipleFiles (void)
{	
	int		size, i;
	Str255	fname;
	int length;
	
//
// open all the files, load headers, and count lumps
//
	numlumps = 0;
	lumpinfo = (lumpinfo_t *) NewPtr(1);	// will be realloced as lumps are added
	if ((lumpinfo == NULL) || (MemError()))
		I_Error ("W_InitFiles: couldn't allocate pointer of 1 byte size.");
	
	for (i = 0; i < gNumWads; i++)
	{
		CopyPStr(gWadFiles[i].name, fname);
		//ParamText("\pOpening "", "\pWAD Files", fname, "\p"...");
		DrawStatusDialog(kTrue);
		W_AddFile (i);
	}
	
	if (!numlumps)
		I_Error ("W_InitFiles: no files found");
		
    // set up caching
	size = numlumps * sizeof(*lumpcache);
	lumpcache = (void **) NewPtr(size);
	if ((lumpcache == NULL) || (MemError()))
		I_Error ("Couldn't allocate lumpcache. Try allocating more memory for DOOM.");
	memset (lumpcache, 0, size);
	
    if (!lumpcache)
      I_Error ("Couldn't allocate lumpcache");

    // killough 1/31/98: initialize lump hash table
    W_InitLumpHash();
}



/*
====================
=
= W_InitFile
=
= Just initialize from a single file
=
====================
*/

void W_InitFile (char *filename)
{
	char	*names[2];

	names[0] = filename;
	names[1] = NULL;
	W_InitMultipleFiles ();
}


/*
====================
=
= W_NumLumps
=
====================
*/

int	W_NumLumps (void)
{
	return numlumps;
}

// Hash function used for lump names.
// Must be mod'ed with table size.
// Can be used for any 8-character names.
// by Lee Killough

unsigned W_LumpNameHash(const char *s)
{
  unsigned hash;
  (void) ((hash =        toupper(s[0]), s[1]) &&
          (hash = hash*3+toupper(s[1]), s[2]) &&
          (hash = hash*2+toupper(s[2]), s[3]) &&
          (hash = hash*2+toupper(s[3]), s[4]) &&
          (hash = hash*2+toupper(s[4]), s[5]) &&
          (hash = hash*2+toupper(s[5]), s[6]) &&
          (hash = hash*2+toupper(s[6]),
           hash = hash*2+toupper(s[7]))
         );
  return hash;
}


//
// W_CheckNumForName
// Returns -1 if name not found.
//
// Rewritten by Lee Killough to use hash table for performance. Significantly
// cuts down on time -- increases Doom performance over 300%. This is the
// single most important optimization of the original Doom sources, because
// lump name lookup is used so often, and the original Doom used a sequential
// search. For large wads with > 1000 lumps this meant an average of over
// 500 were probed during every search. Now the average is under 2 probes per
// search. There is no significant benefit to packing the names into longwords
// with this new hashing algorithm, because the work to do the packing is
// just as much work as simply doing the string comparisons with the new
// algorithm, which minimizes the expected number of comparisons to under 2.
//

int W_CheckNumForName(register  char *name)
{
  // Hash function maps the name to one of possibly numlump chains.
  // It has been tuned so that the average chain length never exceeds 2.

  register int i = lumpinfo[W_LumpNameHash(name) % (unsigned) numlumps].index;

  // We search along the chain until end, looking for case-insensitive
  // matches which also match a namespace tag. Separate hash tables are
  // not used for each namespace, because the performance benefit is not
  // worth the overhead, considering namespace collisions are rare in
  // Doom wads.
  
  // [SL] 9/19/23: I left out namespacing for ease of implementation.
  // Vanilla didn't have it, anyway, so it shouldn't cause problems.

  while (i >= 0 && (strncasecmp(lumpinfo[i].name, name, 8)))
    i = lumpinfo[i].next;

  // Return the matching lump, or -1 if none found.

  return i;
}

//
// killough 1/31/98: Initialize lump hash table
//

static void W_InitLumpHash(void)
{
  int i;

  for (i=0; i<numlumps; i++)
    lumpinfo[i].index = -1;                     // mark slots empty

  // Insert nodes to the beginning of each chain, in first-to-last
  // lump order, so that the last lump of a given name appears first
  // in any chain, observing pwad ordering rules. killough

  for (i=0; i<numlumps; i++)
    {                                           // hash function:
      int j = W_LumpNameHash(lumpinfo[i].name) % (unsigned) numlumps;
      lumpinfo[i].next = lumpinfo[j].index;     // Prepend to list
      lumpinfo[j].index = i;
    }
}

// End of lump hashing -- killough 1/31/98


/*
====================
=
= W_GetNumForName
=
= Calls W_CheckNumForName, but bombs out if not found
=
====================
*/

int	W_GetNumForName (char *name)
{
	return W_CheckNumForName (name);
}


/*
====================
=
= W_LumpLength
=
= Returns the buffer size needed to load the given lump
=
====================
*/

int W_LumpLength (int lump)
{
	if (lump >= numlumps)
		I_Error ("W_LumpLength: %i >= numlumps",lump);
	return lumpinfo[lump].size;
}


/*
====================
=
= W_ReadLump
=
= Loads the lump into the given buffer, which must be >= W_LumpLength()
=
====================
*/

void W_ReadLump (int lump, void *dest)
{
	long			count;
	lumpinfo_t		*l;
	short			fileRefNum, err;
	FSSpec			fileSpec;
	char			tempName[30];
	
	if (lump >= numlumps)
		I_Error ("W_ReadLump: %i >= numlumps",lump);
	
	l = lumpinfo + lump;
	
	I_BeginRead();
	
	if (l->handle == -1)
	{	
		// reloadable file, so use open / read / close
		strcpy(tempName, reloadname);
		c2pstr(tempName);
		
		err = FSMakeFSSpec(gAppVRefNum, gAppDirId, (unsigned char *)tempName, &fileSpec);
		if (err != noErr)
			I_Error ("W_ReadLump: couldn't open %s",reloadname);
		
		err = FSpOpenDF(&fileSpec, fsRdWrPerm, &fileRefNum);
		if (err != noErr)
			I_Error ("W_ReadLump: couldn't open %s",reloadname);
	}
	else
		fileRefNum = l->handle;
	
	err = SetFPos(fileRefNum, fsFromStart, l->position);
	if (err != noErr)
	{
		(void) FSClose(fileRefNum);
		I_Error ("W_ReadLump: seek failed at %i on lump %i", l->position, lump);
	}
	
	count = l->size;
	err = FSRead(fileRefNum, &count, dest);
	
	if ((count != l->size) || (err != noErr))
		I_Error ("W_ReadLump: only read %i of %i on lump %i", count, l->size, lump);	
	
	if (l->handle == -1)
		FSClose (fileRefNum);
	
	I_EndRead ();
}



/*
====================
=
= W_CacheLumpNum
=
====================
*/

void *W_CacheLumpNum (int lump, int tag)
{
	if ((unsigned)lump >= numlumps)
		I_Error ("W_CacheLumpNum: %i >= numlumps",lump);
	
	if (!lumpcache[lump])	// it's NULL.
	{
		// read the lump in
		(void) Z_Malloc (W_LumpLength (lump), tag, &lumpcache[lump]);
		W_ReadLump (lump, lumpcache[lump]);
	}
	else
		Z_ChangeTag (lumpcache[lump], tag);
	
	return lumpcache[lump];
}


/*
====================
=
= W_CacheLumpName
=
====================
*/

void *W_CacheLumpName (char *name, int tag)
{
	return W_CacheLumpNum (W_GetNumForName(name), tag);
}


/*
====================
=
= W_Profile
=
====================
*/

#if __W_PROFILE__

int	info[2500][10];
int	profilecount;

void W_Profile (void)
{
	int		i;
	memblock_t	*block;
	void	*ptr;
	char	ch;
	FILE	*f;
	int		j;
	char	name[9];
	
	
	for (i=0 ; i<numlumps ; i++)
	{	
		ptr = lumpcache[i];
		if (!ptr)
		{
			ch = ' ';
			continue;
		}
		else
		{
			block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));
			if (block->tag < PU_PURGELEVEL)
				ch = 'S';
			else
				ch = 'P';
		}
		info[i][profilecount] = ch;
	}
	profilecount++;
	
	f = fopen ("waddump.txt","w");
	name[8] = 0;
	for (i=0 ; i<numlumps ; i++)
	{
		memcpy (name,lumpinfo[i].name,8);
		for (j=0 ; j<8 ; j++)
			if (!name[j])
				break;
		for ( ; j<8 ; j++)
			name[j] = ' ';
		fprintf (f,"%s ",name);
		for (j=0 ; j<profilecount ; j++)
			fprintf (f,"    %c",info[i][j]);
		fprintf (f,"\n");
	}
	fclose (f);
}

#endif __W_PROFILE__

