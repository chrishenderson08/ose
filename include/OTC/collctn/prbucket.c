#ifndef OTC_COLLCTN_PRBUCKET_C
#define OTC_COLLCTN_PRBUCKET_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/prbucket.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairBucket<T1,T2>::~OTC_PairBucket()
{
  OTC_BaseActions<T1>::remove(key_);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairBucket<T1,T2>::OTC_PairBucket(OTC_PairBucket<T1,T2> const& theBucket)
  : OTC_Bucket<T2>(theBucket), key_(OTC_BaseActions<T1>::add(theBucket.key_))
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairBucket<T1,T2>::OTC_PairBucket(T1 const& theKey, T2 const& theItem)
  : OTC_Bucket<T2>(theItem), key_(OTC_BaseActions<T1>::add(theKey))
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRBUCKET_C */
