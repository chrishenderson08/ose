/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcevent.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/event.hh"
#endif
#endif

#include <OTC/dispatch/event.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/debug/logstrm.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Event::mutex_;

/* ------------------------------------------------------------------------- */
OTC_Event::~OTC_Event()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_Event::deliver(OTC_EVAgent* theAgent)
{
  OTCLIB_ENSURE_FN((theAgent != 0),
   "OTC_Event::deliver(OTC_EVAgent*)",
   "invalid agent");

  theAgent->accept(this);
}

/* ------------------------------------------------------------------------- */
void OTC_Event::deliver(int theAgentId)
{
  OTC_EVAgent* theAgent;
  theAgent = OTC_EVAgent::lookup(theAgentId);

  if (theAgent != 0)
  {
    theAgent->accept(this);
  }
  else
  {
    if (OTC_Dispatcher::warningsEnabled())
    {
      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_WARNING);

      theLogger << "Operation failed" << EOL;
      theLogger << "Class: OTC_Event" << EOL;
      theLogger << "Method: deliver()" << EOL;
      theLogger << "Description: specified agent doesn't exist" << EOL;
      theLogger << "Result: event has been discarded" << EOL;
      theLogger << "Agent: " << theAgentId << EOL;
      theLogger << "Event: " << *this << EOL;
      theLogger << flush;
    }

    cancelSource(theAgentId);

    destroy();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Event::deliver(void (*theFunc)(OTC_Event*))
{
  OTCLIB_ENSURE_FN((theFunc != 0),
   "OTC_Event::deliver(void (*)(OTC_Event*))",
   "invalid function");

  (*theFunc)(this);
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Event::deliver(
 char const* theToAddress,
 char const* theFromAddress,
 char const* theSubject,
 char const* theHeader
)
{
  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(this,theToAddress,
   theFromAddress,theSubject,theHeader);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  theEnvelope->deliver();

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
void OTC_Event::queue(int theAgentId, int theType)
{
  OTC_EventJob* theJob;
  theJob = new OTC_EventJob(theAgentId,this);
  OTCLIB_ASSERT_M(theJob != 0);

  OTC_Dispatcher::schedule(theJob,theType);
}

/* ------------------------------------------------------------------------- */
void OTC_Event::queue(OTC_Iterator<int> theAgentIds, int theType)
{
  while (theAgentIds.isValid())
  {
    OTC_Event* theEvent;
    theEvent = clone();

    int theAgentId;
    theAgentId = theAgentIds.item();

    OTC_EventJob* theJob;
    theJob = new OTC_EventJob(theAgentId,theEvent);
    OTCLIB_ASSERT_M(theJob != 0);

    OTC_Dispatcher::schedule(theJob,theType);

    theAgentIds.next();
  }

  destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_Event::queue(OTC_EVAgent* theAgent, int theType)
{
  OTCLIB_ENSURE_FN((theAgent != 0),
   "OTC_Event::queue(OTC_EVAgent*, int)",
   "invalid agent");

  OTC_EventJob* theJob;
  theJob = new OTC_EventJob(theAgent->id(),this);
  OTCLIB_ASSERT_M(theJob != 0);

  OTC_Dispatcher::schedule(theJob,theType);
}

/* ------------------------------------------------------------------------- */
void OTC_Event::queue(void (*theFunc)(OTC_Event*), int theType)
{
  OTCLIB_ENSURE_FN((theFunc != 0),
   "OTC_Event::queue(OTC_EVAgent*, int)",
   "invalid function");

  OTC_EventJob* theJob;
  theJob = new OTC_EventJob(theFunc,this);
  OTCLIB_ASSERT_M(theJob != 0);

  OTC_Dispatcher::schedule(theJob,theType);
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Event::queue(
 char const* theToAddress,
 char const* theFromAddress,
 int theType
)
{
  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(this,theToAddress,theFromAddress);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  theEnvelope->queue(theType);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Event::queue(
 char const* theToAddress,
 char const* theFromAddress,
 char const* theSubject,
 char const* theHeader,
 int theType
)
{
  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(this,theToAddress,
   theFromAddress,theSubject,theHeader);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  theEnvelope->queue(theType);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTCEV_Message* OTC_Event::deflate() const
{
  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_Event* OTC_Event::clone()
{
  otclib_atomic_inc(refCount_,mutex_);

  return this;
}

/* ------------------------------------------------------------------------- */
void OTC_Event::destroy()
{
  if (otclib_atomic_dec(refCount_,mutex_) < 1)
    delete this;
}

/* ------------------------------------------------------------------------- */
void OTC_Event::cancelSource(int /* theAgentId */)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
