/*	File:		layout edit library.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef layoutEditLibraryIncludes#define layoutEditLibraryIncludes#include <Types.h>/*	#include <ConditionalMacros.h>								*//*	#include <MixedMode.h>										*//*		#include <Traps.h>										*/#include <Memory.h>	#ifndef selectionLibraryIncludes#include "selection library.h"/*	#include "layout types.h"									*//*		#include "math types.h"									*//*		#include "graphics types.h"								*/	#endif	#ifndef layoutTypesIncludes	#endif	#ifdef __cplusplusextern "C" {	#endiftypedef Handle LayoutEditHandle;extern LayoutEditHandle NewLayoutEditHandle(long textRunCount, const short textRunLengths[], const void *text[], long styleRunCount, const short styleRunLengths[], const gxStyle styles[], long levelRunCount, const short levelRunLengths[], const short levels[], gxLayoutOptions *layoutOptions, gxPoint *position, gxStyle defaultStyle);extern LayoutEditHandle LayoutEditHandleFromLayout(gxShape layout);extern long GetLayoutEditHandle(LayoutEditHandle handle, void *text, long *styleRunCount, short styleRunLengths[], gxStyle styles[], long *levelRunCount, short levelRunLengths[], short levels[], gxLayoutOptions *layoutOptions, gxPoint *position);extern void SetLayoutEditHandle(LayoutEditHandle handle, long textRunCount, const short textRunLengths[], const void *text[], long styleRunCount, const short styleRunLengths[], const gxStyle styles[], long levelRunCount, const short levelRunLengths[], const short levels[], const gxLayoutOptions *layoutOptions, const gxPoint *position);extern void SetLayoutEditHandleParts(LayoutEditHandle handle, gxByteOffset oldStartOffset, gxByteOffset oldEndOffset, long newTextRunCount, const short newTextRunLengths[], const void *newText[], long newStyleRunCount, const short newStyleRunLengths[], const gxStyle newStyles[], long newLevelRunCount, const short newLevelRunLengths[], const short newLevels[]);extern void SetLayoutEditHandleSelectedParts(LayoutEditHandle handle, long newTextRunCount, const short newTextRunLengths[], const void *newText[], long newStyleRunCount, const short newStyleRunLengths[], const gxStyle newStyles[], long newLevelRunCount, const short newLevelRunLengths[], const short newLevels[]);extern void SetLayoutEditHandleShapeParts(LayoutEditHandle handle, gxByteOffset startOffset, gxByteOffset endOffset, gxShape insert);extern long GetLayoutEditHandleParts(LayoutEditHandle handle, gxByteOffset startOffset, gxByteOffset endOffset, void *text, long *styleRunCount, short styleRunLengths[], gxStyle styles[], long *levelRunCount, short levelRunLengths[], short levels[]);extern long GetLayoutEditHandleSelectedParts(LayoutEditHandle handle, void *text, long *styleRunCount, short styleRunLengths[], gxStyle styles[], long *levelRunCount, short levelRunLengths[], short levels[]);extern gxShape GetLayoutEditHandleShapeParts(LayoutEditHandle handle, gxByteOffset startOffset, gxByteOffset endOffset, gxShape dest);extern void LayoutEditRotateShape(LayoutEditHandle handle, Fixed degrees, Fixed translateX, Fixed translateY);extern void LayoutEditIdle(LayoutEditHandle handle);extern void LayoutEditClick(LayoutEditHandle handle, gxPoint hitDown);extern void LayoutEditActivate(LayoutEditHandle handle);extern void LayoutEditDeactivate(LayoutEditHandle handle);extern void LayoutEditKey(LayoutEditHandle handle, char key);extern void LayoutEditUpdate(LayoutEditHandle handle);extern SelectionHandle LayoutEditGetSelection(LayoutEditHandle handle);extern void LayoutEditSetSelection(LayoutEditHandle handle, SelectionOffset start, SelectionOffset end);extern void LayoutEditSetSelectionHandle(LayoutEditHandle handle, SelectionHandle selection);extern gxViewPort GetLayoutEditViewPort(LayoutEditHandle handle);extern void LayoutEditSetStyle(LayoutEditHandle handle, gxStyle newStyle);extern void LayoutEditIncrementLevel(LayoutEditHandle handle);extern void LayoutEditDecrementLevel(LayoutEditHandle handle);extern void LayoutEditSetLevel(LayoutEditHandle handle, long level);extern void LayoutEditCut(LayoutEditHandle handle);extern void LayoutEditCopy(LayoutEditHandle handle);extern void LayoutEditPaste(LayoutEditHandle handle);extern void LayoutEditClear(LayoutEditHandle handle);extern void LayoutEditFromScrap(LayoutEditHandle handle);extern void LayoutEditToScrap(LayoutEditHandle handle);extern void DisposeLayoutEditHandle(LayoutEditHandle handle);	#ifdef __cplusplus}	#endif#endif