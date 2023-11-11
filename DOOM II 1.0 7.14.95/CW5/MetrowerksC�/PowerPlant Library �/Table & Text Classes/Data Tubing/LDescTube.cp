//	===========================================================================//	LDescTube.cp				�1994 Metrowerks Inc. All rights reserved.//	===========================================================================//	For routine descriptions see LDataTube.cp.#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include	<PP_Prefix.h>#include	"LDescTube.h"#include	<UMemoryMgr.h>#include	<Scrap.h>#include	<Errors.h>#include	<LDynamicArray.h>#include	<LTubeableItem.h>#include	<URange32.h>LDescTube::LDescTube(){	Assert_(false);		//	Parameters required}LDescTube::LDescTube(const AEDesc &inDesc)/*	Create a tube for existing data on the Desc.*/{	OSErr	err;		err = AEDuplicateDesc(&inDesc, &mDesc.mDesc);	ThrowIfOSErr_(err);}LDescTube::LDescTube(	const LTubeableItem	*inTubeableItem,	Boolean				inReqdFlavorsOnly):	LDataTube(inTubeableItem, inReqdFlavorsOnly){	Assert_(mTubeableItem);	//	member required;	mTubeableItem->AddFlavorTypesTo(this);}LDescTube::~LDescTube(){	mTubeableItem = NULL;}void	LDescTube::AddFlavor(FlavorType inFlavor)/*	Only first added flavor is remembered!*/{	Assert_(mTubeableItem);	if (mDesc.mDesc.descriptorType == typeNull) {		mTubeableItem->SendFlavorTo(inFlavor, this);	}}Boolean	LDescTube::FlavorExists(FlavorType inFlavor) const{	OSErr	err;	Boolean	rval = false;		if (mDesc.mDesc.descriptorType != inFlavor) {		if (mCoercedDesc.mDesc.descriptorType != inFlavor) {			err = AECoerceDesc((AEDesc *)&mDesc.mDesc, inFlavor, (AEDesc *)&mCoercedDesc.mDesc);			if (err == errAECoercionFail)				return false;			ThrowIfOSErr_(err);		}	}	return true;}Size	LDescTube::GetFlavorSize(FlavorType inFlavor){	if (mDesc.mDesc.descriptorType == inFlavor)		return GetHandleSize(mDesc.mDesc.dataHandle);	if (FlavorExists(inFlavor))		return GetHandleSize(mCoercedDesc.mDesc.dataHandle);				return 0;}void *	LDescTube::GetFlavorData(	FlavorType	inFlavor,	void		*outFlavorData,	Int32		inOffset,	Int32		inAmount)//	For routine description see LDataTube.cp.{	Int32	amount = URange32::Min(inAmount, GetFlavorSize(inFlavor));	void	*source = NULL;		if (outFlavorData) {		if (inFlavor != typeNull) {			if (inFlavor == mDesc.mDesc.descriptorType) {				source = *(mDesc.mDesc.dataHandle);			} else if (inFlavor == mCoercedDesc.mDesc.descriptorType) {				source = *(mCoercedDesc.mDesc.dataHandle);			}		}					if (source == NULL)			Throw_(badDragFlavorErr);		source = ((Int8 *) (source)) + inOffset;				if (amount > 0)			BlockMove(source, outFlavorData, amount);	}	return NULL;}void	LDescTube::SetFlavorData(	FlavorType	inFlavor,	void		*inFlavorData,	Int32		inFlavorSize,	Int32		inOffset,	Int32		inAmount)//	For routine description see LDataTube.cp.{	OSErr	err;		Assert_(inOffset == 0);	Assert_(inAmount >= inFlavorSize);		err = AECreateDesc(inFlavor, inFlavorData, inFlavorSize, &mDesc.mDesc);	ThrowIfOSErr_(err);		AEDisposeDesc(&mCoercedDesc.mDesc);}