/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/otcrwmutex.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/thread/rwmutex.hh"
#endif
#endif

#include <OTC/thread/rwmutex.hh>

#if defined(OSE_WITH_THREADS)
#if !defined(NEED_PTHREAD_RWLOCK_INIT)
/* ------------------------------------------------------------------------- */
OTC_RWMutex::OTC_RWMutex()
{
  pthread_rwlock_init(&rwlock_,0);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_RWMutex::~OTC_RWMutex()
{
  valid_ = 0;
  pthread_rwlock_destroy(&rwlock_);
}

/* ------------------------------------------------------------------------- */
void OTC_RWMutex::readLock() const
{
  OTC_RWMutex* self = (OTC_RWMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_rwlock_rdlock(&self->rwlock_);
}

/* ------------------------------------------------------------------------- */
void OTC_RWMutex::writeLock() const
{
  OTC_RWMutex* self = (OTC_RWMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_rwlock_wrlock(&self->rwlock_);
}

/* ------------------------------------------------------------------------- */
void OTC_RWMutex::unlock() const
{
  OTC_RWMutex* self = (OTC_RWMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_rwlock_unlock(&self->rwlock_);
}

#else
/* ------------------------------------------------------------------------- */
OTC_RWMutex::OTC_RWMutex()
  : numReaders_(0),
    readers_(mutex_),
    numWriters_(0),
    writers_(mutex_),
    held_(0)
{
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_RWMutex::~OTC_RWMutex()
{
  valid_ = 0;
}

/* ------------------------------------------------------------------------- */
void OTC_RWMutex::readLock() const
{
  OTC_RWMutex* self = (OTC_RWMutex*)this;

  if (self->valid_ == 0)
    return;

  self->mutex_.lock();

  // while (self->held_ < 0 || self->held_ > 0)
  while (self->held_ < 0)
  {
    self->numReaders_++;
    self->readers_.wait();
    self->numReaders_--;
  }

  self->held_++;

  self->mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_RWMutex::writeLock() const
{
  OTC_RWMutex* self = (OTC_RWMutex*)this;

  if (self->valid_ == 0)
    return;

  self->mutex_.lock();

  while (self->held_ != 0)
  {
    self->numWriters_++;
    self->writers_.wait();
    self->numWriters_--;
  }

  self->held_ = -1;

  self->mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_RWMutex::unlock() const
{
  OTC_RWMutex* self = (OTC_RWMutex*)this;

  if (self->valid_ == 0)
    return;

  self->mutex_.lock();

  if (self->held_ > 0)
    self->held_--;
  else if (self->held_ == -1)
    self->held_ = 0;

  if (self->numWriters_ > 0)
    self->writers_.signal();
  else if (self->numReaders_ > 0)
    self->readers_.broadcast();

  self->mutex_.unlock();
}
#endif
#endif

/* ------------------------------------------------------------------------- */
