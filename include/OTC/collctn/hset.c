#ifndef OTC_COLLCTN_HSET_C
#define OTC_COLLCTN_HSET_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/hset.c
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/icursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HSet<T>::~OTC_HSet()
{
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HSet<T>::OTC_HSet(OTC_HSet<T> const& theSet)
  : rankFn_(theSet.rankFn_)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  OTC_Iterator<T> theItems = 0;
  theItems = theSet.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (theItems.reset(); theItems.isValid(); theItems.next())
    add(theItems.item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HSet<T>::OTC_HSet(int (*theRankFn)(T const&,T const&))
  : rankFn_(theRankFn)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HSet<T>::OTC_HSet(OTC_HSet<T> const& theSet, OTC_ShallowCopy)
  : rankFn_(theSet.rankFn_)
{
  list_ = theSet.list_;
  list_->reference();

  index_ = theSet.index_;
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HSet<T>::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HSet<T>& OTC_HSet<T>::operator=(OTC_HSet<T> const& theSet)
{
  if (&theSet != this)
  {
    removeAll();

    rankFn_ = theSet.rankFn_;
    OTC_Iterator<T> theItems = 0;
    theItems = theSet.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (theItems.reset(); theItems.isValid(); theItems.next())
      add(theItems.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HSet<T>::add(T const& theItem, OTC_Replacement theFlag)
{
  int tmpRank = 0;
  int tmpHashValue = 0;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_Bucket<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theItem);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpAvailableIndex == -1)
        tmpAvailableIndex = tmpEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_Bucket<T>*)tmpEntry->item();

      tmpRank = rank(theItem,tmpIndexEntry->item());

      OTCLIB_ENSURE_FN((tmpRank != 0 || theFlag == OTCLIB_REPLACE),
       "OTC_HSet<T>::add(T const&, OTC_Replacement)",
       "item already exists");

      if (tmpRank == 0 && theFlag == OTCLIB_REPLACE)
	break;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  OTC_Bucket<T>* oldIndexEntry;
  oldIndexEntry = tmpIndexEntry;

  tmpIndexEntry = new OTC_Bucket<T>(theItem);
  OTCLIB_ASSERT_M(tmpIndexEntry != 0);

  list_->addLast(tmpIndexEntry);

  if (tmpEntry != 0 && tmpEntry->isOccupied() && tmpRank == 0)
  {
    oldIndexEntry->kill();
    index_->replace(tmpEntryIndex,tmpIndexEntry,tmpHashValue);
  }
  else
    index_->add(tmpAvailableIndex,tmpIndexEntry,tmpHashValue);
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HSet<T>::add(OTC_Iterator<T> theItems, OTC_Replacement theFlag)
{
  while (theItems.isValid())
  {
    add(theItems.item(),theFlag);

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HSet<T>::remove(T const& theItem, OTC_Protection theFlag)
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_Bucket<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theItem);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_Bucket<T>*)tmpEntry->item();
      tmpRank = rank(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
      {
	index_->remove(tmpEntryIndex);
	tmpIndexEntry->kill();
        return;
      }
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  OTCLIB_ENSURE_FN((theFlag == OTCLIB_UNSAFE),
   "OTC_HSet<T>::remove(T const&, OTC_Protection)",
   "item doesn't exist");
}

/* ------------------------------------------------------------------------- */
template<class T>
bool OTC_HSet<T>::contains(T const& theItem) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_Bucket<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theItem);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_Bucket<T>*)tmpEntry->item();
      tmpRank = rank(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        return true;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return false;
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_HSet<T>::item(T const& theItem) const
{
  T const* tmpItem = pItem(theItem);

  OTCLIB_ENSURE_FN((tmpItem != 0),
   "OTC_HSet<T>::item(T const&)",
   "invalid item");

  return *tmpItem;
}

/* ------------------------------------------------------------------------- */
template<class T>
T const* OTC_HSet<T>::pItem(T const& theItem) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_Bucket<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theItem);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_Bucket<T>*)tmpEntry->item();
      tmpRank = rank(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        return &tmpIndexEntry->item();
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_HSet<T>::pickItem() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_HSet<T>::pickItem()",
   "collection must not be empty");

  return ((OTC_Bucket<T>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Iterator<T> OTC_HSet<T>::items(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T>* theIter;
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_,theDirection,
   theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_HSet<T>::rank(T const& item1, T const& item2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(item1,item2) :
   OTC_RankActions<T>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HSET_C */
