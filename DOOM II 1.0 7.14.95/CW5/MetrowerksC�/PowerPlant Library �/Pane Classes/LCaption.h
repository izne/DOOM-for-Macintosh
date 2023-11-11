// ===========================================================================//	LCaption.h						�1993 Metrowerks Inc. All rights reserved.// ===========================================================================#pragma once#include <LPane.h>class	LCaption : public LPane {public:	enum { class_ID = 'capt' };	static LCaption*	CreateCaptionStream(LStream *inStream);						LCaption();						LCaption(const LCaption &inCaption);						LCaption(const SPaneInfo &inPaneInfo, Str255 inString,										ResIDT inTextTraitsID);						LCaption(LStream *inStream);	virtual				~LCaption();		virtual Int32		GetValue() const;	virtual void		SetValue(Int32 inValue);		virtual StringPtr	GetDescriptor(Str255 outDescriptor) const;	virtual void		SetDescriptor(ConstStr255Param inDescriptor);		ResIDT				GetTextTraitsID() const;	void				SetTextTraitsID(ResIDT inTxtrID);protected:	Str255				mText;	ResIDT				mTxtrID;	virtual void		DrawSelf();};