#ifndef OTC_COLLCTN_PRCURSOR_C
#define OTC_COLLCTN_PRCURSOR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/prcursor.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairCursor<T1,T2>::OTC_PairCursor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairCursor<T1,T2>::~OTC_PairCursor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_PairCursor<T1,T2>::apply(
 OTC_PairCursor<T1,T2>* theCursor,
 OTC_PairVisitor<T1,T2>& theApplicator
)
{
  theApplicator.start();

  if (theCursor != 0)
  {
    theCursor->reset();

    OTC_Progress progress = OTCLIB_CONTINUE;

    while (theCursor->isValid())
    {
      progress = theApplicator.action(theCursor->key(),theCursor->item());

      if (progress == OTCLIB_RESTART)
        theCursor->reset();
      else if (progress == OTCLIB_CONTINUE)
        theCursor->next();
      else
        break;
    }
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_PairCursor<T1,T2>::apply(
 OTC_PairCursor<T1,T2>* theCursor,
 OTC_PairWorker<T1,T2>& theApplicator
)
{
  theApplicator.start();

  if (theCursor != 0)
  {
    theCursor->reset();

    OTC_Progress progress = OTCLIB_CONTINUE;

    while (theCursor->isValid())
    {
      progress = theApplicator.action(theCursor->key(),theCursor->item());

      if (progress == OTCLIB_RESTART)
        theCursor->reset();
      else if (progress == OTCLIB_CONTINUE)
        theCursor->next();
      else
        break;
    }
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_PairCursor<T1,T2>::assign(
 OTC_PairCursor<T1,T2>*& theLeft,
 OTC_PairCursor<T1,T2>* theRight
)
{
  if (theRight != 0)
    theRight->reference();
  theLeft = theRight;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_PairCursor<T1,T2>::replace(
 OTC_PairCursor<T1,T2>*& theLeft,
 OTC_PairCursor<T1,T2>* theRight
)
{
  if (theLeft != theRight)
  {
    if (theRight != 0)
      theRight->reference();
    if (theLeft != 0)
      theLeft->unReference();
  }
  theLeft = theRight;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRCURSOR_C */
