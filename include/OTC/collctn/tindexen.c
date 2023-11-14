#ifndef OTC_COLLCTN_TINDEXEN_C
#define OTC_COLLCTN_TINDEXEN_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/tindexen.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TIndexEntry<T>::~OTC_TIndexEntry()
{
  OTC_BaseActions<T>::remove(key_);
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_TIndexEntry<T>::removeEntry()
{
  unlink();
  delete this;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TIndexEntry<T>::OTC_TIndexEntry(T const& theKey)
  : key_(OTC_BaseActions<T>::add(theKey)), item_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */

#endif
