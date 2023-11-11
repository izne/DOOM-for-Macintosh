//	===========================================================================//	LSharedObject.h					�1994 Metrowerks Inc. All rights reserved.//	===========================================================================#pragma	once/*	A reference counting scheme for object's that are owned by	multiple other objects.*/class LSharedObject  {public:						LSharedObject();	virtual				~LSharedObject();							virtual	void		OwnershipClaimed(void *inClaimer);	virtual Boolean		OwnershipReleased(void *inReleaser);protected:	virtual	void		Release(void);		//	Override as necessary	virtual	Int32		OwnershipCount(void);private:	Int16				mRefCount;};//	---------------------------------------------------------------------------//	A handy define://	ref is the reference causing claimed ownership#define	SHARED_OBJ_REPLACE_REF(ref,obj)		\	if ((ref) != (obj)) {					\		if (ref)							\			(ref)->OwnershipReleased(this);	\		(ref) = (obj);						\		if (ref)							\			(ref)->OwnershipClaimed(this);	\	}