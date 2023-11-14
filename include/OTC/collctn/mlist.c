#ifndef OTC_COLLCTN_MLIST_C
#define OTC_COLLCTN_MLIST_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/mlist.c
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_MList<T>::OTC_MList(OTC_MList<T> const& theList)
{
  OTC_LinkIterator iter = theList.list_.list()->items(OTCLIB_UNSAFE);
  for (iter.resetFirst(); iter.isLink(); iter.next())
    addLast(((OTC_Bucket<T>*)iter.link())->item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_MList<T>& OTC_MList<T>::operator=(OTC_MList<T> const& theList)
{
  if (&theList != this)
  {
    removeAll();
    OTC_LinkIterator iter = theList.list_.list()->items(OTCLIB_UNSAFE);
    for (iter.resetFirst(); iter.isLink(); iter.next())
      addLast(((OTC_Bucket<T>*)iter.link())->item());
  }
  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_MList<T>::apply(
 OTC_Visitor<T>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_LinkIterator theIterator = list_.list()->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T& theItem = ((OTC_Bucket<T>*)theIterator.link())->item();
    progress = theApplicator.action(theItem);

    if (progress == OTCLIB_RESTART)
    {
      if (theDirection == OTCLIB_FORWARD)
	theIterator.resetFirst();
      else
	theIterator.resetLast();
    }
    else if (progress == OTCLIB_CONTINUE)
    {
      if (theDirection == OTCLIB_FORWARD)
	theIterator.next();
      else
	theIterator.prev();
    }
    else
      break;
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_MList<T>::apply(
 OTC_Worker<T>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
{
  OTC_LinkIterator theIterator = list_.list()->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T& theItem = ((OTC_Bucket<T>*)theIterator.link())->item();
    progress = theApplicator.action(theItem);

    if (progress == OTCLIB_RESTART)
    {
      if (theDirection == OTCLIB_FORWARD)
	theIterator.resetFirst();
      else
	theIterator.resetLast();
    }
    else if (progress == OTCLIB_CONTINUE)
    {
      if (theDirection == OTCLIB_FORWARD)
	theIterator.next();
      else
	theIterator.prev();
    }
    else
      break;
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Link* OTC_MList<T>::link_(T const& theItem)
{
  OTC_Bucket<T>* theBucket;
  theBucket = new OTC_Bucket<T>(theItem);
  OTCLIB_ASSERT_M(theBucket != 0);

  return theBucket;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_MLIST_C */
