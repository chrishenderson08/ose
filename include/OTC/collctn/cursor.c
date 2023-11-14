#ifndef OTC_COLLCTN_CURSOR_C
#define OTC_COLLCTN_CURSOR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/cursor.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Cursor<T>::OTC_Cursor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Cursor<T>::~OTC_Cursor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Cursor<T>::apply(
 OTC_Cursor<T>* theCursor,
 OTC_Visitor<T>& theApplicator
)
{
  theApplicator.start();

  if (theCursor != 0)
  {
    theCursor->reset();

    OTC_Progress progress = OTCLIB_CONTINUE;

    while (theCursor->isValid())
    {
      progress = theApplicator.action(theCursor->item());

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
template<class T>
void OTC_Cursor<T>::apply(
 OTC_Cursor<T>* theCursor,
 OTC_Worker<T>& theApplicator
)
{
  theApplicator.start();

  if (theCursor != 0)
  {
    theCursor->reset();

    OTC_Progress progress = OTCLIB_CONTINUE;

    while (theCursor->isValid())
    {
      progress = theApplicator.action(theCursor->item());

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
template<class T>
void OTC_Cursor<T>::assign(
 OTC_Cursor<T>*& theLeft,
 OTC_Cursor<T>* theRight
)
{
  if (theRight != 0)
    theRight->reference();
  theLeft = theRight;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Cursor<T>::replace(
 OTC_Cursor<T>*& theLeft,
 OTC_Cursor<T>* theRight
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

#endif /* OTC_COLLCTN_CURSOR_C */
