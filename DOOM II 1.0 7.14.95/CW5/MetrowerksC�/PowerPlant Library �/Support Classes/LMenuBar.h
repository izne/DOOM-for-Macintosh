// ===========================================================================//	LMenuBar.h						�1993 Metrowerks Inc. All rights reserved.// ===========================================================================#pragma once#include <PP_Prefix.h>#ifndef __EVENTS__#include <Events.h>#endif#ifndef __MENUS__#include <Menus.h>#endifclass	LMenu;const ResIDT	InstallMenu_AtEnd	= 0;class	LMenuBar {public:						LMenuBar(ResIDT inMBARid);	virtual 			~LMenuBar();		virtual CommandT	MenuCommandSelection(const EventRecord &inMouseEvent)							const;	virtual Boolean		CouldBeKeyCommand(const EventRecord &inKeyEvent)							const;	virtual CommandT	FindKeyCommand(const EventRecord &inKeyEvent) const;				CommandT	FindCommand(ResIDT inMENUid, Int16 inItem) const;			void		FindMenuItem(CommandT inCommand, ResIDT &outMENUid,								MenuHandle &outMenuHandle, Int16 &outItem);			Boolean		FindNextCommand(Int16 &ioIndex,										MenuHandle &ioMenuHandle,										LMenu* &ioMenu, CommandT &outCommand);						void		InstallMenu(LMenu *inMenu, ResIDT inBeforeMENUid);			void		RemoveMenu(LMenu *inMenu);						LMenu*		FetchMenu(ResIDT inMENUid) const;			Boolean		FindNextMenu(LMenu* &ioMenu) const;				static LMenuBar*	GetCurrentMenuBar();protected:	static	LMenuBar	*sMenuBar;		LMenu	*mMenuListHead;};