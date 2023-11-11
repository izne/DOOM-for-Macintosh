/*	File:		shape controls library.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef shapeControlsLibraryIncludes#define shapeControlsLibraryIncludes	#ifndef __EVENTS__#include <Events.h>/*	#include <Types.h>											*//*		#include <ConditionalMacros.h>							*//*		#include <MixedMode.h>									*//*			#include <Traps.h>									*//*	#include <Quickdraw.h>										*//*		#include <QuickdrawText.h>								*//*			#include <IntlResources.h>							*//*	#include <OSUtils.h>										*/	#endif	#ifndef graphicsTypesIncludes#include "graphics types.h"/*	#include "math types.h"										*/	#endif	#ifdef __cplusplusextern "C" {	#endiftypedef struct shapeControlRecord **shapeControl;extern shapeControl NewShapeControl(gxShape items, gxShape background, gxShape foreground);extern void DisposeShapeControl(shapeControl target);extern gxShape GetShapeControlSelection(const shapeControl source);extern void SetShapeControlsSelection(shapeControl target, gxShape shapesToSelect, boolean bringToFront, boolean replaceSelection);extern gxShape GetShapeControlSelectionHandles(const shapeControl source);extern boolean SendEventToShapeControl(shapeControl target, EventRecord *event);extern void InvalidateShapeControlShape(shapeControl target, gxShape invalidShape);extern void InvalidateShapeControlRectangle(shapeControl target, gxRectangle *bounds);extern void UpdateShapeControl(shapeControl target);	#ifdef __cplusplus}	#endif#endif