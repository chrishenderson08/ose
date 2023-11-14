/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcaction.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/action.hh"
#endif
#endif

#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/job.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/alarm.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/collctn/bitset.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/iostream.h>

#include <time.h>
#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTCEV_Action");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_ActionSubscription : public OTC_Link
{
  public:

			OTC_ActionSubscription(
			 int theAgentId,
			 int theActionId,
			 char const* theDescription,
			 int theAgentData
			)
			  : agentId_(theAgentId),
			    actionId_(theActionId),
			    description_(theDescription),
			    agentData_(theAgentData),
			    active_(true)
				{}

			~OTC_ActionSubscription();

    int			agent() const
				{ return agentId_; }

    int			action() const
				{ return actionId_; }

    OTC_String const&	description() const
				{ return description_; }

    int			agentData() const
				{ return agentData_; }

    bool		active() const
				{ return active_; }

    void		cancel()
				{ active_ = false; }

  private:

			OTC_ActionSubscription(OTC_ActionSubscription const&);

    OTC_ActionSubscription&	operator=(OTC_ActionSubscription const&);

    int			agentId_;

    int			actionId_;

    OTC_String		description_;

    int			agentData_;

    bool		active_;
};

OTC_ActionSubscription::~OTC_ActionSubscription()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_ActionJob : public OTC_Job
{
  public:

			OTC_ActionJob(OTC_ActionSubscription* theSubscription)
			  : subscription_(theSubscription) {}

			~OTC_ActionJob();

  protected:

    void		execute();

  private:

			OTC_ActionJob(OTC_ActionJob const&);

    OTC_ActionJob&	operator=(OTC_ActionJob const&);

    OTC_ActionSubscription*	subscription_;
};

OTC_ActionJob::~OTC_ActionJob()
{
  if (subscription_ != 0)
  {
    OTCEV_Action::mutex_.lock();

    OTC_MutexReaper<OTC_Mutex> xxxMutex;
    xxxMutex.grab(OTCEV_Action::mutex_);

    subscription_->kill();

    xxxMutex.release();

    OTCEV_Action::mutex_.unlock();
  }
}

void OTC_ActionJob::execute()
{
  OTCLIB_ASSERT(subscription_ != 0);

  if (subscription_->active())
  {
    OTCEV_Action* theEvent;
    theEvent = new OTCEV_Action(
     OTCLIB_ACTION_JOB,
     subscription_->action(),
     subscription_->description(),
     subscription_->agentData());
    OTCLIB_ASSERT_M(theEvent != 0);

    theEvent->deliver(subscription_->agent());
  }

  OTCEV_Action::mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(OTCEV_Action::mutex_);

  subscription_->kill();

  xxxMutex.release();

  OTCEV_Action::mutex_.unlock();

  subscription_ = 0;
}

/* ------------------------------------------------------------------------- */
static const char *DAYSOFWEEK[] = {
  "sun", "mon", "tue", "wed", "thu", "fri", "sat",
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", 0
};

static const char* MONTHS[] = {
  "*", "jan", "feb", "mar", "apr", "may", "jun",
  "jul", "aug", "sep", "oct", "nov", "dec",
  "*", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", 0
};

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_ActionCronJob : public OTC_Link
{
  public:

			~OTC_ActionCronJob();

			OTC_ActionCronJob(
			 int theActionId,
			 int theAgentId,
			 char const* theDescription,
			 int theAgentData
			)
			  : actionId_(theActionId),
			    agentId_(theAgentId),
			    description_(theDescription),
			    agentData_(theAgentData),
			    minutes_(0,60),
			    minutesWildcard_(false),
			    hours_(0,24),
			    hoursWildcard_(false),
			    days_(1,31),
			    daysWildcard_(false),
			    months_(1,12),
			    monthsWildcard_(false),
			    daysOfWeek_(0,7),
			    daysOfWeekWildcard_(false)
				{}

    bool		parse(char const* theSpecification);

    char const*		parseField(
			 char const* theString,
			 OTC_BitSet& theBitSet,
			 bool& theWildcard,
			 char const** theWords=0
			);

    int			actionId() const
				{ return actionId_; }

    int			agentId() const
				{ return agentId_; }

    OTC_String const&	description() const
				{ return description_; }

    int			agentData() const
				{ return agentData_; }

    bool		isReady(time_t theTime) const;

    bool		isReady(time_t theStart, time_t theEnd) const;

  private:

			OTC_ActionCronJob(OTC_ActionCronJob const&);

    OTC_ActionCronJob&	operator=(OTC_ActionCronJob const&);

    int			actionId_;

    int			agentId_;

    OTC_String		description_;

    int			agentData_;

    OTC_BitSet          minutes_;

    bool		minutesWildcard_;

    OTC_BitSet          hours_;

    bool         hoursWildcard_;

    OTC_BitSet          days_;

    bool         daysWildcard_;

    OTC_BitSet          months_;

    bool         monthsWildcard_;

    OTC_BitSet          daysOfWeek_;

    bool         daysOfWeekWildcard_;
};

