/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcevhandle.cc
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
#pragma implementation "OTC/dispatch/evhandle.hh"
#endif
#endif

#include <OTC/dispatch/evhandle.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_EVHandlerCallback::~OTC_EVHandlerCallback()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EVHandlerCallback::execute(OTC_Event* theEvent)
{
  handler_->filter(theEvent);
}

/* ------------------------------------------------------------------------- */
OTC_EVCallback* OTC_EVHandlerCallback::clone()
{
  return this;
}

/* ------------------------------------------------------------------------- */
void OTC_EVHandlerCallback::destroy()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EVHandler");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_EVHandler::~OTC_EVHandler()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::~OTC_EVHandler()");

  if (threadPool_ != 0 && poolInternal_ != false)
  {
    threadPool_->shutdown();
    delete threadPool_;
  }
}

/* ------------------------------------------------------------------------- */
OTC_EVHandler::OTC_EVHandler(
 OTC_EVAgent* theAgent,
 u_int theNumThreads,
 u_int theQueueSize,
 u_int theThreshold
)
  : agent_(theAgent),
    threadPool_(0),
    poolInternal_(false),
    callback_(this)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_EVHandler::OTC_EVHandler(OTC_EVAgent*,u_int,u_int,u_int)");

  if (theNumThreads != 0)
  {
    threadPool_ = new OTC_EVThreadPool(
     theNumThreads,theQueueSize,theThreshold);
    OTCLIB_ASSERT_M(threadPool_ != 0);

    poolInternal_ = true;

    threadPool_->startup();
  }
}

/* ------------------------------------------------------------------------- */
OTC_EVHandler::OTC_EVHandler(
 OTC_EVAgent* theAgent,
 OTC_EVThreadPool* theThreadPool
)
  : agent_(theAgent),
    threadPool_(theThreadPool),
    poolInternal_(false),
    callback_(this)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_EVHandler::OTC_EVHandler(OTC_EVAgent*,OTC_EVThreadPool*)");
}

/* ------------------------------------------------------------------------- */
void OTC_EVHandler::suspend()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::suspend()");

  if (threadPool_ != 0 && poolInternal_ != false)
    threadPool_->suspend();
}

/* ------------------------------------------------------------------------- */
void OTC_EVHandler::resume()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::resume()");

  if (threadPool_ != 0 && poolInternal_ != false)
    threadPool_->resume();
}

/* ------------------------------------------------------------------------- */
void OTC_EVHandler::limit(u_int theNumThreads)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::limit()");

  if (threadPool_ != 0 && poolInternal_ != false)
    threadPool_->limit(theNumThreads);
}

/* ------------------------------------------------------------------------- */
u_int OTC_EVHandler::threadId() const
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::threadId()");

  if (threadPool_ != 0 && poolInternal_ != false)
    return threadPool_->threadId();

  return 0;
}

/* ------------------------------------------------------------------------- */
void OTC_EVHandler::shutdown()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::shutdown()");

  if (threadPool_ != 0 && poolInternal_ != false)
    threadPool_->shutdown();
}

/* ------------------------------------------------------------------------- */
OTC_Event* OTC_EVHandler::process(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::process(OTC_Event*)");

  accept(theEvent);

  return 0;
}

/* ------------------------------------------------------------------------- */
void OTC_EVHandler::accept(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::accept(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << *theEvent << endl;

  if (threadPool_ != 0)
    threadPool_->process(theEvent,&callback_);
  else
    filter(theEvent);
}

/* ------------------------------------------------------------------------- */
void OTC_EVHandler::filter(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVHandler::filter(OTC_Event*)");

  OTCLIB_ENSURE_FN((agent_ != 0),
   "OTC_EVHandler::filter(OTC_Event*)",
   "no agent to perform callback on");

  agent_->filter(theEvent);
}

/* ------------------------------------------------------------------------- */
