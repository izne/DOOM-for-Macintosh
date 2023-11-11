/* 	File:		ControlStrip.h  	Copyright:	� 1984-1994 by Apple Computer, Inc. 				All rights reserved.  	Version:	Universal Interfaces 2.0a3  ETO #16, MPW prerelease.  Friday, November 11, 1994.   	Bugs?:		If you find a problem with this file, send the file and version 				information (from above) and the problem description to:  					Internet:	apple.bugs@applelink.apple.com 					AppleLink:	APPLE.BUGS */#ifndef __CONTROLSTRIP__#define __CONTROLSTRIP__#ifndef __TYPES__#include <Types.h>#endif/*	#include <ConditionalMacros.h>								*/#ifndef __MENUS__#include <Menus.h>#endif/*	#include <Memory.h>											*//*		#include <MixedMode.h>									*//*	#include <Quickdraw.h>										*//*		#include <QuickdrawText.h>								*//*	#include <Events.h>											*//*		#include <OSUtils.h>									*/#ifndef __DIALOGS__#include <Dialogs.h>#endif/*	#include <Errors.h>											*//*	#include <Windows.h>										*//*		#include <Controls.h>									*//*	#include <TextEdit.h>										*/#ifdef __cplusplusextern "C" {#endif#if GENERATINGPOWERPC#pragma options align=mac68k#endif#ifdef __CFM68K__#pragma lib_export on#endifenum {	sdevInitModule				= 0,							/* initialize the module*/	sdevCloseModule				= 1,							/* clean up before being closed*/	sdevFeatures				= 2,							/* return feature bits*/	sdevGetDisplayWidth			= 3,							/* returns the width of the module's display*/	sdevPeriodicTickle			= 4,							/* periodic tickle when nothing else is happening*/	sdevDrawStatus				= 5,							/* update the interface in the Control Strip*/	sdevMouseClick				= 6,							/* user clicked on the module's display area in the Control Strip*/	sdevSaveSettings			= 7,							/* saved any changed settings in module's preferences file*/	sdevShowBalloonHelp			= 8								/* puts up a help balloon, if the module has one to display*/};/*********************************************************************************************	Features supported by the module.  If a bit is set, it means that feature is supported.	All undefined bits are reserved for future use by Apple, and should be set to zero.*********************************************************************************************/enum {	sdevWantMouseClicks			= 0,							/* notify the module of mouseDown events*/	sdevDontAutoTrack			= 1,							/* call the module to do mouse tracking*/	sdevHasCustomHelp			= 2,							/* module provides its own help messages*/	sdevKeepModuleLocked		= 3								/* module needs to be locked in the heap*/};/*********************************************************************************************	Result values returned by the sdevPeriodicTickle and sdevIconMouseClick selectors.	If a bit is set, the module can request that a specific function is performed by	the Control Strip.  A result of zero will do nothing.  All undefined bits are reserved	for future use by Apple, and should be set to zero.*********************************************************************************************/enum {	sdevResizeDisplay			= 0,							/* resize the module's display*/	sdevNeedToSave				= 1,							/* need to save changed settings, when convenient*/	sdevHelpStateChange			= 2,							/* need to update the help message because of a state change*/	sdevCloseNow				= 3								/* close a module because it doesn't want to stay around*/};/*********************************************************************************************	miscellaneous*********************************************************************************************/enum {	sdevFileType				= 'sdev'};enum {	sdevMenuItemMark			= '�'};/*	direction values for SBDrawBarGraph*/enum {	BarGraphSlopeLeft			= -1,							/* max end of sloping bar graph is on the left*/	BarGraphFlatRight			= 0,							/* max end of flat bar graph is on the right*/	BarGraphSlopeRight			= 1								/* max end of sloping bar graph is on the right*/};/*********************************************************************************************	utility routines to provide standard interface elements and support for common functions*********************************************************************************************/extern pascal Boolean SBIsControlStripVisible(void) TWOWORDINLINE(0x7000, 0xAAF2);extern pascal void SBShowHideControlStrip(Boolean showIt) THREEWORDINLINE(0x303C, 0x0101, 0xAAF2);extern pascal Boolean SBSafeToAccessStartupDisk(void) TWOWORDINLINE(0x7002, 0xAAF2);extern pascal short SBOpenModuleResourceFile(OSType fileCreator) THREEWORDINLINE(0x303C, 0x0203, 0xAAF2);extern pascal OSErr SBLoadPreferences(ConstStr255Param prefsResourceName, Handle *preferences) THREEWORDINLINE(0x303C, 0x0404, 0xAAF2);extern pascal OSErr SBSavePreferences(ConstStr255Param prefsResourceName, Handle preferences) THREEWORDINLINE(0x303C, 0x0405, 0xAAF2);extern pascal void SBGetDetachedIndString(StringPtr theString, Handle stringList, short whichString) THREEWORDINLINE(0x303C, 0x0506, 0xAAF2);extern pascal OSErr SBGetDetachIconSuite(Handle *theIconSuite, short theResID, unsigned long selector) THREEWORDINLINE(0x303C, 0x0507, 0xAAF2);extern pascal OSErr SBTrackPopupMenu(const Rect *moduleRect, MenuHandle theMenu) THREEWORDINLINE(0x303C, 0x0408, 0xAAF2);extern pascal OSErr SBTrackSlider(const Rect *moduleRect, short ticksOnSlider, short initialValue) THREEWORDINLINE(0x303C, 0x0409, 0xAAF2);extern pascal OSErr SBShowHelpString(const Rect *moduleRect, StringPtr helpString) THREEWORDINLINE(0x303C, 0x040A, 0xAAF2);extern pascal short SBGetBarGraphWidth(short barCount) THREEWORDINLINE(0x303C, 0x010B, 0xAAF2);extern pascal void SBDrawBarGraph(short level, short barCount, short direction, Point barGraphTopLeft) THREEWORDINLINE(0x303C, 0x050C, 0xAAF2);extern pascal void SBModalDialogInContext(ModalFilterUPP filterProc, short *itemHit) THREEWORDINLINE(0x303C, 0x040D, 0xAAF2);#ifdef __CFM68K__#pragma lib_export off#endif#if GENERATINGPOWERPC#pragma options align=reset#endif#ifdef __cplusplus}#endif#endif /* __CONTROLSTRIP__ */