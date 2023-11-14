#ifndef OTC_COLLCTN_HUNIQMAP_C
#define OTC_COLLCTN_HUNIQMAP_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/huniqmap.c
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

#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/prbucket.hh>

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_HUniqMap<T1,T2>::~OTC_HUniqMap()
{
  keyIndex_->unReference();
  itemIndex_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_HUniqMap<T1,T2>::OTC_HUniqMap(OTC_HUniqMap<T1,T2> const& theMap)
  : keyRankFn_(theMap.keyRankFn_),
    itemRankFn_(theMap.itemRankFn_)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  keyIndex_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(keyIndex_ != 0);
  keyIndex_->reference();

  itemIndex_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(itemIndex_ != 0);
  itemIndex_->reference();

  OTC_PairIterator<T1,T2> iter = 0;
  iter = theMap.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    add(iter.key(),iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_HUniqMap<T1,T2>::OTC_HUniqMap(
 int (*theKeyRankFn)(T1 const&,T1 const&),
 int (*theItemRankFn)(T2 const&,T2 const&)
)
  : keyRankFn_(theKeyRankFn),
    itemRankFn_(theItemRankFn)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  keyIndex_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(keyIndex_ != 0);
  keyIndex_->reference();

  itemIndex_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(itemIndex_ != 0);
  itemIndex_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_HUniqMap<T1,T2>::OTC_HUniqMap(
 OTC_HUniqMap<T1,T2> const& theMap,
 OTC_ShallowCopy
)
  : keyRankFn_(theMap.keyRankFn_),
    itemRankFn_(theMap.itemRankFn_)
{
  list_ = theMap.list_;
  list_->reference();

  keyIndex_ = theMap.keyIndex_;
  keyIndex_->reference();

  itemIndex_ = theMap.itemIndex_;
  itemIndex_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_HUniqMap<T1,T2>::removeAll()
{
  keyIndex_->removeAll();
  itemIndex_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_HUniqMap<T1,T2>& OTC_HUniqMap<T1,T2>::operator=(
 OTC_HUniqMap<T1,T2> const& theMap
)
{
  if (&theMap != this)
  {
    removeAll();

    keyRankFn_ = theMap.keyRankFn_;
    itemRankFn_ = theMap.itemRankFn_;

    OTC_PairIterator<T1,T2> iter = 0;
    iter = theMap.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      add(iter.key(),iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_HUniqMap<T1,T2>::add(T1 const& theKey, T2 const& theItem)
{
  int tmpRank;
  u_int tmpIteration;
  u_int tmpStartIndex;

  int tmpKeyHashValue;
  u_int tmpKeyEntryIndex = 0;
  int tmpKeyAvailableIndex = -1;

  int tmpItemHashValue;
  u_int tmpItemEntryIndex = 0;
  int tmpItemAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpIteration = 0;
  tmpStartIndex = 0;

  tmpKeyHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = keyIndex_->searchHash(tmpKeyHashValue,
   tmpIteration,tmpStartIndex,tmpKeyEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpKeyAvailableIndex == -1)
        tmpKeyAvailableIndex = tmpKeyEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();

      tmpRank = rankKey(theKey,tmpIndexEntry->key());

      OTCLIB_ENSURE_FN((tmpRank != 0),
       "OTC_HUniqMap<T1,T2>::add(T1 const&, T2 const&)",
       "key already exists");
    }

    tmpEntry = keyIndex_->searchHash(tmpKeyHashValue,
     tmpIteration,tmpStartIndex,tmpKeyEntryIndex);
  }

  tmpIteration = 0;
  tmpStartIndex = 0;

  tmpItemHashValue = OTC_HashActions<T2>::hash(theItem);
  tmpEntry = itemIndex_->searchHash(tmpItemHashValue,
   tmpIteration,tmpStartIndex,tmpItemEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpItemAvailableIndex == -1)
        tmpItemAvailableIndex = tmpItemEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();

      tmpRank = rankItem(theItem,tmpIndexEntry->item());

      OTCLIB_ENSURE_FN((tmpRank != 0),
       "OTC_HUniqMap<T1,T2>::add(T1 const&, T2 const&)",
       "item already exists");
    }

    tmpEntry = itemIndex_->searchHash(tmpItemHashValue,
     tmpIteration,tmpStartIndex,tmpItemEntryIndex);
  }

  tmpIndexEntry = new OTC_PairBucket<T1,T2>(theKey,theItem);
  OTCLIB_ASSERT_M(tmpIndexEntry != 0);

  list_->addLast(tmpIndexEntry);
  keyIndex_->add(tmpKeyAvailableIndex,tmpIndexEntry,tmpKeyHashValue);
  itemIndex_->add(tmpItemAvailableIndex,tmpIndexEntry,tmpItemHashValue);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_HUniqMap<T1,T2>::add(OTC_PairIterator<T1,T2> thePairs)
{
  while (thePairs.isValid())
  {
    add(thePairs.key(),thePairs.item());

    thePairs.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_HUniqMap<T1,T2>::removeKey(
 T1 const& theKey,
 OTC_Protection theProtection
)
{
  int tmpRank;
  u_int tmpIteration;
  u_int tmpStartIndex;

  int tmpKeyHashValue;
  u_int tmpKeyEntryIndex = 0;
  int tmpKeyAvailableIndex = -1;

  int tmpItemHashValue;
  u_int tmpItemEntryIndex = 0;
  int tmpItemAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpIteration = 0;
  tmpStartIndex = 0;

  tmpKeyHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = keyIndex_->searchHash(tmpKeyHashValue,
   tmpIteration,tmpStartIndex,tmpKeyEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpKeyAvailableIndex == -1)
        tmpKeyAvailableIndex = tmpKeyEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();

      tmpRank = rankKey(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        break;
    }

    tmpEntry = keyIndex_->searchHash(tmpKeyHashValue,
     tmpIteration,tmpStartIndex,tmpKeyEntryIndex);
  }

  if (theProtection == OTCLIB_UNSAFE &&
   (tmpEntry == 0 || tmpEntry->isEmpty()))
  {
    return;
  }

  OTCLIB_ENSURE_FN((tmpEntry != 0 && tmpEntry->isOccupied()),
   "OTC_HUniqMap<T1,T2>::removeKey(T1 const&)",
   "key doesn't exist");

  tmpIteration = 0;
  tmpStartIndex = 0;

  T2 const& theItem = tmpIndexEntry->item();

  tmpItemHashValue = OTC_HashActions<T2>::hash(theItem);
  tmpEntry = itemIndex_->searchHash(tmpItemHashValue,
   tmpIteration,tmpStartIndex,tmpItemEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpItemAvailableIndex == -1)
        tmpItemAvailableIndex = tmpItemEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();

      tmpRank = rankItem(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        break;
    }

    tmpEntry = itemIndex_->searchHash(tmpItemHashValue,
     tmpIteration,tmpStartIndex,tmpItemEntryIndex);
  }

  OTCLIB_ASSERT(tmpEntry != 0);

  keyIndex_->remove(tmpKeyEntryIndex);
  itemIndex_->remove(tmpItemEntryIndex);
  tmpIndexEntry->kill();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_HUniqMap<T1,T2>::removeItem(
 T2 const& theItem,
 OTC_Protection theProtection
)
{
  int tmpRank;
  u_int tmpIteration;
  u_int tmpStartIndex;

  int tmpKeyHashValue;
  u_int tmpKeyEntryIndex = 0;
  int tmpKeyAvailableIndex = -1;

  int tmpItemHashValue;
  u_int tmpItemEntryIndex = 0;
  int tmpItemAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpIteration = 0;
  tmpStartIndex = 0;

  tmpItemHashValue = OTC_HashActions<T2>::hash(theItem);
  tmpEntry = itemIndex_->searchHash(tmpItemHashValue,
   tmpIteration,tmpStartIndex,tmpItemEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpItemAvailableIndex == -1)
        tmpItemAvailableIndex = tmpItemEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();

      tmpRank = rankItem(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        break;
    }

    tmpEntry = itemIndex_->searchHash(tmpItemHashValue,
     tmpIteration,tmpStartIndex,tmpItemEntryIndex);
  }

  if (theProtection == OTCLIB_UNSAFE && tmpEntry == 0)
    return;

  OTCLIB_ENSURE_FN((tmpEntry != 0),
   "OTC_HUniqMap<T1,T2>::removeItem(T2 const&)",
   "item doesn't exist");

  tmpIteration = 0;
  tmpStartIndex = 0;

  T1 const& theKey = tmpIndexEntry->key();

  tmpKeyHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = keyIndex_->searchHash(tmpKeyHashValue,
   tmpIteration,tmpStartIndex,tmpKeyEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpKeyAvailableIndex == -1)
        tmpKeyAvailableIndex = tmpKeyEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();

      tmpRank = rankKey(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        break;
    }

    tmpEntry = keyIndex_->searchHash(tmpKeyHashValue,
     tmpIteration,tmpStartIndex,tmpKeyEntryIndex);
  }

  OTCLIB_ASSERT(tmpEntry != 0);

  keyIndex_->remove(tmpKeyEntryIndex);
  itemIndex_->remove(tmpItemEntryIndex);
  tmpIndexEntry->kill();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_HUniqMap<T1,T2>::pickKey() const
{
  OTCLIB_ENSURE_FN((population() != 0),
   "OTC_HUniqMap<T1,T2>::pickKey()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const& OTC_HUniqMap<T1,T2>::pickItem() const
{
  OTCLIB_ENSURE_FN((population() != 0),
   "OTC_HUniqMap<T1,T2>::pickItem()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_HUniqMap<T1,T2>::containsKey(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = keyIndex_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rankKey(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        return true;
    }

    tmpEntry = keyIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return false;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_HUniqMap<T1,T2>::containsItem(T2 const& theItem) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T2>::hash(theItem);
  tmpEntry = itemIndex_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rankItem(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        return true;
    }

    tmpEntry = itemIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return false;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const& OTC_HUniqMap<T1,T2>::item(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = keyIndex_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rankKey(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        break;
    }

    tmpEntry = keyIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  OTCLIB_ENSURE_FN((tmpEntry != 0 && tmpEntry->isOccupied()),
   "OTC_HUniqMap<T1,T2>::item()",
   "key does not exist");

  return tmpIndexEntry->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const* OTC_HUniqMap<T1,T2>::pItem(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = keyIndex_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rankKey(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        return &tmpIndexEntry->item();
    }

    tmpEntry = keyIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_HUniqMap<T1,T2>::key(T2 const& theItem) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T2>::hash(theItem);
  tmpEntry = itemIndex_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rankItem(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        break;
    }

    tmpEntry = itemIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  OTCLIB_ENSURE_FN((tmpEntry != 0 && tmpEntry->isOccupied()),
   "OTC_HUniqMap<T1,T2>::key()",
   "item does not exist");

  return tmpIndexEntry->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const* OTC_HUniqMap<T1,T2>::pKey(T2 const& theItem) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T2>::hash(theItem);
  tmpEntry = itemIndex_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rankItem(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        return &tmpIndexEntry->key();
    }

    tmpEntry = itemIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Iterator<T1> OTC_HUniqMap<T1,T2>::keys(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T1>* theIter;
  theIter = new OTC_KeyCursor< T1,OTC_PairBucket<T1,T2> >(list_,theDirection,
   theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Iterator<T2> OTC_HUniqMap<T1,T2>::items(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T2>* theIter;
  theIter = new OTC_ItemCursor< T2,OTC_PairBucket<T1,T2> >(list_,theDirection,
   theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairIterator<T1,T2> OTC_HUniqMap<T1,T2>::pairs(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_PairCursor<T1,T2>* theIter;
  theIter = new OTC_KeyItemCursor< T1,T2,OTC_PairBucket<T1,T2> >(list_,
   theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_HUniqMap<T1,T2>::rankKey(T1 const& key1, T1 const& key2) const
{
  return (keyRankFn_ != 0) ? (*keyRankFn_)(key1,key2) :
   OTC_RankActions<T1>::rank(key1,key2);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_HUniqMap<T1,T2>::rankItem(T2 const& item1, T2 const& item2) const
{
  return (itemRankFn_ != 0) ? (*itemRankFn_)(item1,item2) :
   OTC_RankActions<T2>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HUNIQMAP_C */