OTC_ActionCronJob::~OTC_ActionCronJob()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_ActionCronJob::~OTC_ActionCronJob()");

  OTCLIB_TRACER(MODULE) << "actionId = " << actionId_ << endl;
}

bool OTC_ActionCronJob::isReady(time_t theSeconds) const
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_ActionCronJob::isReady(time_t)");

  OTCLIB_TRACER(MODULE) << "time = " << theSeconds << endl;

#if defined(_REENTRANT)
  tm theTimeStruct;
  localtime_r(&theSeconds,&theTimeStruct);
  tm* theTime = &theTimeStruct;
#else
  tm* theTime = localtime(&theSeconds);
#endif

#if 0
  // XXX This doesn't handle day correctly
  // and runs stuff every day if other fields
  // match.

  return (minutes_.test(theTime->tm_min) && hours_.test(theTime->tm_hour) &&
   (days_.test(theTime->tm_mday) || daysOfWeek_.test(theTime->tm_wday)) &&
   months_.test(theTime->tm_mon+1));
#endif

  // Eliminate obvious mismatches first.

  if (!minutes_.test(theTime->tm_min))
    return false;

  if (!hours_.test(theTime->tm_hour))
    return false;

  if (!months_.test(theTime->tm_mon+1))
    return false;

  // Now for the more tricky day and day of
  // week cases where existance of wildcards
  // come into play.

  OTCLIB_TRACER(MODULE) << "daysWildcard = "
   << daysWildcard_ << endl;
  OTCLIB_TRACER(MODULE) << "daysOfWeekWildcard_ = "
   << daysOfWeekWildcard_ << endl;

  if (daysWildcard_ && daysOfWeekWildcard_)
    return true;

  if (daysWildcard_)
  {
    if (daysOfWeek_.test(theTime->tm_wday))
      return true;

    return false;
  }

  if (daysOfWeekWildcard_)
  {
    if (days_.test(theTime->tm_mday))
      return true;

    return false;
  }

  if (days_.test(theTime->tm_mday) || daysOfWeek_.test(theTime->tm_wday))
    return true;

  return false;
}

bool OTC_ActionCronJob::isReady(time_t theStart, time_t theEnd) const
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_ActionCronJob::isReady(time_t,time_t)");

  OTCLIB_TRACER(MODULE) << "id = " << actionId_ << endl;
  OTCLIB_TRACER(MODULE) << "start = " << theStart << endl;
  OTCLIB_TRACER(MODULE) << "end = " << theEnd << endl;

  time_t theTime;

  for (theTime = theStart-(theStart % 60); theTime <= theEnd; theTime += 60)
  {
    if (theTime > theStart)
    {
      if (isReady(theTime))
      {
	OTCLIB_TRACER(MODULE) << "ready!" << endl;

        return true;
      }
    }
  }

  return false;
}

