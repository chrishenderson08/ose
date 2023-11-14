/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcepserver.cc
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
#pragma implementation "OTC/message/epserver.hh"
#endif
#endif

#include <OTC/message/epserver.hh>

#include <OTC/message/eplisten.hh>
#include <OTC/message/epintrfc.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EPServer");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_EPServer::~OTC_EPServer()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPServer::~OTC_EPServer()");

  if (stopTimer_ != 0)
  {
    OTCEV_Timeout::cancel(stopTimer_);

    stopTimer_ = 0;
    stopDelay_ = 0;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_EPServer::stop(int theDelay)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPServer::stop(int)");

  if (theDelay == 0)
  {
    if (listener_ != 0)
    {
      OTC_EPListener* theListener;
      theListener = listener_;

      listener_ = 0;

      close();

      if (stopTimer_ != 0)
      {
	OTCEV_Timeout::cancel(stopTimer_);

	stopTimer_ = 0;
	stopDelay_ = 0;
      }

      theListener->servers_.remove(localAddress(),OTCLIB_UNSAFE);
    }
  }
  else
  {
    if (stopTimer_ == 0 && stopDelay_ == 0)
    {
      OTCEV_Action::schedule(handler()->id(),OTCLIB_STANDARD_JOB,"CLOSE");

      if (theDelay > 0)
        stopTimer_ = OTCEV_Timeout::start(id(),theDelay,"STOP");

      stopDelay_ = theDelay;
    }
  }
}

/* ------------------------------------------------------------------------- */
OTC_EPServer::OTC_EPServer(
 char const* theInterface,
 char const* theProtocol,
 OTC_EPListener* theListener
)
  : OTC_EndPoint(OTCLIB_ENDPOINT_SERVER,theInterface,theProtocol),
    listener_(theListener),
    stopDelay_(0),
    stopTimer_(0)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_EPServer::OTC_EPServer(...)");

  OTCLIB_ENSURE_FN((theListener != 0),
   "OTC_EPServer::OTC_EPServer(...)",
   "invalid listener");

  OTC_EPServer* thisObject = this;
  theListener->servers_.add(localAddress(),thisObject);
}

/* ------------------------------------------------------------------------- */
void OTC_EPServer::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPServer::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Timeout* theTimeout;
    theTimeout = (OTCEV_Timeout*)theEvent;

    if (theTimeout->description() == "STOP")
    {
      if (theTimeout->timer() == stopTimer_)
      {
        stopDelay_ = 0;
        stopTimer_ = 0;

        stop(0);
      }
    }
  }

  theEvent->destroy();
}
/* ------------------------------------------------------------------------- */
