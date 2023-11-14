#ifndef OTC_COLLCTN_BNDQUEUE_HH
#define OTC_COLLCTN_BNDQUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/bndqueue.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/bndqueue.hh"
#endif
#pragma interface "OTC/collctn/bndqueue.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_BoundedQueue
    // = TITLE
    //     Implements a FIFO or queue with fixed capacity.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_BoundedQueue> implements a FIFO or queue with fixed capacity.
    //     Note, that it is the user's responsibility to deal with deletion
    //     of objects held in the queue when it is parameterised over a
    //     pointer type; ie., this class works independently of the
    //     <OTC_BaseActions> class.
    //     
    // = NOTES
    //     Being a bounded queue, for efficiency, an array is used in the
    //     internal implementation instead of a linked list. The consequences
    //     are, that when parameterised over a class type, the class
    //     must have a constructor which accepts no arguments and also be
    //     capable of having assignment performed upon it.
{
  public:

			~OTC_BoundedQueue();

    // = INITIALISATION

			OTC_BoundedQueue(u_int theCapacity, T* theMem=0);
				// Creates an empty queue which has
				// capacity to hold <theCapacity> items.
				// If memory is provided through <theMem>
				// it will be used, else memory will
				// be allocated from the free store. If
				// the memory is provided by the user, it
				// is the user's responsibility to delete
				// it.

			OTC_BoundedQueue(OTC_BoundedQueue<T> const& theQueue);
				// Creates a queue which is a copy of
				// <theQueue>. Space required by this queue
				// will always be allocated from the free
				// store. If pointers are held in the queue,
				// only the value of the pointers is copied,
				// not what the pointer is pointing at.

    // = ASSIGNMENT

    OTC_BoundedQueue<T>&	operator=(OTC_BoundedQueue<T> const& theQueue);
				// Replaces this queue with the contents of
				// <theQueue>. Space required by this queue
				// will always be allocated from the free
				// store. The capacity of this queue will
				// be changed to that of <theQueue>.
				// If pointers are held in the queue,
				// only the value of the pointers is copied,
				// not what the pointers point at.

    // = QUERY

    bool		isEmpty() const
				{ return count_ == 0; }
				// Returns <true> if the queue is empty.

    bool		isFull() const
				{ return count_ >= capacity_; }
				// Returns <true> if the queue is full.

    u_int		count() const
				{ return count_; }
				// Returns the number of items in the queue.

    u_int		capacity() const
				{ return capacity_; }
				// Returns the maximum number of items which
				// the queue can hold.

    T&			head()
				{ return head_(); }
				// Returns a reference to the item on
				// the head of the queue. If the queue
				// is empty, an exception is raised.

    T const&		head() const
				{ return head_(); }
				// Returns a reference to the item on
				// the head of the queue. If the queue
				// is empty, an exception is raised.

    T&			peek(u_int theIndex)
				{ return peek_(theIndex); }
				// Returns the item in the queue given by
				// <theIndex>. An index of <0> represents the
				// head of the queue. Successive items are
				// numbered from <1> onwards. If
				// <theIndex> is outside the bounds of the
				// queue, an exception is raised.

    T const&		peek(u_int theIndex) const
				{ return peek_(theIndex); }
				// Returns the item in the queue given by
				// <theIndex>. An index of <0> represents the
				// head of the queue. Successive items are
				// numbered from <1> onwards. If
				// <theIndex> is outside the bounds of the
				// queue, an exception is raised.

    // = MODIFICATION

    void		add(T const& theItem);
				// Adds <theItem> onto the tail of the
				// queue. If the capacity of the queue would
				// be exceeded, an exception is raised.

    T			remove();
				// Removes and returns the item at
				// the head of the queue. If the queue
				// is empty, an exception is raised.

    void		clear()
				{ count_ = 0; get_ = 0; put_ = 0; }
				// Clears the queue.

    void		discard(u_int theCount);
				// Discards the first <theCount> items
				// from the head of the queue. If there
				// are not that many items, an
				// exception is raised.

  private:

    T&			head_() const;
				// Returns a reference to the item on
				// the head of the queue. If the queue
				// is empty an exception is raised.

    T&			peek_(u_int theIndex) const;
				// Returns the item in the queue given by
				// <theIndex>. An index of <0> represents the
				// head of the queue. Successive items are
				// numbered from <1> onwards. If
				// <theIndex> is outside the bounds of the
				// queue an exception is raised.

    u_int		capacity_;
				// The maximum number of items which can
				// be held.

    u_int		count_;
				// Number of items held.

    u_int		get_;
				// Index of next item to be fetched.

    u_int		put_;
				// Index of next available location
				// to put an item.

    T*			items_;
				// Items in the queue.

    bool		destroy_;
				// Set to <true> if <items_> needs to be
				// destroyed.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/bndqueue.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_BNDQUEUE_HH */
