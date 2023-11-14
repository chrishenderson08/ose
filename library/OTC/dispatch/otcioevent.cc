/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcioevent.cc
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
#pragma implementation "OTC/dispatch/ioevent.hh"
#endif
#endif

#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/jobqueue.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/hashtble.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/iostream.h>

#include <stdio.h>

// Use poll() instead of select(), unless we are on SunOS 4.1.X.

#if (defined(sun) || defined(__sun)) \
 && defined(HAVE_SELECT) && defined(HAVE_POLL)
#if defined(HAVE_SYS_RESOURCE_H) && !defined(HAVE_SYS_ELF_H)
#undef HAVE_POLL
#endif
#endif

#if defined(LYNX)
#undef HAVE_POLL
#endif

#if defined(HAVE_SELECT) && defined(HAVE_POLL)
#undef HAVE_SELECT
#endif

#if defined(HAVE_POLL)
#if defined(HAVE_STROPTS_H)
#include <stropts.h>
#endif
#define reqevents events
#define rtnevents revents
#include <poll.h>
#undef reqevents
#undef rtnevents
#undef events
#undef revents
#endif

#if defined(HAVE_SELECT)
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#if defined(HAVE_SYS_SELECT_H)
#include <sys/select.h>
#endif
#endif

#if defined(OSE_SYS_WIN32)
#include <winsock2.h>
#include <windows.h>
#undef HAVE_POLL
#define HAVE_SELECT
#endif

#ifndef NOFILE
#ifdef OPEN_MAX
#define NOFILE OPEN_MAX
#else
#ifdef FOPEN_MAX
#define NOFILE FOPEN_MAX
#else
#ifdef NFDS
#define NOFILE NFDS
#else
#ifdef _NFILE
#define NOFILE _NFILE
#else
#ifdef _NFILE_
#define NOFILE _NFILE_
#endif
#endif
#endif
#endif
#endif
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_IOAgentSubscription : public OTC_Link
{
  public:
    OTC_IOAgentSubscription() {}
    ~OTC_IOAgentSubscription();

    int			fd;
    int			agentId;
    int			events;
    int			suspend;
};

OTC_IOAgentSubscription::~OTC_IOAgentSubscription() {}

/* ------------------------------------------------------------------------- */
OTC_Mutex OTCEV_IOEvent::mutex_;
int OTCEV_IOEvent::globMaxFd_ = 0;
OTC_LinkList* OTCEV_IOEvent::globAgentSubscriptions_ = 0;
OTC_HashTable* OTCEV_IOEvent::globSubscriptionIndex_ = 0;
int OTCEV_IOEvent::globTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_IOEvent::~OTCEV_IOEvent()
{
  if (agentId_ != 0)
    resumeSubscription_(agentId_,fd_,events_);
}

/* ------------------------------------------------------------------------- */
OTCEV_IOEvent::OTCEV_IOEvent(int theFd, int theEvents, int theAgentId)
  : fd_(theFd), events_(theEvents), agentId_(theAgentId)
{
  if (agentId_ != 0)
    suspendSubscription_(theAgentId,theFd,theEvents);
}

/* ------------------------------------------------------------------------- */
void* OTCEV_IOEvent::type() const
{
  return &globTypeId_;
}

/* ------------------------------------------------------------------------- */
void OTCEV_IOEvent::dump(ostream& outs) const
{
  outs << "<OTC> IOEVENT - fd = " << fd() << ", events = " << events();
}

