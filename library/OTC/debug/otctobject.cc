/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/otctobject.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/tobject.hh"
#endif
#endif

#include <OTC/debug/tobject.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_TObject::mutex_;
OTC_TObject* OTC_TObject::head_ = 0;

/* ------------------------------------------------------------------------- */
OTC_TObject::~OTC_TObject()
{
  unlink();
}

/* ------------------------------------------------------------------------- */
OTC_TObject::OTC_TObject()
 : next_(0), prev_(0)
{
  link();
}

/* ------------------------------------------------------------------------- */
void OTC_TObject::terminateAll()
{
  mutex_.lock();

  if (head_)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    while (head_)
    {
      head_->cleanup();
      head_ = head_->next_;
    }

    xxxMutex.release();
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_TObject::link()
{
  mutex_.lock();

  if (!head_)
  {
    head_ = this;
  }
  else
  {
    next_ = head_;
    next_->prev_ = this;
    head_ = this;
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_TObject::unlink()
{
  mutex_.lock();

  if (head_ == this)
  {
    head_ = next_;
  }
  else
  {
    prev_->next_ = next_;
    if (next_)
      next_->prev_ = prev_;
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
