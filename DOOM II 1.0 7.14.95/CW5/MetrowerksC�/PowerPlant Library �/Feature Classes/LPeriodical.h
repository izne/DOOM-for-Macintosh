// ===========================================================================//	LPeriodical.h					�1993 Metrowerks Inc. All rights reserved.// ===========================================================================#pragma once#include <PP_Prefix.h>#ifndef __EVENTS__#include <Events.h>#endifclass	LList;class	LPeriodical {public:					LPeriodical();	virtual			~LPeriodical();		virtual	void	StartIdling();	virtual	void	StopIdling();	static	void	DevoteTimeToIdlers(const EventRecord &inMacEvent);		virtual	void	StartRepeating();	virtual	void	StopRepeating();	static	void	DevoteTimeToRepeaters(const EventRecord &inMacEvent);		virtual	void	SpendTime(const EventRecord &inMacEvent) = 0;protected:	static	LList		*sIdlerQ;	static	LList		*sRepeaterQ;};