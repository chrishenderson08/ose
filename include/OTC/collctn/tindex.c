#ifndef OTC_COLLCTN_TINDEX_C
#define OTC_COLLCTN_TINDEX_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/tindex.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TIndex<T>::~OTC_TIndex()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
bool OTC_TIndex<T>::contains(T const& theKey) const
{
  OTC_TIndexEntry<T> const* tmpEntry;
  tmpEntry = entry_(theKey);

  if (tmpEntry != 0)
    return true;

  return false;
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_TIndex<T>::index(T const& theKey) const
{
  OTC_TIndexEntry<T> const* tmpEntry;
  tmpEntry = entry_(theKey);

  if (tmpEntry == 0)
    return -1;

  return tmpEntry->index();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TIndexEntry<T>* OTC_TIndex<T>::insert(T const& theKey)
{
  OTC_AVLNode* theNode = 0;
  OTC_AVLNode* theParent = 0;
  int theRank = 0;

  theNode = index_.root();
  if (theNode == 0)
  {
    theNode = new OTC_TIndexEntry<T>(theKey);
    OTCLIB_ASSERT_M(theNode != 0);
    index_.addRoot(theNode);
  }
  else
  {
    while (theNode != 0)
    {
      theParent = theNode;

      theRank = rank(theKey,((OTC_TIndexEntry<T>*)theNode)->key());
      if (theRank == 0)
	break;

      if (theRank < 0)
	theNode = theNode->left();
      else
	theNode = theNode->right();
    }

    if (theNode == 0)
    {
      theNode = new OTC_TIndexEntry<T>(theKey);
      OTCLIB_ASSERT_M(theNode != 0);
      if (theRank < 0)
	theParent->addBefore(theNode);
      else
	theParent->addAfter(theNode);
    }
  }

  return (OTC_TIndexEntry<T>*)theNode;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_TIndex<T>::removeAtIndex(u_int theIndex)
{
  OTC_TIndexEntry<T>* tmpEntry;
  tmpEntry = entryAtIndex_(theIndex);

  if (tmpEntry != 0)
    tmpEntry->removeEntry();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_TIndex<T>::remove(T const& theKey)
{
  OTC_TIndexEntry<T>* tmpEntry;
  tmpEntry = entry_(theKey);

  if (tmpEntry != 0)
    tmpEntry->removeEntry();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TIndexEntry<T>* OTC_TIndex<T>::entry_(T const& theKey) const
{
  int tmpRank;
  OTC_AVLNode* tmpNode;

  tmpNode = (OTC_AVLNode*)index_.root();

  while (tmpNode != 0)
  {
    tmpRank = rank(theKey,((OTC_TIndexEntry<T> const*)tmpNode)->key());
    if (tmpRank == 0)
      return (OTC_TIndexEntry<T>*)tmpNode;

    if (tmpRank < 0)
      tmpNode = tmpNode->left();
    else
      tmpNode = tmpNode->right();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_TIndexEntry<T>* OTC_TIndex<T>::entryAtIndex_(u_int theIndex) const
{
  OTC_AVLNode* tmpNode;
  tmpNode = (OTC_AVLNode*)index_.node(theIndex);

  if (tmpNode == 0)
    return 0;

  return (OTC_TIndexEntry<T>*)tmpNode;
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_TIndex<T>::rank(T const& item1, T const& item2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(item1,item2) :
   OTC_RankActions<T>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */

#endif
