//	===========================================================================//	LEventHandler.cp				�1994 Metrowerks Inc. All rights reserved.//	===========================================================================#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include	<PP_Prefix.h>#include	"LEventHandler.h"#include	<LSelectHandlerView.h>#include	<UCursor.h>#include	<UEventUtils.h>#include	<Windows.h>#include	<LWindow.h>#include	<UDesktop.h>//	===========================================================================/*	NOTICE	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!		If you try to understand this code make certain you read the separate 	documentation. The documentation includes a state diagram for the state	machine implemented by this event handler.  Knowledge of that state	diagram makes this code much more understandable.*///	===========================================================================//	MaintenanceLEventHandler::LEventHandler(Int16 inMultiClickDistance){	mEvtState = mLastEvtState = evtl_unStarted;	mMultiClickDistance = inMultiClickDistance;	mMouseBounds.top = mMouseBounds.left = mMouseBounds.right =		mMouseBounds.bottom = 0;	mBDArea = mBDAreaOutside = mMouseBounds;	mBDArea.top += ScrollBar_Size / 2;	mBDArea.left += ScrollBar_Size / 2;	mBDArea.right -= ScrollBar_Size / 2;	mBDArea.bottom -= ScrollBar_Size / 2;	mBDDelay = 2;	mBDInitialDelay = 10;	mView = NULL;}LEventHandler::~LEventHandler(){}//	===========================================================================//	Required incoming hooksvoid	LEventHandler::Activate(void){	mActive = true;}void	LEventHandler::Deactivate(void){	mActive = false;}void	LEventHandler::SetMouseBounds(const Rect &inBounds){	Int32	delta;		delta = mBDAreaOutside.top - mMouseBounds.top;	mBDAreaOutside.top = inBounds.top + delta;	delta = mBDArea.top - mMouseBounds.top;	mBDArea.top = inBounds.top + delta;		delta = mBDAreaOutside.left - mMouseBounds.left;	mBDAreaOutside.left = inBounds.left + delta;	delta = mBDArea.left - mMouseBounds.left;	mBDArea.left = inBounds.left + delta;		delta = mBDAreaOutside.right - mMouseBounds.right;	mBDAreaOutside.right = inBounds.right + delta;	delta = mBDArea.right - mMouseBounds.right;	mBDArea.right = inBounds.right + delta;		delta = mBDAreaOutside.bottom - mMouseBounds.bottom;	mBDAreaOutside.bottom = inBounds.bottom + delta;	delta = mBDArea.bottom - mMouseBounds.bottom;	mBDArea.bottom = inBounds.bottom + delta;		mMouseBounds = inBounds;}void	LEventHandler::SetBoundaryRects(const Rect &inInside, const Rect &inOutside){	mBDAreaOutside = inOutside;	mBDArea = inInside;}void	LEventHandler::SetView(LSelectHandlerView *inView){	mView = inView;}Boolean	LEventHandler::DoEvent(const EventRecord &inEvent)/*	Return value indicates whether event was completely	handled and caller need take no further action.		Note:		The following should be appropriately set or		prepared before this member is called:					QuickDraw port			pane or view Prepare'd or FocusDraw'ed.		IGNORE return value for the time being.*/{/*	The complicating factor for an event dispatching routine	(DoLiteralEvent) is that Mac event fields are only valid	for given event types.  This routine stuffs the invalid fields	with the correct information -- this way the event handler has real 	information in its state instance variables.	*/	EventRecord	theEvent = inEvent;		//	Stuff missing info from event records because...	//	Most fields are undefined unless specifically related to a given event.	//	(There might still be some important stuffing missing.)	switch (theEvent.what) {			case osEvt:	//	Hail to Patrick Doane for this case...		{			Uint8	osEvtFlag = ((Uint32) theEvent.message) >> 24;			if (osEvtFlag == mouseMovedMessage) {				theEvent.what = nullEvent;			} else {				break;			}							//	Fall through!		}		case keyDown:		case keyUp:		case autoKey:				case mouseDown:		case mouseUp:		case nullEvent:		{			//	where			if ( !((theEvent.what == mouseDown) || (theEvent.what == mouseUp))) {				GetMouse(&theEvent.where);				LocalToGlobal(&theEvent.where);			}				//	modifiers			theEvent.modifiers = UEventUtils::GetModifiers();					//	fix btnState			if (theEvent.what == mouseDown) {				theEvent.modifiers |= btnState;			} else if (theEvent.what == mouseUp) {				theEvent.modifiers &= ~btnState;			} else {				if (mEvtState != evtl_unStarted) {					theEvent.modifiers &= ~btnState;					theEvent.modifiers |= mEvtRecord.modifiers & btnState;				}			}						break;		}				default:			return false;	}		return DoLiteralEvent(theEvent);}Boolean	LEventHandler::DoLiteralEvent(const EventRecord &inEvent)/*	DoLiteralEvent does the real work of DoEvent.  DoLiteralEvent	is provided as an additional entry point in the unlikely event	you want to send an exact event without the event handler	filling in the blanks that are normally present.		You will likely never need to call DoLiteralEvent directly.*/{/*	Another complicating factor for an event dispatching routine	is that a Mac event can actually be multiple events -- a mouse	move and a mouse down.*/	EventRecord	theEvent = inEvent;	Boolean		rval = false;	Point		localMouse;	Boolean		doMove = false;	Boolean		doDown = false;	Boolean		doUp = false;		switch (theEvent.what) {		case keyDown:		case keyUp:		case autoKey:		case mouseDown:		case mouseUp:		case nullEvent:			break;				default:			return false;	}		localMouse = theEvent.where;	GlobalToLocal(&localMouse);		//	Check startup state info...	if (mEvtState == evtl_unStarted) {		theEvent.modifiers = 0;		mLastEvtRecord = mEvtRecord = theEvent;		mEvtMouse = mLastEvtMouse = localMouse;		mEvtState = mLastEvtState = evtl_idle;		mEvtClickCount = 0;		CheckCursor();		return(true);	}		Try_ {		//	Boundary checking...		if (!PointInHandlerRgn(localMouse)) {			if (mInBounds)				NoteOutOfBounds(localMouse);		} else {			if (!mInBounds) {				NoteInBounds(localMouse);				doMove = true;			}		}			//	Find out what to do before it's done and the state info		//	gets messed up.		if (!EqualPt(localMouse, mEvtMouse))			doMove = true;		if ((theEvent.modifiers & btnState) && !(mEvtRecord.modifiers & btnState))			doDown = true;		if (!(theEvent.modifiers & btnState) && (mEvtRecord.modifiers & btnState))			doUp = true;						//	Always update mouse first...		if (doMove) {			UpdateLastEvt(theEvent, localMouse);			MouseMove();			rval = true;		}		//	Now a prioritized list of other event types...		if (doDown && mInBounds) {							//	note mInBounds check					//	mouseDown			UpdateLastEvt(theEvent, localMouse);			MouseDown();			rval = true;				} else if (doUp) {				//	mouseUp			UpdateLastEvt(theEvent, localMouse);			MouseUp();			rval = true;				} else if (theEvent.what == keyDown) {				//	KeyDown			UpdateLastEvt(theEvent, localMouse);			KeyDown();			rval = true;				} else if (theEvent.what == autoKey) {				//	AutoKey (KeyDown)			UpdateLastEvt(theEvent, localMouse);			KeyDown();			rval = true;				} else if (theEvent.what == keyUp) {				//	KeyUp			UpdateLastEvt(theEvent, localMouse);			KeyUp();			rval = true;				} else if (!doMove) {				//	Idle			UpdateLastEvt(theEvent, localMouse);			Idle();			rval = true;				}	} Catch_(err) {		//	Make sure exceptions leave handler in a safe state		Reset();		Throw_(err);	} EndCatch_;	return rval;}void	LEventHandler::Reset(void)/*	Subclasses may find it necessary to override and inherit this	method.		This method is called when an exception occurs during DoEvent	and allows the handler to "reset" itself to a known or usable	state.		Code executed during Reset must not throw another exception	UNLESS it is caught and handled by subclass::Reset.*/{	mEvtState = evtl_unStarted;	mBDHasEntered = false;	UCursor::Reset();}//	===========================================================================//	QueryInt16	LEventHandler::GetClickCount(void){	return mEvtClickCount;}#ifndef	abs#define	abs(a)	((a) > 0 ? (a) : -(a))#endifBoolean	LEventHandler::PointIsClose(	Point	oldPt,	Point	newPt){	Int16	dx = newPt.h - oldPt.h,			dy = newPt.v - oldPt.v;	dx = abs(dx);	dy = abs(dy);		return ( (dx <= mMultiClickDistance) && (dy <= mMultiClickDistance) );}Boolean	LEventHandler::PointInHandlerRgn(Point inWhere)/*	Returns true if the mouse is over the mMouseBounds rectangle AND	there is not a different (floating) window under the mouse location.*/{	if (PtInRect(inWhere, &mMouseBounds)) {		WindowPtr	handlerWindow,					windowOver;			handlerWindow = (WindowPtr)mView->GetMacPort();		LocalToGlobal(&inWhere);		FindWindow(inWhere, &windowOver);		if (windowOver == handlerWindow)			return true;	}		return false;}//	===========================================================================//	Overridesvoid	LEventHandler::NoteOutOfBounds(Point inWhere)/*	Returns whether "NoteOutOfBounds" handled event.		Seldom will client code need to override this.*/{	Assert_(mInBounds);		mInBounds = false;	UCursor::Reset();}void	LEventHandler::NoteInBounds(Point inWhere)/*	Returns whether "NoteInBounds" handled event.		Seldom will client code need to override this.*/{	Assert_(!mInBounds);	mInBounds = true;	UCursor::Reset();}void	LEventHandler::NoteBoundaryDrag(Point inWhere)/*	Default behavior is for autoscrolling*/{	Point	vector = {0, 0};		if (inWhere.h <= mBDArea.left)		vector.h = inWhere.h - mBDArea.left;	else if (mBDArea.right <= inWhere.h)		vector.h = inWhere.h - mBDArea.right;			if (inWhere.v <= mBDArea.top)		vector.v = inWhere.v - mBDArea.top;	else if (mBDArea.bottom <= inWhere.v)		vector.v = inWhere.v - mBDArea.bottom;		NoteAutoScrollVector(vector);}void	LEventHandler::NoteAutoScrollVector(Point inVector)/*	Default method calls AutoScrollImage of owning view.		Note:  This default method defeats the "ramping" nature of inVector.*/{	Point	simulatedWhere;		if (inVector.h < 0)		simulatedWhere.h = mMouseBounds.left + inVector.h;	if (inVector.h == 0)		simulatedWhere.h = mMouseBounds.left;	if (inVector.h > 0)		simulatedWhere.h = mMouseBounds.right + inVector.h;		if (inVector.v < 0)		simulatedWhere.v = mMouseBounds.top + inVector.v;	if (inVector.v == 0)		simulatedWhere.v = mMouseBounds.top;	if (inVector.v > 0)		simulatedWhere.v = mMouseBounds.bottom + inVector.v;		if (mView && ((inVector.h != 0) || (inVector.v != 0))) {		PreScroll(inVector);		mView->AutoScrollImage(simulatedWhere);		PostScroll(inVector);	}}void	LEventHandler::PreScroll(Point inVector){	FocusDraw();}void	LEventHandler::PostScroll(Point inVector){	FocusDraw();}void	LEventHandler::NoteNthClick(Int16 inClickCount, Point inWhere)/*	Override!		To send your code notification of an nth click.		Note:	Macintosh Human Interface Guidelines suggest that additional			clicks past those recognized by the application should be			ignored.  See the code suggestion below.*/{/*		switch (inClickCount)		 case 1:		 	...;	//	Code to handle single click		 	break;		 			 case 2:		 	...;	//	Code to handle double click		 	break;		 		 default:		 	//	DO NOTHING! rather than defaulting to single click.		 	break;	}	*/}void	LEventHandler::NoteDragStart(void)/*	Override!		To send your code notification of event handler drag start.*/{}void	LEventHandler::NoteDragStop(void)/*	Override!		To send your code notification of event handler drag completion.*/{}void	LEventHandler::NoteDragMove(void)/*	Override!		To send your code notification of event handler drag move.*/{}void	LEventHandler::CheckCursor(void){}//	===========================================================================//	ImplementationBoolean	LEventHandler::IsActive(void){	return mActive;}Int32	LEventHandler::GetEvtState(){	return(mEvtState);}void	LEventHandler::SetEvtState(Int32 inState){/*	Functionality could be driven by a 2d array of function (member?)	pointers.  However, its seems only a few of the transitions are	actually interesting.*/	Int32	oldState = GetEvtState();		if (inState != oldState) {		Try_ {			mEvtState = (EventStateT) inState;						switch(oldState) {							case evtl_idle:					switch(inState) {						case evtl_maybeDrag:							CheckCursor();							NoteNthClick(mEvtClickCount, mEvtClickCountDown);							break;					}					break;								case evtl_maybeDrag:					switch(inState) {						case evtl_dragging:							//	Prevent mousedown drift							mEvtMouse = mEvtClickCountDown;							mEvtRecord.where = mEvtMouse;							LocalToGlobal(&mEvtRecord.where);							NoteDragStart();							break;					}					break;								case evtl_dragging:					switch(inState) {						case evtl_idle:							NoteDragStop();							break;					}					break;								case evtl_clickCounting:					switch(inState) {						case evtl_maybeDrag:							CheckCursor();							NoteNthClick(mEvtClickCount, mEvtClickCountDown);							break;					}					break;			}		} Catch_(err) {			//	Is there an appropriate action?			Throw_(err);		} EndCatch_;	}}void	LEventHandler::UpdateLastEvt(EventRecord &inEvent, Point inLocalMouse){	//	A new event for the handler	mLastEvtRecord = mEvtRecord;	mLastEvtMouse = mEvtMouse;	mLastEvtState = mEvtState;	mEvtMouse = inLocalMouse;	mEvtRecord = inEvent;//	SetEvtState(?);	//	Determined by subsequent action and state info}void	LEventHandler::FocusDraw(void){	Assert_(mView);	if (mView)		mView->FocusDraw();}//	===========================================================================//	Verbsvoid	LEventHandler::MouseMove(void){//	Don't do the following check because when moving in and out of bounds//	and performing auto scrolling, it can be quite a common and plausible//	occurance.//	Assert_(!EqualPt(mEvtMouse, mLastEvtMouse));	switch(GetEvtState()) {		case evtl_idle:			//	Non dragging mouse move -- whoop de do			break;		case evtl_maybeDrag:			//	Start dragging if moved far enough			if (	(!PointIsClose(mEvtMouse, mLastEvtMouse)) ||					(!mInBounds)	) {				SetEvtState(evtl_dragging);				CheckBoundaryDrag();			} else {				mEvtMouse = mLastEvtMouse;	//	Don't degrade original location			}			break;		case evtl_dragging:			//	Continue dragging			if (mInBounds)				NoteDragMove();			else				CheckBoundaryDrag();			break;		case evtl_clickCounting:			//	Stop click couting when...			if ((TickCount() - mEvtClickCountUpTime) > GetDblTime()) {				//	Enough time elapsed				mEvtClickCount = 0;				SetEvtState(evtl_idle);			} else {				if (!PointIsClose(mEvtMouse, mLastEvtMouse)) {					//	Moved far enough					mEvtClickCount = 0;					SetEvtState(evtl_idle);				} else {					mEvtMouse = mLastEvtMouse;	//	Don't degrade original location				} 			}			break;	}}void	LEventHandler::MouseUp(void){	mEvtClickCountUpTime = mEvtRecord.when;	switch(GetEvtState()) {		case evtl_maybeDrag:			//	There should be a better location for this code but...			//	Make sure if a maybeDrag was never a drag, the view			//	is the target...			if (mView) {				LWindow	*window = LWindow::FetchWindowObject(mView->GetMacPort());				if (window) {					if (UDesktop::WindowIsSelected(window)) {						LCommander::SwitchTarget(mView);					} else {						window->SetLatentSub(mView);					}				}			}						//	A simple, non dragging click.  Notify and start click counting			mEvtClickCount = 1;			SetEvtState(evtl_clickCounting);						break;					case evtl_dragging:			//	Drag done.  Notify			SetEvtState(evtl_idle);			break;	}}void	LEventHandler::MouseDown(void){	mEvtClickCountDown = mEvtMouse;		switch(GetEvtState()) {		case evtl_idle:			//	Possible selection and drag			mEvtClickCount = 1;			SetEvtState(evtl_maybeDrag);			break;		case evtl_clickCounting:			//	Possible nth click and drag			if ((mEvtRecord.when - mEvtClickCountUpTime) <= GetDblTime()) {				mEvtClickCount++;			} else {				mEvtClickCount = 1;			}			SetEvtState(evtl_maybeDrag);			break;	}}void	LEventHandler::KeyDown(void){	switch(GetEvtState()) {		case evtl_idle:		case evtl_maybeDrag:		case evtl_dragging:		case evtl_clickCounting:			break;	}}void	LEventHandler::KeyUp(void){	switch(GetEvtState()) {		case evtl_idle:		case evtl_maybeDrag:		case evtl_dragging:		case evtl_clickCounting:			break;	}}void	LEventHandler::Idle(void){	CheckBoundaryDrag();}Boolean	LEventHandler::PtInBoundaryArea(Point inWhere){	return !PtInRect(inWhere, &mBDArea);}void	LEventHandler::CheckBoundaryDrag(void){	switch (GetEvtState()) {		case evtl_dragging:			//	IsActive() test required to prevent views in non			//	front-most windows from auto scrolling.  Allowing such			//	would me a Mac HI violation.			if (PtInBoundaryArea(mEvtMouse) && mView->IsActive()) {				Int32	tc = TickCount();				Boolean	doNotify = false;								if (!mBDHasEntered) {					mBDHasEntered = true;					mBDFirstTime = PtInRect(mEvtMouse, &mBDAreaOutside);					mLastBDTime = tc;				} else {					if (!PtInRect(mEvtMouse, &mBDAreaOutside))						mBDFirstTime = false;				}								if (mBDFirstTime) {					doNotify = (tc - mLastBDTime) > mBDInitialDelay;				} else {					doNotify = (tc - mLastBDTime) > mBDDelay;				}								if (doNotify) {					NoteBoundaryDrag(mEvtMouse);					mBDFirstTime = false;					mLastBDTime = tc;				}			} else {				mBDHasEntered = false;			}			break;	}}