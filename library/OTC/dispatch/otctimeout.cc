/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otctimeout.cc
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
#pragma implementation "OTC/dispatch/timeout.hh"
#endif
#endif

#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/iostream.h>

#if !defined(_AIX)
#include <time.h>
#endif

#if defined(HAVE_GETTIMEOFDAY)
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_TimeoutTimeval
{
  public:

    long		sec;

    long		usec;
};

class OSE_EXPORT OTC_TimeoutSubscription
{
  public:

    void*		operator new(size_t theSize)
				{ return OTC_CommonPool::allocate(theSize); }

    void		operator delete(void* theMem, size_t theSize)
				{ OTC_CommonPool::release(theMem,theSize); }

    int			timerId;

    int			agentId;

    OTC_String		description;

    int			agentData;

    bool		expired;

    OTC_TimeoutTimeval	time;

    OTC_TimeoutSubscription*	next;
};

/* ------------------------------------------------------------------------- */
enum { ONE_SECOND = 1000000 };

/* ------------------------------------------------------------------------- */
OTC_TimeoutTimeval operator+(OTC_TimeoutTimeval src1, OTC_TimeoutTimeval src2)
{
  OTC_TimeoutTimeval sum;
  sum.sec = src1.sec + src2.sec;
  sum.usec = src1.usec + src2.usec;
  if (sum.usec >= ONE_SECOND)
  {
    sum.usec -= ONE_SECOND;
    sum.sec++;
  }
  else if (sum.sec >= 1 && sum.usec < 0)
  {
    sum.usec += ONE_SECOND;
    sum.sec--;
  }
  return sum;
}

/* ------------------------------------------------------------------------- */
OTC_TimeoutTimeval operator-(OTC_TimeoutTimeval src1, OTC_TimeoutTimeval src2)
{
  OTC_TimeoutTimeval delta;
  delta.sec = src1.sec - src2.sec;
  delta.usec = src1.usec - src2.usec;
  if (delta.usec < 0)
  {
    delta.usec += ONE_SECOND;
    delta.sec--;
  }
  else if (delta.usec >= ONE_SECOND)
  {
      delta.usec -= ONE_SECOND;
      delta.sec++;
  }
  return delta;
}

/* ------------------------------------------------------------------------- */
bool operator==(OTC_TimeoutTimeval src1, OTC_TimeoutTimeval src2)
{
  if (src1.sec != src2.sec)
  {
    return false;
  }
  else if (src1.usec != src2.usec)
  {
    return false;
  }
  else
  {
    return true;
  }
}