bool OTC_ActionCronJob::parse(char const* theSpecification)
{
  char const* theString;
  theString = theSpecification;

  if (theString == 0)
    return 0;

  while (*theString != EOS && isspace(*theString))
    theString++;

  if (*theString == EOS)
    return 0;

  theString = parseField(theString,minutes_,minutesWildcard_,0);
  theString = parseField(theString,hours_,hoursWildcard_,0);
  theString = parseField(theString,days_,daysWildcard_,0);
  theString = parseField(theString,months_,monthsWildcard_,MONTHS);
  theString = parseField(theString,daysOfWeek_,daysOfWeekWildcard_,DAYSOFWEEK);

  if (theString == 0)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_ActionCronJob" << EOL;
    theLogger << "Method: parse()" << EOL;
    theLogger << "Description: bad cron job specification" << EOL;
    theLogger << "Result: job has been discarded" << EOL;
    theLogger << "Specification: " << theSpecification << EOL;
    theLogger << "AgentId: " << agentId_ << EOL;
    theLogger << "Description: " << description_ << EOL;
    theLogger << "AgentData: " << agentData_ << EOL;
    theLogger << flush;
  }

  return theString != 0;
}

char const* OTC_ActionCronJob::parseField(
 char const* theString,
 OTC_BitSet& theBitSet,
 bool& theWildcard,
 char const** theWords
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_ActionCronJob::parseField(char const*,OTC_BitSet&,char const*)");

  OTCLIB_TRACER(MODULE) << "str (fn) = " << theString << endl;

  if (theString == 0 || *theString == EOS)
    return 0;

  int theStart = -1;
  int theEnd = -1;

  theWildcard = false;

  while (*theString != EOS && !isspace(*theString))
  {
    OTCLIB_TRACER(MODULE) << "str (while) = " << theString << endl;

    int skip = 0;

    if (*theString == '*')
    {
      OTCLIB_TRACER(MODULE) << "str (star) = " << theString << endl;

      theStart = theBitSet.range().lower();
      theEnd = theBitSet.range().upper()-1;

      theWildcard = true;

      ++theString;

      skip = 1;
    }
    else if (*theString >= '0' && *theString <= '9')
    {
      OTCLIB_TRACER(MODULE) << "str (number) = " << theString << endl;

      if (theStart < 0)
	theStart = strtol(theString,(char**)&theString,10);
      else
	theEnd = strtol(theString,(char**)&theString,10);

      skip = 1;
    }
    else if (theWords)
    {
      OTCLIB_TRACER(MODULE) << "str (words) = " << theString << endl;

      int i;

      for (i=0; theWords[i] != 0; ++i)
      {
	OTCLIB_TRACER(MODULE) << "word = " << theWords[i] << endl;

	if (strncmp(theString,theWords[i],strlen(theWords[i])) == 0)
	  break;
      }
      if (theWords[i] != 0)
      {
	OTCLIB_TRACER(MODULE) << "word/index = " << i << endl;

	theString += strlen(theWords[i]);

	OTCLIB_TRACER(MODULE) << "upper = "
	 << theBitSet.range().upper() << endl;

	if (theStart < 0)
	  theStart = i % theBitSet.range().upper();
	else
	  theEnd = i % theBitSet.range().upper();

	skip = 1;
      }
    }

    OTCLIB_TRACER(MODULE) << "skip = " << skip << endl;
    OTCLIB_TRACER(MODULE) << "start = " << theStart << endl;
    OTCLIB_TRACER(MODULE) << "end = " << theEnd << endl;

    if (skip == 0)
      return 0;

    if (*theString == '-' && theEnd < 0)
    {
      theString++;
      continue;
    }

    if (theEnd < 0)
      theEnd = theStart;

    if (theEnd < theStart)
      return 0;

    if (*theString == '/')
      skip = strtol(theString+1,(char**)&theString,10);

    if (!theBitSet.range().contains(theStart))
      return 0;

    if (!theBitSet.range().contains(theEnd))
      return 0;

    if (skip == 1)
    {
      OTCLIB_TRACER(MODULE) << "bitset = "
       << theStart << "..." << theEnd << endl;

      theBitSet.set(theStart,theEnd-theStart+1);
    }
    else
    {
      OTCLIB_TRACER(MODULE) << "bitset =";

      for (int i=theStart; i<=theEnd; i += skip)
      {
	OTCLIB_TRACER(MODULE) << " " << i;

	theBitSet.set(i);
      }

      OTCLIB_TRACER(MODULE) << endl;
    }

    OTCLIB_TRACER(MODULE) << "start (fixed) = " << theStart << endl;
    OTCLIB_TRACER(MODULE) << "end (fixed) = " << theEnd << endl;

    if (*theString != ',')
      break;

    theString++;

    theStart = -1;
    theEnd = -1;
  }

  if (*theString != EOS && !isspace(*theString))
    return 0;

  while (*theString != EOS && isspace(*theString))
    theString++;

  return theString;
}

