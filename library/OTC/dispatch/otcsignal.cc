/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcsignal.cc
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
#pragma implementation "OTC/dispatch/signal.hh"
#endif
#endif

#include <OTC/dispatch/signal.hh>
#include <OTC/system/sigblock.hh>
#include <OTC/dispatch/joblist.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/system/sighndle.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/iostream.h>

#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(OSE_SYS_WIN32)
#include <windows.h>
#include <process.h>
#include <io.h>
#ifndef getpid
#define getpid _getpid
#endif
#endif

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_SignalSubscription
{
  public:

    int			agentId;

    OTC_SignalSubscription*	next;

    void*		operator new(size_t theSize)
				{ return OTC_CommonPool::allocate(theSize); }

    void		operator delete(void* theMem, size_t theSize)
				{ OTC_CommonPool::release(theMem,theSize); }
};

class OSE_EXPORT OTC_SignalInfo
{
  public:

    int			signal;

    int			count;
};

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTCEV_Signal::mutex_;
OTC_SignalSubscription** OTCEV_Signal::gSubscriptions_ = 0;
OTC_SignalInfo* OTCEV_Signal::gPendingSignals_ = 0;
u_int OTCEV_Signal::gPendingSize_ = 0;
u_int OTCEV_Signal::gNextAvailable_ = 0;
bool OTCEV_Signal::gSignalsLost_ = false;
u_int OTCEV_Signal::gSigCount_ = 0;
OTC_JobList* OTCEV_Signal::gSignalJobs_ = 0;
int OTCEV_Signal::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Signal::~OTCEV_Signal()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Signal::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Signal::dump(ostream& outs) const
{
  outs << "<OTC> SIGNAL - number = " << signal();
}

