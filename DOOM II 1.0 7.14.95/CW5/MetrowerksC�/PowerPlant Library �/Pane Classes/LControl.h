// ===========================================================================//	LControl.h						�1993 Metrowerks Inc. All rights reserved.// ===========================================================================#pragma once#include <LPane.h>#include <LBroadcaster.h>class	LCommander;#if defined(powerc) || defined (__powerc)#pragma options align=mac68k#endiftypedef struct	SControlInfo {	MessageT	valueMessage;	Int32		value;	Int32		minValue;	Int32		maxValue;} SControlInfo;#if defined(powerc) || defined(__powerc)#pragma options align=reset#endifclass	LControl : public LPane,				   public LBroadcaster {public:						LControl();						LControl(const LControl &inOriginal);						LControl(const SPaneInfo &inPaneInfo,							MessageT inValueMessage, Int32 inValue,							Int32 inMinValue, Int32 inMaxValue);						LControl(LStream *inStream);	virtual				~LControl();		MessageT			GetValueMessage() const;	void				SetValueMessage(MessageT inValueMessage);		virtual Int32		GetValue() const;	virtual void		SetValue(Int32 inValue);	virtual void		IncrementValue(Int32 inIncrement);		Int32				GetMinValue() const;	virtual void		SetMinValue(Int32 inMinValue);	Int32				GetMaxValue() const;	virtual void		SetMaxValue(Int32 inMaxValue);		virtual void		SimulateHotSpotClick(Int16 inHotSpot);	protected:	MessageT	mValueMessage;	Int32		mValue;	Int32		mMinValue;	Int32		mMaxValue;	virtual Int16		FindHotSpot(Point inPoint);	virtual Boolean		PointInHotSpot(Point inPoint, Int16 inHotSpot);	virtual Boolean		TrackHotSpot(Int16 inHotSpot, Point inPoint);	virtual void		HotSpotAction(Int16 inHotSpot, Boolean inCurrInside, Boolean inPrevInside);	virtual void		HotSpotResult(Int16 inHotSpot);		virtual void		BroadcastValueMessage();		virtual void		ClickSelf(const SMouseDownEvent &inMouseDown);};