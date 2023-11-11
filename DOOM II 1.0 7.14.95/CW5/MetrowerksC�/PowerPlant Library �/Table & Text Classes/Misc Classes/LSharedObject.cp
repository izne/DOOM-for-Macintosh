//	===========================================================================//	LSharedObject.cp				�1994 Metrowerks Inc. All rights reserved.//	===========================================================================#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include	<PP_Prefix.h>#include	"LSharedObject.h"LSharedObject::LSharedObject(){	mRefCount = -1;}LSharedObject::~LSharedObject(){	Assert_(mRefCount <= 0);	//	Someone isn't following SharedObject policies.}void	LSharedObject::OwnershipClaimed(void *inClaimer)/*	inClaimer is not remembered!  ... but it could be for debugging purposes*/{	if (mRefCount >= 0) {		mRefCount++;		Assert_(mRefCount != 0);	/*	If this fails, something real bad is happening --										this has been called so many times that mRefCount										flipped over.									*/			} else {		mRefCount = 1;	}}Boolean	LSharedObject::OwnershipReleased(void *inReleaser)/*	inReleaser is not checked!  ... but it could be for debugging purposes		If, this releases the last claim on this object, Release (delete) will	be called.*/{	if (mRefCount > 0)		mRefCount--;		if (mRefCount <= 0) {		Release();		return true;	} else {		return false;	}}void	LSharedObject::Release(void)/*	Override this to perform special memory clean-up operations for	shared objects.*/{	delete this;	//	Ie or something like delete this.}Int32	LSharedObject::OwnershipCount(void)/*	Returns the number of claims on this object.		Initially this value is -1.  After the first claim has been made,	the ownership count will be 1.  An ownership count of zero will	never be returned because when the ownership count goes down to	zero, the object will have its Release method call -- the Release	method typically deletes the object.*/{	if (mRefCount >= 0)		return mRefCount;	else		return 0;}