//	===========================================================================//	LClipboardTube.cp				�1994 Metrowerks Inc. All rights reserved.//	===========================================================================//	For routine descriptions see LDataTube.cp.#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include	<PP_Prefix.h>#include	"LClipboardTube.h"#include	<UMemoryMgr.h>#include	<Scrap.h>#include	<Errors.h>#include	<LDynamicArray.h>#include	<LTubeableItem.h>LClipboardTube::LClipboardTube()/*	Create a tube for existing data on the clipboard.*/{	//	Building list of available "flavors" is unnecessary -- just use	//	the scrap manager.}LClipboardTube::LClipboardTube(	const LTubeableItem	*inTubeableItem,	Boolean				inReqdFlavorsOnly):	LDataTube(inTubeableItem, inReqdFlavorsOnly){	ZeroScrap();		Assert_(mTubeableItem);	//	member required;	mTubeableItem->AddFlavorTypesTo(this);}LClipboardTube::~LClipboardTube(){	mTubeableItem = NULL;}void	LClipboardTube::AddFlavor(FlavorType inFlavor){//	Assert_(!mTypeList.FetchIndexOf(&inFlavor));	//	There's a type clash or someone													//	is being redundant.													//	Or there are multiple items of the													//	type.  -- LET IT BE		Assert_(mTubeableItem);	mTubeableItem->SendFlavorTo(inFlavor, this);}Boolean	LClipboardTube::FlavorExists(FlavorType inFlavor) const{	Size	dataSize;	Int32	offset;		dataSize = GetScrap(NULL, inFlavor, &offset);		if (dataSize == noTypeErr)		return false;	if (dataSize >= 0)		return true;	ThrowIfOSErr_(dataSize);		return false;}Size	LClipboardTube::GetFlavorSize(FlavorType inFlavor){	Size	dataSize;	Int32	offset;		dataSize = GetScrap(NULL, inFlavor, &offset);		if (dataSize > 0)		return dataSize;	else		Throw_(noTypeErr);	return 0;}void *	LClipboardTube::GetFlavorData(	FlavorType	inFlavor,	void		*outFlavorData,	Int32		inOffset,	Int32		inAmount)//	For routine description see LDataTube.cp.{	Size			dataSize = GetFlavorSize(inFlavor),					preDataSize = dataSize;	StTempHandle	h(dataSize);	Int32			offset;		Assert_(outFlavorData);	dataSize = GetScrap(h.mHandle, inFlavor, &offset);		if (dataSize <= 0)		ThrowIfOSErr_(dataSize);	Assert_(dataSize == preDataSize);	BlockMoveData(*(h.mHandle), outFlavorData, dataSize);		return NULL;}void	LClipboardTube::SetFlavorData(	FlavorType	inFlavor,	void		*inFlavorData,	Int32		inFlavorSize,	Int32		inOffset,	Int32		inAmount)//	For routine description see LDataTube.cp./*	Set functions should only be called by initialization routines	or a tubeable item in response to a send flavor message.*/{	long	err;	//	Long for unusual scrap calling convention		Assert_(inFlavorData);		err = PutScrap(inFlavorSize, inFlavor, (Ptr)inFlavorData);	ThrowIfOSErr_(err);}void	LClipboardTube::GetFlavorHandle(FlavorType inFlavor, Handle outFlavorData){	Size			dataSize;	Int32			offset;	Assert_(outFlavorData);		dataSize = GetScrap(outFlavorData, inFlavor, &offset);		if (dataSize <= 0)		ThrowIfOSErr_(dataSize);}LDataTube *	LClipboardTube::GetSubTube(Int32 inTubeIndex)/*	Unless you could determine sub part sections of the scrap,	this routine makes no sense in a vanilla clipboard	setting.*/{	Assert_(false);	//	Can't do that with a clipboard tube	return NULL;}LDataTube *	LClipboardTube::MakeNewSubTubeFor(LTubeableItem *inItem)/*	This routine makes little sense in a vanilla clipboard	setting.		Simply let callers "append" data.*/{//	Assert_(false);	//	Can't do that with a clipboard tube	return this;}void	LClipboardTube::AddSubTube(LDataTube *inTube)/*	This routine makes little sense in a vanilla clipboard	setting.		Simply let callers "append" data.*/{//	Assert_(false);	//	Can't do that with a clipboard tube}