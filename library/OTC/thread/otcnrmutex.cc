/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/otcnrmutex.cc
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
#pragma implementation "OTC/thread/nrmutex.hh"
#endif
#endif

#include <OTC/thread/nrmutex.hh>

#if defined(OSE_WITH_THREADS)
/* ------------------------------------------------------------------------- */
OTC_NRMutex::OTC_NRMutex()
{
  pthread_mutex_init(&mutex_,0);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_NRMutex::~OTC_NRMutex()
{
  valid_ = 0;
  pthread_mutex_destroy(&mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_NRMutex::lock() const
{
  OTC_NRMutex* self = (OTC_NRMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_mutex_lock(&self->mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_NRMutex::unlock() const
{
  OTC_NRMutex* self = (OTC_NRMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_mutex_unlock(&self->mutex_);
}
#endif

/* ------------------------------------------------------------------------- */
