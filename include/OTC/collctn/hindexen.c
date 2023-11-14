#ifndef OTC_COLLCTN_HINDEXEN_C
#define OTC_COLLCTN_HINDEXEN_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/hindexen.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndexEntry<T>::~OTC_HIndexEntry()
{
  OTC_BaseActions<T>::remove(key_);
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HIndexEntry<T>::removeEntry()
{
  index_->remove(this,hashValue_);

  kill();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndexEntry<T>::OTC_HIndexEntry(
 T const& theKey,
 OTC_HashTable* theIndex,
 int theHashValue
)
  : key_(OTC_BaseActions<T>::add(theKey)),
    index_(theIndex),
    hashValue_(theHashValue),
    item_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndexEntry<T>* OTC_HIndexEntry<T>::prevEntry_() const
{
  OTC_Linkable* tmpLink;

  tmpLink = ((OTC_Linkable*)this)->prev();
  while (tmpLink != 0 && (!tmpLink->isLink() || tmpLink->isDead()))
    tmpLink = tmpLink->prev();

  return (OTC_HIndexEntry<T>*)tmpLink;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HIndexEntry<T>* OTC_HIndexEntry<T>::nextEntry_() const
{
  OTC_Linkable* tmpLink;

  tmpLink = ((OTC_Linkable*)this)->next();
  while (tmpLink != 0 && (!tmpLink->isLink() || tmpLink->isDead()))
    tmpLink = tmpLink->next();

  return (OTC_HIndexEntry<T>*)tmpLink;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HINDEXEN_C */
