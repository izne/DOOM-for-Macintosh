//	===========================================================================//	LWindowAEOM.cp					�1994 Metrowerks Inc. All rights reserved.//	===========================================================================//	===========================================================================/*	This is a temporary class mainly to allow AEOM linking to the LCoreAEOM	hiearchy.  This class will either be fleshed out or replaced by added	functionality in elsewhere.*///	===========================================================================#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include	<PP_Prefix.h>#include	"LWindowAEOM.h"#include	<AERegistry.h>#include	<UReanimator.h>#include	<UAppleEventsMgr.h>#include	<String_Utils.h>#include	<LowMem.h>#include	<AEObjects.h>#include	<AEPackObject.h>#include	<Windows.h>#include	<Errors.h>LWindowAEOM *	LWindowAEOM::CreateWindowAEOMStream(	LStream	*inStream){	return (new LWindowAEOM(inStream));}LWindowAEOM *	LWindowAEOM::CreateWindowAEOM(	ResIDT		inWindowID,	LCommander	*inSuperCommander){	SetDefaultCommander(inSuperCommander);	LWindowAEOM	*theWindow = (LWindowAEOM *) UReanimator::ReadObjects('PPob', inWindowID);	Assert_(theWindow);		theWindow->FinishCreate();	if (theWindow->HasAttribute(windAttr_ShowNew)) {		theWindow->Show();	}		return theWindow;}LWindowAEOM::LWindowAEOM(ResIDT inWINDid, Uint32 inAttributes, LCommander *inSuperCommander)	:	LWindow(inWINDid, inAttributes, inSuperCommander)	,	LCoreAEOM(LCoreAEOM::GetDefaultModel(), cWindow){	SetLaziness(false);	SetUseSubModelList(true);	FixWindowName();}LWindowAEOM::LWindowAEOM(LStream *inStream)	:	LWindow(inStream)	,	LCoreAEOM(LCoreAEOM::GetDefaultModel(), cWindow)	{	SetLaziness(false);	SetUseSubModelList(true);	FixWindowName();}LWindowAEOM::LWindowAEOM(){	Assert_(false);		//	Must use parameters}LWindowAEOM::~LWindowAEOM(){}void	LWindowAEOM::FixWindowName(void)/*	AppleScripts will not work properly if window names are not 	unique and windows are being accessed by name (the default for	recording events with LWindowAEOM's).  This method ensures	the window's name is unique by adding a number after the name.*/{	WindowPeek	p;	Str255		myName;	Str255		myNewName;	Str255		windowName;	Str31		myCountString;	Int32		myCount = 0;		GetDescriptor(myName);	GetDescriptor(myNewName);	do {		p = (WindowPeek)LMGetWindowList();		while (p) {			if (p != (WindowPeek)this->GetMacPort()) {				GetWTitle((WindowPtr) p, windowName);				if (IUEqualString(myNewName, windowName) == 0) {					break;				}			}			p = p->nextWindow;		}				if (p) {			if (myName[0] >= 240)				Throw_(paramErr);						CopyPStr(myName, myNewName);			ConcatPStr(myNewName, "\p ");			myCount++;			NumToString(myCount, myCountString);			ConcatPStr(myNewName, myCountString);		}	} while (p);		if (myCount > 0)		SetDescriptor(myNewName);}DescType	LWindowAEOM::GetModelKind() const								{	return LCoreAEOM::GetModelKind();}void	LWindowAEOM::GetAEProperty(DescType inProperty,	  const AEDesc& inRequestedType,	  AEDesc& outPropertyDesc) const		{	LWindow::GetAEProperty(inProperty, inRequestedType, outPropertyDesc);}void	LWindowAEOM::HandleAppleEvent(	const AppleEvent	&inAppleEvent,	AppleEvent			&outAEReply,	AEDesc				&outResult,	Int32				inAENumber)	{	LWindow::HandleAppleEvent(inAppleEvent, outAEReply, outResult, inAENumber);}LModelProperty*	LWindowAEOM::GetModelProperty(DescType inProperty)	{	return LWindow::GetModelProperty(inProperty);}Boolean	LWindowAEOM::IsLazy(void) const{	return LWindow::IsLazy();}void	LWindowAEOM::Finalize(void)	{	LCoreAEOM::Finalize();}void	LWindowAEOM::AddSubModel(LModelObject *inSubModel)	{	LCoreAEOM::AddSubModel(inSubModel);}void	LWindowAEOM::RemoveSubModel(LModelObject *inSubModel)			{	LCoreAEOM::RemoveSubModel(inSubModel);}Int32	LWindowAEOM::CountSubModels(DescType inModelID) const			{	return LCoreAEOM::CountSubModels(inModelID);}void	LWindowAEOM::GetSubModelByPosition(	DescType		inModelID,	Int32			inPosition,	AEDesc			&outToken) const			{	LCoreAEOM::GetSubModelByPosition(inModelID, inPosition, outToken);}Int32	LWindowAEOM::GetPositionOfSubModel(	DescType			inModelID,	const LModelObject	*inSubModel) const		{	return LCoreAEOM::GetPositionOfSubModel(inModelID, inSubModel);}void	LWindowAEOM::MakeSelfSpecifier(	AEDesc	&inSuperSpecifier,	AEDesc	&outSelfSpecifier) const/*	This method makes LWindowAEOM use window names for specification.	Otherwise, cross-window semantic actions will not work properly.*/{	Str255			theName;	OSErr			err;	StAEDescriptor	windowName;	GetDescriptor(theName);	err = AECreateDesc(typeChar, (Ptr) theName + 1,		StrLength(theName), &windowName.mDesc);	ThrowIfOSErr_(err);		err = CreateObjSpecifier(cWindow, &inSuperSpecifier,		formName, &windowName.mDesc, false, &outSelfSpecifier);	ThrowIfOSErr_(err);	//	Cleanup since MakeSelfSpecifier is responsible for disposal of inSuperSpecifier...	err = AEDisposeDesc(&inSuperSpecifier);}