//	===========================================================================//	LWindowAEOM.h					�1994 Metrowerks Inc. All rights reserved.//	===========================================================================//	This is a fudge class mainly to get a demo going...#pragma	once#include	<LWindow.h>#include	<LCoreAEOM.h>class	LWindowAEOM ://			public virtual LCoreAEOM,			public LCoreAEOM,			public LWindow{private:							LWindowAEOM();	//	Must use parameterspublic:	static LWindowAEOM*		CreateWindowAEOMStream(LStream *inStream);	static LWindowAEOM*		CreateWindowAEOM(ResIDT inWindowID, LCommander *inSuperCommander);							LWindowAEOM(ResIDT inWINDid, Uint32 inAttributes,										LCommander *inSuperCommander);							LWindowAEOM(LStream *inStream);	virtual					~LWindowAEOM();	virtual void			FixWindowName(void);		virtual	Boolean			IsLazy(void) const;						//	If LWindow had declared LModelObject as a virtual						//	base class the following may not have been required.	DescType				GetModelKind() const;	virtual void			GetAEProperty(DescType inProperty,										  const AEDesc& inRequestedType,										  AEDesc& outPropertyDesc) const;	virtual void			HandleAppleEvent(									const AppleEvent	&inAppleEvent,									AppleEvent			&outAEReply,									AEDesc				&outResult,									Int32				inAENumber);	virtual LModelProperty*	GetModelProperty(DescType inProperty);		virtual void			Finalize(void);	virtual void			AddSubModel(LModelObject *inSubModel);	virtual void			RemoveSubModel(LModelObject *inSubModel);	virtual Int32			CountSubModels(DescType inModelID) const;	virtual void			GetSubModelByPosition(									DescType		inModelID,									Int32			inPosition,									AEDesc			&outToken) const;	virtual Int32			GetPositionOfSubModel(									DescType			inModelID,									const LModelObject	*inSubModel) const;protected:	virtual void			MakeSelfSpecifier(									AEDesc		&inSuperSpecifier,									AEDesc		&outSelfSpecifier) const;};