/* ------------------------------------------------------------------------- */
int OTCEV_IOEvent::agent(int theFd)
{
  if (validFd_(theFd) == false)
    return 0;

  int theResult;
  theResult = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptionIndex_ != 0 && !globSubscriptionIndex_->isEmpty())
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_IOAgentSubscription* tmpIndexEntry = 0;

    tmpHashValue = theFd;
    tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_IOAgentSubscription*)tmpEntry->item();
	tmpRank = theFd - tmpIndexEntry->fd;
	if (tmpRank == 0)
	  break;

	tmpIndexEntry = 0;
      }

      tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    if (tmpIndexEntry != 0)
      theResult = tmpIndexEntry->agentId;
  }

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTCEV_IOEvent::subscribe(int theAgentId, int theFd, int theEvents)
{
  OTCLIB_ENSURE_FN((validFd_(theFd) != false),
   "OTCEV_IOEvent::subscribe(int, int, int)",
   "invalid file descriptor");
  OTCLIB_ENSURE_FN((theAgentId != 0),
   "OTCEV_IOEvent::subscribe(int, int, int)",
   "invalid agent ID");
  OTCLIB_ENSURE_FN((numSubscriptions() < maxSubscriptions()),
   "OTCEV_IOEvent::subscribe(int, int, int)",
   "too many subscriptions");
  
  theEvents &= (OTCLIB_POLLIN | OTCLIB_POLLPRI | OTCLIB_POLLOUT);

  if (theEvents == 0)
    return;

  int oldAgentId = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globAgentSubscriptions_ == 0)
  {
    globAgentSubscriptions_ = new OTC_LinkList;
    OTCLIB_ASSERT_M(globAgentSubscriptions_ != 0);
    globSubscriptionIndex_ = new OTC_HashTable;
    OTCLIB_ASSERT_M(globSubscriptionIndex_ != 0);

    globMaxFd_ = -1;
  }

  int tmpRank;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_IOAgentSubscription* tmpIndexEntry = 0;

  tmpEntry = globSubscriptionIndex_->searchHash(theFd,
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
      tmpIndexEntry = (OTC_IOAgentSubscription*)tmpEntry->item();
      tmpRank = theFd - tmpIndexEntry->fd;
      if (tmpRank == 0)
        break;

      tmpIndexEntry = 0;
    }

    tmpEntry = globSubscriptionIndex_->searchHash(theFd,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  if (tmpIndexEntry != 0)
    oldAgentId = tmpIndexEntry->agentId;

  if (oldAgentId == 0 || oldAgentId == theAgentId)
  {
    if (theFd > globMaxFd_)
      globMaxFd_ = theFd;

    if (tmpIndexEntry == 0)
    {
      tmpIndexEntry = new OTC_IOAgentSubscription;
      OTCLIB_ASSERT_M(tmpIndexEntry != 0);

      tmpIndexEntry->fd = theFd;
      tmpIndexEntry->agentId = 0;
      tmpIndexEntry->events = 0;
      tmpIndexEntry->suspend = 0;

      globAgentSubscriptions_->addLast(tmpIndexEntry);
      globSubscriptionIndex_->add(tmpAvailableIndex,(void*)tmpIndexEntry,theFd);
    }

    tmpIndexEntry->agentId = theAgentId;
    tmpIndexEntry->events |= theEvents;
  }

  xxxMutex.release();

  mutex_.unlock();

  OTCLIB_ENSURE_FN((oldAgentId == 0 || oldAgentId == theAgentId),
   "OTCEV_IOEvent::subscribe(int, int, int)",
   "file descriptor subscribed by other agent");

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_IOEVENT);
}

/* ------------------------------------------------------------------------- */
void OTCEV_IOEvent::unsubscribe(int theAgentId, int theFd, int theEvents)
{
  if (theAgentId == 0 || validFd_(theFd) == false)
    return;

  theEvents &= (OTCLIB_POLLIN | OTCLIB_POLLPRI | OTCLIB_POLLOUT);

  if (theEvents == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptionIndex_ != 0 && !globSubscriptionIndex_->isEmpty())
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_IOAgentSubscription* tmpIndexEntry = 0;

    tmpHashValue = theFd;
    tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_IOAgentSubscription*)tmpEntry->item();
	tmpRank = theFd - tmpIndexEntry->fd;
	if (tmpRank == 0)
	  break;

	tmpIndexEntry = 0;
      }

      tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    if (tmpIndexEntry != 0)
    {
      if (tmpIndexEntry->agentId == theAgentId)
      {
	int tmpEvents = tmpIndexEntry->events & ~theEvents;
	tmpEvents &= (OTCLIB_POLLIN | OTCLIB_POLLPRI | OTCLIB_POLLOUT);
	tmpIndexEntry->events = tmpEvents;

	if (tmpIndexEntry->events == 0)
	{
	  globSubscriptionIndex_->remove(tmpEntryIndex);
	  tmpIndexEntry->kill();
	}
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_IOEVENT);
}

