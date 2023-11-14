/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/otclinkiter.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/linkiter.hh"
#endif
#endif

#include <OTC/collctn/linkiter.hh>

/* ------------------------------------------------------------------------- */
OTC_LinkIterator::~OTC_LinkIterator()
{
  if (protection_ == OTCLIB_SAFE)
  {
    link_->unReference();
    start_->unReference();
    end_->unReference();
  }
}

/* ------------------------------------------------------------------------- */
OTC_LinkIterator::OTC_LinkIterator(
 OTC_Linkable* theStart,
 OTC_Linkable* theEnd,
 OTC_Protection theProtection
)
  : start_(theStart),
    end_(theEnd),
    link_(theStart),
    protection_(theProtection)
{
  OTCLIB_ENSURE_FN((theStart != 0 && theEnd != 0),
   "OTC_LinkIterator::OTC_LinkIterator()",
   "invalid anchor point");

  if (protection_ == OTCLIB_SAFE)
  {
    start_->reference();
    end_->reference();
    link_->reference();
  }
}

/* ------------------------------------------------------------------------- */
OTC_LinkIterator::OTC_LinkIterator(OTC_LinkIterator const& theIter)
  : start_(theIter.start_),
    end_(theIter.end_),
    link_(theIter.link_),
    protection_(theIter.protection_)
{
  if (protection_ == OTCLIB_SAFE)
  {
    start_->reference();
    end_->reference();
    link_->reference();
  }
}

/* ------------------------------------------------------------------------- */
OTC_LinkIterator& OTC_LinkIterator::operator=(OTC_LinkIterator const& theIter)
{
  if (theIter.protection_ == OTCLIB_SAFE)
  {
    theIter.start_->reference();
    theIter.end_->reference();
    theIter.link_->reference();
  }

  if (protection_ == OTCLIB_SAFE)
  {
    start_->unReference();
    end_->unReference();
    link_->unReference();
  }

  start_ = theIter.start_;
  end_ = theIter.end_;
  link_ = theIter.link_;
  protection_ = theIter.protection_;

  return *this;
}

/* ------------------------------------------------------------------------- */
void OTC_LinkIterator::next()
{
  OTCLIB_ASSERT(link_ != 0);

  if (link_ == end_)
    return;

  else
  {
    OTC_Linkable* aLink = link_->next();
    while (true)
    {
      OTCLIB_ASSERT(aLink != 0);

      if (aLink == end_)
	break;

      else if (aLink->isLink() && !aLink->isDead())
	break;

      else
	aLink = aLink->next();
    }
    locate(aLink);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_LinkIterator::prev()
{
  OTCLIB_ASSERT(link_ != 0);

  if (link_ == start_)
    return;

  else
  {
    OTC_Linkable* aLink = link_->prev();
    while (true)
    {
      OTCLIB_ASSERT(aLink != 0);

      if (aLink == start_)
	break;

      else if (aLink->isLink() && !aLink->isDead())
	break;

      else
	aLink = aLink->prev();
    }
    locate(aLink);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_LinkIterator::locate(OTC_Linkable* theLink)
{
  if (link_ != theLink)
  {
    if (protection_ == OTCLIB_SAFE)
    {
      theLink->reference();
      link_->unReference();
    }
    link_ = theLink;
  }
}

/* ------------------------------------------------------------------------- */
