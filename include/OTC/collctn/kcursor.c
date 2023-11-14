#ifndef OTC_COLLCTN_KCURSOR_C
#define OTC_COLLCTN_KCURSOR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/kcursor.c
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
OTC_KeyCursor<T,L>::~OTC_KeyCursor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_KeyCursor<T,L>::OTC_KeyCursor(
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
OTC_KeyCursor<T,L>::OTC_KeyCursor(
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
OTC_KeyCursor<T,L>::OTC_KeyCursor(OTC_KeyCursor<T,L> const& theIter)
 : OTC_LinkCursor(theIter)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
void OTC_KeyCursor<T,L>::reset()
{
  OTC_LinkCursor::reset();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
T& OTC_KeyCursor<T,L>::item()
{
  OTCLIB_ENSURE_FN((OTC_LinkCursor::isValid()),
   "OTC_KeyCursor<T,L>::item()",
   "no item under cursor");

  // return ((L*)OTC_LinkCursor::link())->key();

  OTC_Link* theLink;
  theLink = OTC_LinkCursor::link();

  L* theTLink;
  theTLink = (L*)theLink;

  T* theKey = (T*)&theTLink->key();

  return *theKey;
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
void OTC_KeyCursor<T,L>::next()
{
  OTC_LinkCursor::next();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
bool OTC_KeyCursor<T,L>::isValid() const
{
  return OTC_LinkCursor::isValid();
}

/* ------------------------------------------------------------------------- */
template<class T, class L>
OTC_Cursor<T>* OTC_KeyCursor<T,L>::clone()
{
  OTC_KeyCursor<T,L>* theIter;
  theIter = new OTC_KeyCursor<T,L>(*this);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_KCURSOR_C */
