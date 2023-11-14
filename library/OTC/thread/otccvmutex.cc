/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/otccvmutex.cc
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
#pragma implementation "OTC/thread/cvmutex.hh"
#endif
#endif

#include <OTC/thread/cvmutex.hh>

#if defined(OSE_WITH_THREADS)
/* ------------------------------------------------------------------------- */
OTC_CVMutex::OTC_CVMutex(OTC_NRMutex const& theMutex)
  : mutex_(theMutex)
{
  pthread_cond_init(&variable_,0);
  valid_ = 1;
}

/* ------------------------------------------------------------------------- */
OTC_CVMutex::~OTC_CVMutex()
{
  valid_ = 0;
  pthread_cond_destroy(&variable_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::wait() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_cond_wait(&self->variable_,(pthread_mutex_t*)&self->mutex_.mutex_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::signal() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_cond_signal(&self->variable_);
}

/* ------------------------------------------------------------------------- */
void OTC_CVMutex::broadcast() const
{
  OTC_CVMutex* self = (OTC_CVMutex*)this;

  if (self->valid_ == 0)
    return;

  pthread_cond_broadcast(&self->variable_);
}
#endif

/* ------------------------------------------------------------------------- */
