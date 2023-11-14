/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/otcmutex.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/thread/mutex.hh"
#endif
#endif

#include <OTC/thread/mutex.hh>

#if defined(OSE_WITH_THREADS)
/* ------------------------------------------------------------------------- */
OTC_Mutex::OTC_Mutex()
{
#if defined(PTHREAD_MUTEX_RECURSIVE)
  pthread_mutexattr_init(&attr_);
  pthread_mutexattr_settype(&attr_,PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mutex_,&attr_);
#else
  pthread_mutex_init(&mutex_,0);
  thread_ = 0;
  nest_ = 0;
#endif

  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_Mutex::~OTC_Mutex()
{
  valid_ = 0;
  pthread_mutex_destroy(&mutex_);

#if defined(PTHREAD_MUTEX_RECURSIVE)
  pthread_mutexattr_destroy(&attr_);
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_Mutex::lock() const
{
  OTC_Mutex* self = (OTC_Mutex*)this;

  if (self->valid_ == 0)
    return;

#if defined(PTHREAD_MUTEX_RECURSIVE)
  pthread_mutex_lock(&self->mutex_);
#else
  pthread_t theThread;
 
  theThread = pthread_self();
 
  if (theThread == self->thread_)
  {
    self->nest_++;
  }
  else
  {
    if (pthread_mutex_lock(&self->mutex_) == 0)
    {
      self->thread_ = theThread;
      self->nest_ = 0;
    }
  }
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_Mutex::unlock() const
{
  OTC_Mutex* self = (OTC_Mutex*)this;

  if (self->valid_ == 0)
    return;

#if defined(PTHREAD_MUTEX_RECURSIVE)
  pthread_mutex_unlock(&self->mutex_);
#else
  if (self->nest_ > 0)
  {
    self->nest_--;
  }
  else
  {
    self->thread_ = 0;
    pthread_mutex_unlock(&self->mutex_);
  }
#endif
}
#endif

/* ------------------------------------------------------------------------- */
