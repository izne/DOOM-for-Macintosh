/* 	File:		Lists.h  	Copyright:	� 1984-1994 by Apple Computer, Inc. 				All rights reserved.  	Version:	Universal Interfaces 2.0a3  ETO #16, MPW prerelease.  Friday, November 11, 1994.   	Bugs?:		If you find a problem with this file, send the file and version 				information (from above) and the problem description to:  					Internet:	apple.bugs@applelink.apple.com 					AppleLink:	APPLE.BUGS */#ifndef __LISTS__#define __LISTS__#ifndef __TYPES__#include <Types.h>#endif/*	#include <ConditionalMacros.h>								*/#ifndef __CONTROLS__#include <Controls.h>#endif/*	#include <Quickdraw.h>										*//*		#include <MixedMode.h>									*//*		#include <QuickdrawText.h>								*//*	#include <Menus.h>											*//*		#include <Memory.h>										*//*		#include <Events.h>										*//*			#include <OSUtils.h>								*/#ifdef __cplusplusextern "C" {#endif#if GENERATINGPOWERPC#pragma options align=mac68k#endif#ifdef __CFM68K__#pragma lib_export on#endifenum {	lDoVAutoscroll				= 2,	lDoHAutoscroll				= 1,	lOnlyOne					= -128,	lExtendDrag					= 64,	lNoDisjoint					= 32,	lNoExtend					= 16,	lNoRect						= 8,	lUseSense					= 4,	lNoNilHilite				= 2,	lInitMsg					= 0,	lDrawMsg					= 1,	lHiliteMsg					= 2,	lCloseMsg					= 3};/*	Set STRICT_LISTS to 1 to make sure your code 	doesn't access the list records directly*/#ifndef STRICT_LISTS#define STRICT_LISTS 0#endif#if STRICT_LISTStypedef struct OpaqueListRef *ListRef;typedef ListRef ListHandle;#elsetypedef struct ListRec ListRec, *ListPtr, **ListHandle;typedef ListHandle ListRef;#endiftypedef Point Cell;typedef char DataArray[32001];typedef char *DataPtr, **DataHandle;typedef pascal short (*ListSearchProcPtr)(Ptr aPtr, Ptr bPtr, short aLen, short bLen);typedef Boolean (*ListClickLoopProcPtr)(void);#if GENERATINGCFMtypedef UniversalProcPtr ListSearchUPP;typedef UniversalProcPtr ListClickLoopUPP;#elsetypedef ListSearchProcPtr ListSearchUPP;typedef ListClickLoopProcPtr ListClickLoopUPP;#endif#if !STRICT_LISTSstruct ListRec {	Rect							rView;	GrafPtr							port;	Point							indent;	Point							cellSize;	Rect							visible;	ControlHandle					vScroll;	ControlHandle					hScroll;	SInt8							selFlags;	Boolean							lActive;	SInt8							lReserved;	SInt8							listFlags;	long							clikTime;	Point							clikLoc;	Point							mouseLoc;	ListClickLoopUPP				lClickLoop;	Cell							lastClick;	long							refCon;	Handle							listDefProc;	Handle							userHandle;	Rect							dataBounds;	DataHandle						cells;	short							maxIndex;	short							cellArray[1];};#endiftypedef pascal void (*ListDefProcPtr)(short lMessage, Boolean lSelect, Rect *lRect, Cell lCell, short lDataOffset, short lDataLen, ListHandle lHandle);#if GENERATINGCFMtypedef UniversalProcPtr ListDefUPP;#elsetypedef ListDefProcPtr ListDefUPP;#endifenum {	uppListSearchProcInfo = kPascalStackBased		 | RESULT_SIZE(SIZE_CODE(sizeof(short)))		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Ptr)))		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Ptr)))		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(short)))		 | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(short))),	uppListClickLoopProcInfo = kCStackBased		 | RESULT_SIZE(SIZE_CODE(sizeof(Boolean))),	uppListDefProcInfo = kPascalStackBased		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Boolean)))		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Rect*)))		 | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Cell)))		 | STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(short)))		 | STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(short)))		 | STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(ListHandle)))};#if GENERATINGCFM#define NewListSearchProc(userRoutine)		\		(ListSearchUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppListSearchProcInfo, GetCurrentArchitecture())#define NewListClickLoopProc(userRoutine)		\		(ListClickLoopUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppListClickLoopProcInfo, GetCurrentArchitecture())#define NewListDefProc(userRoutine)		\		(ListDefUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppListDefProcInfo, GetCurrentArchitecture())#else#define NewListSearchProc(userRoutine)		\		((ListSearchUPP) (userRoutine))#define NewListClickLoopProc(userRoutine)		\		((ListClickLoopUPP) (userRoutine))#define NewListDefProc(userRoutine)		\		((ListDefUPP) (userRoutine))#endif#if GENERATINGCFM#define CallListSearchProc(userRoutine, aPtr, bPtr, aLen, bLen)		\		CallUniversalProc((UniversalProcPtr)(userRoutine), uppListSearchProcInfo, (aPtr), (bPtr), (aLen), (bLen))#define CallListClickLoopProc(userRoutine)		\		CallUniversalProc((UniversalProcPtr)(userRoutine), uppListClickLoopProcInfo)#define CallListDefProc(userRoutine, lMessage, lSelect, lRect, lCell, lDataOffset, lDataLen, lHandle)		\		CallUniversalProc((UniversalProcPtr)(userRoutine), uppListDefProcInfo, (lMessage), (lSelect), (lRect), (lCell), (lDataOffset), (lDataLen), (lHandle))#else#define CallListSearchProc(userRoutine, aPtr, bPtr, aLen, bLen)		\		(*(userRoutine))((aPtr), (bPtr), (aLen), (bLen))#define CallListClickLoopProc(userRoutine)		\		(*(userRoutine))()#define CallListDefProc(userRoutine, lMessage, lSelect, lRect, lCell, lDataOffset, lDataLen, lHandle)		\		(*(userRoutine))((lMessage), (lSelect), (lRect), (lCell), (lDataOffset), (lDataLen), (lHandle))#endifextern pascal ListHandle LNew(const Rect *rView, const Rect *dataBounds, Point cSize, short theProc, WindowPtr theWindow, Boolean drawIt, Boolean hasGrow, Boolean scrollHoriz, Boolean scrollVert) THREEWORDINLINE(0x3F3C, 0x0044, 0xA9E7);extern pascal void LDispose(ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0028, 0xA9E7);extern pascal short LAddColumn(short count, short colNum, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0004, 0xA9E7);extern pascal short LAddRow(short count, short rowNum, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0008, 0xA9E7);extern pascal void LDelColumn(short count, short colNum, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0020, 0xA9E7);extern pascal void LDelRow(short count, short rowNum, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0024, 0xA9E7);extern pascal Boolean LGetSelect(Boolean next, Cell *theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x003C, 0xA9E7);extern pascal Cell LLastClick(ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0040, 0xA9E7);extern pascal Boolean LNextCell(Boolean hNext, Boolean vNext, Cell *theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0048, 0xA9E7);extern pascal Boolean LSearch(const void *dataPtr, short dataLen, ListSearchUPP searchProc, Cell *theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0054, 0xA9E7);extern pascal void LSize(short listWidth, short listHeight, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0060, 0xA9E7);extern pascal void LSetDrawingMode(Boolean drawIt, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x002C, 0xA9E7);extern pascal void LScroll(short dCols, short dRows, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0050, 0xA9E7);extern pascal void LAutoScroll(ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0010, 0xA9E7);extern pascal void LUpdate(RgnHandle theRgn, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0064, 0xA9E7);extern pascal void LActivate(Boolean act, ListHandle lHandle) TWOWORDINLINE(0x4267, 0xA9E7);extern pascal void LCellSize(Point cSize, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0014, 0xA9E7);extern pascal Boolean LClick(Point pt, short modifiers, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0018, 0xA9E7);extern pascal void LAddToCell(const void *dataPtr, short dataLen, Cell theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x000C, 0xA9E7);extern pascal void LClrCell(Cell theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x001C, 0xA9E7);extern pascal void LGetCell(void *dataPtr, short *dataLen, Cell theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0038, 0xA9E7);extern pascal void LGetCellDataLocation(short *offset, short *len, Cell theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0034, 0xA9E7);extern pascal void LRect(Rect *cellRect, Cell theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x004C, 0xA9E7);extern pascal void LSetCell(const void *dataPtr, short dataLen, Cell theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0058, 0xA9E7);extern pascal void LSetSelect(Boolean setIt, Cell theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x005C, 0xA9E7);extern pascal void LDraw(Cell theCell, ListHandle lHandle) THREEWORDINLINE(0x3F3C, 0x0030, 0xA9E7);#if CGLUESUPPORTEDextern void laddtocell(Ptr dataPtr, short dataLen, Cell *theCell, ListHandle lHandle);extern void lclrcell(Cell *theCell, ListHandle lHandle);extern void lgetcelldatalocation(short *offset, short *len, Cell *theCell, ListHandle lHandle);extern void lgetcell(Ptr dataPtr, short *dataLen, Cell *theCell, ListHandle lHandle);extern ListHandle lnew(Rect *rView, Rect *dataBounds, Point *cSize, short theProc, WindowPtr theWindow, Boolean drawIt, Boolean HasGrow, Boolean ScrollHoriz, Boolean ScrollVert);extern void lrect(Rect *cellRect, Cell *theCell, ListHandle lHandle);extern void lsetcell(Ptr dataPtr, short dataLen, Cell *theCell, ListHandle lHandle);extern void lsetselect(Boolean setIt, Cell *theCell, ListHandle lHandle);extern void ldraw(Cell *theCell, ListHandle lHandle);extern Boolean lclick(Point *pt, short modifiers, ListHandle lHandle);extern void lcellsize(Point *cSize, ListHandle lHandle);#endif#if OLDROUTINENAMES#define LDoDraw(drawIt, lHandle) LSetDrawingMode(drawIt, lHandle)#define LFind(offset, len, theCell, lHandle)  \	LGetCellDataLocation(offset, len, theCell, lHandle)#if CGLUESUPPORTED#define lfind(offset, len, theCell, lHandle)  \	lgetcelldatalocation(offset, len, theCell, lHandle)#endif#endif#ifdef __CFM68K__#pragma lib_export off#endif#if GENERATINGPOWERPC#pragma options align=reset#endif#ifdef __cplusplus}#endif#endif /* __LISTS__ */