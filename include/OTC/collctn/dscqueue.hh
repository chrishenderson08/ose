#ifndef OTC_COLLCTN_DSCQUEUE_HH
#define OTC_COLLCTN_DSCQUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/dscqueue.hh
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

#include <OTC/collctn/priqueue.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/dscqueue.hh"
#endif
#pragma interface "OTC/collctn/dscqueue.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_DescendingQueue : public OTC_PriorityQueue<T>
    // = TITLE
    //     Implements a descending priority queue.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class derives from <OTC_PriorityQueue>, ensuring that the
    //     priority queue is of the descending type.
    //     
    // = SEE ALSO
    //     <OTC_PriorityQueue>
{
  public:

			~OTC_DescendingQueue();

    // = INITIALISATION

			OTC_DescendingQueue(
			 int (*theRankFn)(T const&,T const&)=0
			)
			  : OTC_PriorityQueue<T>(theRankFn,OTCLIB_DESCENDING) {}
				// Creates an empty descending priority queue.
				// <theRankFn> is an optional comparison
				// function to be used in preference to
				// <OTC_RankActions>.

			OTC_DescendingQueue(
			 OTC_DescendingQueue<T> const& theQueue
			)
                          : OTC_PriorityQueue<T>(theQueue) {}
				// Creates a queue which is a copy of
				// <theQueue>. If the queue holds pointers,
				// only the pointers are copied, not what the
				// pointers point at.

    // = ASSIGNMENT

    OTC_DescendingQueue<T>&	operator=(
				 OTC_DescendingQueue<T> const& theQueue
				)
                                {
                                  OTC_PriorityQueue<T>::operator=(theQueue);
                                  return *this;
                                }
                                // Replaces the contents of this queue, with
                                // the contents of <theQueue>. If the queue
                                // holds pointers, only the pointers are
                                // copied, not what the pointers point at.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/dscqueue.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_DSCQUEUE_HH */
