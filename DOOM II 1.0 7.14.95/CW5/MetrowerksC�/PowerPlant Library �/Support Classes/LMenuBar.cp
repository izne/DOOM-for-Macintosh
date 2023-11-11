// ===========================================================================//	LMenuBar.cp				 		�1993 Metrowerks Inc. All rights reserved.// ===========================================================================////	Manages a Mac menu bar. Contains a list a Menu objects.////	Dependencies://		LMenu////	Resources://		'MBAR'		Standard Mac resource for a Menu Bar////	�� Techniques////	� Adding/Removing a Menu depending on runtime conditions//		During initialization, create a Menu object//		Store pointer to Menu object in an appropriate place//		When you want to add the Menu to the MenuBar://			LMenuBar::GetCurrentMenuBar()->InstallMenu(myMenu, beforeID);//			where "myMenu" is the pointer to the Menu object//				"beforeID" is the ID of the Menu before which to put//				this Menu (use InstallMenu_AtEnd to put the Menu last or//				the Toolbox constant hierMenu for a hierarchical or popup)//		When you want to remove the Menu from the MenuBar://			LMenuBar::GetCurrentMenuBar()->RemoveMenu(myMenu);////	� Toggling a Menu item//		For a menu item that toggles between two states (such as Show Toolbar//		and Hide Toolbar), you can change the command as well as the text//		of the menu item.////		The follow code fragment assumes that you have declared oldCommand//		and newCommand as CommandT variables or constants and newName as//		as some kind of string.//										//		ResIDT		theID;//		MenuHandle	theMenuH;//		Int16		theItem;			// Locate oldCommand//		FindMenuItem(oldCommand, theID, theMenuH, theItem);//		if (theItem != 0) {				// Replace with newCommand//			LMenu	*theMenu = LMenuBar::GetCurrentMenuBar->FetchMenu(theID);//			theMenu->SetCommand(theItem, newCommand);//			SetMenuItemText(theMenuH, theItem, newName);//		}		#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include <LMenuBar.h>#include <LMenu.h>#include <UMemoryMgr.h>#include <PP_Resources.h>#include <PP_Messages.h>#ifndef __RESOURCES__#include <Resources.h>#endif#ifndef __MEMORY__#include <Memory.h>#endif#ifndef __TOOLUTILS__#include <ToolUtils.h>#endif// === Static Members ===LMenuBar*	LMenuBar::sMenuBar;// ---------------------------------------------------------------------------//		� LMenuBar// ---------------------------------------------------------------------------//	Construct Menu Bar from a MBAR resourceLMenuBar::LMenuBar(	ResIDT	inMBARid){	StResource	theMBAR('MBAR', inMBARid);	::HLockHi(theMBAR.mResourceH);		sMenuBar = this;	mMenuListHead = nil;									// Install each menu in the MBAR resource	Int16	*menuIDP = (Int16 *) *theMBAR.mResourceH;	Int16	numMenus = *menuIDP++;	for (Int16 i = 1; i <= numMenus; ++i) {		InstallMenu(new LMenu(*menuIDP++), InstallMenu_AtEnd);	}									// Populate the Apple Menu	MenuHandle	macAppleMenuH = GetMenuHandle(MENU_Apple);	if (macAppleMenuH != nil) {		::AppendResMenu(macAppleMenuH, 'DRVR');	}	DrawMenuBar();}// ---------------------------------------------------------------------------//		� ~LMenuBar// ---------------------------------------------------------------------------//	DestructorLMenuBar::~LMenuBar(){	LMenu	*theMenu = nil;	while (FindNextMenu(theMenu)) {		RemoveMenu(theMenu);	}}// ---------------------------------------------------------------------------//		� MenuCommandSelection// ---------------------------------------------------------------------------//	Handle menu selection with the Mouse and return the command number for//	the item chosen////	When to Override://		To implement alternative menu selection behavior.//		To change menu commands based on what modifier keys are downlongLMenuBar::MenuCommandSelection(	const EventRecord	&inMouseEvent) const{	long	menuChoice =  MenuSelect(inMouseEvent.where);		long	menuCmd = cmd_Nothing;	if (HiWord(menuChoice) != 0) {		menuCmd = FindCommand(HiWord(menuChoice), LoWord(menuChoice));	}		return menuCmd;}// ---------------------------------------------------------------------------//		� CouldBeKeyCommand// ---------------------------------------------------------------------------//	Return whether the keystoke could be a key equivalent for a menu command////	When to Override://		To implement keyboard equivalents that use modifier keys other than//		just the command key. This function returns true if the command//		key is down.BooleanLMenuBar::CouldBeKeyCommand(	const EventRecord	&inKeyEvent) const{	return (inKeyEvent.modifiers & cmdKey) != 0;}// ---------------------------------------------------------------------------//		� FindKeyCommand// ---------------------------------------------------------------------------//	Return the Command number corresponding to a keystroke//		Returns cmd_Nothing if the keystroke is not a menu equivalent////	Usage Note: Call this function when CouldBeKeyCommand() is true.////	When to Override://		To implement keyboard equivalents that use modifier keys other than//		just the command key. This function calls the Toolbox routine//		MenuKey to find the associated menu item, if any. Override this//		function (as well as CouldBeKeyCommand) to implement key equivalents//		that use other modifier keys, such as Option, Shift, or Control.longLMenuBar::FindKeyCommand(	const EventRecord	&inKeyEvent) const{	CommandT	theCommand = cmd_Nothing;		char	theChar = inKeyEvent.message & charCodeMask;	long	menuChoice = MenuKey(theChar);	if (HiWord(menuChoice) != 0) {		theCommand = FindCommand(HiWord(menuChoice), LoWord(menuChoice));	}		return theCommand;}// ---------------------------------------------------------------------------//		� FindCommand// ---------------------------------------------------------------------------//	Return the Command number corresponding to a Menu (ID, item) pairlongLMenuBar::FindCommand(	ResIDT	inMENUid,	Int16	inItem) const{									// Start with synthetic command number	CommandT	theCommand = -(((Int32)inMENUid) << 16) - inItem;	LMenu	*theMenu = mMenuListHead;	while (theMenu) {				// Search all installed Menus		if (inMENUid == theMenu->GetMenuID()) {			theCommand = theMenu->CommandFromIndex(inItem);			break;		}		theMenu = theMenu->GetNextMenu();	}		return theCommand;}// ---------------------------------------------------------------------------//		� FindMenuItem// ---------------------------------------------------------------------------//	Passes back the MENU id, MenuHandle, and item number corresponding to a//	Command number////	If the Command is not associated with any item in the MenuBar,//		outMENUid is 0, outMenuHandle is nil, and outItem is 0voidLMenuBar::FindMenuItem(	CommandT	inCommand,	ResIDT		&outMENUid,	MenuHandle	&outMenuHandle,	Int16		&outItem){	Int16	theItem = 0;			// Search menu list for the command	LMenu	*theMenu = mMenuListHead;	while (theMenu) {		theItem = theMenu->IndexFromCommand(inCommand);		if (theItem != 0) {			break;		}		theMenu = theMenu->GetNextMenu();	}		if (theItem != 0) {				// Command found, get ID and MenuHandle		outMENUid = theMenu->GetMenuID();		outMenuHandle = theMenu->GetMacMenuH();			} else {						// Command not found		outMENUid = 0;		outMenuHandle = nil;	}	outItem = theItem;}// ---------------------------------------------------------------------------//		� FindNextCommand// ---------------------------------------------------------------------------//	Passes back the next command in the MenuBar////	On entry,//		ioIndex, ioMenuHandle, and ioMenu specify an item in a Menu//		ioMenuHandle of nil means to start at the beginning, so the//		next command will be the first one in the MenuBar//	On exit,//		ioIndex, ioMenuHandle, and ioMenu specify the next item in//		the MenuBar. If the next item is in the same menu, ioIndex//		is incremented by one and ioMenuHandle and ioMenu are unchanged.//		If the next item is in another menu, ioIndex is one, and//		ioMenuHandle and ioMenu refer to the next menu.//		outCommand is the command number associated with that item////	Returns true if the next command exists//	Returns false if there is no next command////	Use this function to iterate over all commands in the MenuBar:////		LMenuBar	*theMenuBar = LMenuBar::GetCurrentMenuBar();//		Int16		menuItem;//		MenuHandle	macMenuH = nil;//		LMenu		*theMenu;//		CommandT	theCommand;////		while (theMenuBar->FindNextCommand(menuItem, macMenuH,//											theMenu, theCommand)) {//			// Do something with theCommand//		}									BooleanLMenuBar::FindNextCommand(	Int16		&ioIndex,	MenuHandle	&ioMenuHandle,	LMenu*		&ioMenu,	CommandT	&outCommand){	if (ioMenuHandle == nil) {		// Special case: first time		ioIndex = 0;				//   Start at beginning of our Menu list		ioMenu = mMenuListHead;		if (ioMenu == nil) {			return false;			// Quick exit if there are no Menus		}	}			Boolean		cmdFound;	do {									// Get MenuHandle for current Menu		ioMenuHandle = ioMenu->GetMacMenuH();									// Search in current Menu		cmdFound = ioMenu->FindNextCommand(ioIndex, outCommand);				if (!cmdFound) {			// No next command in current Menu			ioIndex = 0;			// Move to start of next Menu			ioMenu = ioMenu->GetNextMenu();		}									// End search upon finding next command									//   or reaching end of menu list 	} while (!cmdFound && (ioMenu != nil));		return cmdFound;}// ---------------------------------------------------------------------------//		� InstallMenu// ---------------------------------------------------------------------------//	Install a Menu object in the MenuBarvoidLMenuBar::InstallMenu(	LMenu	*inMenuToInstall,	ResIDT	inBeforeMENUid){		// It's possible to add a Menu twice to the Mac menu list--		// once as a regular menu and once as a submenu (hierarchical		// or popup menu). However, we only need one copy of the		// menu in our menu list.			if (!inMenuToInstall->IsInstalled()) {									// Add it to our singly-linked list		inMenuToInstall->SetNextMenu(mMenuListHead);		mMenuListHead = inMenuToInstall;		inMenuToInstall->SetInstalled(true);	}										// Add it to the Mac MenuBar	MenuHandle	macMenuH = inMenuToInstall->GetMacMenuH();	::InsertMenu(macMenuH, inBeforeMENUid);											// Search menu items for submenus									//   and install them also	Int16	itemCount = ::CountMItems(macMenuH);	for (Int16 item = 1; item <= itemCount; item++) {		Int16	itemCmd;		::GetItemCmd(macMenuH, item, &itemCmd);		if (itemCmd == hMenuCmd) {			Int16	subMenuID;		// Submenu found. Get its ID, create									//   a new Menu object, and recursively									//   call this function to install it.									//   Recursion means that sub-submenus									//   get installed too.			::GetItemMark(macMenuH, item, &subMenuID);			InstallMenu(new LMenu(subMenuID), hierMenu);		}	}	if (inBeforeMENUid != hierMenu) {									// Not a submenu, so force a redraw		::InvalMenuBar();			//   of the MenuBar	}}// ---------------------------------------------------------------------------//		� RemoveMenu// ---------------------------------------------------------------------------//	Remove a Menu object from the MenuBarvoidLMenuBar::RemoveMenu(	LMenu	*inMenuToRemove){									// Search for Menu in our list	LMenu	*theMenu = mMenuListHead;	LMenu	*prevMenu = nil;	while ((theMenu != nil) && (theMenu != inMenuToRemove)) {		prevMenu = theMenu;		theMenu = theMenu->GetNextMenu();	}		if (theMenu != nil) {			// Menu is in our list									// Remove it from our singly-linked list		if (prevMenu == nil) {			mMenuListHead = inMenuToRemove->GetNextMenu();		} else {			prevMenu->SetNextMenu(inMenuToRemove->GetNextMenu());		}		inMenuToRemove->SetNextMenu(nil);		inMenuToRemove->SetInstalled(false);																		// Remove it from the Mac MenuBar		::DeleteMenu(inMenuToRemove->GetMenuID());		::InvalMenuBar();				// Force redraw of MenuBar									// ??? don't redraw if a submenu ???	}}// ---------------------------------------------------------------------------//		� FetchMenu// ---------------------------------------------------------------------------//	Return the Menu object for the specified MENU resource ID////	Returns nil if there is no such Menu objectLMenu*LMenuBar::FetchMenu(	ResIDT	inMENUid) const{	LMenu	*theMenu = mMenuListHead;									// Search menu list until reaching the									//   end or finding a match	while ((theMenu != nil) && (theMenu->mMENUid != inMENUid)) {		theMenu = theMenu->GetNextMenu();	}	return theMenu;}// ---------------------------------------------------------------------------//		� FindNextMenu// ---------------------------------------------------------------------------//	Pass back the next Menu in a MenuBar////	On entry, ioMenu is a pointer to a Menu object//		Pass nil to get the first Menu//	On exit, ioMenu is a pointer to the next Menu in the MenuBar//		ioMenu is nil if there is no next menu////	Returns false if there is no next menu////	Use this function to loop thru all the Menus in a MenuBar://		LMenuBar	*theMenuBar = LMenuBar::GetCurrentMenuBar();//		LMenu	*theMenu = nil;//		while (theMenuBar->FindNextMenu(theMenu) {//			// ... do something with theMenu//		}BooleanLMenuBar::FindNextMenu(	LMenu*	&ioMenu) const{	if (ioMenu == nil) {		ioMenu = mMenuListHead;			} else {		ioMenu = ioMenu->GetNextMenu();	}		return (ioMenu != nil);}// ---------------------------------------------------------------------------//		� GetCurrentMenuBar [static]// ---------------------------------------------------------------------------//	Return a pointer to the current MenuBar objectLMenuBar*LMenuBar::GetCurrentMenuBar()		// +++ possible inline{	return sMenuBar;}