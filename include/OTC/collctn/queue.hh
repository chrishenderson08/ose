#ifndef OTC_COLLCTN_QUEUE_HH
#define OTC_COLLCTN_QUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/queue.hh
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

#include <OTC/collctn/linklist.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/queue.hh"
#endif
#pragma interface "OTC/collctn/queue.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Queue
    // = TITLE
    //     Implements a FIFO or queue.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_Queue> implements a FIFO or queue. Note, that it is the
    //     user's responsibility to deal with deletion of objects held in the
    //     queue when it is parameterised over a pointer type; ie., this
    //     class works independently of the <OTC_BaseActions> class.
    //     
    // = NOTES
    //     To allow any number of items to be placed into the queue, it is
    //     implemented using a linked list. If efficiency is a concern, both
    //     in terms of speed and memory use, and there is a fixed upper bound
    //     on the number of items to be held, the class <OTC_BoundedQueue> may
    //     be more appropriate.
    //
    // = SEE ALSO
    //     <OTC_BoundedQueue>
{
  public:

			~OTC_Queue()
				{ delete list_; }

    // = INITIALISATION

			OTC_Queue();
				// Creates an empty queue.

			OTC_Queue(OTC_Queue<T> const& theQueue);
				// Creates a queue which is a copy of
				// <theQueue>. If the queue is holding
				// pointers, only the value of the pointer
				// is copied and not the object being
				// pointed at.

    // = ASSIGNMENT

    OTC_Queue<T>&	operator=(OTC_Queue<T> const& theQueue);
				// Replaces this queue with a copy of
				// <theQueue>. If the queue is holding
				// pointers, only the value of the pointer
				// is copied and not the object being
				// pointed at.

    // = QUERY

    bool		isEmpty() const
				{ return count_ == 0; }
				// Returns <true> if the queue is empty.

    u_int		count() const
				{ return count_; }
				// Returns the number of items in
				// the queue.

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

    // = MODIFICATION

    void		add(T const& theItem);
				// Adds <theItem> onto the tail of the
				// queue.

    T			remove();
				// Removes and returns the item at
				// the head of the queue. If the queue
				// is empty, an exception is raised.

    void		clear()
				{ list_->removeAll(); count_ = 0; }
				// Clears the queue of all items held.

    void		discard(u_int theCount);
				// Discards the first <theCount> items
				// on the queue. If there are not that
				// many items on the queue, an exception
				// is raised.

  private:

    T&			head_() const;
				// Returns a reference to the item on
				// the head of the queue. If the queue
				// is empty, an exception is raised.

    OTC_LinkList*	list_;
				// The list containing the items.

    u_int		count_;
				// The number of items in the queue.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/queue.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_QUEUE_HH */
