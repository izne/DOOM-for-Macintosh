// ===========================================================================//	UMemoryMgr.cp					�1993 Metrowerks Inc. All rights reserved.// ===========================================================================////	Memory management utility classes and functions////	Stack-based Classes://		Several utility classes are designed for creating stack-based//		objects, where the Constructor performs some action and the//		Destructor undoes the action. The advantage of stack-based//		objects is that the Destructor automatically gets called,//		even when there is an exception thrown.#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include <UMemoryMgr.h>#ifndef __RESOURCES__#include <Resources.h>#endif#ifndef __ERRORS__#include <Errors.h>#endif// ===========================================================================//	� StHandleLocker Class// ===========================================================================//	Constructor Locks the Handle//	Destructor restores the Handle's original stateStHandleLocker::StHandleLocker(	Handle	inHandle){	mHandle = inHandle;	mSaveState = ::HGetState(inHandle);	::HLock(inHandle);}StHandleLocker::~StHandleLocker(){	::HSetState(mHandle, mSaveState);}// ===========================================================================//	� StHandleBlock Class// ===========================================================================//	Constructor allocates the Handle//	Destructor disposes of the HandleStHandleBlock::StHandleBlock(	Size	inSize,					// Bytes to allocate	Boolean	inThrowFail)			// Whether to Throw if allocate fails{	mHandle = ::NewHandle(inSize);	if (inThrowFail) {		ThrowIfMemFail_(mHandle);	}}StHandleBlock::~StHandleBlock(){	if (mHandle != nil) {		::DisposeHandle(mHandle);	}}// ===========================================================================//	� StTempHandle Class// ===========================================================================//	Constructor allocates the Handle  using temporary (System) memory//	Destructor disposes of the HandleStTempHandle::StTempHandle(	Size	inSize,					// Bytes to allocate	Boolean	inThrowFail)			// Whether to Throw if allocate fails{	OSErr	err;	mHandle = ::TempNewHandle(inSize, &err);	if (inThrowFail  &&  (mHandle == nil)) {		ThrowOSErr_(err);	}}StTempHandle::~StTempHandle(){	if (mHandle != nil) {		::DisposeHandle(mHandle);	}}// ===========================================================================//	� StPointerBlock Class// ===========================================================================//	Constructor allocates the Ptr//	Destructor disposes of the PtrStPointerBlock::StPointerBlock(	Size	inSize,					// Bytes to allocate	Boolean	inThrowFail)			// Whether to Throw if allocate fails{	mPtr = ::NewPtr(inSize);	if (inThrowFail) {		ThrowIfMemFail_(mPtr);	}}StPointerBlock::~StPointerBlock(){	if (mPtr != nil) {		::DisposePtr(mPtr);	}}// ===========================================================================//	� StResource Class// ===========================================================================//	Constructor gets the resource handle//	Destructor releases the resource handleStResource::StResource(	ResType	inResType,	ResIDT	inResID,	Boolean	inThrowFail)			// Whether to Throw if allocate fails{	mResourceH = ::GetResource(inResType, inResID);	if (inThrowFail) {		ThrowIfResFail_(mResourceH);	}}StResource::~StResource(){	if (mResourceH != nil) {		::ReleaseResource(mResourceH);	}}// ===========================================================================//	� Utility Functions// ===========================================================================// ---------------------------------------------------------------------------//		� InitializeHeap// ---------------------------------------------------------------------------//	Call this function at the beginning of your program (before initializing//	the Toolbox) to expand the heap zone to its maximum size and preallocate//	a specified number of Master Pointer blocks.voidInitializeHeap(	Int16	inMoreMasters){	MaxApplZone();	for (Int16 i = 1; i <= inMoreMasters; i++) {		MoreMasters();	}}// ---------------------------------------------------------------------------//		� BlocksAreEqual// ---------------------------------------------------------------------------//	Blocks are equal if the first n bytes pointed to by s1 have the same//	values as the first n bytes pointed to by s2. Note that this always//	returns true if n is zero.BooleanBlocksAreEqual(	const void	*s1,	const void	*s2,	Uint32		n){	const unsigned char	*ucp1 = (const unsigned char *) s1;	const unsigned char	*ucp2 = (const unsigned char *) s2;		while (n > 0) {		if (*ucp1++ != *ucp2++) {			return false;		}		n--;	}		return true;}