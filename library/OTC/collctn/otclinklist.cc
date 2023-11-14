/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/otclinklist.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/linklist.hh"
#endif
#endif

#include <OTC/collctn/linklist.hh>

/* ------------------------------------------------------------------------- */
OTC_LinkList::OTC_LinkList()
{
  start_ = new OTC_Anchor;
  OTCLIB_ASSERT_M(start_ != 0);

  end_ = new OTC_Anchor;
  OTCLIB_ASSERT_M(end_ != 0);

  start_->addAfter(end_);
}

/* ------------------------------------------------------------------------- */
OTC_LinkList::~OTC_LinkList()
{
  removeAll();
  start_->kill();
  end_->kill();
}

/* ------------------------------------------------------------------------- */
void OTC_LinkList::removeAll()
{
  OTC_Linkable* theLink = start_->next();
  while (theLink != end_)
  {
    OTCLIB_ASSERT(theLink != 0);

    OTC_Linkable* nextLink = theLink->next();
    if (!theLink->isDead())
      theLink->kill();
    theLink = nextLink;
  }
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_LinkList::first() const
{
  OTC_Linkable* theLink = start_->next();
  while (theLink != (OTC_Linkable*)end_)
  {
    OTCLIB_ASSERT(theLink != 0);

    if (theLink->isLink() && !theLink->isDead())
      return (OTC_Link*)theLink;

    theLink = theLink->next();
  }
  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_LinkList::last() const
{
  OTC_Linkable* theLink = end_->prev();
  while (theLink != (OTC_Linkable*)start_)
  {
    OTCLIB_ASSERT(theLink != 0);

    if (theLink->isLink() && !theLink->isDead())
      return (OTC_Link*)theLink;

    theLink = theLink->prev();
  }
  return 0;
}

/* ------------------------------------------------------------------------- */
u_int OTC_LinkList::population() const
{
  u_int thePopulation = 0;

  OTC_Linkable* theLink = start_->next();
  while (theLink != (OTC_Linkable*)end_)
  {
    OTCLIB_ASSERT(theLink != 0);

    if (theLink->isLink() && !theLink->isDead())
      thePopulation++;

    theLink = theLink->next();
  }
  return thePopulation;
}

/* ------------------------------------------------------------------------- */
bool OTC_LinkList::isEmpty() const
{
  OTC_Linkable* theLink = start_->next();
  while (theLink != (OTC_Linkable*)end_)
  {
    OTCLIB_ASSERT(theLink != 0);

    if (theLink->isLink() && !theLink->isDead())
      return false;

    theLink = theLink->next();
  }
  return true;
}

/* ------------------------------------------------------------------------- */
void OTC_LinkList::removeFirst()
{
  OTC_Link* theLink;
  theLink = first();

  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_LinkList::removeFirst()",
   "list is empty");

  theLink->kill();
}

/* ------------------------------------------------------------------------- */
void OTC_LinkList::removeLast()
{
  OTC_Link* theLink;
  theLink = last();

  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_LinkList::removeLast()",
   "list is empty");

  theLink->kill();
}

/* ------------------------------------------------------------------------- */