/* ------------------------------------------------------------------------- */
void OTCEV_IOEvent::unsubscribeFd(int theFd)
{
  if (validFd_(theFd) == false)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptionIndex_ != 0 && !globSubscriptionIndex_->isEmpty())
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_IOAgentSubscription* tmpIndexEntry = 0;

    tmpHashValue = theFd;
    tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_IOAgentSubscription*)tmpEntry->item();
	tmpRank = theFd - tmpIndexEntry->fd;
	if (tmpRank == 0)
	  break;

	tmpIndexEntry = 0;
      }

      tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    if (tmpIndexEntry != 0)
    {
      globSubscriptionIndex_->remove(tmpEntryIndex);
      tmpIndexEntry->kill();
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_IOEVENT);
}

/* ------------------------------------------------------------------------- */
void OTCEV_IOEvent::unsubscribeAgent(int theAgentId)
{
  if (theAgentId == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globAgentSubscriptions_ != 0 && !globAgentSubscriptions_->isEmpty())
  {
    OTC_LinkIterator theItems = globAgentSubscriptions_->items();
    theItems.resetFirst();
    while (theItems.isLink())
    {
      OTC_IOAgentSubscription* theSubscription;
      theSubscription = (OTC_IOAgentSubscription*)theItems.link();
      if (theSubscription->agentId == theAgentId)
	unsubscribeFd(theSubscription->fd);
      theItems.next();
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_IOEVENT);
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTCEV_IOEvent::job(int theFd, int theEvents)
{
  OTC_EventJob* theJob;
  theJob = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptionIndex_ != 0 && !globSubscriptionIndex_->isEmpty())
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_IOAgentSubscription* tmpIndexEntry = 0;

    tmpHashValue = theFd;
    tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_IOAgentSubscription*)tmpEntry->item();
	tmpRank = theFd - tmpIndexEntry->fd;
	if (tmpRank == 0)
	  break;

	tmpIndexEntry = 0;
      }

      tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    if (tmpIndexEntry != 0)
    {
      int tmpEvents = 0;
      tmpEvents |= theEvents & tmpIndexEntry->events;
      tmpEvents |= theEvents &
       (OTCLIB_POLLERR | OTCLIB_POLLHUP | OTCLIB_POLLNVAL);

      if (tmpEvents != 0)
      {
	OTCEV_IOEvent* theEvent;
	theEvent = new OTCEV_IOEvent(theFd,tmpEvents,tmpIndexEntry->agentId);
	OTCLIB_ASSERT_M(theEvent != 0);

	theJob = new OTC_EventJob(tmpIndexEntry->agentId,theEvent);
	OTCLIB_ASSERT_M(theJob != 0);
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  return theJob;
}

/* ------------------------------------------------------------------------- */
int OTCEV_IOEvent::events(int theFd)
{
  if (validFd_(theFd) == false)
    return 0;

  int theResult;
  theResult = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptionIndex_ != 0 && !globSubscriptionIndex_->isEmpty())
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_IOAgentSubscription* tmpIndexEntry = 0;

    tmpHashValue = theFd;
    tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_IOAgentSubscription*)tmpEntry->item();
	tmpRank = theFd - tmpIndexEntry->fd;
	if (tmpRank == 0)
	  break;

	tmpIndexEntry = 0;
      }

      tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    if (tmpIndexEntry != 0)
      theResult = tmpIndexEntry->events;
  }

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
int OTCEV_IOEvent::maxSubscriptions()
{
#if defined(HAVE_POLL)
  return nfds_();
#else
  return FD_SETSIZE;
#endif
}

/* ------------------------------------------------------------------------- */
int OTCEV_IOEvent::numSubscriptions()
{
  int theResult;
  theResult = 0;

  mutex_.lock();

  if (globSubscriptionIndex_ != 0)
    theResult = globSubscriptionIndex_->population();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
int OTCEV_IOEvent::subscriptions(OTC_IOSubscription* theSubscriptions)
{
  OTCLIB_ENSURE_FN((theSubscriptions != 0),
   "OTCEV_IOEvent::subscriptions(OTC_IOSubscription*)",
   "invalid array to fill out");

  mutex_.lock();

  u_int theIndex = 0;

  if (globAgentSubscriptions_ != 0)
  {
    OTC_LinkIterator theItems = globAgentSubscriptions_->items();
    theItems.resetFirst();
    while (theItems.isLink())
    {
      OTC_IOAgentSubscription* theSubscription;
      theSubscription = (OTC_IOAgentSubscription*)theItems.link();

      int theEvents = 0;

      theEvents |= ~theSubscription->suspend;
      theEvents &= theSubscription->events;

      theEvents &= (OTCLIB_POLLIN | OTCLIB_POLLPRI | OTCLIB_POLLOUT);

      if (theEvents != 0)
      {
	theSubscriptions[theIndex].fd = theSubscription->fd;
	theSubscriptions[theIndex].events = theEvents;
	theIndex++;
      }
      theItems.next();
    }
  }

  mutex_.unlock();

  return theIndex;
}

/* ------------------------------------------------------------------------- */
void OTCEV_IOEvent::cancelSource(int theAgentId)
{
  OTCEV_IOEvent::unsubscribeAgent(theAgentId);

  if (OTC_Dispatcher::warningsEnabled())
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTCEV_IOEvent" << EOL;
    theLogger << "Method: cancelSource()" << EOL;
    theLogger << "Description: specified agent doesn't exist" << EOL;
    theLogger << "Result: i/o event subscription cancelled" << EOL;
    theLogger << "AgentId: " << theAgentId << EOL;
    theLogger << flush;
  }
}

