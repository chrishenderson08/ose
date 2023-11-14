/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcshutdown.cc
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
#pragma implementation "OTC/dispatch/shutdown.hh"
#endif
#endif

#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_ShutdownSubscription
{
  public:

    void*		operator new(size_t theSize)
				{ return OTC_CommonPool::allocate(theSize); }

    void		operator delete(void* theMem, size_t theSize)
				{ OTC_CommonPool::release(theMem,theSize); }

    int			agentId;

    OTC_ShutdownSubscription*	next;
};

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTCEV_Shutdown::mutex_;
OTC_NRMutex OTCEV_Shutdown::amutex_;
OTC_ShutdownSubscription* OTCEV_Shutdown::gSubscriptions_ = 0;
OTC_FNAgent* OTCEV_Shutdown::gSignalAgent_ = 0;
OTC_FNAgent* OTCEV_Shutdown::gShutdownAgent_ = 0;
int OTCEV_Shutdown::gTypeId_ = 0;
int OTCEV_Shutdown::gDelayCount_ = 0;
bool OTCEV_Shutdown::gShutdownArrived_ = false;

/* ------------------------------------------------------------------------- */
OTCEV_Shutdown::~OTCEV_Shutdown()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Shutdown::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::dump(ostream& outs) const
{
  outs << "<OTC> SHUTDOWN";

  if (category_ == OTCLIB_SHUTDOWN_PENDING)
    outs << " PENDING";
  else
    outs << " ARRIVED";

  if (!description_.isEmpty())
    outs << " " << description_;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::subscribe(int theAgentId)
{
  OTCLIB_ENSURE_FN((theAgentId != 0),
   "OTCEV_Shutdown::subscribe(int)",
   "invalid agent ID");

  mutex_.lock();

  OTC_ShutdownSubscription* aSubscription;
  aSubscription = gSubscriptions_;

  while (aSubscription != 0)
  {
    if (aSubscription->agentId == theAgentId)
      break;

    aSubscription = aSubscription->next;
  }

  if (aSubscription == 0)
  {
    aSubscription = new OTC_ShutdownSubscription;
    OTCLIB_ASSERT_M(aSubscription != 0);

    aSubscription->agentId = theAgentId;
    aSubscription->next = gSubscriptions_;
    gSubscriptions_ = aSubscription;
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::unsubscribe(int theAgentId)
{
  if (theAgentId == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSubscriptions_ != 0)
  {
    OTC_ShutdownSubscription* tmpSubscription1;
    OTC_ShutdownSubscription* tmpSubscription2;

    tmpSubscription1 = gSubscriptions_;
    tmpSubscription2 = 0;

    while (tmpSubscription1 != 0)
    {
      if (tmpSubscription1->agentId == theAgentId)
      {
	if (tmpSubscription2 != 0)
	{
	  tmpSubscription2->next = tmpSubscription1->next;
	  delete tmpSubscription1;
	  tmpSubscription1 = tmpSubscription2->next;
	}
	else
	{
	  gSubscriptions_ = tmpSubscription1->next;
	  delete tmpSubscription1;
	  tmpSubscription1 = gSubscriptions_;
	}
      }
      else
      {
	tmpSubscription2 = tmpSubscription1;
	tmpSubscription1 = tmpSubscription1->next;
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::monitor(int theSignal)
{
  amutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(amutex_);

  if (gSignalAgent_ == 0)
  {
    gSignalAgent_ = new OTC_FNAgent(signalCallback_);
    OTCLIB_ASSERT_M(gSignalAgent_ != 0);
  }

  xxxMutex.release();

  amutex_.unlock();

  OTCEV_Signal::subscribe(gSignalAgent_->id(),theSignal);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::schedule(long theDelay)
{
  OTC_FNAgent* theAgent = 0;

  OTC_LogStream theLogger;
  theLogger.setLevel(OTCLIB_LOG_INFO);

  amutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(amutex_);

  if (gShutdownAgent_ == 0)
  {
    theAgent = new OTC_FNAgent(shutdownCallback_);
    OTCLIB_ASSERT_M(theAgent != 0);
    gShutdownAgent_ = theAgent;

    theLogger << "Shutdown is pending" << endl;

    if (gDelayCount_ != 0)
      theLogger << "Shutdown suspended" << endl;
  }

  xxxMutex.release();

  amutex_.unlock();

  if (theAgent == 0)
    return;

  if (theDelay == 0)
  {
    theDelay = 1000;
    char const* env;
    env = getenv("OTCLIB_SHUTDOWNDELAY");
    if (env != 0)
    {
      long tmpDelay;
      tmpDelay = atoi(env);
      if (tmpDelay > 0)
	theDelay = tmpDelay;
    }
  }

  OTCEV_Shutdown::notify(OTCLIB_SHUTDOWN_PENDING);
  OTCEV_Timeout::start(theAgent->id(),theDelay);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::suspend()
{
  amutex_.lock();

  if (gDelayCount_ == 0 && gShutdownAgent_ != 0)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_INFO);

    theLogger << "Shutdown suspended" << endl;
  }

  gDelayCount_++;

  amutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::resume()
{
  amutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(amutex_);

  if (gDelayCount_ != 0)
  {
    gDelayCount_--;

    if (gDelayCount_ == 0)
    {
      if (gShutdownArrived_ != false)
      {
	OTC_LogStream theLogger;
	theLogger.setLevel(OTCLIB_LOG_INFO);

	theLogger << "Shutdown resumed" << endl;
	theLogger << "Shutdown has arrived" << endl;

	OTCEV_Shutdown::notify(OTCLIB_SHUTDOWN_ARRIVED);
	OTCEV_Action::schedule(gShutdownAgent_->id(),OTCLIB_PRIORITY_JOB);
      }
      else if (gShutdownAgent_ != 0)
      {
	OTC_LogStream theLogger;
	theLogger.setLevel(OTCLIB_LOG_INFO);

	theLogger << "Shutdown resumed" << endl;
      }
    }
  }

  xxxMutex.release();

  amutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Shutdown::isScheduled()
{
  bool theResult;
  theResult = false;

  amutex_.lock();

  if (gShutdownAgent_ != 0)
    theResult = true;

  amutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Shutdown::isSuspended()
{
  bool theResult;
  theResult = false;

  amutex_.lock();

  if (gDelayCount_ != 0)
    theResult = true;

  amutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::notify(OTC_ShutdownNotification theCategory)
{
  OTC_JobList theJobList;

  fill_(&theJobList,theCategory);

  OTC_Job* theJob;
  theJob = theJobList.next();

  while (theJob != 0)
  {
    OTC_Dispatcher::schedule(theJob,OTCLIB_PRIORITY_JOB);

    theJob = theJobList.next();
  }
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::notifyImmediately(OTC_ShutdownNotification theCategory)
{
  OTC_JobList theJobList;

  fill_(&theJobList,theCategory);

  OTC_Job* theJob;
  theJob = theJobList.next();

  while (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();

    theJob = theJobList.next();
  }
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::deliverImmediately(
 int theAgentId,
 OTC_ShutdownNotification theCategory,
 char const* theDescription
)
{
  OTCEV_Shutdown* theEvent;
  theEvent = new OTCEV_Shutdown(theCategory,theDescription);
  OTCLIB_ASSERT_M(theEvent != 0);

  theEvent->deliver(theAgentId);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::cancelSource(int theAgentId)
{
  unsubscribe(theAgentId);

  if (OTC_Dispatcher::warningsEnabled())
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTCEV_Shutdown" << EOL;
    theLogger << "Method: cancelSource()" << EOL;
    theLogger << "Description: specified agent doesn't exist" << EOL;
    theLogger << "Result: service subscription cancelled" << EOL;
    theLogger << "AgentId: " << theAgentId << EOL;
    theLogger << flush;
  }
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::fill_(
 OTC_JobList* theJobList,
 OTC_ShutdownNotification theCategory
)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_ShutdownSubscription* aSubscription;
  aSubscription = gSubscriptions_;

  if (aSubscription != 0)
  {
    OTC_Event* theEvent;
    theEvent = new OTCEV_Shutdown(theCategory);
    OTCLIB_ASSERT_M(theEvent != 0);

    while (aSubscription != 0)
    {
      OTC_Event* theClone;
      theClone = theEvent->clone();

      OTC_Job* theJob;
      theJob = new OTC_EventJob(aSubscription->agentId,theClone);
      OTCLIB_ASSERT_M(theJob != 0);

      theJobList->add(theJob);

      aSubscription = aSubscription->next;
    }

    theEvent->destroy();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::signalCallback_(OTC_Event* theEvent, int theAgentId)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Signal::typeId())
  {
    bool inProgress = false;

    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (gShutdownAgent_ != 0)
      inProgress = true;

    xxxMutex.release();

    mutex_.unlock();

    if (inProgress != false)
    {
      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_INFO);

      theLogger << "Forced shutdown" << endl;

      exit(1);
    }
    else
    {
      OTCEV_Shutdown::schedule();
    }
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Shutdown::shutdownCallback_(OTC_Event* theEvent, int theAgentId)
{
  if (theEvent == 0)
    return;

  OTC_LogStream theLogger;
  theLogger.setLevel(OTCLIB_LOG_INFO);

  if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    gShutdownArrived_ = true;

    if (gDelayCount_ == 0)
    {
      theLogger << "Shutdown has arrived" << endl;

      OTCEV_Shutdown::notify(OTCLIB_SHUTDOWN_ARRIVED);
      OTCEV_Action::schedule(theAgentId,OTCLIB_PRIORITY_JOB);
    }
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    theLogger << "Shutdown is complete" << endl;

    OTC_Dispatcher::stop();

    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_FNAgent* tmpAgent;
    tmpAgent = gShutdownAgent_;

    amutex_.lock();

    gShutdownAgent_ = 0;

    amutex_.unlock();

    delete tmpAgent;

    gShutdownArrived_ = false;

    xxxMutex.release();

    mutex_.unlock();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
