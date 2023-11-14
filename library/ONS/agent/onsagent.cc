/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onsagent.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "ONS/agent/agent.hh"
#endif
#endif

#include <ONS/agent/agent.hh>

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/evreaper.hh>
#include <OTC/system/program.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("ONS_Agent");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}
 
#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
ONS_Agent::~ONS_Agent()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::~ONS_Agent()");

  OTCEV_IOEvent::unsubscribeAgent(id());
  OTCEV_Action::cancelAgent(id());
  OTCEV_Timeout::cancelAgent(id());
  OTCEV_Alarm::cancelAgent(id());
  OTCEV_Signal::unsubscribeAgent(id());
  OTCEV_Shutdown::unsubscribe(id());

  if (thread_ != 0)
  {
    thread_->shutdown();
 
    delete thread_;
  }
}

/* ------------------------------------------------------------------------- */
ONS_Agent::ONS_Agent()
  : thread_(0)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::ONS_Agent()");
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::activateThreading(u_int theNumThreads)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::activateThreading()");

  if (theNumThreads == 0)
    return;

  // If threading is not compiled into the
  // library don't even bother with creating
  // the thread pool in the first place.
  // Also no need to allocate and contexts.

#if defined(OSE_WITH_THREADS)
  allocateContexts(theNumThreads);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (thread_ == 0)
  {
    thread_ = new OTC_EVHandler(this,theNumThreads,0);
    OTCLIB_ASSERT_M(thread_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();
#endif
}

/* ------------------------------------------------------------------------- */
bool ONS_Agent::threadingEnabled() const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::threadingEnabled()");

  bool theResult;
  theResult = false;

  mutex_.lock();

  if (thread_ != 0)
    theResult = true;

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
u_int ONS_Agent::threadingContext() const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::threadingContext()");

  u_int theContext;

  mutex_.lock();

  if (thread_ != 0)
    theContext = thread_->threadId();
  else
    theContext = 0;

  mutex_.unlock();

  return theContext;
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::suspendThreading()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::suspendThreading()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (thread_ != 0)
    thread_->suspend();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::resumeThreading()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::resumeThreading()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (thread_ != 0)
    thread_->resume();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::limitThreading(u_int theNumThreads)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::limitThreading()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (thread_ != 0)
    thread_->limit(theNumThreads);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::shutdownThreading()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::shutdownThreading()");

  // Must make sure we do not attempt to
  // shutdown the threads from inside the mutex
  // lock because an active thread may try to
  // do something which requires the lock while
  // we are waiting for it to shutdown.

  OTC_EVHandler* theHandler;

  mutex_.lock();

  theHandler = thread_;

  mutex_.unlock();

  if (theHandler != 0)
    theHandler->shutdown();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::allocateContexts(u_int theNumThreads)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::allocateContexts()");
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::scheduleStartup()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::scheduleStartup()");

  OTCEV_Startup::schedule(id());
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::unsubscribeStartup()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::unsubscribeStartup()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  agentCallbacks_.remove("STARTUP",OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::scheduleShutdown(double theDelay)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::scheduleShutdown()");

  OTCLIB_TRACER(MODULE) << "delay = " << theDelay << endl;

  OTCEV_Shutdown::schedule(u_int(1000*theDelay));
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::suspendShutdown()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::suspendShutdown()");

  OTCEV_Shutdown::suspend();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::resumeShutdown()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::resumeShutdown()");

  OTCEV_Shutdown::resume();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::unsubscribeShutdown()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::unsubscribeShutdown()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Shutdown::unsubscribe(id());

  agentCallbacks_.remove("SHUTDOWN",OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::unsubscribeSignal(int theSignal)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::unsubscribeSignal()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Signal::unsubscribe(id(),theSignal);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "SIGNAL";
  tmpKey += EOS;

  char tmpBuffer[32];
  sprintf(tmpBuffer,"%d",theSignal);

  tmpKey += tmpBuffer;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::unsubscribeAllSignals()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::unsubscribeAllSignals()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Signal::unsubscribeAgent(id());

  OTC_Iterator<OTC_String> theKeys;
  theKeys = agentCallbacks_.keys();

  while (theKeys.isValid())
  {
    if (theKeys.item().index("SIGNAL") == 0)
      agentCallbacks_.remove(theKeys.item());

    theKeys.next();
  }

  theKeys = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::cancelAlarm(char const* theAlarm)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::cancelAlarm()");

  if (theAlarm == 0)
    theAlarm = "";

  OTCLIB_TRACER(MODULE) << "alarm = " << theAlarm << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Alarm::cancelAgent(id(),theAlarm,-1);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "ALARM";
  tmpKey += EOS;
  tmpKey += theAlarm;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::cancelAllAlarms()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::cancelAllAlarms()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Alarm::cancelAgent(id());

  OTC_Iterator<OTC_String> theKeys;
  theKeys = agentCallbacks_.keys();

  while (theKeys.isValid())
  {
    if (theKeys.item().index("ALARM") == 0)
      agentCallbacks_.remove(theKeys.item());

    theKeys.next();
  }

  theKeys = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool ONS_Agent::alarmPending(char const* theAlarm) const
{
  bool theResult;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "ALARM";
  tmpKey += EOS;
  tmpKey += theAlarm;

  theResult = agentCallbacks_.contains(tmpKey);

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::cancelTimer(char const* theTimer)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::cancelTimer()");

  if (theTimer == 0)
    theTimer = "";

  OTCLIB_TRACER(MODULE) << "timer = " << theTimer << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Timeout::cancelAgent(id(),theTimer,-1);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "TIMEOUT";
  tmpKey += EOS;
  tmpKey += theTimer;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::cancelAllTimers()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::cancelAllTimers()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Timeout::cancelAgent(id());

  OTC_Iterator<OTC_String> theKeys;
  theKeys = agentCallbacks_.keys();

  while (theKeys.isValid())
  {
    if (theKeys.item().index("TIMEOUT") == 0)
      agentCallbacks_.remove(theKeys.item());

    theKeys.next();
  }

  theKeys = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool ONS_Agent::timerPending(char const* theTimer) const
{
  bool theResult;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "TIMEOUT";
  tmpKey += EOS;
  tmpKey += theTimer;

  theResult = agentCallbacks_.contains(tmpKey);

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::triggerAction(
 char const* theAction,
 OTC_JobType theType
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::triggerAction()");

  OTCEV_Action::schedule(id(),theType,theAction,-1);
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::cancelAction(char const* theAction)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::cancelAction()");

  if (theAction == 0)
    theAction = "";

  OTCLIB_TRACER(MODULE) << "action = " << theAction << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Action::cancelAgent(id(),theAction,-1);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "ACTION-JOB";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-CRON";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-TRANSIENT";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-PERSISTENT";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::cancelAllActions()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::cancelAllActions()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Action::cancelAgent(id());

  OTC_Iterator<OTC_String> theKeys;
  theKeys = agentCallbacks_.keys();

  while (theKeys.isValid())
  {
    if (theKeys.item().index("ACTION") == 0)
    {
      agentCallbacks_.remove(theKeys.item());
    }

    theKeys.next();
  }

  theKeys = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::unsubscribeSocket(int theSocket, int theEvents)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::unsubscribesocket()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_IOEvent::unsubscribe(id(),theSocket,theEvents);

  char tmpBuffer[32];
  sprintf(tmpBuffer,"%d",theSocket);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  if (theEvents & OTCLIB_POLLIN)
  {
    tmpKey = "SOCKET-IN";
    tmpKey += EOS;
    tmpKey += tmpBuffer;

    agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);
  }
  else if (theEvents & OTCLIB_POLLPRI)
  {
    tmpKey = "SOCKET-PRIORITY";
    tmpKey += EOS;
    tmpKey += tmpBuffer;

    agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);
  }
  else if (theEvents & OTCLIB_POLLOUT)
  {
    tmpKey = "SOCKET-OUT";
    tmpKey += EOS;
    tmpKey += tmpBuffer;

    agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::unsubscribeAllSockets()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::unsubscribeAllSockets()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_IOEvent::unsubscribeAgent(id());

  OTC_Iterator<OTC_String> theKeys;
  theKeys = agentCallbacks_.keys();

  while (theKeys.isValid())
  {
    if (theKeys.item().index("SOCKET") == 0)
      agentCallbacks_.remove(theKeys.item());

    theKeys.next();
  }

  theKeys = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::execute(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::execute()");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (thread_ != 0)
    thread_->process(theEvent);
  else
    filter(theEvent);
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::filter(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::filter()");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  OTC_EventReaper xxxEvent;

  xxxEvent.grab(theEvent);

  if (theEvent->type() == OTCEV_Startup::typeId())
  {
    OTCEV_Startup* theStartup;
    theStartup = (OTCEV_Startup*)theEvent;

    handleStartup(theStartup);
  }
  else if (theEvent->type() == OTCEV_Shutdown::typeId())
  {
    OTCEV_Shutdown* theShutdown;
    theShutdown = (OTCEV_Shutdown*)theEvent;

    handleShutdown(theShutdown);
  }
  else if (theEvent->type() == OTCEV_Signal::typeId())
  {
    OTCEV_Signal* theSignal;
    theSignal = (OTCEV_Signal*)theEvent;

    handleSignal(theSignal);
  }
  else if (theEvent->type() == OTCEV_Alarm::typeId())
  {
    OTCEV_Alarm* theAlarm;
    theAlarm = (OTCEV_Alarm*)theEvent;

    handleAlarm(theAlarm);
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Timeout* theTimeout;
    theTimeout = (OTCEV_Timeout*)theEvent;

    handleTimeout(theTimeout);
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction;
    theAction = (OTCEV_Action*)theEvent;

    handleAction(theAction);
  }
  else if (theEvent->type() == OTCEV_IOEvent::typeId())
  {
    OTCEV_IOEvent* theIOEvent;
    theIOEvent = (OTCEV_IOEvent*)theEvent;

    handleSocket(theIOEvent);
  }
  else
  {
    xxxEvent.release();

    OTC_EVAgent::filter(theEvent);

    return;
  }

  xxxEvent.release();

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::handle()");

  if (theEvent != 0)
    theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::handleStartup(OTCEV_Startup* theStartup)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::handleStartup()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (agentCallbacks_.contains("STARTUP"))
    {
      theCallback = agentCallbacks_.item("STARTUP");
      theCallback->reference();

      agentCallbacks_.remove("STARTUP");
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
      theCallback->execute(theStartup,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Agent" << EOL;
    theLogger << "Method: handleStartup()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::handleShutdown(OTCEV_Shutdown* theShutdown)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::handleShutdown()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (agentCallbacks_.contains("SHUTDOWN"))
      theCallback = agentCallbacks_.item("SHUTDOWN");

    if (theCallback != 0)
      theCallback->reference();

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
      theCallback->execute(theShutdown,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Agent" << EOL;
    theLogger << "Method: handleShutdown()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::handleSignal(OTCEV_Signal* theSignal)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::handleSignal()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_Capacity tmpCapacity(31);
    OTC_String tmpKey(tmpCapacity);

    tmpKey += "SIGNAL";
    tmpKey += EOS;

    char tmpBuffer[32];
    sprintf(tmpBuffer,"%d",theSignal->signal());

    tmpKey += tmpBuffer;

    if (agentCallbacks_.contains(tmpKey))
    {
      theCallback = agentCallbacks_.item(tmpKey);
      theCallback->reference();
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
      theCallback->execute(theSignal,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Agent" << EOL;
    theLogger << "Method: handleSignal()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::handleAlarm(OTCEV_Alarm* theAlarm)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::handleAlarm()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_Capacity tmpCapacity(31);
    OTC_String tmpKey(tmpCapacity);

    tmpKey += "ALARM";
    tmpKey += EOS;
    tmpKey += theAlarm->description();

    if (agentCallbacks_.contains(tmpKey))
    {
      theCallback = agentCallbacks_.item(tmpKey);
      theCallback->reference();

      agentCallbacks_.remove(tmpKey);
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
      theCallback->execute(theAlarm,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Agent" << EOL;
    theLogger << "Method: handleAlarm()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::handleTimeout(OTCEV_Timeout* theTimeout)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::handleTimeout()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_Capacity tmpCapacity(31);
    OTC_String tmpKey(tmpCapacity);

    tmpKey += "TIMEOUT";
    tmpKey += EOS;
    tmpKey += theTimeout->description();

    if (agentCallbacks_.contains(tmpKey))
    {
      theCallback = agentCallbacks_.item(tmpKey);
      theCallback->reference();

      agentCallbacks_.remove(tmpKey);
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
      theCallback->execute(theTimeout,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Agent" << EOL;
    theLogger << "Method: handleTimeout()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::handleAction(OTCEV_Action* theAction)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::handleAction()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_Capacity tmpCapacity(31);
    OTC_String tmpKey(tmpCapacity);

    tmpKey = "ACTION-JOB";
    tmpKey += EOS;
    tmpKey += theAction->description();

    if (agentCallbacks_.contains(tmpKey))
    {
      theCallback = agentCallbacks_.item(tmpKey);
      theCallback->reference();
      agentCallbacks_.remove(tmpKey);
    }

    if (theCallback == 0)
    {
      tmpKey = "ACTION-CRON";
      tmpKey += EOS;
      tmpKey += theAction->description();

      if (agentCallbacks_.contains(tmpKey))
      {
	theCallback = agentCallbacks_.item(tmpKey);
	theCallback->reference();
      }
    }

    if (theCallback == 0)
    {
      tmpKey = "ACTION-TRANSIENT";
      tmpKey += EOS;
      tmpKey += theAction->description();

      if (agentCallbacks_.contains(tmpKey))
      {
	theCallback = agentCallbacks_.item(tmpKey);
	theCallback->reference();
	agentCallbacks_.remove(tmpKey);
      }
    }

    if (theCallback == 0)
    {
      tmpKey = "ACTION-PERSISTENT";
      tmpKey += EOS;
      tmpKey += theAction->description();

      if (agentCallbacks_.contains(tmpKey))
      {
	theCallback = agentCallbacks_.item(tmpKey);
	theCallback->reference();
      }
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
      theCallback->execute(theAction,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Agent" << EOL;
    theLogger << "Method: handleAction()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::handleSocket(OTCEV_IOEvent* theIOEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::handleSocket()");

  ONS_EventCallback* theOutCallback = 0;
  ONS_EventCallback* thePriCallback = 0;
  ONS_EventCallback* theInCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    int tmpEvents = theIOEvent->events();

    char tmpBuffer[32];
    sprintf(tmpBuffer,"%d",theIOEvent->fd());

    OTC_Capacity tmpCapacity(31);
    OTC_String tmpKey(tmpCapacity);

    if (tmpEvents & OTCLIB_POLLOUT)
    {
      tmpKey = "SOCKET-OUT";
      tmpKey += EOS;
      tmpKey += tmpBuffer;

      if (agentCallbacks_.contains(tmpKey))
      {
	theOutCallback = agentCallbacks_.item(tmpKey);
	theOutCallback->reference();
      }
    }

    tmpEvents &= ~OTCLIB_POLLOUT;

    if (tmpEvents & OTCLIB_POLLPRI)
    {
      tmpKey = "SOCKET-PRIORITY";
      tmpKey += EOS;
      tmpKey += tmpBuffer;

      if (agentCallbacks_.contains(tmpKey))
      {
	thePriCallback = agentCallbacks_.item(tmpKey);
	thePriCallback->reference();
      }
    }

    tmpEvents &= ~OTCLIB_POLLPRI;

    if (tmpEvents != 0)
    {
      tmpKey = "SOCKET-IN";
      tmpKey += EOS;
      tmpKey += tmpBuffer;

      if (agentCallbacks_.contains(tmpKey))
      {
	theInCallback = agentCallbacks_.item(tmpKey);
	theInCallback->reference();
      }
    }

    xxxMutex.release();

    mutex_.unlock();

    if (thePriCallback != 0)
    {
      OTCEV_IOEvent theEvent(theIOEvent->fd(),OTCLIB_POLLPRI);

      thePriCallback->execute(&theEvent,0);
    }

    if (theInCallback != 0)
    {
      OTCEV_IOEvent theEvent(theIOEvent->fd(),tmpEvents);

      theInCallback->execute(&theEvent,0);
    }

    if (theOutCallback != 0)
    {
      OTCEV_IOEvent theEvent(theIOEvent->fd(),OTCLIB_POLLOUT);

      theOutCallback->execute(&theEvent,0);
    }
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Agent" << EOL;
    theLogger << "Method: handleSocket()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theInCallback != 0)
    theInCallback->unReference();
  if (thePriCallback != 0)
    thePriCallback->unReference();
  if (theOutCallback != 0)
    theOutCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::subscribeStartup_(ONS_EventCallback* theCallback)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::subscribeStartup_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::subscribeStartup_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  agentCallbacks_.add("STARTUP",theCallback,OTCLIB_REPLACE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::subscribeShutdown_(ONS_EventCallback* theCallback)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::subscribeShutdown_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::subscribeShutdown_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  agentCallbacks_.add("SHUTDOWN",theCallback,OTCLIB_REPLACE);

  OTCEV_Shutdown::subscribe(id());

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::subscribeSignal_(
 ONS_EventCallback* theCallback,
 int theSignal)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::subscribeSignal_()");

  OTCLIB_TRACER(MODULE) << "signal = " << theSignal << endl;

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::subscribeSignal_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "SIGNAL";
  tmpKey += EOS;

  char tmpBuffer[32];
  sprintf(tmpBuffer,"%d",theSignal);

  tmpKey += tmpBuffer;

  agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  OTCEV_Signal::subscribe(id(),theSignal);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Agent::setAlarm_(
 ONS_EventCallback* theCallback,
 time_t theTime,
 char const* theAlarm
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::setAlarm_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::setAlarm_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return OTC_String::undefinedString();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (theAlarm != 0)
    OTCEV_Alarm::cancelAgent(id(),theAlarm,-1);

  OTC_String tmpAlarm(theAlarm);

  OTCLIB_TRACER(MODULE) << "time = " << theTime << endl;
  OTCLIB_TRACER(MODULE) << "alarm = " << tmpAlarm << endl;

  if (theAlarm == 0)
    tmpAlarm = OTC_Program::uniqueId("$ALARM?",OTCLIB_ID_SHORT_FORMAT);

  OTCLIB_TRACER(MODULE) << "id = " << tmpAlarm << endl;

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "ALARM";
  tmpKey += EOS;
  tmpKey += tmpAlarm;

  agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  OTCEV_Alarm::set(id(),theTime,tmpAlarm,-1);

  xxxMutex.release();

  mutex_.unlock();

  return tmpAlarm;
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Agent::startTimer_(
 ONS_EventCallback* theCallback,
 double theDelay,
 char const* theTimer
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::startTimer_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::startTimer_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return OTC_String::nullString();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (theTimer != 0)
    OTCEV_Timeout::cancelAgent(id(),theTimer,-1);

  OTC_String tmpTimer(theTimer);

  OTCLIB_TRACER(MODULE) << "delay = " << theDelay << endl;
  OTCLIB_TRACER(MODULE) << "timer = " << tmpTimer << endl;

  if (theTimer == 0)
    tmpTimer = OTC_Program::uniqueId("$TIMER?",OTCLIB_ID_SHORT_FORMAT);

  OTCLIB_TRACER(MODULE) << "id = " << tmpTimer << endl;

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "TIMEOUT";
  tmpKey += EOS;
  tmpKey += tmpTimer;

  agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  OTCEV_Timeout::start(id(),long(theDelay*1000),tmpTimer,-1);

  xxxMutex.release();

  mutex_.unlock();

  return tmpTimer;
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Agent::scheduleAction_(
 ONS_EventCallback* theCallback,
 OTC_JobType theType,
 char const* theAction
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::scheduleAction_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::scheduleAction_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return OTC_String::nullString();

  OTC_String tmpAction(theAction);

  OTCLIB_TRACER(MODULE) << "type = " << theType << endl;
  OTCLIB_TRACER(MODULE) << "action = " << tmpAction << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  // Need to first remove any prior action
  // callbacks with the same name regardless of
  // the type.

  OTCEV_Action::cancelAgent(id(),theAction,-1);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "ACTION-JOB";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-CRON";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-TRANSIENT";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-PERSISTENT";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  if (theAction == 0)
    tmpAction = OTC_Program::uniqueId("$ACTION?",OTCLIB_ID_SHORT_FORMAT);

  OTCLIB_TRACER(MODULE) << "id = " << tmpAction << endl;

  tmpKey = "ACTION-JOB";
  tmpKey += EOS;
  tmpKey += tmpAction;

  agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  OTCEV_Action::schedule(id(),theType,tmpAction,-1);

  xxxMutex.release();

  mutex_.unlock();

  return tmpAction;
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Agent::scheduleAction_(
 ONS_EventCallback* theCallback,
 char const* theSpecification,
 char const* theAction
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::scheduleAction_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::scheduleAction_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return OTC_String::nullString();

  if (theSpecification == 0)
    theSpecification = "";

  OTC_String tmpAction(theAction);

  OTCLIB_TRACER(MODULE) << "specification = " << theSpecification << endl;
  OTCLIB_TRACER(MODULE) << "action = " << tmpAction << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  // Need to first remove any prior action
  // callbacks with the same name regardless of
  // the type.

  OTCEV_Action::cancelAgent(id(),theAction,-1);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "ACTION-JOB";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-CRON";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-TRANSIENT";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-PERSISTENT";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  if (theAction == 0)
    tmpAction = OTC_Program::uniqueId("$ACTION?",OTCLIB_ID_SHORT_FORMAT);

  OTCLIB_TRACER(MODULE) << "id = " << tmpAction << endl;

  tmpKey = "ACTION-CRON";
  tmpKey += EOS;
  tmpKey += tmpAction;

  agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  OTCEV_Action::schedule(id(),theSpecification,tmpAction,-1);

  xxxMutex.release();

  mutex_.unlock();

  return tmpAction;
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Agent::subscribeAction_(
 ONS_EventCallback* theCallback,
 ONS_LifeTime theLifeTime,
 char const* theAction
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::subscribeAction_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::subscribeAction_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return OTC_String::nullString();

  OTC_String tmpAction(theAction);

  OTCLIB_TRACER(MODULE) << "lifetime = " << int(theLifeTime) << endl;
  OTCLIB_TRACER(MODULE) << "action = " << tmpAction << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  // Need to first remove any prior action
  // callbacks with the same name regardless of
  // the type.

  OTCEV_Action::cancelAgent(id(),theAction,-1);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "ACTION-JOB";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-CRON";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-TRANSIENT";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  tmpKey = "ACTION-PERSISTENT";
  tmpKey += EOS;
  tmpKey += theAction;

  agentCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  if (theAction == 0)
    tmpAction = OTC_Program::uniqueId("$ACTION?",OTCLIB_ID_SHORT_FORMAT);

  OTCLIB_TRACER(MODULE) << "id = " << tmpAction << endl;

  if (theLifeTime == ONSLIB_PERSISTENT)
  {
    tmpKey = "ACTION-PERSISTENT";
    tmpKey += EOS;
    tmpKey += tmpAction;
  }
  else
  {
    tmpKey = "ACTION-TRANSIENT";
    tmpKey += EOS;
    tmpKey += tmpAction;
  }

  agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  xxxMutex.release();

  mutex_.unlock();

  return tmpAction;
}

/* ------------------------------------------------------------------------- */
void ONS_Agent::subscribeSocket_(
 ONS_EventCallback* theCallback,
 int theSocket,
 int theEvents
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Agent::subscribeSocket_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Agent::subscribeSocket_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCLIB_TRACER(MODULE) << "socket = " << theSocket << endl;
  OTCLIB_TRACER(MODULE) << "events = " << theEvents << endl;

  char tmpBuffer[32];
  sprintf(tmpBuffer,"%d",theSocket);

  OTC_Capacity tmpCapacity(31);
  OTC_String tmpKey(tmpCapacity);

  if (theEvents & OTCLIB_POLLIN)
  {
    tmpKey = "SOCKET-IN";
    tmpKey += EOS;
    tmpKey += tmpBuffer;

    agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);
  }
  else if (theEvents & OTCLIB_POLLPRI)
  {
    tmpKey = "SOCKET-PRIORITY";
    tmpKey += EOS;
    tmpKey += tmpBuffer;

    agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);
  }
  else if (theEvents & OTCLIB_POLLOUT)
  {
    tmpKey = "SOCKET-OUT";
    tmpKey += EOS;
    tmpKey += tmpBuffer;

    agentCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);
  }

  OTCEV_IOEvent::subscribe(id(),theSocket,theEvents);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
