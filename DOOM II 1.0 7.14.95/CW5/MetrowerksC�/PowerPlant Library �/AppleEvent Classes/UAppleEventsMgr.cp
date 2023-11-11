// ===========================================================================//	UAppleEventsMgr.cp				�1993 Metrowerks Inc. All rights reserved.// ===========================================================================////	Utility functions for dealing with AppleEvents#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include <UAppleEventsMgr.h>#include <UExtractFromAEDesc.h>#ifndef __AEREGISTRY__#include <AERegistry.h>#endif#ifndef __RESOURCES__#include <Resources.h>#endif#ifndef __TEXTUTILS__#include <TextUtils.h>#endifconst OSType	AETable_Type = 'aedt';	// Return codes for Toolbox string comparison functions	const Int16		strings_Equal		= 0;const Int16		strings_NotEqual	= 1;typedef struct SAETableRec {	OSType	eventClass;	OSType	eventID;	long	refCon;} SAETableRec, *SAETableP;// === Static Member Variables ===AEAddressDesc	UAppleEventsMgr::sSelfTargetAddr;Boolean			UAppleEventsMgr::sDontExecuteWorks;const Int32		Version_AE101 = 0x01018000;// ---------------------------------------------------------------------------//		� Initialize// ---------------------------------------------------------------------------//	Initialize the AppleEvents UnitvoidUAppleEventsMgr::Initialize(){	MakeSelfTargetAddr(sSelfTargetAddr);			// The kAEDontExecute flag to AESend is recognized in		// version 1.01 or later of the AppleEvent Magager.		// In version 1.0, this flag is ignored, meaning that		// you can't send a record-only AppleEvent.		//		// In fact, the AEManagerInfo call is only implemented in		// version 1.01 or later. Under version 1.0, this call		// returns paramErr (-50). Therefore, we assume we don't		// have version 1.01 if the call fails.		Int32	aeVersion;	OSErr	err = ::AEManagerInfo(keyAEVersion, &aeVersion);		sDontExecuteWorks = ((err == noErr) && (aeVersion >= Version_AE101));}// ---------------------------------------------------------------------------//		� InstallAEHandlers// ---------------------------------------------------------------------------//	Install an AEHandler function for all AppleEvents listed in//	a table stored in Resources of type AETable_TypevoidUAppleEventsMgr::InstallAEHandlers(	AEEventHandlerUPP		inHandler){	Int16	numTables = ::CountResources(AETable_Type);		for (Int16 i = 1; i <= numTables; i++) {		Handle tableH = ::GetIndResource(AETable_Type, i);				Int16	numItems = ::GetHandleSize(tableH) / sizeof(SAETableRec);		::HLockHi(tableH);		SAETableP tableP = (SAETableP) *tableH;				for (Int16 j = 1; j <= numItems; j++, tableP++) {			OSErr err = ::AEInstallEventHandler(tableP->eventClass,									tableP->eventID, inHandler,									tableP->refCon, false);			ThrowIfOSErr_(err);		}				::ReleaseResource(tableH);	}}			// ---------------------------------------------------------------------------//		� CheckForMissedParams// ---------------------------------------------------------------------------//	Throw an exception if we missed a parameter in an AppleEventvoidUAppleEventsMgr::CheckForMissedParams(	const AppleEvent&	inAppleEvent){		// We just want to check if there are any leftover parameters and		// don't care about their contents. Therefore, pass nil for the		// buffer and specify a maximum size of zero.			DescType	theType;	Size		theSize;	OSErr		err = ::AEGetAttributePtr(&inAppleEvent, keyMissedKeywordAttr,								typeWildCard, &theType, nil, 0, &theSize);										// A return code of "descriptor not found" is good. It means that		// no missing parameters were found. Any other return code is bad.									if (err == noErr) {				// No Error means that the MissedKeyword									// attribute exists and we missed a param		ThrowOSErr_(errAEParamMissed);			} else if (err != errAEDescNotFound) {		ThrowOSErr_(err);			// Some other AE Manager error	}}// ---------------------------------------------------------------------------//		� MakeAppleEvent// ---------------------------------------------------------------------------//	Make an AppleEvent to send to ourself////		Creates an AppleEvent with standard parametersvoidUAppleEventsMgr::MakeAppleEvent(	AEEventClass	inEventClass,	AEEventID		inEventID,	AppleEvent&		outAppleEvent){	OSErr	err = ::AECreateAppleEvent(inEventClass, inEventID,										&sSelfTargetAddr,										kAutoGenerateReturnID,										kAnyTransactionID,										&outAppleEvent);	ThrowIfOSErr_(err);}// ---------------------------------------------------------------------------//		� SendAppleEvent// ---------------------------------------------------------------------------//	Send an AppleEvent with standard send options and don't ask for a reply////		If inExecute is false, the AppleEvent is sent (so script engines//		can record the event) but not executed (the application does not//		receive the event).////		This function disposes of the input AppleEvent descriptor.voidUAppleEventsMgr::SendAppleEvent(	AppleEvent		&inAppleEvent,	Boolean			inExecute){	AESendMode	theSendMode = kAENoReply;	if (!inExecute) {		if (!sDontExecuteWorks) {	// If kAEDontExecute is not supported,									// just return. The program does not									// expect to receive this event.			::AEDisposeDesc(&inAppleEvent);			return;		}									theSendMode += kAEDontExecute;	}		AppleEvent	theAEReply = {typeNull, nil};	OSErr	err = ::AESend(&inAppleEvent, &theAEReply, theSendMode,						kAENormalPriority, kAEDefaultTimeout, nil, nil);	::AEDisposeDesc(&inAppleEvent);	if (theAEReply.dataHandle != nil) {		::AEDisposeDesc(&theAEReply);	}	ThrowIfOSErr_(err);}// ---------------------------------------------------------------------------//		� MakeSelfTargetAddr// ---------------------------------------------------------------------------//	Make an address descriptor for the current process (this application)//	which can be used to send AppleEvents to ourselfvoidUAppleEventsMgr::MakeSelfTargetAddr(	AEAddressDesc	&outSelfTargetAddr){	ProcessSerialNumber	currProcess;		currProcess.highLongOfPSN = 0;	currProcess.lowLongOfPSN = kCurrentProcess;		OSErr	err = ::AECreateDesc(typeProcessSerialNumber, (Ptr) &currProcess,							sizeof(currProcess), &outSelfTargetAddr);	ThrowIfOSErr_(err);}// ---------------------------------------------------------------------------//		� CompareDescriptors// ---------------------------------------------------------------------------//	Return the result of comparing the data from two DescriptorsBooleanUAppleEventsMgr::CompareDescriptors(	const AEDesc	&inLeftDesc,	DescType		inComparisonOperator,	const AEDesc	&inRightDesc){	Boolean	result;	switch (inComparisonOperator) {			case kAEGreaterThan:		// Magnitude comparison		case kAEGreaterThanEquals:		case kAELessThan:		case kAELessThanEquals:			result = CompareAsNumbers(inLeftDesc, inComparisonOperator,										inRightDesc);			break;					case kAEBeginsWith:			// String matching		case kAEEndsWith:		case kAEContains:			result = CompareAsStrings(inLeftDesc, inComparisonOperator,										inRightDesc);			break;					case kAEEquals:				// Compare based on data type of left									//   descriptor			switch (inLeftDesc.descriptorType) {							case typeShortInteger:	// Left descriptor is a number				case typeLongInteger:				case typeMagnitude:				case typeShortFloat:				case typeLongFloat:				case typeExtended:				case typeComp:					result = CompareAsNumbers(inLeftDesc, 										inComparisonOperator, inRightDesc);					break;									case typeBoolean:		// Left descriptor is a Boolean				case typeTrue:				case typeFalse: {					Boolean	leftBoolean;					Boolean	rightBoolean;					UExtractFromAEDesc::TheBoolean(inLeftDesc, leftBoolean);					UExtractFromAEDesc::TheBoolean(inRightDesc, rightBoolean);					result = (leftBoolean == rightBoolean);					break;				}									default:				// String compare for all other types					result = CompareAsStrings(inLeftDesc,										inComparisonOperator, inRightDesc);					break;			}			break;					default:					// Unknown comparison operator			ThrowOSErr_(errAEEventNotHandled);			break;	}		return result;}// ---------------------------------------------------------------------------//		� CompareAsNumbers// ---------------------------------------------------------------------------//	Return the result of comparing two Descriptors, extracting the data//	as numbersBooleanUAppleEventsMgr::CompareAsNumbers(	const AEDesc	&inLeftDesc,	DescType		inComparisonOperator,	const AEDesc	&inRightDesc){	Boolean	result = false;		Int32	leftNumber;	Int32	rightNumber;	UExtractFromAEDesc::TheInt32(inLeftDesc, leftNumber);	UExtractFromAEDesc::TheInt32(inRightDesc, rightNumber);		switch (inComparisonOperator) {			case kAEGreaterThan:			result = (leftNumber > rightNumber);			break;					case kAEGreaterThanEquals:			result = (leftNumber >= rightNumber);			break;					case kAEEquals:			result = (leftNumber == rightNumber);			break;					case kAELessThan:			result = (leftNumber < rightNumber);			break;					case kAELessThanEquals:			result = (leftNumber <= rightNumber);			break;	}		return result;}// ---------------------------------------------------------------------------//		� CompareAsStrings// ---------------------------------------------------------------------------//	Return the result of comparing two Descriptors, extracting the data//	as stringsBooleanUAppleEventsMgr::CompareAsStrings(	const AEDesc	&inLeftDesc,	DescType		inComparisonOperator,	const AEDesc	&inRightDesc){	Boolean	result = false;		Str255	baseString;	Str255	subString;	UExtractFromAEDesc::ThePString(inLeftDesc, baseString);	UExtractFromAEDesc::ThePString(inRightDesc, subString);									// No need to compare if subString									//   is longer than baseSTring	if (StrLength(baseString) >= StrLength(subString)) {		switch (inComparisonOperator) {					case kAEBeginsWith:				result = IUMagIDPString(							baseString + 1, subString + 1,							StrLength(subString), StrLength(subString),							nil) == strings_Equal;				break;							case kAEEndsWith:				result = IUMagIDPString(							baseString + StrLength(baseString) + 1							- StrLength(subString), subString + 1,							StrLength(subString), StrLength(subString),							nil) == strings_Equal;				break;							case kAEContains:		// Compare all substrings of inBaseString									//   to inSubString				Int8	subCount = StrLength(baseString) -										StrLength(subString) + 1;				for (Int8 i = 1; i <= subCount; i++) {					result = IUMagIDPString(								baseString + i, subString + 1,								StrLength(subString), StrLength(subString),								nil) == strings_Equal;					if (result) {						break;					}				}				break;							case kAEEquals:				result = IUEqualPString(baseString, subString, nil)							== strings_Equal;				break;		}	}		return result;}// ===========================================================================//	StAEDescriptor Class// ===========================================================================////	StAEDescriptor class encapsulates an Apple Event Descriptor (AEDesc).////	The Constructor creates an empty AEDesc. GetParamDesc and//	GetOptionalParamDesc load information from an Apple Event into//	the AEDesc. The Destructor disposes of the AEDesc.////	The public member mDesc is an AEDesc record which you can pass//	to AppleEvent routines or other functions.////	Use StAEDescriptors as stack-based objects to ensure that Descriptors//	are properly disposed, even if an exception occurs. For example,////		RADescriptor	theDesc;//		theDesc.GetParamDesc(myAppleEvent, myKeyword, typeQDRectangle);//		UExtractFromAEDesc::TheRect(theDesc.mDesc);////	This class has constructors for creating Descriptors for the following//	common data types://		Boolean, Short Integer, Long Interger, OSType, Pascal String// ---------------------------------------------------------------------------//		� StAEDescriptor// ---------------------------------------------------------------------------//	Default ConstructorStAEDescriptor::StAEDescriptor(){	mDesc.descriptorType = typeNull;	mDesc.dataHandle = nil;}StAEDescriptor::StAEDescriptor(	DescType	inType,	const void	*inData,	Int32		inSize){	mDesc.descriptorType = typeNull;	mDesc.dataHandle = NULL;		OSErr	err = ::AECreateDesc(inType, inData, inSize, &mDesc);	ThrowIfOSErr_(err);}StAEDescriptor::StAEDescriptor(	Boolean		inValue){	mDesc.descriptorType = typeNull;	mDesc.dataHandle = NULL;		OSErr	err = ::AECreateDesc(typeBoolean, &inValue, sizeof(inValue), &mDesc);	ThrowIfOSErr_(err);}StAEDescriptor::StAEDescriptor(	Int16		inValue){	mDesc.descriptorType = typeNull;	mDesc.dataHandle = NULL;		OSErr	err = ::AECreateDesc(typeShortInteger, &inValue, sizeof(inValue),									&mDesc);	ThrowIfOSErr_(err);}StAEDescriptor::StAEDescriptor(	Int32		inValue){	mDesc.descriptorType = typeNull;	mDesc.dataHandle = NULL;		OSErr	err = ::AECreateDesc(typeLongInteger, &inValue, sizeof(inValue),									&mDesc);	ThrowIfOSErr_(err);}StAEDescriptor::StAEDescriptor(	OSType		inValue){	mDesc.descriptorType = typeNull;	mDesc.dataHandle = NULL;		OSErr	err = ::AECreateDesc(typeType, &inValue, sizeof(inValue), &mDesc);	ThrowIfOSErr_(err);}StAEDescriptor::StAEDescriptor(	const StringPtr	inString){	mDesc.descriptorType = typeNull;	mDesc.dataHandle = NULL;		OSErr	err = ::AECreateDesc(typeChar, inString + 1, inString[0], &mDesc);	ThrowIfOSErr_(err);}// ---------------------------------------------------------------------------//		� ~StAEDescriptor// ---------------------------------------------------------------------------//	DestructorStAEDescriptor::~StAEDescriptor(){	if (mDesc.dataHandle != nil) {		::AEDisposeDesc(&mDesc);	}}// ---------------------------------------------------------------------------//		� GetParamDesc// ---------------------------------------------------------------------------//	Load Descriptor data from an AppleEvent////	Throws an OSErr exception if it can't get the datavoidStAEDescriptor::GetParamDesc(	const AppleEvent	&inAppleEvent,	AEKeyword			inKeyword,	DescType			inDesiredType){	OSErr	err = ::AEGetParamDesc(&inAppleEvent, inKeyword, inDesiredType,									&mDesc);	ThrowIfOSErr_(err);}// ---------------------------------------------------------------------------//		� GetOptionalParamDesc// ---------------------------------------------------------------------------//	Load optional Descriptor data from an AppleEvent////	Differs from GetParamDesc in that it does not throw an exception//	if the request fails because the specified keyword data does not//	exist. Use this function to extract optional parameters from//	an AppleEvent.voidStAEDescriptor::GetOptionalParamDesc(	const AppleEvent&	inAppleEvent,	AEKeyword			inKeyword,	DescType			inDesiredType){	OSErr	err = ::AEGetParamDesc(&inAppleEvent, inKeyword, inDesiredType,									&mDesc);	if ((err != errAEDescNotFound) && (err != noErr)) ThrowOSErr_(err);}