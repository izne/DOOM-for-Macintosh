//	===========================================================================//	LDescTube.h			�1994 Metrowerks Inc. All rights reserved.//	===========================================================================#pragma	once#include	<LDataTube.h>#include	<UAppleEventsMgr.h>class	LDescTube			:	public	LDataTube{private:						LDescTube();	//	Parameters required;public:				//	Maintenance						LDescTube(const AEDesc &inDesc);						LDescTube(							const LTubeableItem	*inTubeableItem,							Boolean				inReqdFlavorsOnly = true);	virtual				~LDescTube();	virtual FlavorForT	GetFlavorForType(void)	{return flavorForAEDesc;}				//	Implementation	virtual void		AddFlavor(FlavorType inFlavor);	virtual	Boolean		FlavorExists(FlavorType inFlavor) const;	virtual	Int32		GetFlavorSize(FlavorType inFlavor);	virtual void *		GetFlavorData(							FlavorType	inFlavor,							void		*outFlavorData,							Int32		inOffset = 0,							Int32		inAmount = max_Int32);	virtual void		SetFlavorData(							FlavorType	inFlavor,							void		*inFlavorData,							Int32		inFlavorSize,							Int32		inOffset = 0,							Int32		inAmount = max_Int32);public:	StAEDescriptor	mDesc;protected:	StAEDescriptor	mCoercedDesc;	//	Cached, coerced descriptor};