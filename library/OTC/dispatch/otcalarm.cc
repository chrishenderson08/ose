/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcalarm.cc
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
#pragma implementation "OTC/dispatch/alarm.hh"
#endif
#endif

#include <OTC/dispatch/alarm.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/iostream.h>

#include <time.h>

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_AlarmSubscription
{
  public:

    void*		operator new(size_t theSize)
				{ return OTC_CommonPool::allocate(theSize); }

    void		operator delete(void* theMem, size_t theSize)
				{ OTC_CommonPool::release(theMem,theSize); }

    int			alarmId;

    int			agentId;

    OTC_String		description;

    int			agentData;

    bool		expired;

    long		time;

    OTC_AlarmSubscription*	next;
};

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTCEV_Alarm::mutex_;
int OTCEV_Alarm::globAlarmIdCount_ = 0;
OTC_AlarmSubscription* OTCEV_Alarm::globSubscriptions_ = 0;
int OTCEV_Alarm::globTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Alarm::~OTCEV_Alarm()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Alarm::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Alarm::dump(ostream& outs) const
{
  outs << "<OTC> ALARM - alarm = " << alarm() << ", time = " << time();
  outs << ", description = " << description() << ", data = " << data();
}

/* ------------------------------------------------------------------------- */
int OTCEV_Alarm::set(
 int theAgentId,
 long theTime,
 char const* theDescription,
 int theAgentData
)
{
  OTCLIB_ENSURE_FN((theTime >= 0),
   "OTCEV_Alarm::set(int, long)",
   "invalid time");
  OTCLIB_ENSURE_FN((theAgentId != 0),
   "OTCEV_Alarm::set(int, long)",
   "invalid agent ID");

  OTC_AlarmSubscription* aSubscription;
  aSubscription = new OTC_AlarmSubscription;
  OTCLIB_ASSERT_M(aSubscription != 0);

  mutex_.lock();

  if (globAlarmIdCount_ == 0)
    globAlarmIdCount_++;

  aSubscription->alarmId = globAlarmIdCount_++;
  aSubscription->agentId = theAgentId;
  aSubscription->description = theDescription;
  aSubscription->agentData = theAgentData;
  aSubscription->expired = (theTime == 0) ? true : false;
  aSubscription->time = theTime;
  aSubscription->next = 0;

  if (globSubscriptions_ == 0)
  {
    globSubscriptions_ = aSubscription;
  }
  else if (theTime < globSubscriptions_->time)
  {
    aSubscription->expired = globSubscriptions_->expired;
    aSubscription->next = globSubscriptions_;
    globSubscriptions_ = aSubscription;
  }
  else
  {
    OTC_AlarmSubscription* tmpSubscription1;
    OTC_AlarmSubscription* tmpSubscription2;
    tmpSubscription1 = globSubscriptions_->next;
    tmpSubscription2 = globSubscriptions_;

    while (tmpSubscription1 != 0 && theTime >= tmpSubscription1->time)
    {
      tmpSubscription2 = tmpSubscription1;
      tmpSubscription1 = tmpSubscription1->next;
    }

    aSubscription->next = tmpSubscription1;
    tmpSubscription2->next = aSubscription;

    if (theTime != 0)
    {
      if (tmpSubscription2->time == theTime)
	aSubscription->expired = tmpSubscription2->expired;
      else if (tmpSubscription1 != 0)
	aSubscription->expired = tmpSubscription1->expired;
    }
  }

  mutex_.unlock();

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_ALARM);

  return aSubscription->alarmId;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Alarm::cancel(int theAlarmId)
{
  if (theAlarmId == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptions_ != 0)
  {
    OTC_AlarmSubscription* tmpSubscription1;
    OTC_AlarmSubscription* tmpSubscription2;

    if (globSubscriptions_->alarmId == theAlarmId)
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
	if (tmpSubscription1->alarmId == theAlarmId)
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

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_ALARM);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Alarm::cancelAgent(int theAgentId)
{
  if (theAgentId == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptions_ != 0)
  {
    OTC_AlarmSubscription* tmpSubscription1;
    OTC_AlarmSubscription* tmpSubscription2;

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

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_ALARM);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Alarm::cancelAgent(
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
    OTC_AlarmSubscription* tmpSubscription1;
    OTC_AlarmSubscription* tmpSubscription2;

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

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_ALARM);
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Alarm::active(int theAlarmId)
{
  if (theAlarmId == 0)
    return false;

  bool theStatus;
  theStatus = false;

  mutex_.lock();

  if (globSubscriptions_ != 0)
  {
    OTC_AlarmSubscription* aSubscription;
    aSubscription = globSubscriptions_;

    while (aSubscription != 0)
    {
      if (aSubscription->alarmId == theAlarmId)
      {
	theStatus = true;
	break;
      }

      aSubscription = aSubscription->next;
    }
  }

  mutex_.unlock();

  return theStatus;
}

/* ------------------------------------------------------------------------- */
long OTCEV_Alarm::period()
{
  long theResult;

  mutex_.lock();

  if (globSubscriptions_ == 0)
  {
    theResult = -1;
  }
  else
  {
    long theTime = ::time(0);

    if (globSubscriptions_->time < theTime)
      theResult = 0;
    else
      theResult = globSubscriptions_->time - theTime;
  }

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Alarm::isPending()
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
      long theTime = ::time(0);

      OTC_AlarmSubscription* tmpSubscription1;
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
OTC_Job* OTCEV_Alarm::pending()
{
  OTC_Job* theJob;
  theJob = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptions_ != 0)
  {
    if (globSubscriptions_->expired == false)
    {
      long theTime = ::time(0);

      OTC_AlarmSubscription* tmpSubscription1;
      tmpSubscription1 = globSubscriptions_;

      while (tmpSubscription1 != 0 && theTime >= tmpSubscription1->time)
      {
	tmpSubscription1->expired = true;
	tmpSubscription1 = tmpSubscription1->next;
      }
    }

    if (globSubscriptions_->expired != false)
    {
      OTC_AlarmSubscription* tmpSubscription1;
      tmpSubscription1 = globSubscriptions_->next;

      OTCEV_Alarm* theEvent;
      theEvent = new OTCEV_Alarm(globSubscriptions_->alarmId,
       globSubscriptions_->time,globSubscriptions_->description,
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
