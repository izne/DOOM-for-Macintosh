/*	File:		graphics state library.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef graphicsStateLibraryIncludes#define graphicsStateLibraryIncludes	#ifndef graphicsTypesIncludes#include "graphics types.h"/*	#include "math types.h"										*//*		#include <Types.h>										*//*			#include <ConditionalMacros.h>						*//*			#include <MixedMode.h>								*//*				#include <Traps.h>								*/	#endif	#ifdef __cplusplusextern "C" {	#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct graphicsStateRecord {	gxShape						defaultShapes[gxPictureType - 1];};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endiftypedef struct graphicsStateRecord *graphicsState;	#ifndef __cplusplustypedef struct graphicsStateRecord graphicsStateRecord;	#endifextern graphicsState lastGraphicsState;extern graphicsState NewGraphicsState(void);extern void DisposeGraphicsState(graphicsState);extern void GetGraphicsState(graphicsState);extern void SetGraphicsState(graphicsState);extern graphicsState SwapGraphicsState(graphicsState);	#ifdef __cplusplus}	#endif#endif