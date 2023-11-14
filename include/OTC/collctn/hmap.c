#ifndef OTC_COLLCTN_HMAP_C
#define OTC_COLLCTN_HMAP_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/hmap.c
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
OTC_HMap<T1,T2>::~OTC_HMap()
{
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_HMap<T1,T2>::OTC_HMap(OTC_HMap<T1,T2> const& theMap)
  : rankFn_(theMap.rankFn_)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  OTC_PairIterator<T1,T2> iter = 0;
  iter = theMap.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    add(iter.key(),iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_HMap<T1,T2>::OTC_HMap(int (*theRankFn)(T1 const&,T1 const&))
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
template<class T1, class T2>
OTC_HMap<T1,T2>::OTC_HMap(OTC_HMap<T1,T2> const& theMap, OTC_ShallowCopy)
  : rankFn_(theMap.rankFn_)
{
  list_ = theMap.list_;
  list_->reference();

  index_ = theMap.index_;
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_HMap<T1,T2>::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_HMap<T1,T2>& OTC_HMap<T1,T2>::operator=(OTC_HMap<T1,T2> const& theMap)
{
  if (&theMap != this)
  {
    removeAll();

    rankFn_ = theMap.rankFn_;
    OTC_PairIterator<T1,T2> iter = 0;
    iter = theMap.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      add(iter.key(),iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_HMap<T1,T2>::add(
 T1 const& theKey,
 T2 const& theItem,
 OTC_Replacement theFlag
)
{
  int tmpRank = 0;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
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
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();

      tmpRank = rank(theKey,tmpIndexEntry->key());

      OTCLIB_ENSURE_FN((tmpRank != 0 || theFlag == OTCLIB_REPLACE),
       "OTC_HMap<T1,T2>::add(T1 const&, T2 const&, OTC_Replacement)",
       "key already exists");

      if (tmpRank == 0 && theFlag == OTCLIB_REPLACE)
	break;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  OTC_PairBucket<T1,T2>* oldIndexEntry;
  oldIndexEntry = tmpIndexEntry;

  tmpIndexEntry = new OTC_PairBucket<T1,T2>(theKey,theItem);
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
template<class T1, class T2>
void OTC_HMap<T1,T2>::add(
 OTC_PairIterator<T1,T2> thePairs,
 OTC_Replacement theFlag
)
{
  while (thePairs.isValid())
  {
    add(thePairs.key(),thePairs.item(),theFlag);

    thePairs.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_HMap<T1,T2>::remove(T1 const& theKey, OTC_Protection theFlag)
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
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
   "OTC_HMap<T1,T2>::remove(T1 const&, OTC_Protection)",
   "key doesn't exist");
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_HMap<T1,T2>::contains(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        return true;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return false;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2& OTC_HMap<T1,T2>::item_(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        break;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  OTCLIB_ENSURE_FN((tmpEntry != 0 && tmpEntry->isOccupied()),
   "OTC_HMap<T1,T2>::item_()",
   "key does not exist");

  return tmpIndexEntry->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2* OTC_HMap<T1,T2>::pItem_(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
	return &tmpIndexEntry->item();
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_HMap<T1,T2>::pickKey() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_HMap<T1,T2>::pickKey()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const& OTC_HMap<T1,T2>::pickItem() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_HMap<T1,T2>::pickItem()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Iterator<T1> OTC_HMap<T1,T2>::keys(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T1>* theIter;
  theIter = new OTC_KeyCursor< T1,OTC_PairBucket<T1,T2> >(list_,
   theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Cursor<T2>* OTC_HMap<T1,T2>::items_(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T2>* theIter;
  theIter = new OTC_ItemCursor< T2,OTC_PairBucket<T1,T2> >(list_,
   theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairCursor<T1,T2>* OTC_HMap<T1,T2>::pairs_(
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
int OTC_HMap<T1,T2>::rank(T1 const& key1, T1 const& key2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(key1,key2) :
   OTC_RankActions<T1>::rank(key1,key2);
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HMAP_C */