/* ------------------------------------------------------------------------- */
u_int OTCEV_Signal::signalsSubscribed()
{
  u_int theResult;

  mutex_.lock();

  OTC_SignalBlock sigblock;

  theResult = gSigCount_;

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Signal::subscribe(int theAgentId, int theSignal)
{
  OTCLIB_ENSURE_FN((theSignal > 0 && theSignal < NSIG),
   "OTCEV_Signal::subscribe(int, int)",
   "invalid signal number");
  OTCLIB_ENSURE_FN((theAgentId != 0),
   "OTCEV_Signal::subscribe(int, int)",
   "invalid agent ID");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_SignalBlock sigblock;

  if (gPendingSize_ == 0)
  {
    gPendingSize_ = 64;

    gPendingSignals_ = new OTC_SignalInfo[gPendingSize_];
    OTCLIB_ASSERT_M(gPendingSignals_ != 0);
  }

  bool isSubscribed = false;

  if (gSubscriptions_ == 0)
  {
    gSubscriptions_ = new OTC_SignalSubscription*[NSIG];
    OTCLIB_ASSERT_M(gSubscriptions_ != 0);
    for (int i=0; i<NSIG; i++)
      gSubscriptions_[i] = 0;
  }
  else
  {
    OTC_SignalSubscription* aSubscription;
    aSubscription = gSubscriptions_[theSignal];
    while (aSubscription != 0)
    {
      if (aSubscription->agentId == theAgentId)
      {
	isSubscribed = true;
	break;
      }
      else
	aSubscription = aSubscription->next;
    }
  }

  if (isSubscribed == false)
  {
    OTC_SignalSubscription* aSubscription;
    aSubscription = new OTC_SignalSubscription;
    OTCLIB_ASSERT_M(aSubscription != 0);
    aSubscription->agentId = theAgentId;
    aSubscription->next = gSubscriptions_[theSignal];
    gSubscriptions_[theSignal] = aSubscription;

    if (aSubscription->next == 0)
    {
      OTC_SignalHandler::install(theSignal,sighandler_);
      gSigCount_++;
    }
  }

  sigblock.release();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Signal::unsubscribe(int theAgentId, int theSignal)
{
  if (theSignal <= 0 || theSignal >= NSIG || theAgentId == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSubscriptions_ != 0 && gSigCount_ != 0)
  {
    OTC_SignalSubscription* aSubscription;
    aSubscription = gSubscriptions_[theSignal];

    if (aSubscription != 0)
    {
      OTC_SignalBlock sigblock;

      if (aSubscription->agentId == theAgentId)
      {
	gSubscriptions_[theSignal] = aSubscription->next;
	delete aSubscription;
      }
      else
      {
	OTC_SignalSubscription* tmpSubscription;
	tmpSubscription = aSubscription;
	aSubscription = aSubscription->next;
	while (aSubscription != 0)
	{
	  if (aSubscription->agentId == theAgentId)
	  {
	    tmpSubscription->next = aSubscription->next;
	    delete aSubscription;
	    break;
	  }
	  else
	  {
	    tmpSubscription = aSubscription;
	    aSubscription = aSubscription->next;
	  }
	}
      }

      if (gSubscriptions_[theSignal] == 0)
      {
	OTC_SignalHandler::install(theSignal,0);
	gSigCount_--;
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Signal::unsubscribeAgent(int theAgentId)
{
  if (theAgentId == 0)
    return;

  for (int i=1; i<NSIG; i++)
    unsubscribe(theAgentId,i);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Signal::capacity(u_int theCapacity)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gPendingSize_ == 0)
  {
    OTCLIB_ENSURE_FN((theCapacity != 0),
     "OTCEV_Signal::capacity(u_int)",
     "capacity must be non zero");

    gPendingSize_ = theCapacity;
    gPendingSignals_ = new OTC_SignalInfo[gPendingSize_];
    OTCLIB_ASSERT_M(gPendingSignals_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Signal::isPending()
{
  bool theResult;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSignalJobs_ == 0)
  {
    gSignalJobs_ = new OTC_JobList;
    OTCLIB_ASSERT_M(gSignalJobs_ != 0);
  }

  fill_(gSignalJobs_);

  theResult = !gSignalJobs_->isEmpty();

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTCEV_Signal::pending()
{
  OTC_Job* theJob;
  theJob = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSignalJobs_ == 0)
  {
    gSignalJobs_ = new OTC_JobList;
    OTCLIB_ASSERT_M(gSignalJobs_ != 0);
  }

  fill_(gSignalJobs_);

  theJob = gSignalJobs_->next();

  xxxMutex.release();

  mutex_.unlock();

  return theJob;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Signal::fill_(OTC_JobList* theJobQueue)
{
  // Assumed that this is called from within thread lock. This is
  // the case where called from pending().

  // Mask further signals.

  OTC_SignalBlock sigblock;

  // Check whether we have any subscriptions.

  if (gPendingSize_ == 0 || gSigCount_ == 0)
    return;

  // Check whether there were signals.

  if (gNextAvailable_ == 0)
    return;

  // Check for discarded signals.

  if (gSignalsLost_ != false)
  {
    gSignalsLost_ = false;

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTCEV_Signal" << EOL;
    theLogger << "Method: fill_()" << EOL;
    theLogger << "Description: two many signals have arrived" << EOL;
    theLogger << "Result: signals were lost" << EOL;
    theLogger << "Comment: increase pending signal capacity" << EOL;
    theLogger << flush;
  }

  // Fill the queue.

  u_int i = 0;
  while (i != gNextAvailable_)
  {
    OTC_SignalInfo& theInfo = gPendingSignals_[i];

    OTCEV_Signal* theSignal;
    theSignal = new OTCEV_Signal(theInfo.signal);
    OTCLIB_ASSERT_M(theSignal);

    while (theInfo.count != 0)
    {
      OTC_SignalSubscription* aSubscription;
      aSubscription = gSubscriptions_[theInfo.signal];

      while (aSubscription != 0)
      {
	OTC_Event* aEvent;
	aEvent = theSignal->clone();

	OTC_EventJob* theJob;
	theJob = new OTC_EventJob(aSubscription->agentId,aEvent);
	OTCLIB_ASSERT_M(theJob);

	theJobQueue->add(theJob);

	aSubscription = aSubscription->next;
      }
      theInfo.count--;
    }

    theSignal->destroy();

    i++;
  }

  gNextAvailable_ = 0;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Signal::sighandler_(int theSignal)
{
  // Micro$oft C++ unregisters signal handler, must reregister
  // it ourselves.

#if defined(OSE_SYS_WIN32)
  OTC_SignalHandler::install(theSignal,sighandler_);
#endif

#if defined(__linux)
  if (long(getpid()) != OTC_Dispatcher::processId())
    return;
#endif

  // On Win32, a signal handler is actually a
  // separate thread. As such, on Win32 need to
  // use the thread mutex to protect access
  // to the signal information. For other platforms
  // rely on the signal block to stop concurrent
  // access.

#if defined(OSE_SYS_WIN32)
  // Since there are multiple return points,
  // rely on reaper doing its job and unlocking
  // the thread mutex for us.

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);
#endif

  OTC_SignalBlock sigblock;

  if (gNextAvailable_ == gPendingSize_)
  {
    gSignalsLost_ = true;
    return;
  }

  if (gNextAvailable_ != 0)
  {
    OTC_SignalInfo& theInfo = gPendingSignals_[gNextAvailable_-1];
    if (theInfo.signal == theSignal)
    {
      theInfo.count++;
      return;
    }

    theInfo.signal = theSignal;
    theInfo.count = 1;

    gNextAvailable_++;
  }
  else
  {
    OTC_SignalInfo& theInfo = gPendingSignals_[gNextAvailable_];
    theInfo.signal = theSignal;
    theInfo.count = 1;

    gNextAvailable_++;

    OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_SIGNAL);
  }
}

/* ------------------------------------------------------------------------- */
void OTCEV_Signal::cancelSource(int theAgentId)
{
  OTCEV_Signal::unsubscribeAgent(theAgentId);

  if (OTC_Dispatcher::warningsEnabled())
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTCEV_Signal" << EOL;
    theLogger << "Method: cancelSource()" << EOL;
    theLogger << "Description: specified agent doesn't exist" << EOL;
    theLogger << "Result: signal sevent subscription cancelled" << EOL;
    theLogger << "AgentId: " << theAgentId << EOL;
    theLogger << flush;
  }
}

/* ------------------------------------------------------------------------- */
