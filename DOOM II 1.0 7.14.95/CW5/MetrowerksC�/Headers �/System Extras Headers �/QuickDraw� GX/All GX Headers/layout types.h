/*	File:		layout types.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef layoutTypesIncludes#define layoutTypesIncludes#ifndef mathTypesIncludes#include "math types.h"/*	#include <Types.h>											*//*		#include <ConditionalMacros.h>							*//*		#include <MixedMode.h>									*//*			#include <Traps.h>									*/#endif#ifndef graphicsTypesIncludes#include "graphics types.h"#endiftypedef unsigned short gxGlyphcode;typedef long gxByteOffset;typedef unsigned short gxRunFeatureType;typedef unsigned short gxRunFeatureSelector;#define gxNoTracking gxNegativeInfinity#define gxNoStake -1enum gxBaselineTypes {	gxRomanBaseline				= 0,	gxIdeographicCenterBaseline,	gxIdeographicLowBaseline,	gxHangingBaseline,	gxMathBaseline,	gxLastBaseline				= 31,	gxNumberOfBaselineTypes		= gxLastBaseline + 1,	gxNoOverrideBaseline		= 255};typedef unsigned long gxBaselineType;typedef Fixed gxBaselineDeltas[gxNumberOfBaselineTypes];enum gxJustificationPriorities {	gxKashidaPriority			= 0,	gxWhiteSpacePriority		= 1,	gxInterCharPriority			= 2,	gxNullJustificationPriority	= 3,	gxNumberOfJustificationPriorities};typedef unsigned char gxJustificationPriority;#define gxOverridePriority 0x8000#define gxOverrideLimits 0x4000#define gxOverrideUnlimited 0x2000#define gxUnlimitedGapAbsorption 0x1000#define gxJustificationPriorityMask 0x000F#define gxAllJustificationFlags (gxOverridePriority | gxOverrideLimits |  \	gxOverrideUnlimited | gxUnlimitedGapAbsorption | gxJustificationPriorityMask)typedef unsigned short gxJustificationFlags;enum gxDirectionOverrides {	gxNoDirectionOverride		= 0,	gxImposeLeftToRight			= 1,	gxImposeRightToLeft			= 2,	gxImposeArabic				= 3};typedef unsigned short gxDirectionOverride;#define gxNoLigatureSplits 0x80000000#define gxNoCaretAngle 0x40000000#define gxImposeWidth 0x20000000#define gxNoCrossKerning 0x10000000#define gxNoOpticalAlignment 0x08000000#define gxForceHanging 0x04000000#define gxNoSpecialJustification 0x02000000#define gxDirectionOverrideMask 0x00000003#define gxAllRunControlFlags (gxNoLigatureSplits|gxNoCaretAngle|gxImposeWidth|gxNoCrossKerning|gxNoOpticalAlignment|gxForceHanging|gxNoSpecialJustification|gxDirectionOverrideMask)typedef unsigned long gxRunControlFlags;enum gxHighlightTypes {	gxHighlightStraight			= 0,	gxHighlightAverageAngle		= 1};typedef unsigned long gxHighlightType;enum gxCaretTypes {	gxSplitCaretType			= 0,	gxLeftRightKeyboardCaret	= 1,	gxRightLeftKeyboardCaret	= 2};typedef unsigned long gxCaretType;enum gxLayoutOffsetStates {	gxOffset8_8					= 0,	gxOffset8_16				= 1,	gxOffset16_8				= 2,	gxOffset16_16				= 3,	gxOffsetInvalid				= 4};#define gxOffsetInsideLigature 0x8000typedef unsigned short gxLayoutOffsetState;#define gxNoLayoutOptions 0#define gxLineIsDisplayOnly 0x00000001#define gxAllLayoutOptionsFlags (gxLineIsDisplayOnly)#define gxMaxRunLevel 15#define gxFlushLeft 0#define gxFlushCenter (fract1/2)#define gxFlushRight fract1#define gxNoJustification 0#define gxFullJustification fract1typedef unsigned long gxLayoutOptionsFlags;#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxRunFeature {	gxRunFeatureType			featureType;	gxRunFeatureSelector		featureSelector;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxWidthDeltaRecord {	Fixed						beforeGrowLimit;	Fixed						beforeShrinkLimit;	Fixed						afterGrowLimit;	Fixed						afterShrinkLimit;	gxJustificationFlags		growFlags;	gxJustificationFlags		shrinkFlags;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxPriorityJustificationOverride {	struct gxWidthDeltaRecord	deltas[gxNumberOfJustificationPriorities];};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxGlyphJustificationOverride {	gxGlyphcode					glyph;	struct gxWidthDeltaRecord	override;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxRunControls {	gxRunControlFlags			flags;	Fixed						beforeWithStreamShift;	Fixed						afterWithStreamShift;	Fixed						crossStreamShift;	Fixed						imposedWidth;	Fixed						track;	fract						hangingInhibitFactor;	fract						kerningInhibitFactor;	Fixed						decompositionAdjustmentFactor;	gxBaselineType				baselineType;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxGlyphSubstitution {	gxGlyphcode					originalGlyph;	gxGlyphcode					substituteGlyph;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxKerningAdjustmentFactors {	fract						scaleFactor;	Fixed						adjustmentPointSizeFactor;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxKerningAdjustment {	gxGlyphcode					firstGlyph;	gxGlyphcode					secondGlyph;	struct gxKerningAdjustmentFactors withStreamFactors;	struct gxKerningAdjustmentFactors crossStreamFactors;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#define gxResetCrossStreamFactor gxNegativeInfinity#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxLayoutHitInfo {	Fixed						firstPartialDist;	Fixed						lastPartialDist;	gxByteOffset				hitSideOffset;	gxByteOffset				nonHitSideOffset;	boolean						leadingEdge;	boolean						inLoose;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxLineBaselineRecord {	gxBaselineDeltas			deltas;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct gxLayoutOptions {	Fixed						width;	fract						flush;	fract						just;	gxLayoutOptionsFlags		flags;	struct gxLineBaselineRecord	*baselineRec;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endif#ifndef __cplusplustypedef struct gxRunFeature gxRunFeature;typedef struct gxWidthDeltaRecord gxWidthDeltaRecord;typedef struct gxPriorityJustificationOverride gxPriorityJustificationOverride;typedef struct gxGlyphJustificationOverride gxGlyphJustificationOverride;typedef struct gxRunControls gxRunControls;typedef struct gxGlyphSubstitution gxGlyphSubstitution;typedef struct gxKerningAdjustmentFactors gxKerningAdjustmentFactors;typedef struct gxKerningAdjustment gxKerningAdjustment;typedef struct gxLayoutHitInfo gxLayoutHitInfo;typedef struct gxLineBaselineRecord gxLineBaselineRecord;typedef struct gxLayoutOptions gxLayoutOptions;#endif#endif