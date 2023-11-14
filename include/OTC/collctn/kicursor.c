#ifndef OTC_COLLCTN_KICURSOR_C
#define OTC_COLLCTN_KICURSOR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/kicursor.c
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
template<class T1, class T2, class L>
OTC_KeyItemCursor<T1,T2,L>::~OTC_KeyItemCursor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2, class L>
OTC_KeyItemCursor<T1,T2,L>::OTC_KeyItemCursor(
 OTC_LinkList* aList,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
 : OTC_LinkCursor(aList,theDirection,theProtection)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2, class L>
OTC_KeyItemCursor<T1,T2,L>::OTC_KeyItemCursor(
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
template<class T1, class T2, class L>
OTC_KeyItemCursor<T1,T2,L>::OTC_KeyItemCursor(
 OTC_KeyItemCursor<T1,T2,L> const& theIter
)
 : OTC_LinkCursor(theIter)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2, class L>
void OTC_KeyItemCursor<T1,T2,L>::reset()
{
  OTC_LinkCursor::reset();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2, class L>
T1& OTC_KeyItemCursor<T1,T2,L>::key()
{
  OTCLIB_ENSURE_FN((OTC_LinkCursor::isValid()),
   "OTC_KeyItemCursor<T1,T2,L>::key()",
   "no item under cursor");

  // return ((L*)OTC_LinkCursor::link())->key();

  OTC_Link* theLink;
  theLink = OTC_LinkCursor::link();

  L* theTLink;
  theTLink = (L*)theLink;

  T1* theKey = (T1*)&theTLink->key();

  return *theKey;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2, class L>
T2& OTC_KeyItemCursor<T1,T2,L>::item()
{
  OTCLIB_ENSURE_FN((OTC_LinkCursor::isValid()),
   "OTC_KeyItemCursor<T1,T2,L>::item()",
   "no item under cursor");

  // return ((L*)OTC_LinkCursor::link())->item();

  OTC_Link* theLink;
  theLink = OTC_LinkCursor::link();

  L* theTLink;
  theTLink = (L*)theLink;

  T2* theItem = (T2*)&theTLink->item();

  return *theItem;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2, class L>
void OTC_KeyItemCursor<T1,T2,L>::next()
{
  OTC_LinkCursor::next();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2, class L>
bool OTC_KeyItemCursor<T1,T2,L>::isValid() const
{
  return OTC_LinkCursor::isValid();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2, class L>
OTC_PairCursor<T1,T2>* OTC_KeyItemCursor<T1,T2,L>::clone()
{
  OTC_KeyItemCursor<T1,T2,L>* theIter;
  theIter = new OTC_KeyItemCursor<T1,T2,L>(*this);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_KICURSOR_C */
