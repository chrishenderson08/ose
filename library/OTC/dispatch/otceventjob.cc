/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otceventjob.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/eventjob.hh"
#endif
#endif

#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/event.hh>

/* ------------------------------------------------------------------------- */
OTC_EventJob::~OTC_EventJob()
{
  if (event_ != 0)
    event_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_EventJob::OTC_EventJob(int theAgentId, OTC_Event* theEvent)
  : agentId_(theAgentId), func_(0), event_(theEvent)
{
  OTCLIB_ENSURE_FN((theAgentId != 0),
   "OTC_EventJob::OTC_EventJob(int, OTC_Event*)",
   "invalid agent id");

  OTCLIB_ENSURE_FN((theEvent != 0),
   "OTC_EventJob::OTC_EventJob(int, OTC_Event*)",
   "invalid event");
}

/* ------------------------------------------------------------------------- */
OTC_EventJob::OTC_EventJob(void (*theFunc)(OTC_Event*), OTC_Event* theEvent)
  : agentId_(0), func_(theFunc), event_(theEvent)
{
  OTCLIB_ENSURE_FN((theFunc != 0),
   "OTC_EventJob::OTC_EventJob(void (*)(OTC_Event*), OTC_Event*)",
   "invalid function");

  OTCLIB_ENSURE_FN((theEvent != 0),
   "OTC_EventJob::OTC_EventJob(void (*)(OTC_Event*), OTC_Event*)",
   "invalid event");
}

/* ------------------------------------------------------------------------- */
void OTC_EventJob::execute()
{
  OTCLIB_ENSURE_FN((event_ != 0),
   "OTC_EventJob::execute()",
   "invalid event");

  if (func_ != 0)
    event_->deliver(func_);
  else
    event_->deliver(agentId_);

  event_ = 0;
}

/* ------------------------------------------------------------------------- */