/* ------------------------------------------------------------------------- */
bool operator>=(OTC_TimeoutTimeval src1, OTC_TimeoutTimeval src2)
{
  if (src1.sec > src2.sec)
  {
    return true;
  }
  else if (src1.sec == src2.sec && src1.usec >= src2.usec)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/* ------------------------------------------------------------------------- */
bool operator<(OTC_TimeoutTimeval src1, OTC_TimeoutTimeval src2)
{
  if (src1.sec < src2.sec)
  {
    return true;
  }
  else if (src1.sec == src2.sec && src1.usec < src2.usec)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/* ------------------------------------------------------------------------- */
OTC_TimeoutTimeval otclib_current_time()
{
  OTC_TimeoutTimeval curTime;
#if defined(HAVE_GETTIMEOFDAY)
  timeval tmpTime;
  gettimeofday(&tmpTime,0);
  curTime.sec = tmpTime.tv_sec;
  curTime.usec = tmpTime.tv_usec;
#else
  timeb t;
  ftime(&t);
  curTime.sec = t.time;
  curTime.usec = 1000*t.millitm;
#endif
  return curTime;
}

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTCEV_Timeout::mutex_;
int OTCEV_Timeout::globTimerIdCount_ = 0;
OTC_TimeoutSubscription* OTCEV_Timeout::globSubscriptions_ = 0;
int OTCEV_Timeout::globTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Timeout::~OTCEV_Timeout()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Timeout::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Timeout::dump(ostream& outs) const
{
  outs << "<OTC> TIMEOUT - timer = " << timer()
   << ", description = " << description() << ", data = " << data();
}

/* ------------------------------------------------------------------------- */
int OTCEV_Timeout::start(
 int theAgentId,
 long thePeriod,
 char const* theDescription,
 int theAgentData
)
{
  OTCLIB_ENSURE_FN((thePeriod >= 0),
   "OTCEV_Timeout::start(int, int, int)",
   "invalid period");
  OTCLIB_ENSURE_FN((theAgentId != 0),
   "OTCEV_Timeout::start(int, int, int)",
   "invalid agent ID");

  OTC_TimeoutSubscription* aSubscription;
  aSubscription = new OTC_TimeoutSubscription;
  OTCLIB_ASSERT_M(aSubscription != 0);

  mutex_.lock();

  if (globTimerIdCount_ == 0)
    globTimerIdCount_++;

  aSubscription->timerId = globTimerIdCount_++;
  aSubscription->agentId = theAgentId;
  aSubscription->description = theDescription;
  aSubscription->agentData = theAgentData;

  OTC_TimeoutTimeval tmpPeriod;
  tmpPeriod.sec = thePeriod / 1000;
  tmpPeriod.usec = (thePeriod % 1000) * 1000;

  OTC_TimeoutTimeval theTime;
  theTime = otclib_current_time() + tmpPeriod;

  aSubscription->expired = (thePeriod == 0) ? true : false;
  aSubscription->time = theTime;
  aSubscription->next = 0;

  if (globSubscriptions_ == 0)
  {
    globSubscriptions_ = aSubscription;
  }
  else if (aSubscription->time < globSubscriptions_->time)
  {
    aSubscription->expired = globSubscriptions_->expired;
    aSubscription->next = globSubscriptions_;
    globSubscriptions_ = aSubscription;
  }
  else
  {
    OTC_TimeoutSubscription* tmpSubscription1;
    OTC_TimeoutSubscription* tmpSubscription2;
    tmpSubscription1 = globSubscriptions_->next;
    tmpSubscription2 = globSubscriptions_;

    while (tmpSubscription1 != 0 && theTime >= tmpSubscription1->time)
    {
      tmpSubscription2 = tmpSubscription1;
      tmpSubscription1 = tmpSubscription1->next;
    }

    aSubscription->next = tmpSubscription1;
    tmpSubscription2->next = aSubscription;

    if (thePeriod != 0)
    {
      if (tmpSubscription2->time == theTime)
	aSubscription->expired = tmpSubscription2->expired;
      else if (tmpSubscription1 != 0)
	aSubscription->expired = tmpSubscription1->expired;
    }
  }

  mutex_.unlock();

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_TIMEOUT);

  return aSubscription->timerId;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Timeout::cancel(int theTimerId)
{
  if (theTimerId == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptions_ != 0)
  {
    OTC_TimeoutSubscription* tmpSubscription1;
    OTC_TimeoutSubscription* tmpSubscription2;

    if (globSubscriptions_->timerId == theTimerId)
    {
      tmpSubscription1 = globSubscriptions_;
      globSubscriptions_ = globSubscriptions_->next;
      delete tmpSubscription1;
    }
    else
    {
      tmpSubscription1 = globSubscriptions_->next;
      tmpSubscription2 = globSubscriptions_;

      while (tmpSubscription1 != 0)
      {
	if (tmpSubscription1->timerId == theTimerId)
	{
	  tmpSubscription2->next = tmpSubscription1->next;
	  delete tmpSubscription1;
	  break;
	}

	tmpSubscription2 = tmpSubscription1;
	tmpSubscription1 = tmpSubscription1->next;
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_TIMEOUT);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Timeout::cancelAgent(int theAgentId)
{
  if (theAgentId == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptions_ != 0)
  {
    OTC_TimeoutSubscription* tmpSubscription1;
    OTC_TimeoutSubscription* tmpSubscription2;

    tmpSubscription1 = globSubscriptions_;
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
	  globSubscriptions_ = tmpSubscription1->next;
	  delete tmpSubscription1;
	  tmpSubscription1 = globSubscriptions_;
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

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_TIMEOUT);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Timeout::cancelAgent(
 int theAgentId,
 char const* theDescription,
 int theAgentData
)
{
  if (theAgentId == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptions_ != 0)
  {
    OTC_TimeoutSubscription* tmpSubscription1;
    OTC_TimeoutSubscription* tmpSubscription2;

    tmpSubscription1 = globSubscriptions_;
    tmpSubscription2 = 0;

    while (tmpSubscription1 != 0)
    {
      if (tmpSubscription1->agentId == theAgentId &&
	tmpSubscription1->description == theDescription &&
	tmpSubscription1->agentData == theAgentData)
      {
	if (tmpSubscription2 != 0)
	{
	  tmpSubscription2->next = tmpSubscription1->next;
	  delete tmpSubscription1;
	  tmpSubscription1 = tmpSubscription2->next;
	}
	else
	{
	  globSubscriptions_ = tmpSubscription1->next;
	  delete tmpSubscription1;
	  tmpSubscription1 = globSubscriptions_;
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

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_TIMEOUT);
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Timeout::active(int theTimerId)
{
  if (theTimerId == 0)
    return false;

  bool theResult;
  theResult = false;

  mutex_.lock();

  if (globSubscriptions_ != 0)
  {
    OTC_TimeoutSubscription* aSubscription;
    aSubscription = globSubscriptions_;

    while (aSubscription != 0)
    {
      if (aSubscription->timerId == theTimerId)
      {
	theResult = true;
	break;
      }

      aSubscription = aSubscription->next;
    }
  }

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
long OTCEV_Timeout::period()
{
  long theTime;
  theTime = 0;

  mutex_.lock();

  if (globSubscriptions_ == 0)
  {
    theTime = -1;
  }
  else
  {
    OTC_TimeoutTimeval thePeriod;
    thePeriod = globSubscriptions_->time - otclib_current_time();

    if (thePeriod.sec < 0)
      theTime = 0;
    else
      theTime = thePeriod.sec*1000 + thePeriod.usec/1000;
  }

  mutex_.unlock();

  return theTime;
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Timeout::isPending()
{
  bool theResult;
  theResult = false;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptions_ != 0)
  {
    if (globSubscriptions_->expired == false)
    {
      OTC_TimeoutTimeval theTime = otclib_current_time();

      OTC_TimeoutSubscription* tmpSubscription1;
      tmpSubscription1 = globSubscriptions_;

      while (tmpSubscription1 != 0 && theTime >= tmpSubscription1->time)
      {
	tmpSubscription1->expired = true;
	tmpSubscription1 = tmpSubscription1->next;
      }
    }

    if (globSubscriptions_->expired != false)
      theResult = true;
  }

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTCEV_Timeout::pending()
{
  OTC_EventJob* theJob;
  theJob = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptions_ != 0)
  {
    if (globSubscriptions_->expired == false)
    {
      OTC_TimeoutTimeval theTime = otclib_current_time();

      OTC_TimeoutSubscription* tmpSubscription1;
      tmpSubscription1 = globSubscriptions_;

      while (tmpSubscription1 != 0 && theTime >= tmpSubscription1->time)
      {
	tmpSubscription1->expired = true;
	tmpSubscription1 = tmpSubscription1->next;
      }
    }

    if (globSubscriptions_->expired != false)
    {
      OTC_TimeoutSubscription* tmpSubscription1;
      tmpSubscription1 = globSubscriptions_->next;

      OTCEV_Timeout* theEvent;
      theEvent = new OTCEV_Timeout(
       globSubscriptions_->timerId,
       globSubscriptions_->description,
       globSubscriptions_->agentData);
      OTCLIB_ASSERT_M(theEvent != 0);

      theJob = new OTC_EventJob(globSubscriptions_->agentId,theEvent);
      OTCLIB_ASSERT_M(theJob != 0);

      delete globSubscriptions_;
      globSubscriptions_ = tmpSubscription1;
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  return theJob;
}

/* ------------------------------------------------------------------------- */
