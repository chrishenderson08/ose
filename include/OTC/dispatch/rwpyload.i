#ifndef OTC_DISPATCH_RWPYLOAD_I
#define OTC_DISPATCH_RWPYLOAD_I
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     dispatch/rwpyload.i
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/pritertr.hh>

/* ------------------------------------------------------------------------- */
template<class T>
void operator<<=(OTC_RWPayload theWriter, OTC_Iterator<T> theItems)
{
  theWriter <<= OTC_ROPayload::nullArray();

  while (theItems.isValid())
  {
    theWriter[-1] <<= theItems.item();

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void operator<<=(
 OTC_RWPayload theWriter,
 OTC_PairIterator<OTC_String,T> thePairs
)
{
  theWriter <<= OTC_ROPayload::nullIndex();

  while (thePairs.isValid())
  {
    theWriter.sibling(thePairs.key()) <<= thePairs.item();

    thePairs.next();
  }
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_RWPYLOAD_I */
