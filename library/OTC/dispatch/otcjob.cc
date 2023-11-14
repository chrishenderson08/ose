/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcjob.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/job.hh"
#endif
#endif

#include <OTC/dispatch/job.hh>
#include <OTC/dispatch/dispatch.hh>

/* ------------------------------------------------------------------------- */
int OTC_Job::globTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTC_Job::~OTC_Job()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_Job::destroy()
{
  delete this;
}

/* ------------------------------------------------------------------------- */
void* OTC_Job::type() const
{
  return &globTypeId_;
}

/* ------------------------------------------------------------------------- */
void OTC_Job::schedule(OTC_JobType theType)
{
  OTC_Dispatcher::schedule(this,theType);
}

/* ------------------------------------------------------------------------- */
