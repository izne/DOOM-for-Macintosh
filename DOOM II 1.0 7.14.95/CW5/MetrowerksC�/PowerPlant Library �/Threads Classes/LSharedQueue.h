// ===========================================================================//	LSharedQueue.h					�1994 Metrowerks Inc. All rights reserved.////									Author:  Paul Lalonde// ===========================================================================#ifndef _H_LSharedQueue#define _H_LSharedQueue#ifdef __MWERKS__#	pragma once#endif// PowerPlant headers#include <LMutexSemaphore.h>#include <LQueue.h>const Int32	sharedQueue_WaitForever	= -1;const Int32	sharedQueue_NoWait		= 0;class LSharedQueue : public LQueue, private LMutexSemaphore {//	I N T E R F A C E ============================================public:		// INSTANCE METHODS -----------------		// constructors / destructors					LSharedQueue();	virtual			~LSharedQueue();		// queue manipulation	virtual LLink	*Next(Int32 milliSeconds = sharedQueue_WaitForever);	virtual void	NextPut(LLink *aLink);	virtual Boolean	Remove(LLink *aLink);		// iterators	virtual void	DoForEach(LQueueIterator proc, void *arg);		//	I M P L E M E N T A T I O N ==================================	protected:		// INSTANCE METHODS -----------------		// constructors & destructors					LSharedQueue(const LSharedQueue&);		// INSTANCE VARIABLES ---------------		LSemaphore		mAvailable;		// will block threads when queue is empty};#endif // _H_LSharedQueue