/* ------------------------------------------------------------------------- */
OTC_Mutex OTCEV_Action::mutex_;
OTC_LinkList* OTCEV_Action::gSubscriptions_ = 0;
OTC_LinkList* OTCEV_Action::gCronJobs_ = 0;
OTC_HashTable* OTCEV_Action::gCronIndex_ = 0;
OTC_EVAgent* OTCEV_Action::gAgent_ = 0;
time_t OTCEV_Action::gLastTime_ = 0;
int OTCEV_Action::gTypeId_ = 0;
int OTCEV_Action::gActionIdCount_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Action::~OTCEV_Action()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Action::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Action::dump(ostream& outs) const
{
  outs << "<OTC> ACTION - type = " << actionType_
   << ", action = " << action() << ", description = "
   << description() << ", data = " << data();
}

/* ------------------------------------------------------------------------- */
int OTCEV_Action::schedule(
 int theAgentId,
 int theType,
 char const* theDescription,
 int theAgentData
)
{
  OTCLIB_ENSURE_FN((theAgentId != 0),
   "OTCEV_Action::schedule(int, int, char const*, int)",
   "invalid agent ID");

  int theActionId = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSubscriptions_ == 0)
  {
    gSubscriptions_ = new OTC_LinkList;
    OTCLIB_ASSERT_M(gSubscriptions_ != 0);

    gCronJobs_ = new OTC_LinkList;
    OTCLIB_ASSERT_M(gCronJobs_);

    gCronIndex_ = new OTC_HashTable;
    OTCLIB_ASSERT_M(gCronIndex_);
  }

  gActionIdCount_++;
  if (gActionIdCount_ == 0)
    gActionIdCount_++;

  theActionId = gActionIdCount_;

  OTC_ActionSubscription* theSubscription;
  theSubscription = new OTC_ActionSubscription(
   theAgentId,theActionId,theDescription,theAgentData);
  OTCLIB_ASSERT_M(theSubscription != 0);
  gSubscriptions_->addLast(theSubscription);

  OTC_ActionJob* theJob;
  theJob = new OTC_ActionJob(theSubscription);
  OTCLIB_ASSERT_M(theJob != 0);

  OTC_Dispatcher::schedule(theJob,theType);

  xxxMutex.release();

  mutex_.unlock();

  return theActionId;
}

