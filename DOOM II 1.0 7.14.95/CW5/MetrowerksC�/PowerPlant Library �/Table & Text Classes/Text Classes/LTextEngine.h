#pragma	once//	===========================================================================//	LTextEngine.h				�1994 Metrowerks Inc. All rights reserved.//	===========================================================================#include	<LBroadcaster.h>#include	<LRecalcAccumulator.h>#include	<URange32.h>typedef	Range32T	TextRangeT;#include	<TextEdit.h>#include	<Drag.h>#include	<LView.h>#include	<LDynamicBuffer.h>#include	<UAppleEventsMgr.h>#include	<UTextTraits.h>class	LDataTube;class	LNTextEdit;class	LAETypingAction;class	LStyleSet;enum {	msg_ModelChanged = 10000,	msg_ModelScopeChanged,	msg_ViewSelectionChanged,	msg_ViewParmsChanged,	msg_ViewFocusNow};/*	For a text pane...		msg_ModelChanged				(was ...TextHasChanged)				some characters have changed and the text is now "dirty"		msg_ModelScopeChanged			(was ...TextImageSizeChanged)				The size of the text image has changed.		msg_ViewSelectionChanged		(was ...TextSelectionChanged)				The text selection has changed.		msg_ViewParmsChanged			(was ...TextViewChanged)				The text view has been auto scrolled.*/const	RecalcFlagsT	recalc_FormatAllText = recalc_LRecalcAccumulator_Next;const	RecalcFlagsT	recalc_LTextEngine_Next = recalc_FormatAllText << 1;class	LTextEngine			:	public LBroadcaster			,	public LRecalcAccumulator{public:						LTextEngine();	virtual				~LTextEngine();				//	Text changed status (reset w/ SetTextChange(false))	virtual	Boolean		GetTextChanged(void) ;	virtual	void		SetTextChanged(Boolean inChanged = true);					//	Linkage	virtual void		Focus(void);	virtual void		Activate(void);	virtual void		Deactivate(void);	virtual void		Idle(void);	virtual Boolean		DoKey(EventRecord &inEventRecord);	virtual void		Click(SPoint32 inWhere, EventRecord &inEventRecord, Int16 inClickCount = 1);				//	Appearance	virtual	void		SetAttributes(Uint16 inAttributes);	virtual Uint16		GetAttributes(void) const;	virtual	void		GetTextTraits(TextTraitsRecord *outTraits) const;	virtual	void		SetTextTraits(const TextTraitsRecord &inTraits);	virtual void		SetTextMargins(const Rect &inMargins);	virtual void		GetTextMargins(Rect *outMargins) const;	virtual void		GetImageSize(SDimension32 *outSize) const;	virtual void		SetImageSize(const SDimension32 &inSize);	virtual void		GetViewRect(								SPoint32		*outLocation,								SDimension32	*outSize) const;	virtual void		SetViewRect(								const SPoint32		&inLocation,								const SDimension32	&inSize);	virtual void		ScrollView(								Int32			inLeftDelta,								Int32			inTopDelta,								Boolean			inRefresh);	virtual	void		ScrollToSelection(Boolean inBottom = true);	virtual void		DrawArea(								const SPoint32		&inLocation,								const SDimension32	&inSize);				//	Important ranges	virtual void		GetTotalRange(TextRangeT *outRange) const;	virtual void		CropRange(TextRangeT *ioRange) const;	virtual void		SetSelectionRange(const TextRangeT &inRange);	virtual void		GetSelectionRange(TextRangeT *inRange) const;static const TextRangeT	sTextAll;		//	Range indicating all textstatic const TextRangeT	sTextEnd;		//	Range indicating the end of textstatic const TextRangeT	sTextUndefined;	//	Undefined range				//	Character query	virtual Int32		ThisCharSize(Int32 inInsertionLocation) const;	virtual Int32		NextCharSize(Int32 inInsertionLocation) const;	virtual Int32		PrevCharSize(Int32 inInsertionLocation) const;	virtual Int16		ThisChar(Int32 inInsertionLocation) const;//	virtual Handle		GetTextHandle(void);					//	Support for data tubes...	virtual void		AddFlavorTypesTo(							const TextRangeT	&inRange,							LDataTube			*inTube);	virtual Boolean		SendFlavorTo(							FlavorType			inFlavor,							const TextRangeT	&inRange,							LDataTube			*inTube);	virtual	FlavorType	PickFlavorFrom(							const TextRangeT	&inRange,							const LDataTube		*inTube);	virtual void		ReceiveDataFrom(							const TextRangeT	&inRange,							LDataTube			*inTube);				//	Text query and editing	virtual void		TextMove(const TextRangeT &inRangeA, const TextRangeT &inRangeB);	virtual void		TextDelete(const TextRangeT &inRange);						//	To avoid using tubes...						//		Note:	in the future, tubes wont lose style information						//				and the following are implemented with tubes anyway.						//				So... use the tube functions above unless really						//				working with plain text.	virtual void		TextGetThruPtr(const TextRangeT &inRange, Ptr outBuffer);	virtual void		TextGetThruHandle(const TextRangeT &inRange, Handle outTextH);	virtual void		TextReplaceByPtr(const TextRangeT &inRange, const Ptr inSrcText, Int32 inSrcLength);	virtual void		TextReplaceByHandle(const TextRangeT &inRange, const Handle inNewTextH);					//	Presentation	virtual void		Hilite(TextRangeT &inRange);	virtual void		UnHilite(TextRangeT &inRange);	virtual void		LatentHilite(TextRangeT &inRange);	virtual void		UnLatentHilite(TextRangeT &inRange);	virtual void		DrawDropHilite(TextRangeT &inRange, Boolean inTrailingEdge = true);	virtual void		UnDrawDropHilite(TextRangeT &inRange, Boolean inTrailingEdge = true);					//	Presentation query	virtual	Int32		GetRangeHeight(const TextRangeT &inRange) const;	virtual Int32		GetRangeWidth(const TextRangeT &inRange) const;	virtual	Boolean		PointInRange(SPoint32 inWhere, const TextRangeT &inRange) const;	virtual	Boolean		Range2Image( 								TextRangeT			&inRange,								Boolean				inLeadingEdge,								SPoint32			*outWhere) const;	virtual Boolean		Image2Range(								SPoint32			inLocation,								Boolean				*outLeadingEdge,								TextRangeT			*outRange) const;	virtual	void		FindDropRange(SPoint32 inWhere, TextRangeT *outRange);				//	Typing undo support	virtual	void		RedoTyping(void);	virtual void		UndoTyping(void);	virtual	Boolean		CanRedoTyping(void);	virtual Boolean		CanUndoTyping(void);	virtual Boolean		TypingBuffered(void);	virtual void		RecordTyping(void);	virtual void		GetPreTypingSelection(AEDesc *outDesc);					//	Indexing methods to support text parts (req'd for the AEOM)	virtual	Int32		CountParts(								const TextRangeT	&inQueryRange,								DescType			inPartType) const;	virtual	DescType	FindNthPart(								const TextRangeT	&inQueryRange,								DescType			inPartType,								Int32				inPartIndex,								TextRangeT			*outPartRange) const;	virtual	DescType	FindSubRangePartIndices(								const TextRangeT	&inQueryRange,								const TextRangeT	&inSubRange,								DescType			inSubRangeType,								Int32				*outFirstIndex,								Int32				*outLastIndex) const;	virtual Boolean		DoClickLoop(void);					//	Styles				//		Note that these are presently un-implemented.	virtual	Int32		CountStyleSets(const TextRangeT &inRange) const;	virtual	LStyleSet *	GetStyleSet(const TextRangeT &inRange, Int32 inIndex = 1) const;	virtual void		SetStyleSet(const TextRangeT &inRange, LStyleSet *inNewStyle);	virtual	void		StyleSetChanged(LStyleSet *inChangedStyle);	virtual	LStyleSet *	GetNewStyleSet(LStyleSet *inOriginal = NULL);	virtual	Int32		GetStyleLength(Int32 inCharacterOffset) const;					//	Miscellaneous	virtual void		SetView(LView *inView);					//	Archaic	virtual Boolean		DoKey(char inKey);//	----------------------------------------------------------------------//	Implementation details...protected:	LNTextEdit			*mView;	Uint16				mAttributes;	//	textAttr_MultiStyle, textAttr_Editable, 										//	textAttr_Selectable, textAttr_WordWrap	TextTraitsRecord	mTraits;	Boolean				mIsChanged;	Rect				mMargins;					//	Hiliting implementation uglyness				//	a real text engine wouldn't need these.	TextRangeT			mHiliteRange;	RgnHandle			mHiliteRgn;	virtual RgnHandle	GetHiliteRgn(void);	virtual void		ClearHiliteRgn(void);					//	General implementation help	virtual void		CheckImageSize(void);	SDimension32		mImageSize;	virtual void		AdjustSelection(							const TextRangeT 	&inStartSelection,							const TextRangeT	&inStartRange,							Int32				inRangeDelta);	virtual Boolean		SpaceForBytes(Int32 inAdditionalBytes);	virtual	void		Refresh(RecalcFlagsT inFlags = recalc_WindowUpdate);	public:	static LTextEngine	*sActiveEngine;protected:				//	Text part implementation support	virtual	Int32		CountParas(const TextRangeT &inQueryRange) const;	virtual	Int32		CountLines(const TextRangeT &inQueryRange) const;	virtual	Int32		CountWords(const TextRangeT &inQueryRange) const;	virtual	Int32		CountChars(const TextRangeT &inQueryRange) const;	virtual	DescType	FindNthPara(								const TextRangeT	&inQueryRange,								Int32				inPartIndex,								TextRangeT			*outRange) const;	virtual	DescType	FindNthLine(								const TextRangeT	&inQueryRange,								Int32				inPartIndex,								TextRangeT			*outRange) const;	virtual	DescType	FindNthWord(								const TextRangeT	&inQueryRange,								Int32				inPartIndex,								TextRangeT			*outRange) const;	virtual	DescType	FindNthChar(								const TextRangeT	&inQueryRange,								Int32				inPartIndex,								TextRangeT			*outRange) const;	virtual	DescType	FindParaSubRangeIndices(								const TextRangeT	&inQueryRange,								const TextRangeT	&inSubRange,								Int32				*inIndex1,								Int32				*inIndex2) const;	virtual	DescType	FindLineSubRangeIndices(								const TextRangeT	&inQueryRange,								const TextRangeT	&inSubRange,								Int32				*inIndex1,								Int32				*inIndex2) const;	virtual	DescType	FindWordSubRangeIndices(								const TextRangeT	&inQueryRange,								const TextRangeT	&inSubRange,								Int32				*inIndex1,								Int32				*inIndex2) const;	virtual	DescType	FindCharSubRangeIndices(								const TextRangeT	&inQueryRange,								const TextRangeT	&inSubRange,								Int32				*inIndex1,								Int32				*inIndex2) const;	virtual	Int32		CountParts_(								const TextRangeT	inQueryRange,								DescType			inPartType) const;	virtual	DescType	FindNthPart_(								const TextRangeT	&inQueryRange,								Int32				inPartIndex,								DescType			inPartType,								TextRangeT			*outRange) const;	virtual	DescType	FindPartSubRangeIndices(								const TextRangeT	&inQueryRange,								const TextRangeT	&inSubRange,								DescType			inSubPartType,								Int32				*inIndex1,								Int32				*inIndex2) const;	virtual	DescType	FindPart(								const TextRangeT	&inQueryRange,								TextRangeT			*ioSubRange,								DescType			inSubPartType,								Int32				*outNextOffset) const;	virtual	DescType	FindPara(								const TextRangeT	&inQueryRange,								TextRangeT			*ioRange,								Int32				*outEndOffset) const;	virtual	DescType	FindLine(								const TextRangeT	&inQueryRange,								TextRangeT			*ioRange,								Int32				*outEndOffset) const;	virtual	DescType	FindWord(								const TextRangeT	&inQueryRange,								TextRangeT			*ioRange,								Int32				*outEndOffset) const;				//	Typing undo implementation support	Boolean				mTypingIsUndone;	TextRangeT			mLastRange,			//	Used to detect a new typing sequence						mNewTextRange;	static	Int32		sExpectedLength;		StAEDescriptor		mInsertSuperSpec;	//	Super specifier of selection	StAEDescriptor		mInsertTextHere;	//	Object specifier for new text											//		often an insertion loc with											//		a position of kAEReplace	Int32				mTypeCharIndex1,	//	NOT byte offsets but character offsets.						mTypeCharIndex2;	Boolean				mUseIndexes;		//	User has fwd|back deleted outside											//		original selection -- use & maintain											//		mTypeCharIndex_'s.	TextRangeT			mOrigInsertRange;	//	Original insertion range.		LAETypingAction		*mTypingAction;	LDynamicBuffer		mRemovedText;		//	Must be augmented by another storage facility											//	when undo of styled text typing is supported.	LDynamicBuffer		mNewCharacters;		//	Never needs to be styled --											//		style changes would constitute another											//		undoable action	virtual	void		DoKeyPre(const EventRecord &inEventRecord);	virtual	void		DoKeyPost(const EventRecord &inEventRecord);	virtual	void		KeyStreamCheck(void);	virtual void		KeyStreamPurge(void);	virtual	void		KeyStreamRemember(const TextRangeT &inRange);	virtual void		KeyStreamRememberAppend(const TextRangeT &inRange);	virtual void		KeyStreamRememberPrepend(const TextRangeT &inRange);	virtual void		KeyStreamRememberBackspace(void);	virtual void		KeyStreamRememberRubout(void);	virtual void		KeyStreamRememberClear(void);};