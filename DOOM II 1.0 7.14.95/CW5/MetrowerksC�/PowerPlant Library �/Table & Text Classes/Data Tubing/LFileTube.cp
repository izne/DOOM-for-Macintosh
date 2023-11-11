//	===========================================================================//	LFileTube.cp				�1994 Metrowerks Inc. All rights reserved.//	===========================================================================//	For routine descriptions see LDataTube.cp./*	NOTE:			This is not a complete implementation of flavored items in the			resource fork!*/#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include	<PP_Prefix.h>#include	"LFileTube.h"#include	<UMemoryMgr.h>#include	<Errors.h>#include	<LTubeableItem.h>#include	<LFile.h>#include	<URange32.h>#include	<Resources.h>const Int16		refNum_Undefined	= -1;LFileTube::LFileTube(){	Assert_(false);		//	Parameters required.}LFileTube::LFileTube(LFile *inFile, Int16 inPrivileges){	FSSpec	fsSpec;	FInfo	fndrInfo;	OSErr	err;	mFile = inFile;	mPrivileges = inPrivileges;		//	mFileType	mFile->GetSpecifier(fsSpec);	err = FSpGetFInfo(&fsSpec, &fndrInfo);	ThrowIfOSErr_(err);	//	Bad to do in a constructor?	mFileType = fndrInfo.fdType;}LFileTube::LFileTube(	LFile				*inFile,	const LTubeableItem	*inTubeableItem,	Int16				inPrivileges,	Boolean				inReqdFlavorsOnly):	LDataTube(inTubeableItem, inReqdFlavorsOnly){	FSSpec	fsSpec;	FInfo	fndrInfo;	OSErr	err;	mFile = inFile;	mPrivileges = inPrivileges;		//	mFileType	mFile->GetSpecifier(fsSpec);	err = FSpGetFInfo(&fsSpec, &fndrInfo);	ThrowIfOSErr_(err);	//	Bad to do in a constructor?	mFileType = fndrInfo.fdType;		mTubeableItem->AddFlavorTypesTo(this);}LFileTube::~LFileTube(){	if (mFile) {		mFile->CloseDataFork();		mFile->CloseResourceFork();	}}void	LFileTube::AddFlavor(FlavorType inFlavor){	CheckFlavor(inFlavor);		if (IsDataForkFlavor(inFlavor)) {		//	Okay, flavor is implicitly specified		if (mTubeableItem)			mTubeableItem->SendFlavorTo(inFlavor, this);	} else {		if (!HasResourceFork()) {			//	Create resource fork			//	...Not until resource is actually written.		}	}}Boolean	LFileTube::FlavorExists(FlavorType inFlavor) const{	((LFileTube *)this)->CheckFlavor(inFlavor);		if (((LFileTube *)this)->IsDataForkFlavor(inFlavor)) {		return true;	} else {		if (((LFileTube *)this)->HasResourceFork()) {			((LFileTube *)this)->MakeResourceForkCurrent();			if (Count1Resources(inFlavor) > 0)				return true;		}	}	return false;}Size	LFileTube::GetFlavorSize(FlavorType inFlavor){	long	size = 0;;	OSErr	err;		CheckFlavor(inFlavor);		if (IsDataForkFlavor(inFlavor)) {		err = GetEOF(mFile->GetDataForkRefNum(), &size);		ThrowIfOSErr_(err);	} else {		if (FlavorExists(inFlavor)) {			Handle	h;						h = Get1IndResource(inFlavor, 1);			ThrowIfResError_();			size = GetHandleSize(h);			ReleaseResource(h);		}	}	return size;}void *	LFileTube::GetFlavorData(	FlavorType	inFlavor,	void		*outFlavorData,	Int32		inOffset,	Int32		inAmount)//	For routine description see LDataTube.cp.{	long	count;	OSErr	err;		CheckFlavor(inFlavor);		if (outFlavorData == NULL)		return NULL;	inAmount = URange32::Min(inAmount, GetFlavorSize(inFlavor));	count = inAmount;	if (IsDataForkFlavor(inFlavor)) {		err = FSRead(mFile->GetDataForkRefNum(), &count, outFlavorData);		ThrowIfOSErr_(err);	} else {		if (FlavorExists(inFlavor) && outFlavorData) {			Handle	h;			Uint32	size, count;						h = Get1IndResource(inFlavor, 1);			ThrowIfResError_();			size = GetHandleSize(h);						count = URange32::Min(inAmount, size - inOffset);						if (count > 0)				BlockMoveData(*h + inOffset, outFlavorData, count);						ReleaseResource(h);		}	}	return NULL;}void	LFileTube::SetFlavorData(	FlavorType	inFlavor,	void		*inFlavorData,	Int32		inFlavorSize,	Int32		inOffset,	Int32		inAmount)//	For routine description see LDataTube.cp./*	Set functions should only be called by initialization routines	or a tubeable item in response to a send flavor message.*/{	OSErr	err;	long	count;		ThrowIfNULL_(inFlavorData);		CheckFlavor(inFlavor);		inAmount = URange32::Min(inAmount, inFlavorSize);	count = inAmount;		if (IsDataForkFlavor(inFlavor)) {		err = FSWrite(mFile->GetDataForkRefNum(), &count, inFlavorData);		ThrowIfOSErr_(err);		Assert_(inAmount == count);		err = SetEOF(mFile->GetDataForkRefNum(), inFlavorSize);		ThrowIfOSErr_(err);	} else {		Handle	h;				if (!HasResourceFork()) {			//	Create a resource fork			AddResourceFork();			mFile->OpenResourceFork(mPrivileges);		}		MakeResourceForkCurrent();				if ((inOffset != 0) || (inFlavorSize != inAmount))			Throw_(paramErr);	//	Resource fork flavors not yet fully supported.		if (FlavorExists(inFlavor)) {			h = Get1IndResource(inFlavor, 1);			ThrowIfNULL_(h);			SetHandleSize(h, inFlavorSize);			ThrowIfMemError_();			BlockMoveData(inFlavorData, *h, inFlavorSize);			ChangedResource(h);			ThrowIfResError_();		} else {			h = NewHandle(inFlavorSize);			ThrowIfNULL_(h);			BlockMoveData(inFlavorData, *h, inFlavorSize);			AddResource(h, inFlavor, 1005, "\p");	//	1005 is ugliness to match MPSR for a demo.			ThrowIfResError_();		}		WriteResource(h);		ThrowIfResError_();		ReleaseResource(h);	}}Boolean	LFileTube::IsDataForkFlavor(FlavorType inFlavor){	return mFileType == inFlavor;}void	LFileTube::CheckFlavor(FlavorType inFlavor){	Assert_(mFile);		if (IsDataForkFlavor(inFlavor)) {		if (mFile->GetDataForkRefNum() == refNum_Undefined) {			mFile->OpenDataFork(mPrivileges);		}	}}Boolean	LFileTube::HasResourceFork(void){	Boolean	rval = true;	Try_ {		if (mFile->GetResourceForkRefNum() == refNum_Undefined)			mFile->OpenResourceFork(mPrivileges);	} Catch_(err) {		if (err != eofErr)			ThrowIfOSErr_(err);		rval = false;	} EndCatch_;		return rval;}void	LFileTube::AddResourceFork(void){	OSErr		err;	FSSpec		fSpec;	FInfo		fInfo;		if (!HasResourceFork()) {		mFile->GetSpecifier(fSpec);		err = FSpGetFInfo(&fSpec, &fInfo);		ThrowIfOSErr_(err);		mFile->CreateNewFile(fInfo.fdCreator, fInfo.fdType);	}}void	LFileTube::MakeResourceForkCurrent(void) {	if (CurResFile() != mFile->GetResourceForkRefNum()) {		UseResFile(mFile->GetResourceForkRefNum());		ThrowIfResError_();	}}