/* ------------------------------------------------------------------------- */
int OTCEV_Action::schedule(
 int theAgentId,
 char const* theSpecification,
 char const* theDescription,
 int theAgentData
)
{
  OTCLIB_ENSURE_FN((theAgentId != 0),
   "OTCEV_Action::schedule(int, char const*, int)",
   "invalid agent ID");

  int theActionId = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSubscriptions_ == 0)
  {
    gSubscriptions_ = new OTC_LinkList;
    OTCLIB_ASSERT_M(gSubscriptions_ != 0);

    gCronJobs_ = new OTC_LinkList;
    OTCLIB_ASSERT_M(gCronJobs_);

    gCronIndex_ = new OTC_HashTable;
    OTCLIB_ASSERT_M(gCronIndex_);
  }

  gActionIdCount_++;
  if (gActionIdCount_ == 0)
    gActionIdCount_++;

  theActionId = gActionIdCount_;

  int tmpHashValue = 0;
  int tmpRank;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_ActionCronJob* tmpIndexEntry = 0;

  while (tmpHashValue != theActionId)
  {
    tmpHashValue = theActionId;
    tmpEntry = gCronIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0)
    {
      if (!tmpEntry->isOccupied())
      {
	if (tmpAvailableIndex == -1)
	  tmpAvailableIndex = tmpEntryIndex;
      }

      if (tmpEntry->isEmpty())
	break;

      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_ActionCronJob*)tmpEntry->item();

	tmpRank = theActionId - tmpIndexEntry->actionId();
	if (tmpRank == 0)
	{
	  theActionId = gActionIdCount_++;
	  if (theActionId == 0)
	    theActionId = gActionIdCount_++;

	  break;
	}
      }

      tmpEntry = gCronIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }
  }

  OTC_ActionCronJob* theCronJob;
  theCronJob = new OTC_ActionCronJob(theActionId,
   theAgentId,theDescription,theAgentData);
  OTCLIB_ASSERT_M(theCronJob != 0);

  if (theCronJob->parse(theSpecification))
  {
    gCronJobs_->addLast(theCronJob);
    gCronIndex_->add(tmpAvailableIndex,(void*)theCronJob,tmpHashValue);

    if (gAgent_ == 0)
    {
      gAgent_ = new OTC_FNAgent(OTCEV_Action::handle_);
      OTCLIB_ASSERT_M(gAgent_ != 0);
    }

    if (gLastTime_ == 0)
    {
      time_t theTime = time(0);

      gLastTime_ = time(0);

      theTime = theTime - (theTime % 60) + 60;

      OTCEV_Alarm::set(gAgent_->id(),theTime);
    }
  }
  else
  {
    theCronJob->kill();
    theActionId = 0;
  }

  xxxMutex.release();

  mutex_.unlock();

  return theActionId;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Action::cancel(int theActionId)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSubscriptions_ != 0)
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_ActionCronJob* tmpIndexEntry = 0;

    tmpHashValue = theActionId;
    tmpEntry = gCronIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_ActionCronJob*)tmpEntry->item();
	tmpRank = theActionId - tmpIndexEntry->actionId();
	if (tmpRank == 0)
	{
	  gCronIndex_->remove(tmpEntryIndex);
	  tmpIndexEntry->kill();

	  break;
	}
      }

      tmpEntry = gCronIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    OTC_LinkIterator items = gSubscriptions_->items();
    for (items.next(); !items.isEnd(); items.next())
    {
      OTC_ActionSubscription* theSubscription;
      theSubscription = (OTC_ActionSubscription*)items.link();
      if (theSubscription->action() == theActionId)
      {
	theSubscription->cancel();
	break;
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Action::cancelAgent(int theAgentId)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSubscriptions_ != 0)
  {
    OTC_LinkIterator items1 = gCronJobs_->items();
    for (items1.next(); !items1.isEnd(); items1.next())
    {
      OTC_ActionCronJob* theCronJob;
      theCronJob = (OTC_ActionCronJob*)items1.link();
      if (theCronJob->agentId() == theAgentId)
	cancel(theCronJob->actionId());
    }

    OTC_LinkIterator items2 = gSubscriptions_->items();
    for (items2.next(); !items2.isEnd(); items2.next())
    {
      OTC_ActionSubscription* theSubscription;
      theSubscription = (OTC_ActionSubscription*)items2.link();
      if (theSubscription->agent() == theAgentId)
	theSubscription->cancel();
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Action::cancelAgent(
 int theAgentId,
 char const* theDescription,
 int theAgentData
)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gSubscriptions_ != 0)
  {
    OTC_LinkIterator items1 = gCronJobs_->items();
    for (items1.next(); !items1.isEnd(); items1.next())
    {
      OTC_ActionCronJob* theCronJob;
      theCronJob = (OTC_ActionCronJob*)items1.link();
      if (theCronJob->agentId() == theAgentId &&
       theCronJob->description() == theDescription &&
       theCronJob->agentData() == theAgentData)
      {
	cancel(theCronJob->actionId());
      }
    }

    OTC_LinkIterator items2 = gSubscriptions_->items();
    for (items2.next(); !items2.isEnd(); items2.next())
    {
      OTC_ActionSubscription* theSubscription;
      theSubscription = (OTC_ActionSubscription*)items2.link();
      if (theSubscription->agent() == theAgentId &&
       theSubscription->description() == theDescription &&
       theSubscription->agentData() == theAgentData)
      {
	theSubscription->cancel();
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Action::active(int theActionId)
{
  bool theStatus;
  theStatus = false;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_ActionSubscription* theSubscription = 0;

  if (gSubscriptions_ != 0)
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_ActionCronJob* tmpIndexEntry = 0;

    tmpHashValue = theActionId;
    tmpEntry = gCronIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_ActionCronJob*)tmpEntry->item();
	tmpRank = theActionId - tmpIndexEntry->actionId();
	if (tmpRank == 0)
	{
	  theStatus = true;
	  break;
	}
      }

      tmpEntry = gCronIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    if (theStatus == false)
    {
      OTC_LinkIterator items = gSubscriptions_->items();
      for (items.next(); theStatus==false
       && !items.isEnd(); items.next())
      {
	theSubscription = (OTC_ActionSubscription*)items.link();
	if (theSubscription->action() == theActionId)
	  theStatus = true;
      }
    }
  }

  if (theStatus != false)
    theStatus = theSubscription->active();

  xxxMutex.release();

  mutex_.unlock();

  return theStatus;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Action::cancelSource(int theAgentId)
{
  OTCLIB_MARKBLOCK(MODULE,"OTCEV_Action::cancelSource(int)");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;

  OTCEV_Action::cancelAgent(theAgentId);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Action::handle_(OTC_Event* theEvent, int theAgentId)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Alarm::typeId())
  {
    time_t theTime = time(0);

    theTime = theTime - (theTime % 60);

    mutex_.lock();

    OTC_MutexReaper<OTC_Mutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (!gCronJobs_->isEmpty())
    {
      long theDelta = theTime - gLastTime_;

      if (theDelta < -60*60 || theDelta > 60*60)
      {
	gLastTime_ = theTime;
      }
      else if (theDelta > 0)
      {
	OTC_LinkIterator items1 = gCronJobs_->items();
	for (items1.next(); !items1.isEnd(); items1.next())
	{
	  OTC_ActionCronJob* theCronJob;
	  theCronJob = (OTC_ActionCronJob*)items1.link();

	  if (theCronJob->isReady(gLastTime_,theTime))
	  {
	    OTCEV_Action* tmpEvent;

	    tmpEvent = new OTCEV_Action(OTCLIB_ACTION_CRON,
	     theCronJob->actionId(),theCronJob->description(),
	     theCronJob->agentData());
	    OTCLIB_ASSERT_M(tmpEvent != 0);

	    tmpEvent->queue(theCronJob->agentId(),OTCLIB_PRIORITY_JOB);
	  }
	}

	gLastTime_ = theTime;
      }

      theTime = theTime + 60;

      OTCEV_Alarm::set(theAgentId,theTime);
    }
    else
    {
      gLastTime_ = 0;
    }

    xxxMutex.release();

    mutex_.unlock();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
bool OTCEV_Action::matches(char const* theDescription, time_t theTime)
{
  OTCLIB_MARKBLOCK(MODULE,"OTCEV_Action::matches(char const*,time_t)");

  OTC_ActionCronJob* theJob;
  theJob = new OTC_ActionCronJob(0,0,0,0);
  OTCLIB_ASSERT_M(theJob != 0);

  theJob->reference();

  if (!theJob->parse(theDescription))
  {
    theJob->unReference();

    return false;
  }

  bool theResult;

  theResult = theJob->isReady(theTime);

  theJob->unReference();

  return theResult;
}

/* ------------------------------------------------------------------------- */
