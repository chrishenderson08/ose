#ifndef OTC_COLLCTN_ASCQUEUE_HH
#define OTC_COLLCTN_ASCQUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ascqueue.hh
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
#pragma implementation "OTC/collctn/ascqueue.hh"
#endif
#pragma interface "OTC/collctn/ascqueue.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_AscendingQueue : public OTC_PriorityQueue<T>
    // = TITLE
    //     Implements an ascending priority queue.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class derives from <OTC_PriorityQueue>, ensuring that
    //     the priority queue is of the ascending type.
    //     
    // = SEE ALSO
    //     <OTC_PriorityQueue>
{
  public:

			~OTC_AscendingQueue();

    // = INITIALISATION

			OTC_AscendingQueue(
			 int (*theRankFn)(T const&,T const&)=0
			)
			  : OTC_PriorityQueue<T>(theRankFn,OTCLIB_ASCENDING) {}
				// Creates an empty ascending priority queue.
				// <theRankFn> is an optional comparison
				// function to be used in preference to
				// <OTC_RankActions>.

			OTC_AscendingQueue(
			 OTC_AscendingQueue<T> const& theQueue
			)
			  : OTC_PriorityQueue<T>(theQueue) {}
				// Creates a queue which is a copy of
				// <theQueue>. If the queue holds pointers,
				// only the pointers are copied, not what
				// the pointers point at.

    // = ASSIGNMENT

    OTC_AscendingQueue<T>&	operator=(
				 OTC_AscendingQueue<T> const& theQueue
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
#include <OTC/collctn/ascqueue.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ASCQUEUE_HH */
