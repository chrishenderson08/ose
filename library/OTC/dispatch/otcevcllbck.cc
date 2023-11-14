/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcevcllbck.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/evcllbck.hh"
#endif
#endif

#include <OTC/dispatch/evcllbck.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EVCallback::mutex_;

/* ------------------------------------------------------------------------- */
OTC_EVCallback::~OTC_EVCallback()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_EVCallback* OTC_EVCallback::clone()
{
  otclib_atomic_inc(refCount_,mutex_);

  return this;
}

/* ------------------------------------------------------------------------- */
void OTC_EVCallback::destroy()
{
  if (otclib_atomic_dec(refCount_,mutex_) < 1)
    delete this;
}

/* ------------------------------------------------------------------------- */
