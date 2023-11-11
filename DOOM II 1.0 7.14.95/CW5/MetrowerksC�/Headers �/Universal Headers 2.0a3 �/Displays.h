/* 	File:		Displays.h  	Copyright:	� 1984-1994 by Apple Computer, Inc. 				All rights reserved.  	Version:	Universal Interfaces 2.0a3  ETO #16, MPW prerelease.  Friday, November 11, 1994.   	Bugs?:		If you find a problem with this file, send the file and version 				information (from above) and the problem description to:  					Internet:	apple.bugs@applelink.apple.com 					AppleLink:	APPLE.BUGS */#ifndef __DISPLAYS__#define __DISPLAYS__#ifndef __CONDITIONALMACROS__#include <ConditionalMacros.h>#endif#ifndef __COMPONENTS__#include <Components.h>#endif/*	#include <Types.h>											*//*	#include <MixedMode.h>										*/#ifndef __SKIP_DISPLAY_INCLUDES__#ifndef __APPLEEVENTS__#include <AppleEvents.h>#endif/*	#include <Errors.h>											*//*	#include <Memory.h>											*//*	#include <OSUtils.h>										*//*	#include <Events.h>											*//*		#include <Quickdraw.h>									*//*			#include <QuickdrawText.h>							*//*	#include <EPPC.h>											*//*		#include <PPCToolbox.h>									*//*			#include <AppleTalk.h>								*//*		#include <Processes.h>									*//*			#include <Files.h>									*//*	#include <Notification.h>									*/#ifndef __WINDOWS__#include <Windows.h>#endif/*	#include <Controls.h>										*//*		#include <Menus.h>										*/#endif#ifdef __cplusplusextern "C" {#endif#if GENERATINGPOWERPC#pragma options align=mac68k#endif#ifdef __CFM68K__#pragma lib_export on#endifenum {/* AppleEvents Core Suite */	kAESystemConfigNotice		= 'cnfg',/* Core Suite types */	kAEDisplayNotice			= 'dspl',	kAEDisplaySummary			= 'dsum',	keyDMConfigVersion			= 'dmcv',	keyDMConfigFlags			= 'dmcf',	keyDMConfigReserved			= 'dmcr',	keyDisplayID				= 'dmid',	keyDisplayComponent			= 'dmdc',	keyDisplayDevice			= 'dmdd',	keyDisplayFlags				= 'dmdf',	keyDisplayMode				= 'dmdm',	keyDisplayModeReserved		= 'dmmr',	keyDisplayReserved			= 'dmdr',	keyDisplayMirroredId		= 'dmmi',	keyDeviceFlags				= 'dddf',	keyDeviceDepthMode			= 'dddm',	keyDeviceRect				= 'dddr',	keyPixMapRect				= 'dpdr',	keyPixMapHResolution		= 'dphr',	keyPixMapVResolution		= 'dpvr',	keyPixMapPixelType			= 'dppt',	keyPixMapPixelSize			= 'dpps',	keyPixMapCmpCount			= 'dpcc',	keyPixMapCmpSize			= 'dpcs',	keyPixMapAlignment			= 'dppa',	keyPixMapResReserved		= 'dprr',	keyPixMapReserved			= 'dppr',	keyPixMapColorTableSeed		= 'dpct',	keySummaryMenubar			= 'dsmb',	keySummaryChanges			= 'dsch',	keyDisplayOldConfig			= 'dold',	keyDisplayNewConfig			= 'dnew'};enum {	dmOnlyActiveDisplays		= true,	dmAllDisplays				= false};enum {/* Switch Flags */	kNoSwitchConfirmBit			= 0,							/* Flag indicating that there is no need to confirm a switch to this mode */	kDepthNotAvailableBit,										/* Current depth not available in new mode */	kShowModeBit				= 3,							/* Show this mode even though it requires a confirm. */	kModeNotResizeBit			= 4								/* Do not use this mode to resize display (for cards that mode drives a different connector). */};enum {/*	Summary Change Flags (sticky bits indicating an operation was performed)	For example, moving a display then moving it back will still set the kMovedDisplayBit.*/	kBeginEndConfigureBit		= 0,	kMovedDisplayBit,	kSetMainDisplayBit,	kSetDisplayModeBit,	kAddDisplayBit,	kRemoveDisplayBit,	kNewDisplayBit,	kDisposeDisplayBit,	kEnabledDisplayBit,	kDisabledDisplayBit,	kMirrorDisplayBit,	kUnMirrorDisplayBit};enum {/* Notification Messages for extended call back routines */	kDMNotifyInstalled			= 1,							/* At install time */	kDMNotifyEvent				= 2,							/* Post change time */	kDMNotifyRemoved			= 3,							/* At remove time */	kDMNotifyPrep				= 4,							/* Pre change time */	kDMNotifyExtendEvent		= 5,							/* Allow registrees to extend apple event before it is sent */	kDMNotifyDependents			= 6,							/* Minor notification check without full update *//* Notification Flags */	kExtendedNotificationProc	= (1L << 16)};/* Selectors for tablecloths */enum {	kTableclothInit,	kTableclothDraw,	kTableclothAnimate,	kTableclothAnimateContinue,	kTableclothClose};typedef short TableclothMessagesType;/* types for notifyType */enum {	kFullNotify,												/* This is the appleevent whole nine yards notify */	kFullDependencyNotify										/* Only sends to those who want to know about interrelated functionality (used for updating UI) */};/* DisplayID/DeviceID constants */enum {	kDummyDeviceID				= 0x0FF,						/* This is the ID of the dummy display, used when the last �real� display is disabled.*/	kInvalidDisplayID			= 0x000,						/* This is the invalid ID*/	kFirstDisplayID				= 0x100};enum {/* bits for panelListFlags */	kAllowDuplicatesBit			= 0};/* Constants for fidelity checks */enum {	kNoFidelity					= 0,	kMinimumFidelity			= 1,	kDefaultFidelity			= 500,							/* I'm just picking a number for Apple default panels and engines*/	kDefaultManufacturorFidelity = 1000							/* I'm just picking a number for Manufacturor's panels and engines (overrides apple defaults)*/};enum {	kAnyPanelType				= 0,							/* Pass to DMNewEngineList for list of all panels (as opposed to specific types)*/	kAnyEngineType				= 0,							/* Pass to DMNewEngineList for list of all engines*/	kAnyDeviceType				= 0,							/* Pass to DMNewDeviceList for list of all devices*/	kAnyPortType				= 0								/* Pass to DMNewDevicePortList for list of all devices*/};/* portListFlags for DM_NewDevicePortList */enum {/* Should offline devices be put into the port list (such as dummy display) */	kPLIncludeOfflineDevicesBit	= 0};typedef unsigned long DMFidelityType;typedef unsigned long DisplayIDType;typedef void *DMListType;typedef unsigned long DMListIndexType;struct DMListEntryRec {	DisplayIDType					itemID;						/* DisplayID Manager*/	Component						itemComponent;				/* Component Manager*/	ComponentDescription			itemDescription;			/* We can always construct this if we use something beyond the compontent mgr.*/	ResType							itemClass;					/* Class of group to put this panel (eg geometry/color/etc for panels, brightness/contrast for engines, video out/sound/etc for devices)*/	DMFidelityType					itemFidelity;				/* How good is this item for the specified search?*/	unsigned long					itemFlags;					/* Set to 0 (future expansion)*/	ResType							itemReserved;				/* What kind of code does the itemReference point to  (right now - kPanelEntryTypeComponentMgr only)*/	unsigned long					itemFuture;					/* Set to 0 (future expansion - probably an alternate code style)*/};typedef struct DMListEntryRec DMListEntryRec;typedef DMListEntryRec *DMListEntryPtr;struct DependentNotifyRec {	ResType							notifyType;					/* What type was the engine that made the change (may be zero)*/	ResType							notifyClass;				/* What class was the change (eg geometry, color etc)*/	DisplayIDType					displayID;					/* Which device was touched (kInvalidDisplayID -> all or none)*/	ComponentInstance				notifyComponent;			/* What engine did it (may be 0)?*/	unsigned long					notifyVersion;				/* Set to 0 (future expansion)*/	unsigned long					notifyFlags;				/* Set to 0 (future expansion)*/	unsigned long					notifyReserved;				/* Set to 0 (future expansion)*/	unsigned long					notifyFuture;				/* Set to 0 (future expansion)*/};typedef struct DependentNotifyRec DependentNotifyRec;typedef DependentNotifyRec *DependentNotifyPtr;typedef pascal void (*DMNotificationProcPtr)(AppleEvent *theEvent);typedef pascal void (*DMExtendedNotificationProcPtr)(void *userData, short theMessage, void *notifyData);typedef pascal OSErr (*DMTableclothProcPtr)(void *refCon, TableclothMessagesType theMsg, void *theData);typedef pascal void (*DMListIteratorProcPtr)(void *userData, DMListIndexType itemIndex, DMListEntryPtr listInfo);#if GENERATINGCFMtypedef UniversalProcPtr DMNotificationUPP;typedef UniversalProcPtr DMExtendedNotificationUPP;typedef UniversalProcPtr DMTableclothUPP;typedef UniversalProcPtr DMListIteratorUPP;#elsetypedef DMNotificationProcPtr DMNotificationUPP;typedef DMExtendedNotificationProcPtr DMExtendedNotificationUPP;typedef DMTableclothProcPtr DMTableclothUPP;typedef DMListIteratorProcPtr DMListIteratorUPP;#endifenum {	uppDMNotificationProcInfo = kPascalStackBased		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(AppleEvent*))),	uppDMExtendedNotificationProcInfo = kPascalStackBased		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void*)))		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void*))),	uppDMTableclothProcInfo = kPascalStackBased		 | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void*)))		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(TableclothMessagesType)))		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void*))),	uppDMListIteratorProcInfo = kPascalStackBased		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void*)))		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(DMListIndexType)))		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(DMListEntryPtr)))};#if GENERATINGCFM#define NewDMNotificationProc(userRoutine)		\		(DMNotificationUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDMNotificationProcInfo, GetCurrentArchitecture())#define NewDMExtendedNotificationProc(userRoutine)		\		(DMExtendedNotificationUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDMExtendedNotificationProcInfo, GetCurrentArchitecture())#define NewDMTableclothProc(userRoutine)		\		(DMTableclothUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDMTableclothProcInfo, GetCurrentArchitecture())#define NewDMListIteratorProc(userRoutine)		\		(DMListIteratorUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDMListIteratorProcInfo, GetCurrentArchitecture())#else#define NewDMNotificationProc(userRoutine)		\		((DMNotificationUPP) (userRoutine))#define NewDMExtendedNotificationProc(userRoutine)		\		((DMExtendedNotificationUPP) (userRoutine))#define NewDMTableclothProc(userRoutine)		\		((DMTableclothUPP) (userRoutine))#define NewDMListIteratorProc(userRoutine)		\		((DMListIteratorUPP) (userRoutine))#endif#if GENERATINGCFM#define CallDMNotificationProc(userRoutine, theEvent)		\		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDMNotificationProcInfo, (theEvent))#define CallDMExtendedNotificationProc(userRoutine, userData, theMessage, notifyData)		\		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDMExtendedNotificationProcInfo, (userData), (theMessage), (notifyData))#define CallDMTableclothProc(userRoutine, refCon, theMsg, theData)		\		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDMTableclothProcInfo, (refCon), (theMsg), (theData))#define CallDMListIteratorProc(userRoutine, userData, itemIndex, listInfo)		\		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDMListIteratorProcInfo, (userData), (itemIndex), (listInfo))#else#define CallDMNotificationProc(userRoutine, theEvent)		\		(*(userRoutine))((theEvent))#define CallDMExtendedNotificationProc(userRoutine, userData, theMessage, notifyData)		\		(*(userRoutine))((userData), (theMessage), (notifyData))#define CallDMTableclothProc(userRoutine, refCon, theMsg, theData)		\		(*(userRoutine))((refCon), (theMsg), (theData))#define CallDMListIteratorProc(userRoutine, userData, itemIndex, listInfo)		\		(*(userRoutine))((userData), (itemIndex), (listInfo))#endifstruct TableclothInfoRec {	DMTableclothUPP					tableclothInstance;	unsigned long					nextAnimateTick;	void							*refCon;	Boolean							allDisplays;	Boolean							filler[3];};typedef struct TableclothInfoRec TableclothInfoRec, *TableclothInfoPtr;/* Trap interfaces */extern pascal GDHandle DMGetFirstScreenDevice(Boolean activeOnly) TWOWORDINLINE(0x7000, 0xABEB);extern pascal GDHandle DMGetNextScreenDevice(GDHandle theDevice, Boolean activeOnly) TWOWORDINLINE(0x7001, 0xABEB);extern pascal void DMDrawDesktopRect(Rect *globalRect) TWOWORDINLINE(0x7002, 0xABEB);extern pascal void DMDrawDesktopRegion(RgnHandle globalRgn) TWOWORDINLINE(0x7003, 0xABEB);extern pascal OSErr DMGetGDeviceTablecloth(DisplayIDType displayID, TableclothInfoPtr tableclothInstance) THREEWORDINLINE(0x303C, 0x0404, 0xABEB);extern pascal OSErr DMSetGDeviceTablecloth(DisplayIDType displayID, TableclothInfoPtr tableclothInstance) THREEWORDINLINE(0x303C, 0x0405, 0xABEB);extern pascal OSErr DMBeginConfigureDisplays(Handle *displayState) THREEWORDINLINE(0x303C, 0x0206, 0xABEB);extern pascal OSErr DMEndConfigureDisplays(Handle displayState) THREEWORDINLINE(0x303C, 0x0207, 0xABEB);extern pascal OSErr DMAddDisplay(GDHandle newDevice, short driver, unsigned long mode, unsigned long reserved, unsigned long displayID, ComponentInstance displayComponent, Handle displayState) THREEWORDINLINE(0x303C, 0x0D08, 0xABEB);extern pascal OSErr DMMoveDisplay(GDHandle moveDevice, short x, short y, Handle displayState) THREEWORDINLINE(0x303C, 0x0609, 0xABEB);extern pascal OSErr DMDisableDisplay(GDHandle disableDevice, Handle displayState) THREEWORDINLINE(0x303C, 0x040A, 0xABEB);extern pascal OSErr DMEnableDisplay(GDHandle enableDevice, Handle displayState) THREEWORDINLINE(0x303C, 0x040B, 0xABEB);extern pascal OSErr DMRemoveDisplay(GDHandle removeDevice, Handle displayState) THREEWORDINLINE(0x303C, 0x040C, 0xABEB);/* OBSOLETE ******************pascal OSErr DMGetComponentAnimateTicks(DMTableclothUPP animationComponent,unsigned long *goodDelay,unsigned long *maxDelay)    = {0x303C,0x060D,0xABEB}		{ ; fix pascal headerspascal OSErr DMSetComponentAnimateTicks(DMTableclothUPP animationComponent,unsigned long goodDelay,unsigned long maxDelay)    = {0x303C,0x060E,0xABEB}		{ ; fix pascal headerspascal OSErr DMGetNextAnimateTime(unsigned long *nextAnimateTime)    = {0x303C,0x020F,0xABEB}		{ ; fix pascal headers************************** */extern pascal OSErr DMSetMainDisplay(GDHandle newMainDevice, Handle displayState) THREEWORDINLINE(0x303C, 0x0410, 0xABEB);extern pascal OSErr DMSetDisplayMode(GDHandle theDevice, unsigned long mode, unsigned long *depthMode, unsigned long reserved, Handle displayState) THREEWORDINLINE(0x303C, 0x0A11, 0xABEB);extern pascal OSErr DMCheckDisplayMode(GDHandle theDevice, unsigned long mode, unsigned long depthMode, unsigned long *switchFlags, unsigned long reserved, Boolean *modeOk) THREEWORDINLINE(0x303C, 0x0C12, 0xABEB);extern pascal OSErr DMGetDeskRegion(RgnHandle *desktopRegion) THREEWORDINLINE(0x303C, 0x0213, 0xABEB);extern pascal OSErr DMRegisterNotifyProc(DMNotificationUPP notificationProc, ProcessSerialNumberPtr whichPSN) THREEWORDINLINE(0x303C, 0x0414, 0xABEB);extern pascal OSErr DMRemoveNotifyProc(DMNotificationUPP notificationProc, ProcessSerialNumberPtr whichPSN) THREEWORDINLINE(0x303C, 0x0415, 0xABEB);extern pascal OSErr DMQDIsMirroringCapable(Boolean *qdIsMirroringCapable) THREEWORDINLINE(0x303C, 0x0216, 0xABEB);extern pascal OSErr DMCanMirrorNow(Boolean *canMirrorNow) THREEWORDINLINE(0x303C, 0x0217, 0xABEB);extern pascal OSErr DMIsMirroringOn(Boolean *isMirroringOn) THREEWORDINLINE(0x303C, 0x0218, 0xABEB);extern pascal OSErr DMMirrorDevices(GDHandle gD1, GDHandle gD2, Handle displayState) THREEWORDINLINE(0x303C, 0x0619, 0xABEB);extern pascal OSErr DMUnmirrorDevice(GDHandle gDevice, Handle displayState) THREEWORDINLINE(0x303C, 0x041A, 0xABEB);extern pascal OSErr DMGetNextMirroredDevice(GDHandle gDevice, GDHandle *mirroredDevice) THREEWORDINLINE(0x303C, 0x041B, 0xABEB);extern pascal OSErr DMBlockMirroring(void) TWOWORDINLINE(0x701C, 0xABEB);extern pascal OSErr DMUnblockMirroring(void) TWOWORDINLINE(0x701D, 0xABEB);extern pascal OSErr DMGetDisplayMgrA5World(unsigned long *dmA5) THREEWORDINLINE(0x303C, 0x021E, 0xABEB);extern pascal OSErr DMGetDisplayIDByGDevice(GDHandle displayDevice, DisplayIDType *displayID, Boolean failToMain) THREEWORDINLINE(0x303C, 0x051F, 0xABEB);extern pascal OSErr DMGetGDeviceByDisplayID(DisplayIDType displayID, GDHandle *displayDevice, Boolean failToMain) THREEWORDINLINE(0x303C, 0x0520, 0xABEB);extern pascal OSErr DMSetDisplayComponent(GDHandle theDevice, ComponentInstance displayComponent) THREEWORDINLINE(0x303C, 0x0421, 0xABEB);extern pascal OSErr DMGetDisplayComponent(GDHandle theDevice, ComponentInstance *displayComponent) THREEWORDINLINE(0x303C, 0x0422, 0xABEB);extern pascal OSErr DMNewDisplay(GDHandle *newDevice, short driverRefNum, unsigned long mode, unsigned long reserved, DisplayIDType displayID, ComponentInstance displayComponent, Handle displayState) THREEWORDINLINE(0x303C, 0x0D23, 0xABEB);extern pascal OSErr DMDisposeDisplay(GDHandle disposeDevice, Handle displayState) THREEWORDINLINE(0x303C, 0x0424, 0xABEB);extern pascal OSErr DMResolveDisplayComponents(void) TWOWORDINLINE(0x7025, 0xABEB);extern pascal OSErr DMRegisterExtendedNotifyProc(DMExtendedNotificationUPP notifyProc, void *notifyUserData, unsigned short nofifyOnFlags, ProcessSerialNumberPtr whichPSN) THREEWORDINLINE(0x303C, 0x07EF, 0xABEB);extern pascal OSErr DMRemoveExtendedNotifyProc(DMExtendedNotificationUPP notifyProc, void *notifyUserData, ProcessSerialNumberPtr whichPSN, unsigned short removeFlags) THREEWORDINLINE(0x303C, 0x0026, 0xABEB);extern pascal OSErr DMNewPanelList(DisplayIDType displayID, ResType panelType, DMFidelityType minimumFidelity, unsigned long panelListFlags, unsigned long reserved, DMListIndexType *thePanelCount, DMListType *thePanelList) THREEWORDINLINE(0x303C, 0x0027, 0xABEB);extern pascal OSErr DMNewEngineList(DisplayIDType displayID, ResType engineType, DMFidelityType minimumFidelity, unsigned long engineListFlags, unsigned long reserved, DMListIndexType *engineCount, DMListType *engineList) THREEWORDINLINE(0x303C, 0x0028, 0xABEB);extern pascal OSErr DMNewDeviceList(ResType deviceType, unsigned long deviceListFlags, unsigned long reserved, DMListIndexType *deviceCount, DMListType *deviceList) THREEWORDINLINE(0x303C, 0x0029, 0xABEB);extern pascal OSErr DMNewDevicePortList(ResType subType, unsigned long portListFlags, unsigned long reserved, DMListIndexType *devicePortCount, DMListType *theDevicePortList) THREEWORDINLINE(0x303C, 0x002A, 0xABEB);extern pascal OSErr DMGetIndexedItemFromList(DMListType panelList, DMListIndexType itemIndex, unsigned long reserved, DMListIteratorUPP listIterator, void *userData) THREEWORDINLINE(0x303C, 0x002B, 0xABEB);extern pascal OSErr DMDisposeList(DMListType panelList) THREEWORDINLINE(0x303C, 0x002C, 0xABEB);extern pascal OSErr DMGetNameByDisplayID(DisplayIDType displayID, Str255 *name) THREEWORDINLINE(0x303C, 0x042D, 0xABEB);extern pascal OSErr DMNewDisplayIDByPortComponent(Component thePortComponent, ResType portKind, unsigned long reserved, DisplayIDType *newID) THREEWORDINLINE(0x303C, 0x082E, 0xABEB);extern pascal OSErr DMGetPortComponentByPortID(DisplayIDType thePortID, Component *thePortComponent, ComponentDescription *theDesciption, ResType *thePortKind) THREEWORDINLINE(0x303C, 0x082F, 0xABEB);extern pascal OSErr DMSendDependentNotification(ResType notifyType, ResType notifyClass, DisplayIDType displayID, unsigned long portID, ComponentInstance notifyComponent) THREEWORDINLINE(0x303C, 0x002D, 0xABEB);extern pascal OSErr DMDisposePortComponent(Component thePortComponent) THREEWORDINLINE(0x303C, 0x0231, 0xABEB);/*pascal OSErr DMGetPortIDByDeviceComponent(Component deviceComponent, DisplayIDType* displayID)*//*    = {0x303C,0x0032,0xABEB};*/extern pascal OSErr DMGetDisplayIDByDeviceComponent(Component deviceComponent, DisplayIDType *displayID) THREEWORDINLINE(0x303C, 0x002E, 0xABEB);/*	Required calls in components:		GeneralDeviceCalls		Return information specific to all devices	OSErr		CountDevicePorts(unsigned long portCount, ResType deviceType);	OSErr		GetPortInfo(unsigned long portNumber,PortInfoRec* portInfo);					PortInfo gives information for multiported devices (such as Telecaster).	OSErr		CountPortEngineComponents(unsigned long portNumber,unsigned long* engineCount);	OSErr		GetPortEngineComponentInfo(unsigned long portNumber,unsigned long engineCount, DevicePortInfoRec* componentInfo);	Video Engine Components		Return information specific to video devices	OSErr		CheckTiming					Give the device a chance to okay or modify device settings					especially useful for overridding the "needsConfirm" bit.	OSErr		GetVideoInfo					It would be nice to be able to get direct info about screen size					dot pitch etc.  This obviously needs to be fleshed out.	OSErr		SetWiggleDevice(Boolean wiggleDevice);	OSErr		GetWiggleDevice(Boolean* isWiggling);					Returns an error if device is not capable of wiggling	Sound Engine Components		OSErr		SetWiggleDevice(Boolean wiggleDevice);	OSErr		GetWiggleDevice(Boolean* isWiggling);					Returns an error if device is not capable of wiggling	HI Component Calls		OSErr		TargetDevice(DisplayID displayID);	We could also define a standard notify message and call that when we had an update issue (saving on components having to register for notifies)			That would by us not having to handle an extended remove notification that handles "you data" specific removes.*/#ifdef __CFM68K__#pragma lib_export off#endif#if GENERATINGPOWERPC#pragma options align=reset#endif#ifdef __cplusplus}#endif#endif /* __DISPLAYS__ */