#ifndef OTC_DISPATCH_PLREADER_I
#define OTC_DISPATCH_PLREADER_I
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     dispatch/plreader.i
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/deque.hh>
#include <OTC/collctn/list.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/tmap.hh>
#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/prbucket.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kicursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
void operator>>=(OTC_PLReader theReader, OTC_Iterator<T>& theIterator)
{
  if (theReader.errorOccurred())
    return;

  OTC_ROPayload theObject;
  
  theObject = theReader.node();

  if (theObject.nodeType() != "array")
  {
    theReader.flagError();

    return;
  }

  theIterator = 0;

  OTC_LinkList* theList;
  theList = new OTC_LinkList;
  OTCLIB_ASSERT_M(theList != 0);

  theList->reference();

  OTC_ROPayload tmpObject;
  OTC_Boolean theStatus;

  T theValue;

  theStatus = OTCLIB_TRUE;

  tmpObject = theObject.firstChild();
  while (tmpObject.isValid())
  {
    (tmpObject,theStatus) >>= theValue;

    if (theStatus == OTCLIB_FALSE)
    {
      theReader.flagError();

      theList->unReference();

      return;
    }

    OTC_Bucket<T>* theBucket;
    theBucket = new OTC_Bucket<T>(theValue);
    OTCLIB_ASSERT_M(theBucket != 0);

    theList->addLast(theBucket);

    tmpObject = tmpObject.nextSibling();
  }

  OTC_ItemCursor< T,OTC_Bucket<T> >* theCursor;
  theCursor = new OTC_ItemCursor< T,OTC_Bucket<T> >(theList);
  OTCLIB_ASSERT_M(theCursor != 0);

  theIterator = theCursor;

  theList->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
void operator>>=(
 OTC_PLReader theReader,
 OTC_PairIterator<OTC_String,T>& theIterator
)
{
  if (theReader.errorOccurred())
    return;

  OTC_ROPayload theObject;
  
  theObject = theReader.node();

  if (theObject.nodeType() != "index")
  {
    theReader.flagError();

    return;
  }

  theIterator = 0;

  OTC_LinkList* theList;
  theList = new OTC_LinkList;
  OTCLIB_ASSERT_M(theList != 0);

  theList->reference();

  OTC_ROPayload tmpObject;
  OTC_Boolean theStatus;

  OTC_String theKey;
  T theValue;

  theStatus = OTCLIB_TRUE;

  tmpObject = theObject.firstChild();
  while (tmpObject.isValid())
  {
    (tmpObject,theStatus) >>= theValue;

    if (theStatus == OTCLIB_FALSE)
    {
      theReader.flagError();

      theList->unReference();

      return;
    }

    theKey = tmpObject.property("name").string();

    OTC_PairBucket<OTC_String,T>* theBucket;
    theBucket = new OTC_PairBucket<OTC_String,T>(theKey,theValue);
    OTCLIB_ASSERT_M(theBucket != 0);

    theList->addLast(theBucket);

    tmpObject = tmpObject.nextSibling();
  }

  OTC_KeyItemCursor< OTC_String,T,OTC_PairBucket<OTC_String,T> >* theCursor;
  theCursor = new OTC_KeyItemCursor< OTC_String,T,
   OTC_PairBucket<OTC_String,T> >(theList);
  OTCLIB_ASSERT_M(theCursor != 0);

  theIterator = theCursor;

  theList->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
void operator>>=(OTC_PLReader theReader, OTC_Deque<T>& theDeque)
{
  if (theReader.errorOccurred())
    return;

  OTC_ROPayload theObject;
  
  theObject = theReader.node();

  if (theObject.nodeType() != "array")
  {
    theReader.flagError();

    return;
  }

  theDeque.removeAll();

  OTC_ROPayload tmpObject;
  OTC_Boolean theStatus;

  T theValue;

  theStatus = OTCLIB_TRUE;

  tmpObject = theObject.firstChild();
  while (tmpObject.isValid())
  {
    (tmpObject,theStatus) >>= theValue;

    if (theStatus == OTCLIB_FALSE)
    {
      theReader.flagError();

      return;
    }

    theDeque.addLast(theValue);

    tmpObject = tmpObject.nextSibling();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void operator>>=(OTC_PLReader theReader, OTC_List<T>& theList)
{
  if (theReader.errorOccurred())
    return;

  OTC_ROPayload theObject;
  
  theObject = theReader.node();

  if (theObject.nodeType() != "array")
  {
    theReader.flagError();

    return;
  }

  theList.removeAll();

  OTC_ROPayload tmpObject;
  OTC_Boolean theStatus;

  T theValue;

  theStatus = OTCLIB_TRUE;

  tmpObject = theObject.firstChild();
  while (tmpObject.isValid())
  {
    (tmpObject,theStatus) >>= theValue;

    if (theStatus == OTCLIB_FALSE)
    {
      theReader.flagError();

      return;
    }

    theList.addLast(theValue);

    tmpObject = tmpObject.nextSibling();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void operator>>=(OTC_PLReader theReader, OTC_HMap<OTC_String,T>& theMap)
{
  if (theReader.errorOccurred())
    return;

  OTC_ROPayload theObject;
  
  theObject = theReader.node();

  if (theObject.nodeType() != "index")
  {
    theReader.flagError();

    return;
  }

  theMap.removeAll();

  OTC_ROPayload tmpObject;
  OTC_Boolean theStatus;

  OTC_String theKey;
  T theValue;

  theStatus = OTCLIB_TRUE;

  tmpObject = theObject.firstChild();
  while (tmpObject.isValid())
  {
    (tmpObject,theStatus) >>= theValue;

    if (theStatus == OTCLIB_FALSE)
    {
      theReader.flagError();

      return;
    }

    theKey = tmpObject.property("name").string();

    if (theMap.contains(theKey))
    {
      theReader.flagError();

      return;
    }

    theMap.add(theKey,theValue);

    tmpObject = tmpObject.nextSibling();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void operator>>=(OTC_PLReader theReader, OTC_TMap<OTC_String,T>& theMap)
{
  if (theReader.errorOccurred())
    return;

  OTC_ROPayload theObject;
  
  theObject = theReader.node();

  if (theObject.nodeType() != "index")
  {
    theReader.flagError();

    return;
  }

  theMap.removeAll();

  OTC_ROPayload tmpObject;
  OTC_Boolean theStatus;

  OTC_String theKey;
  T theValue;

  theStatus = OTCLIB_TRUE;

  tmpObject = theObject.firstChild();
  while (tmpObject.isValid())
  {
    (tmpObject,theStatus) >>= theValue;

    if (theStatus == OTCLIB_FALSE)
    {
      theReader.flagError();

      return;
    }

    theKey = tmpObject.property("name").string();

    if (theMap.contains(theKey))
    {
      theReader.flagError();

      return;
    }

    theMap.add(theKey,theValue);

    tmpObject = tmpObject.nextSibling();
  }
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_PLREADER_I */
