#ifndef OTC_COLLCTN_HOLDER_C
#define OTC_COLLCTN_HOLDER_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/holder.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Holder<T>::OTC_Holder(T const& theItem)
 : item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Holder<T>::OTC_Holder(OTC_Holder<T> const& theHolder)
 : item_(theHolder.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Holder<T>::~OTC_Holder()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HOLDER_C */
