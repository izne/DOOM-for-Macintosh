/*	File:		PrintingLibraries.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef __PRINTINGMANAGER__#include <PrintingManager.h>/*	#include <Printing.h>										*//*		#include <Quickdraw.h>									*//*			#include <Types.h>									*//*				#include <ConditionalMacros.h>					*//*				#include <MixedMode.h>							*//*					#include <Traps.h>							*//*			#include <QuickdrawText.h>							*//*				#include <IntlResources.h>						*//*		#include <Dialogs.h>									*//*			#include <Windows.h>								*//*				#include <Events.h>								*//*					#include <OSUtils.h>						*//*				#include <Controls.h>							*//*					#include <Menus.h>							*//*			#include <TextEdit.h>								*//*	#include <Files.h>											*//*		#include <SegLoad.h>									*//*	#include <Collections.h>									*//*	#include <PrintingErrors.h>									*//*	#include <graphics types.h>									*//*		#include "math types.h"									*//*	#include <font types.h>										*//*	#include <scaler types.h>									*//*		#include "sfnt types.h"									*/#endifextern OSErr GetCollectionItemCategory(Collection, CollectionTag, long tagID, gxCollectionCategory *);extern OSErr SetCollectionItemCategory(Collection, CollectionTag, long tagID, gxCollectionCategory);extern OSErr RemoveCollectionCategory(Collection, gxCollectionCategory);extern OSErr GetCollectionItemLock(Collection, CollectionTag, long tagID, Boolean *isLocked);extern OSErr SetCollectionItemLock(Collection, CollectionTag, long tagID, Boolean lockIt);extern OSErr AddJobItem(gxJob, CollectionTag, long id, long itemSize, void *itemData);extern OSErr AddFormatItem(gxFormat, CollectionTag, long id, long itemSize, void *itemData);extern OSErr AddVolatileJobItem(gxJob, CollectionTag, long id, long itemSize, void *itemData);extern OSErr AddVolatileFormatItem(gxFormat, CollectionTag, long id, long itemSize, void *itemData);extern long GetPaperTypeBaseType(gxPaperType thePaperType);extern void SetPaperTypeBaseType(gxPaperType thePaperType, long theBasePaperType);extern OSType GetPaperTypeCreator(gxPaperType thePaperType);extern void SetPaperTypeCreator(gxPaperType thePaperType, OSType theCreator);extern unsigned char GetPaperTypeUnits(gxPaperType thePaperType);extern void SetPaperTypeUnits(gxPaperType thePaperType, unsigned char theUnits);extern unsigned long GetPaperTypeFlags(gxPaperType thePaperType);extern void SetPaperTypeFlags(gxPaperType thePaperType, unsigned long theFlags);extern void GetPaperTypeComment(gxPaperType thePaperType, Str255 theComment);extern OSErr SetPaperTypeComment(gxPaperType thePaperType, Str255 theComment);extern void LockFormatPaperType(gxFormat, Boolean lockPaperType);extern gxPositionConstraintTableHdl GetJobFormatLineConstraint(gxJob, gxPositionConstraintTableHdl);extern gxFontTableHdl GetJobFormatFonts(gxJob, gxFontTableHdl);extern gxStyleNameTableHdl GetJobFormatFontCommonStyles(gxJob, gxFont, gxStyleNameTableHdl);extern gxPositionConstraintTableHdl GetJobFormatFontConstraint(gxJob, gxFont, gxPositionConstraintTableHdl);extern gxStyle SetStyleJobFormatCommonStyle(gxJob, Str255, gxStyle);extern PicHandle PictureToPICT(gxShape theShape, Boolean simpleProxy);