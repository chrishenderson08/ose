#ifndef OTC_COLLCTN_PRIQUEUE_HH
#define OTC_COLLCTN_PRIQUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/priqueue.hh
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

#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/avltree.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/priqueue.hh"
#endif
#pragma interface "OTC/collctn/priqueue.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_QueueType
{
  OTCLIB_ASCENDING,
  OTCLIB_DESCENDING
};

class OTC_LinkList;

template<class T>
class OTC_PriorityQueue
    // = TITLE
    //     Implements a priority queue.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class implements a priority queue. This is different to a
    //     normal queue in that items are not added to the end of
    //     the queue, but are inserted into the queue based on an intrinsic
    //     ordering. This ordering can be dictated by the user, by defining
    //     an explicit version of <OTC_RankActions> for the type that
    //     the queue is parameterised over. For a particular instance of
    //     the class, a function which determines the ordering, may also be
    //     passed to the constructor.
    //     
    //     The priority queue can be instantiated in two forms, either
    //     as an ascending queue, or a descending queue. In an ascending
    //     queue, a <remove()> operation will remove the least ranked
    //     item in the queue. In a descending queue, the highest ranked
    //     item will be removed. In situations where a particular type
    //     of queue must be ensured, the derived versions of this class
    //     can be used. The derived classes are <OTC_AscendingQueue> and
    //     <OTC_DescendingQueue>.
    //
    //     Note that it is the user's responsibility to deal with deletion of
    //     objects held in the queue, when it is parameterised over a pointer
    //     type, ie., this class works independently of the <OTC_BaseActions>
    //     class.
    //     
    // = NOTES
    //     Duplicates, or items of equivalent rank, are permitted in the
    //     queue. When duplicates are inserted the ordering of similar items
    //     is undefined. A duplicate item will not necessarily be placed into
    //     the queue after that which already existed; it may be placed
    //     before it.
    //     
    // = SEE ALSO
    //     <OTC_AscendingQueue>, <OTC_DescendingQueue>, <OTC_Queue>,
    //     <OTC_RankActions>
{
  public:

    // = INITIALISATION

			OTC_PriorityQueue(
			 OTC_QueueType theType=OTCLIB_ASCENDING
			);
				// Creates an empty queue. <theType>
				// should be the type of queue required.
				// Valid types are <OTCLIB_ASCENDING>
				// and <OTCLIB_DESCENDING>.

			OTC_PriorityQueue(
			 int (*theRankFn)(T const&,T const&),
			 OTC_QueueType theType=OTCLIB_ASCENDING
			);
				// Creates an empty queue. <theRankFn>
				// is a comparison function to be used
				// instead of <OTC_RankActions>. <theType>
				// should be the type of queue required.
				// Valid types are <OTCLIB_ASCENDING>
				// and <OTCLIB_DESCENDING>.

			OTC_PriorityQueue(OTC_PriorityQueue<T> const& theQueue);
				// Creates a priority queue which is a
				// copy of <theQueue>. If the queue holds
				// pointers, only the pointers are copied
				// and not what the pointers point at.

    // = DESTRUCTION

    virtual		~OTC_PriorityQueue();
				// Deletes all the structures used to hold
				// the queue. Note that this does not delete
				// objects held by the queue, when the
				// class is parameterised over a pointer type.

    // = QUERY

    bool		isEmpty() const;
				// Return <true> if the queue is empty.

    u_int		count() const
				{ return tree_->population(); }
				// Returns the number of items in the queue.

    T const&		head() const;
				// Returns a reference to the item on the
				// head of the queue. If the queue is of the
				// ascending type, this will be the item
				// of lowest rank. If it is a descending
				// queue, it will be the item of highest
				// rank. When the queue is empty, an exception
				// is raised.

    // = MODIFICATION

    void		add(T const& theItem);
				// Adds <theItem> into the queue based.

    T			remove();
				// Removes and returns the item at the head
				// of the queue. If the queue is of the
				// ascending type, this will be the item
				// of lowest rank. If it is a descending
				// queue, it will be the item of highest
				// rank. When the queue is empty, an exception
				// is raised.

    void		clear();
				// Clears all items from the queue.

    void		discard(u_int theCount);
				// Discards the first <theCount> items
				// in the queue. If the queue does not
				// hold that many items, an exception
				// is raised.

  protected:

    OTC_PriorityQueue<T>&	operator=(OTC_PriorityQueue<T> const& theQueue);
				// Replaces this queue with the contents of
				// <theQueue>. Queues must be both ascending
				// or both descending. If they are of a
				// different type an exception is raised.
				// If the queue holds pointers, only the
				// pointers are copied and not what the
				// pointers point at.

  protected:

    // = RANKING

    virtual int		rank(T const& item1, T const& item2) const;
				// Rank two items.

  private:

    OTC_QueueType	type_;
				// The type of queue, ascending or
				// descending.

    OTC_AVLTree*	tree_;
				// The index for items in the queue.

    OTC_LinkList*	list_;
				// The list of items in the queue.

    int			(*rankFn_)(T const&,T const&);
				// Comparison function to be used in
				// place of <OTC_RankActions>.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/priqueue.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRIQUEUE_HH */
