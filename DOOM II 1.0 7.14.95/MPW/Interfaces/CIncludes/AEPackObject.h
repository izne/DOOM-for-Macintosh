/* 	File:		AEPackObject.h  	Copyright:	� 1984-1994 by Apple Computer, Inc. 				All rights reserved.  	Version:	Universal Interfaces 2.0a3  ETO #16, MPW prerelease.  Friday, November 11, 1994.   	Bugs?:		If you find a problem with this file, send the file and version 				information (from above) and the problem description to:  					Internet:	apple.bugs@applelink.apple.com 					AppleLink:	APPLE.BUGS */#ifndef __AEPACKOBJECT__#define __AEPACKOBJECT__#ifndef __APPLEEVENTS__#include <AppleEvents.h>#endif/*	#include <Errors.h>											*//*		#include <ConditionalMacros.h>							*//*	#include <Types.h>											*//*	#include <Memory.h>											*//*		#include <MixedMode.h>									*//*	#include <OSUtils.h>										*//*	#include <Events.h>											*//*		#include <Quickdraw.h>									*//*			#include <QuickdrawText.h>							*//*	#include <EPPC.h>											*//*		#include <PPCToolbox.h>									*//*			#include <AppleTalk.h>								*//*		#include <Processes.h>									*//*			#include <Files.h>									*//*	#include <Notification.h>									*/#ifdef __cplusplusextern "C" {#endif#if GENERATINGPOWERPC#pragma options align=mac68k#endif#ifdef __CFM68K__#pragma lib_export on#endifextern pascal OSErr CreateOffsetDescriptor(long theOffset, AEDesc *theDescriptor);extern pascal OSErr CreateCompDescriptor(DescType comparisonOperator, AEDesc *operand1, AEDesc *operand2, Boolean disposeInputs, AEDesc *theDescriptor);extern pascal OSErr CreateLogicalDescriptor(AEDescList *theLogicalTerms, DescType theLogicOperator, Boolean disposeInputs, AEDesc *theDescriptor);extern pascal OSErr CreateObjSpecifier(DescType desiredClass, AEDesc *theContainer, DescType keyForm, AEDesc *keyData, Boolean disposeInputs, AEDesc *objSpecifier);extern pascal OSErr CreateRangeDescriptor(AEDesc *rangeStart, AEDesc *rangeStop, Boolean disposeInputs, AEDesc *theDescriptor);#ifdef __CFM68K__#pragma lib_export off#endif#if GENERATINGPOWERPC#pragma options align=reset#endif#ifdef __cplusplus}#endif#endif /* __AEPACKOBJECT__ */