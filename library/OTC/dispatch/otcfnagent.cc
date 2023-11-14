/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcfnagent.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/fnagent.hh"
#endif
#endif

#include <OTC/dispatch/fnagent.hh>

/* ------------------------------------------------------------------------- */
OTC_FNAgent::~OTC_FNAgent()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_FNAgent::OTC_FNAgent(void (*theCallback)(OTC_Event*,int))
  : callback_(theCallback)
{
  OTCLIB_ENSURE_FN((theCallback != 0),
   "OTC_FNAgent::OTC_FNAgent(void (*)(OTC_Event*, int))",
   "callback invalid");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_FNAgent::handle(OTC_Event* theEvent)
{
  (*callback_)(theEvent,id());
}

/* ------------------------------------------------------------------------- */
