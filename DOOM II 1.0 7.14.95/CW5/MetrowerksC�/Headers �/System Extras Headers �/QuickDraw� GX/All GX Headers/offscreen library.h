/*	File:		offscreen library.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef offscreenLibraryIncludes#define offscreenLibraryIncludes    #ifndef graphicsTypesIncludes#include "graphics types.h"/*	#include "math types.h"										*//*		#include <Types.h>										*//*			#include <ConditionalMacros.h>						*//*			#include <MixedMode.h>								*//*				#include <Traps.h>								*/    #endif	#ifdef __cplusplusextern "C" {	#endif#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endifstruct offscreen {	gxShape						draw;	gxTransform					xform;	gxViewDevice				device;	gxViewPort					port;	gxViewGroup					group;};#if defined(powerc) || defined(__powerc)#pragma options align=reset#endiftypedef struct viewPortBufferRecord **viewPortBuffer;	#ifndef __cplusplustypedef struct offscreen offscreen;	#endifextern void CreateOffscreen(offscreen *target, gxShape bitmapShape);extern void DisposeOffscreen(offscreen *target);extern void CopyToBitmaps(gxShape target, gxShape source);extern viewPortBuffer NewViewPortBuffer(gxViewPort originalPort);extern void DisposeViewPortBuffer(viewPortBuffer target);extern boolean ValidViewPortBuffer(viewPortBuffer target);extern boolean UpdateViewPortBuffer(viewPortBuffer target);extern gxViewPort GetViewPortBufferViewPort(viewPortBuffer source);extern gxShape GetViewPortBufferShape(viewPortBuffer source);	#ifdef __cplusplus}	#endif#endif