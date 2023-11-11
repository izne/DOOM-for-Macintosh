// ===========================================================================//	UMemoryMgr.h					�1993 Metrowerks Inc. All rights reserved.// ===========================================================================#pragma once#include <PP_Prefix.h>#ifndef __MEMORY__#include <Memory.h>#endif// ---------------------------------------------------------------------------class	StHandleLocker {public:	StHandleLocker(Handle inHandle);	~StHandleLocker();private:	Handle	mHandle;	char	mSaveState;};// ---------------------------------------------------------------------------class	StHandleBlock {public:	StHandleBlock(Size inSize, Boolean inThrowFail = true);	~StHandleBlock();	Handle	mHandle;};// ---------------------------------------------------------------------------class	StTempHandle {public:	StTempHandle(Size inSize, Boolean inThrowFail = true);	~StTempHandle();	Handle	mHandle;};// ---------------------------------------------------------------------------class	StPointerBlock {public:	StPointerBlock(Size inSize, Boolean inThrowFail = true);	~StPointerBlock();		Ptr		mPtr;};// ---------------------------------------------------------------------------class	StResource {public:	StResource(ResType inResType, ResIDT inResID, Boolean inThrowFail = true);	~StResource();		Handle	mResourceH;};// ---------------------------------------------------------------------------void		InitializeHeap(Int16 inMoreMasters);Boolean		BlocksAreEqual(const void *s1, const void *s2, Uint32 n);