/* 	File:		DiskInit.h  	Copyright:	� 1984-1994 by Apple Computer, Inc. 				All rights reserved.  	Version:	Universal Interfaces 2.0a3  ETO #16, MPW prerelease.  Friday, November 11, 1994.   	Bugs?:		If you find a problem with this file, send the file and version 				information (from above) and the problem description to:  					Internet:	apple.bugs@applelink.apple.com 					AppleLink:	APPLE.BUGS */#ifndef __DISKINIT__#define __DISKINIT__#ifndef __TYPES__#include <Types.h>#endif/*	#include <ConditionalMacros.h>								*/#ifdef __cplusplusextern "C" {#endif#if GENERATINGPOWERPC#pragma options align=mac68k#endif#ifdef __CFM68K__#pragma lib_export on#endifstruct HFSDefaults {	char							sigWord[2];					/* signature word */	long							abSize;						/* allocation block size in bytes */	long							clpSize;					/* clump size in bytes */	long							nxFreeFN;					/* next free file number */	long							btClpSize;					/* B-Tree clump size in bytes */	short							rsrv1;						/* reserved */	short							rsrv2;						/* reserved */	short							rsrv3;						/* reserved */};typedef struct HFSDefaults HFSDefaults;#if SystemSevenOrLaterextern pascal void DILoad(void) THREEWORDINLINE(0x7002, 0x3F00, 0xA9E9);extern pascal void DIUnload(void) THREEWORDINLINE(0x7004, 0x3F00, 0xA9E9);extern pascal short DIBadMount(Point where, long evtMessage) THREEWORDINLINE(0x7000, 0x3F00, 0xA9E9);extern pascal OSErr DIFormat(short drvNum) THREEWORDINLINE(0x7006, 0x3F00, 0xA9E9);extern pascal OSErr DIVerify(short drvNum) THREEWORDINLINE(0x7008, 0x3F00, 0xA9E9);extern pascal OSErr DIZero(short drvNum, ConstStr255Param volName) THREEWORDINLINE(0x700A, 0x3F00, 0xA9E9);#elseextern pascal void DILoad(void);extern pascal void DIUnload(void);extern pascal short DIBadMount(Point where, long evtMessage);extern pascal OSErr DIFormat(short drvNum);extern pascal OSErr DIVerify(short drvNum);extern pascal OSErr DIZero(short drvNum, ConstStr255Param volName);#endif#if CGLUESUPPORTEDextern OSErr dibadmount(Point *where, long evtMessage);extern OSErr dizero(short drvnum, const char *volName);#endif#ifdef __CFM68K__#pragma lib_export off#endif#if GENERATINGPOWERPC#pragma options align=reset#endif#ifdef __cplusplus}#endif#endif /* __DISKINIT__ */