// ===========================================================================//	LTextEdit.cp					�1993 Metrowerks Inc. All rights reserved.// ===========================================================================////	### Still under moderate construction//			* Doesn't autoscroll while typing or selecting//			* Doesn't adjust to display whole lines//			* Doesn't undo#include <LTextEdit.h>#include <LStream.h>#include <UTextTraits.h>#include <UDrawingState.h>#include <UMemoryMgr.h>#include <UKeyFilters.h>#include <PP_KeyCodes.h>#include <PP_Messages.h>#ifndef __MEMORY__#include <Memory.h>#endif#ifndef __TOOLUTILS__#include <ToolUtils.h>#endif#ifndef __SCRIPT__#include <Script.h>#endif#ifndef __SCRAP__#include <Scrap.h>#endif#ifndef __RESOURCES__#include <Resources.h>#endif// ---------------------------------------------------------------------------//		� CreateTextEditStream// ---------------------------------------------------------------------------//	Create a new TextEdit object from the data in a StreamLTextEdit*LTextEdit::CreateTextEditStream(	LStream	*inStream){	return (new LTextEdit(inStream));}// ---------------------------------------------------------------------------//		� LTextEdit// ---------------------------------------------------------------------------//	Default ContructorLTextEdit::LTextEdit(){	mTextAttributes = 0;	InitTextEdit(0);			// Initialize member variables	AlignTextEditRects();}// ---------------------------------------------------------------------------//		� LTextEdit// ---------------------------------------------------------------------------//	Construct from input parametersLTextEdit::LTextEdit(	const SPaneInfo	&inPaneInfo,	const SViewInfo	&inViewInfo,	Uint16			inTextAttributes,	ResIDT			inTextTraitsID)		: LView(inPaneInfo, inViewInfo){	mTextAttributes = inTextAttributes;	InitTextEdit(inTextTraitsID);	AlignTextEditRects();}// ---------------------------------------------------------------------------//		� LTextEdit(LStream*)// ---------------------------------------------------------------------------//	Contruct an TextEdit from the data in a StreamLTextEdit::LTextEdit(	LStream	*inStream)		: LView(inStream){	inStream->ReadData(&mTextAttributes, sizeof(Uint16));	ResIDT	textTraitsID;	inStream->ReadData(&textTraitsID, sizeof(ResIDT));		ResIDT	initialTextID;	inStream->ReadData(&initialTextID, sizeof(ResIDT));		InitTextEdit(textTraitsID);		Handle	initialTextH = GetResource('TEXT', initialTextID);	if (initialTextH != nil) {		SetTextHandle(initialTextH);	}		AlignTextEditRects();}// ---------------------------------------------------------------------------//		� InitTextEdit// ---------------------------------------------------------------------------//	Initialize member variables of a TextEdit to default valuesvoidLTextEdit::InitTextEdit(	ResIDT	inTextTraitsID){	Rect	viewRect = {0, 0, 0, 0};	mTextEditH = ::TENew(&viewRect, &viewRect);		SetTextTraitsID(inTextTraitsID);			// If word wrap is on, then the Image width is always the		// same as the Frame width, which forces text to wrap to		// the Frame.				// If the Image width is zero (or negative), the user		// probably forgot to set it. To accommodate this error,		// we set the Image width to the Frame width. However, the		// Image will not change if the Frame resizes.		if ((mTextAttributes & textAttr_WordWrap) ||		(mImageSize.width <= 0)) {		mImageSize.width = mFrameSize.width;	}}// ---------------------------------------------------------------------------//		� ~LTextEdit// ---------------------------------------------------------------------------//	DestructorLTextEdit::~LTextEdit(){	if (mTextEditH != nil) {		::TEDispose(mTextEditH);	}}// ---------------------------------------------------------------------------//		� SetTextHandle// ---------------------------------------------------------------------------//	Set the text in the LTextEdit to the contents of the specified Handle////	The LTextEdit copies the data in the Handle, so the caller retains//	ownership of the Handle (and should dispose of it as needed)voidLTextEdit::SetTextHandle(	Handle	inTextH){	StHandleLocker	lock(inTextH);	SetTextPtr(*inTextH, ::GetHandleSize(inTextH));}voidLTextEdit::SetTextPtr(	Ptr		inTextP,	Int32	inTextLen){	::TESetText(inTextP, inTextLen, mTextEditH);	::TECalText(mTextEditH);	AdjustImageToText();	Refresh();}// ---------------------------------------------------------------------------//		� GetMacTEH// ---------------------------------------------------------------------------//	Return a Handle to the text in the LTextEdit////	The Handle is the actual Handle used by the Toolbox TextEdit record.//	Treat this Handle as read-onlyHandleLTextEdit::GetTextHandle(){	return (Handle) ::TEGetText(mTextEditH);}// ---------------------------------------------------------------------------//		� GetMacTEH// ---------------------------------------------------------------------------//	Return a Handle to the Mac TextEdit Record associated with an TextEdit////	Caller may change record fields, and is responsible for redrawing the//	TextEdit as necessary to reflect any changes. However, caller must//	not dispose of the TEHandle.TEHandleLTextEdit::GetMacTEH(){	return mTextEditH;}// ---------------------------------------------------------------------------//		� SetTextTraitsID// ---------------------------------------------------------------------------//	Specify the resource ID of the TextTraits for an TextEdit////	This function updates the line height to fit the text characteristics.voidLTextEdit::SetTextTraitsID(	ResIDT	inTextTraitsID){	FocusDraw();	mTextTraitsID = inTextTraitsID;	UTextTraits::SetTETextTraits(mTextTraitsID, mTextEditH);		SPoint32	scrollUnit;	scrollUnit.h = 4;	scrollUnit.v = (**mTextEditH).lineHeight;	SetScrollUnit(scrollUnit);}BooleanLTextEdit::HasAttribute(	Uint16	inAttribute){	return ((mTextAttributes & inAttribute) != 0);}BooleanLTextEdit::FocusDraw(){	Boolean	focused = LView::FocusDraw();	if (focused) {		StColorPenState::Normalize();		UTextTraits::SetPortTextTraits(mTextTraitsID);	}		return focused;}// ---------------------------------------------------------------------------//		� DrawSelf// ---------------------------------------------------------------------------//	Draw a TextEditvoidLTextEdit::DrawSelf(){	Rect	frame;	CalcLocalFrameRect(frame);			// A Mac TERec stores a pointer to its owner port  We have to		// change it to the current port in case we are drawing into		// a port that is not the owner port. This happens when we are		// printing or drawing into an offscreen port.			GrafPtr	savePort = (**mTextEditH).inPort;	(**mTextEditH).inPort = UQDGlobals::GetCurrentPort();	::TEUpdate(&frame, mTextEditH);		(**mTextEditH).inPort = savePort;}// ---------------------------------------------------------------------------//		� ClickSelf// ---------------------------------------------------------------------------//	Respond to Click inside an TextEditvoidLTextEdit::ClickSelf(	const SMouseDownEvent	&inMouseDown){	if (!IsTarget()) {				// If not the Target, clicking in an									//   TextEdit makes it the Target.									// Since TEClick will set a new selection									//   range, clear the current selection									//   range to avoid an ugly flash.		::TESetSelect(0, 0, mTextEditH);		SwitchTarget(this);	}		if (IsTarget()) {		FocusDraw();		::TEClick(inMouseDown.whereLocal,					((inMouseDown.macEvent.modifiers & shiftKey) != 0),					mTextEditH);	}}// ---------------------------------------------------------------------------//		� AdjustCursorSelf// ---------------------------------------------------------------------------//	TextEdit uses the standard I-Beam cursorvoidLTextEdit::AdjustCursorSelf(	Point				inPortPt,	const EventRecord	&inMacEvent){	CursHandle	theCursH = ::GetCursor(iBeamCursor);	if (theCursH != nil) {		::SetCursor(*theCursH);	}}BooleanLTextEdit::ObeyCommand(	CommandT	inCommand,	void*		ioParam){	Boolean		cmdHandled = true;		switch (inCommand) {			case cmd_Cut:			::TECut(mTextEditH);			::ZeroScrap();			::TEToScrap();			AdjustImageToText();			UserChangedText();			break;					case cmd_Copy:			::TECopy(mTextEditH);			::ZeroScrap();			::TEToScrap();			break;					case cmd_Paste:			::TEFromScrap();			::TEPaste(mTextEditH);			AdjustImageToText();			UserChangedText();			break;					case cmd_Clear: {			LAction	*theAction = new LAction('Clr ', "\pUndo Clear", this,										SaveStateForUndo());			::TEDelete(mTextEditH);			if (!mSuperCommander->ObeyCommand(msg_ActionDone, theAction)) {				delete theAction;			}			AdjustImageToText();			UserChangedText();			break;		}					case msg_TabSelect:			if (!IsEnabled()) {				cmdHandled = false;				break;			} // else FALL THRU to SelectAll()		case cmd_SelectAll:			SelectAll();			break;					default:			cmdHandled = LCommander::ObeyCommand(inCommand, ioParam);			break;	}		return cmdHandled;}voidLTextEdit::FindCommandStatus(	CommandT	inCommand,	Boolean		&outEnabled,	Boolean		&outUsesMark,	Char16		&outMark,	Str255		outName){	outUsesMark = false;	switch (inCommand) {			case cmd_Cut:				// Cut, Copy, and Clear enabled		case cmd_Copy:				//   if something is selected		case cmd_Clear:			outEnabled = ((**mTextEditH).selStart != (**mTextEditH).selEnd);			break;							case cmd_Paste: {			// Check if TEXT is in the Scrap			Int32	offset;			outEnabled = (::GetScrap(nil, 'TEXT', &offset) > 0);			break;		}				case cmd_SelectAll:			// Check if any characters are present			outEnabled = (**mTextEditH).teLength > 0;			break;					default:			LCommander::FindCommandStatus(inCommand, outEnabled,									outUsesMark, outMark, outName);			break;	}}// ---------------------------------------------------------------------------//		� HandleKeyPress// ---------------------------------------------------------------------------//	Handle key stroke directed at an TextEdit////	Return true if the TextEdit handles the keystrokeBooleanLTextEdit::HandleKeyPress(	const EventRecord&	inKeyEvent){	Boolean		keyHandled = true;	EKeyStatus	theKeyStatus = keyStatus_Input;	Int16		theKey = inKeyEvent.message & charCodeMask;		if (inKeyEvent.modifiers & cmdKey) {	// Always pass up when the command		theKeyStatus = keyStatus_PassUp;	//   key is down	} else {			theKeyStatus = UKeyFilters::PrintingCharField(inKeyEvent);	}		short	lineCount = (**mTextEditH).nLines;		switch (theKeyStatus) {			case keyStatus_Input:		case keyStatus_TEDelete:			// +++ Set up Action for Undo			FocusDraw();			::TEKey(theKey, mTextEditH);			UserChangedText();			break;					case keyStatus_TECursor:			FocusDraw();			::TEKey(theKey, mTextEditH);			break;					case keyStatus_ExtraEdit:			if (theKey == char_FwdDelete) {				FocusDraw();				if (((**mTextEditH).selStart == (**mTextEditH).selEnd) &&					((**mTextEditH).selStart < (**mTextEditH).teLength) ) {					::TESetSelect((**mTextEditH).selStart + 1,									(**mTextEditH).selStart + 1,									mTextEditH);					::TEKey(char_Backspace, mTextEditH);				} else {					::TEDelete(mTextEditH);				}						} else {				keyHandled = LCommander::HandleKeyPress(inKeyEvent);			}			break;					case keyStatus_Reject:			// +++ Do something			SysBeep(1);			break;					case keyStatus_PassUp:			if (theKey == char_Return) {				FocusDraw();				::TEKey(theKey, mTextEditH);				UserChangedText();			} else {				keyHandled = LCommander::HandleKeyPress(inKeyEvent);			}			break;	}		if (lineCount != (**mTextEditH).nLines) {		AdjustImageToText();	}		return keyHandled;}// ---------------------------------------------------------------------------//		� SelectAll// ---------------------------------------------------------------------------//	Select entire contents of an TextEditvoidLTextEdit::SelectAll(){	FocusDraw();	::TESetSelect(0, 32767, mTextEditH);}// ---------------------------------------------------------------------------//		� AlignTextEditRects// ---------------------------------------------------------------------------//	Align the view and destination rectangles of the Toolbox TextEdit//	record with the Frame of a TextEditvoidLTextEdit::AlignTextEditRects(){	Rect	textFrame;	if (CalcLocalFrameRect(textFrame)) {									// TextEdit view rect same as Frame									//   in local coords		(**mTextEditH).viewRect = textFrame;											// TextEdit dest rect same as Image									//   in local coords		SPoint32	imagePt = {0, 0};		ImageToLocalPoint(imagePt, topLeft((**mTextEditH).destRect));											// Bottom of dest rect is ignored		imagePt.h = mImageSize.width;		ImageToLocalPoint(imagePt, botRight((**mTextEditH).destRect));				::TECalText(mTextEditH);	// Let TextEdit adjust line breaks	}}voidLTextEdit::AdjustImageToText(){	ResizeImageTo(mImageSize.width,				  TEGetHeight((**mTextEditH).nLines, 1, mTextEditH),				  false);}// ---------------------------------------------------------------------------//		� ResizeFrameBy// ---------------------------------------------------------------------------//	Change the Frame size by the specified amounts////		inWidthDelta and inHeightDelta specify, in pixels, how much larger//		to make the Frame. Positive deltas increase the size, negative deltas//		reduce the size.voidLTextEdit::ResizeFrameBy(	Int16		inWidthDelta,	Int16		inHeightDelta,	Boolean		inRefresh){									// Resize Pane	LView::ResizeFrameBy(inWidthDelta, inHeightDelta, inRefresh);		if (mTextAttributes & textAttr_WordWrap) {		ResizeImageTo(mFrameSize.width, mImageSize.height, false);	}		AlignTextEditRects();	AdjustImageToText();}// ---------------------------------------------------------------------------//		� MoveBy// ---------------------------------------------------------------------------//	Move the location of the Frame by the specified amounts////		inHorizDelta and inVertDelta specify, in pixels, how far to move the//		Frame (within its surrounding Image). Positive horiz deltas move to//		the left, negative to the right. Positive vert deltas move down,//		negative up.voidLTextEdit::MoveBy(	Int32		inHorizDelta,	Int32		inVertDelta,	Boolean		inRefresh){	LView::MoveBy(inHorizDelta, inVertDelta, inRefresh);	AlignTextEditRects();}// ---------------------------------------------------------------------------//		� ScrollImageBy// ---------------------------------------------------------------------------//	Scroll the TextvoidLTextEdit::ScrollImageBy(	Int32		inLeftDelta,		// Pixels to scroll horizontally	Int32		inTopDelta,			// Pixels to scroll vertically	Boolean		inRefresh){	OffsetRect(&(**mTextEditH).viewRect, inLeftDelta, inTopDelta);	LView::ScrollImageBy(inLeftDelta, inTopDelta, inRefresh);}// ---------------------------------------------------------------------------//		� BeTarget// ---------------------------------------------------------------------------//	TextEdit is becoming the TargetvoidLTextEdit::BeTarget(){	if (FocusDraw()) {				// Show active selection		::TEActivate(mTextEditH);	// ??? What if we can't Focus ???	}	StartIdling();					// Idle time used to flash the cursor}// ---------------------------------------------------------------------------//		� DontBeTarget// ---------------------------------------------------------------------------//	TextEdit is no longer the Target////	Remove TextEdit from IdleQueuevoidLTextEdit::DontBeTarget(){	if (FocusDraw()) {				// Show inactive selection		::TEDeactivate(mTextEditH);	}	StopIdling();					// Stop flashing the cursor}// ---------------------------------------------------------------------------//		� SpendTime// ---------------------------------------------------------------------------//	Idle time: Flash the insertion cursorvoidLTextEdit::SpendTime(	const EventRecord	&inMacEvent){	if (FocusDraw() & IsVisible()) {		::TEIdle(mTextEditH);	}}// ---------------------------------------------------------------------------//		� UserChangedText// ---------------------------------------------------------------------------//	Text of TextEdit has changed as a result of user action////	Override to validate field and/or dynamically update as the user//	types. This function is not called by SetDescriptor, which is typically//	used to programatically change the text.voidLTextEdit::UserChangedText(){}voidLTextEdit::UndoAction(	ActionIDT	inActionID,	void		*inUndoData){	LAction		*theAction;		switch (inActionID) {			case 'Clr ':			theAction = new LAction('RClr', "\pRedo Clear", this,										SaveStateForUndo());			break;				case 'RClr':			theAction = new LAction('Clr ', "\pUndo Clear", this,										SaveStateForUndo());			break;	}		STextEditUndoH	theUndoH = (STextEditUndoH) inUndoData;	(**mTextEditH).hText = (**theUndoH).textH;	::TECalText(mTextEditH);	::TESetSelect((**theUndoH).selStart, (**theUndoH).selEnd, mTextEditH);	Refresh();		if (!mSuperCommander->ObeyCommand(msg_ActionDone, theAction)) {		delete theAction;	}}voidLTextEdit::FinalizeAction(	ActionIDT	inActionID,	void		*inUndoData){	DisposeHandle((Handle) inUndoData);}STextEditUndoHLTextEdit::SaveStateForUndo(){	STextEditUndoH	theUndoH = (STextEditUndoH)									::NewHandle(sizeof(STextEditUndo));	ThrowIfMemFail_(theUndoH);		Handle	currTextH = (**mTextEditH).hText;	::HandToHand(&currTextH);	(**theUndoH).textH = currTextH;	(**theUndoH).selStart = (**mTextEditH).selStart;	(**theUndoH).selEnd = (**mTextEditH).selEnd;		return theUndoH;}voidLTextEdit::SavePlace(	LStream		*outPlace){	LView::SavePlace(outPlace);		Rect	viewRect = (**mTextEditH).viewRect;	outPlace->WriteData(&viewRect, sizeof(Rect));	Rect	destRect = (**mTextEditH).destRect;	outPlace->WriteData(&destRect, sizeof(Rect));}voidLTextEdit::RestorePlace(	LStream		*inPlace){	LView::RestorePlace(inPlace);	Rect	viewRect;	inPlace->ReadData(&viewRect, sizeof(Rect));	(**mTextEditH).viewRect = viewRect;	Rect	destRect;	inPlace->ReadData(&destRect, sizeof(Rect));	(**mTextEditH).destRect = destRect;}