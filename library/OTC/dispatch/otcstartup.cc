/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcstartup.cc
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
#pragma implementation "OTC/dispatch/startup.hh"
#endif
#endif

#include <OTC/dispatch/startup.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/dispatch.hh>

#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */
int OTCEV_Startup::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Startup::~OTCEV_Startup()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Startup::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Startup::dump(ostream& outs) const
{
  outs << "<OTC> STARTUP";
}

/* ------------------------------------------------------------------------- */
void OTCEV_Startup::schedule(int theAgentId)
{
  OTCEV_Startup* theEvent;
  theEvent = new OTCEV_Startup;
  OTCLIB_ASSERT_M(theEvent != 0);

  theEvent->queue(theAgentId,OTCLIB_STANDARD_JOB);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Startup::deliverImmediately(int theAgentId)
{
  OTCEV_Startup* theEvent;
  theEvent = new OTCEV_Startup;
  OTCLIB_ASSERT_M(theEvent != 0);

  theEvent->deliver(theAgentId);
}

/* ------------------------------------------------------------------------- */
