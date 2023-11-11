/*	File:		selection library.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef selectionLibraryIncludes#define selectionLibraryIncludes	#ifndef layoutTypesIncludes#include "layout types.h"/*	#include "math types.h"										*//*		#include <Types.h>										*//*			#include <ConditionalMacros.h>						*//*			#include <MixedMode.h>								*//*				#include <Traps.h>								*//*	#include "graphics types.h"									*/	#endif	#ifndef graphicsTypesIncludes	#endif	#ifdef __cplusplusextern "C" {	#endif#define selectionExtremeEdge -1enum SelectionTypes {	emptySelection,	simpleCaret,	simpleRange,	discontiguousRange};typedef unsigned short SelectionType;enum SelectionMatches {	notInSelection,	partlyInSelection,	fullyInSelection,	equalToSelection};typedef unsigned short SelectionMatch;typedef long SelectionOffset;#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct SelectionOffsetRange {	SelectionOffset				minOffset;	SelectionOffset				maxOffset;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct SelectionRanges {	long						rangeCount;	struct SelectionOffsetRange	ranges[1];};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct CaretPiece {	SelectionOffset				offset;	short						leadingEdge;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct Selection {	SelectionType				type;	union {		struct CaretPiece			caret;		struct SelectionRanges		range;	}							data;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endiftypedef struct Selection *SelectionPtr, **SelectionHandle;	#ifndef __cplusplustypedef struct SelectionOffsetRange SelectionOffsetRange;typedef struct SelectionRanges SelectionRanges;typedef struct CaretPiece CaretPiece;typedef struct Selection Selection;	#endifextern SelectionHandle NewEmptySelection(void);extern SelectionHandle NewCaretSelection(SelectionOffset offset, long leadingEdge);extern SelectionHandle NewRangeSelection(SelectionOffsetRange *range);extern SelectionHandle NewFullSelection(void);extern SelectionHandle NewStartSelection(SelectionOffset toOffset);extern SelectionHandle NewEndSelection(SelectionOffset fromOffset);extern void SetEmptySelection(SelectionHandle selection);extern void SetCaretSelection(SelectionHandle selection, SelectionOffset offset, long leadingEdge);extern void SetRangeSelection(SelectionHandle selection, SelectionOffsetRange *range);extern void SetFullSelection(SelectionHandle selection);extern void SetStartSelection(SelectionHandle selection, SelectionOffset toOffset);extern void SetEndSelection(SelectionHandle selection, SelectionOffset fromOffset);extern void DisposeSelection(SelectionHandle selection);extern SelectionType GetSelectionType(SelectionHandle selection);extern SelectionOffset GetCaretSelection(SelectionHandle selection, boolean *leadingEdge);extern long GetRangeSelection(SelectionHandle selection, SelectionRanges *selectionRanges);extern SelectionMatch RangeInSelection(SelectionHandle selection, SelectionOffsetRange *range);extern void InvertSelection(SelectionHandle dest);extern void ShiftSelection(SelectionHandle dest, SelectionOffset delta);extern void UnionSelection(SelectionHandle dest, SelectionHandle source);extern void SectSelection(SelectionHandle dest, SelectionHandle source);extern void XorSelection(SelectionHandle dest, SelectionHandle source);extern void DiffSelection(SelectionHandle dest, SelectionHandle source);extern gxShape GetLayoutSelection(gxShape layout, SelectionHandle selection, SelectionOffset lineStart, gxHighlightType highlightType, gxCaretType caretType);extern SelectionHandle NewDiscontiguousSelection(SelectionRanges *ranges);	#ifdef __cplusplus}	#endif#endif