/* ------------------------------------------------------------------------- */
int OTCEV_IOEvent::nfds_()
{
  // Assumed that this is always called from inside a lock.
  // Not really supposed to cache these values as they could
  // change if resource limits of a program are changed. But
  // then we base the size of an array on the first call and
  // thus probably do not want this to change, ie., get larger.
  // Can only assume that when first called that limit is at
  // the max it will for the program.

#ifdef HAVE_SYSCONF_SC_OPEN_MAX
  static int nfds = (int)sysconf(_SC_OPEN_MAX);
#else
#ifdef HAVE_GETDTABLESIZE
  static int nfds = getdtablesize();
#else
  static int nfds = NOFILE;
#endif
#endif
  return nfds;
}

/* ------------------------------------------------------------------------- */
bool OTCEV_IOEvent::validFd_(int theFd)
{
#if defined(OSE_SYS_WIN32)
  if (theFd == -1 || theFd == (int)INVALID_SOCKET)
    return false;
#else
  if (theFd < 0 || theFd >= nfds_())
    return false;
#endif

  return true;
}

/* ------------------------------------------------------------------------- */
void OTCEV_IOEvent::suspendSubscription_(
 int theAgentId,
 int theFd,
 int theEvents
)
{
  if (theAgentId == 0 || validFd_(theFd) == false)
    return;

  theEvents &= (OTCLIB_POLLIN | OTCLIB_POLLPRI | OTCLIB_POLLOUT);

  if (theEvents == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptionIndex_ != 0 && !globSubscriptionIndex_->isEmpty())
  {
    int tmpRank = 0;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_IOAgentSubscription* tmpIndexEntry = 0;

    tmpHashValue = theFd;
    tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_IOAgentSubscription*)tmpEntry->item();
	tmpRank = theFd - tmpIndexEntry->fd;
	if (tmpRank == 0)
	  break;

	tmpIndexEntry = 0;
      }

      tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    if (tmpIndexEntry != 0)
    {
      if (tmpIndexEntry->agentId == theAgentId)
	tmpIndexEntry->suspend |= theEvents;
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTCEV_IOEvent::resumeSubscription_(
 int theAgentId,
 int theFd,
 int theEvents
)
{
  if (theAgentId == 0 || validFd_(theFd) == false)
    return;

  theEvents &= (OTCLIB_POLLIN | OTCLIB_POLLPRI | OTCLIB_POLLOUT);

  if (theEvents == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globSubscriptionIndex_ != 0 && !globSubscriptionIndex_->isEmpty())
  {
    int tmpRank = 0;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_IOAgentSubscription* tmpIndexEntry = 0;

    tmpHashValue = theFd;
    tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_IOAgentSubscription*)tmpEntry->item();
	tmpRank = theFd - tmpIndexEntry->fd;
	if (tmpRank == 0)
	  break;

	tmpIndexEntry = 0;
      }

      tmpEntry = globSubscriptionIndex_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }

    if (tmpIndexEntry != 0)
    {
      if (tmpIndexEntry->agentId == theAgentId)
      {
	int tmpEvents = 0;

	tmpEvents |= ~theEvents;
	tmpEvents &= tmpIndexEntry->suspend;

	tmpEvents &= (OTCLIB_POLLIN | OTCLIB_POLLPRI | OTCLIB_POLLOUT);

	tmpIndexEntry->suspend = tmpEvents;
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  OTC_Dispatcher::wakeup(OTCLIB_WAKEUP_IOEVENT);
}

/* ------------------------------------------------------------------------- */
