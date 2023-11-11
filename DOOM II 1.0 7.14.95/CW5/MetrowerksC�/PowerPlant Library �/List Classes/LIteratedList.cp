// ===========================================================================//	LIteratedList.cp				�1994 Metrowerks Inc. All rights reserved.// ===========================================================================////	An abstract mix-in class for a List that can be associated with a//	ListIterator////	This class maintains a linked list of ListIterators that are//	associated with the List. The constructors and destructors for//	this class and the ListIterator class take care of ensuring the//	integrity of the associations.////	Normally, subclasses will multiply inherit from LIteratedList and//	another class which defines a means of storing and accessing//	items in the List.////	Subclasses must implement GetCount() and FetchItemAt().////	To allow Iterators to work properly when the List changes,//	subclasses should call ItemsInserted() and ItemsRemoved() whenever//	items are added or deleted from the List.#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include <LIteratedList.h>#include <LListIterator.h>LIteratedList::LIteratedList(){	mIteratorListHead = nil;}LIteratedList::~LIteratedList(){									// Notify Iterators of our death	LListIterator	*theIter = mIteratorListHead;	while (theIter != nil) {		theIter->ListDied();		theIter = theIter->GetNextIterator();	}}// ---------------------------------------------------------------------------//		� AttachIterator// ---------------------------------------------------------------------------//	Associate an Iterator with a ListvoidLIteratedList::AttachIterator(	LListIterator	*inIterator){	inIterator->SetNextIterator(mIteratorListHead);	mIteratorListHead = inIterator;}// ---------------------------------------------------------------------------//		� DetachIterator// ---------------------------------------------------------------------------//	Dissociate an Iterator from a ListvoidLIteratedList::DetachIterator(	LListIterator	*inIterator){	LListIterator	*currIter = mIteratorListHead;	LListIterator	*prevIter = nil;		while (currIter != nil) {		// Search for the specified Iterator		if (currIter == inIterator) {									// Found it. Adjust linked list.			if (prevIter == nil) {				mIteratorListHead = currIter->GetNextIterator();			} else {				prevIter->SetNextIterator(currIter->GetNextIterator());			}			break;					} else {					// Continue search			prevIter = currIter;			currIter = currIter->GetNextIterator();		}	}}// ---------------------------------------------------------------------------//		� ItemsInserted// ---------------------------------------------------------------------------//	Notify Iterators associated with a List that items have been insertedvoidLIteratedList::ItemsInserted(	Uint32	inCount,	Int32	inAtIndex){	LListIterator	*theIter = mIteratorListHead;	while (theIter != nil) {		theIter->ItemsInserted(inCount, inAtIndex);		theIter = theIter->GetNextIterator();	}}// ---------------------------------------------------------------------------//		� ItemsRemoved// ---------------------------------------------------------------------------//	Notify Iterators associated with a List that items have been removedvoidLIteratedList::ItemsRemoved(	Uint32	inCount,	Int32	inAtIndex){	LListIterator	*theIter = mIteratorListHead;	while (theIter != nil) {		theIter->ItemsRemoved(inCount, inAtIndex);		theIter = theIter->GetNextIterator();	}}