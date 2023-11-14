#ifndef OTC_COLLCTN_ICURSOR_C
#define OTC_COLLCTN_ICURSOR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/icursor.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_ItemCursor<T,L>::~OTC_ItemCursor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_ItemCursor<T,L>::OTC_ItemCursor(
 OTC_LinkList* aList,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
 : OTC_LinkCursor(aList,theDirection,theProtection)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_ItemCursor<T,L>::OTC_ItemCursor(
 OTC_LinkList* aList,
 OTC_Linkable* theStart,
 OTC_Linkable* theEnd,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
 : OTC_LinkCursor(aList,theStart,theEnd,theDirection,theProtection)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_ItemCursor<T,L>::OTC_ItemCursor(OTC_ItemCursor<T,L> const& theIter)
 : OTC_LinkCursor(theIter)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
void OTC_ItemCursor<T,L>::reset()
{
  OTC_LinkCursor::reset();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
T& OTC_ItemCursor<T,L>::item()
{
  OTCLIB_ENSURE_FN((OTC_LinkCursor::isValid()),
   "OTC_ItemCursor<T,L>::item()",
   "no item under cursor");

  // return ((L*)OTC_LinkCursor::link())->item();

  OTC_Link* theLink;
  theLink = OTC_LinkCursor::link();

  L* theTLink;
  theTLink = (L*)theLink;

  T* theItem = (T*)&theTLink->item();

  return *theItem;
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
void OTC_ItemCursor<T,L>::next()
{
  OTC_LinkCursor::next();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
bool OTC_ItemCursor<T,L>::isValid() const
{
  return OTC_LinkCursor::isValid();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_Cursor<T>* OTC_ItemCursor<T,L>::clone()
{
  OTC_ItemCursor<T,L>* theIter;
  theIter = new OTC_ItemCursor<T,L>(*this);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ICURSOR_C */
