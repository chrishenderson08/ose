/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcjobqueue.cc
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
#pragma implementation "OTC/dispatch/jobqueue.hh"
#endif
#endif

#include <OTC/dispatch/jobqueue.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/alarm.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/system/sigblock.hh>
#include <OTC/system/program.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#include <stdlib.h>
#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

// Avoid poll() on Lynx OS for some reason.

#if defined(LYNX)
#undef HAVE_POLL
#endif

// Avoid poll() if it is an emulation layer around select(). This
// is the case on Mac OS 10.3 (Panther).

#if defined(__APPLE__)
#if defined(HAVE_SELECT) && defined(HAVE_POLL)
#undef HAVE_POLL
#endif
#endif

// Avoid poll() on SunOS 4.1.X. Code probably will not compile on
// that platform anymore anyway.

#if (defined(sun) || defined(__sun)) \
 && defined(HAVE_SELECT) && defined(HAVE_POLL)
#if defined(HAVE_SYS_RESOURCE_H) && !defined(HAVE_SYS_ELF_H)
#undef HAVE_POLL
#endif
#endif

// Otherwise always use poll() instead of select().

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
#include <io.h>
#undef HAVE_POLL
#define HAVE_SELECT
// #ifndef WINSOCK_VERSION
// #define WINSOCK_VERSION MAKEWORD(1,0)
// #endif
#endif

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#if defined(HAVE_SYS_PARAM_H)
#include <sys/param.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
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

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_JobQueue");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
// #if defined(OSE_SYS_WIN32)
// extern "C" void otclib_winsock_cleanup() { WSACleanup(); }
// #endif

/* ------------------------------------------------------------------------- */
OTC_JobQueue::OTC_JobQueue(void (*theSetupFunc)())
  : state_(OTCLIB_WAITING),
    setup_(theSetupFunc)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_JobQueue:OTC_JobQueue()");

  pendingIdleJobs_ = new OTC_JobList;
  OTCLIB_ASSERT_M(pendingIdleJobs_ != 0);

  pendingStandardJobs_ = new OTC_JobList;
  OTCLIB_ASSERT_M(pendingStandardJobs_ != 0);

  idleJobs_ = new OTC_JobList;
  OTCLIB_ASSERT_M(idleJobs_ != 0);

  standardJobs_ = new OTC_JobList;
  OTCLIB_ASSERT_M(standardJobs_ != 0);

  priorityJobs_ = new OTC_JobList;
  OTCLIB_ASSERT_M(priorityJobs_ != 0);

  pendingFdEvents_ = new OTC_IOSubscription[OTCEV_IOEvent::maxSubscriptions()];
  OTCLIB_ASSERT_M(pendingFdEvents_ != 0);

  for (int i=OTCEV_IOEvent::maxSubscriptions(); i>0; i--)
  {
    pendingFdEvents_[i-1].fd = INVALID_SOCKET;
    pendingFdEvents_[i-1].events = 0;
  }

  // Force Winsock startup if required.
  // This is done by calling routine to
  // generate a unique ID which has a
  // side affect of initialising Winsock.

#if defined(OSE_SYS_WIN32)
  OTC_Program::uniqueId();
#endif

// #if defined(OSE_SYS_WIN32)
//   WSAData wsaData;
//   WSAStartup(WINSOCK_VERSION,&wsaData);
//   atexit(otclib_winsock_cleanup);
// #endif

  signalFds_[0] = INVALID_SOCKET;
  signalFds_[1] = INVALID_SOCKET;

  wakeupFds_[0] = INVALID_SOCKET;
  wakeupFds_[1] = INVALID_SOCKET;

  signalCount_ = 0;
  wakeupCount_ = 0;
  wakeupFlag_ = 0;

#if defined(OSE_SYS_WIN32)
  wmutex_ = CreateMutex(0,FALSE,0);

  SOCKET listensock = INVALID_SOCKET;
  SOCKET insock = INVALID_SOCKET;
  SOCKET outsock = INVALID_SOCKET;

  int addrlen = 0;
  sockaddr_in inaddr;
  sockaddr_in outaddr;

  listensock = socket(AF_INET,SOCK_STREAM,0);
  if (listensock != INVALID_SOCKET)
  {
    OTCLIB_TRACER(MODULE) << "listensock = " << listensock << endl;

    inaddr.sin_family = AF_INET;
    inaddr.sin_port = 0;
    inaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listensock,(LPSOCKADDR)&inaddr,sizeof(inaddr)) != SOCKET_ERROR)
    {
      OTCLIB_TRACER(MODULE) << "bind in" << endl;

      addrlen = sizeof(inaddr);
      if (getsockname(listensock,(LPSOCKADDR)&inaddr,&addrlen) != SOCKET_ERROR)
      {
	OTCLIB_TRACER(MODULE) << "in port = " << inaddr.sin_port << endl;

	outsock = socket(AF_INET,SOCK_STREAM,0);

        int optval = 1;
        setsockopt(outsock,IPPROTO_TCP,TCP_NODELAY,
         (LPSTR)&optval,sizeof(optval));
        setsockopt(outsock,SOL_SOCKET,SO_SNDBUF,
         (LPSTR)&optval,sizeof(optval));

	if (outsock != INVALID_SOCKET)
	{
	  OTCLIB_TRACER(MODULE) << "outsock = " << outsock << endl;

	  outaddr.sin_family = AF_INET;
	  outaddr.sin_port = 0;
	  outaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	  if (bind(outsock,(LPSOCKADDR)&outaddr,
	   sizeof(outaddr)) != SOCKET_ERROR)
	  {
	    if (listen(listensock,5) != SOCKET_ERROR)
	    {
	      OTCLIB_TRACER(MODULE) << "listen" << endl;

	      inaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	      if (connect(outsock,(LPSOCKADDR)&inaddr,
	       sizeof(inaddr)) != SOCKET_ERROR)
	      {
		OTCLIB_TRACER(MODULE) << "connect" << endl;

		addrlen = sizeof(outaddr);
		insock = accept(listensock,(LPSOCKADDR)&outaddr,&addrlen);
		if (insock != INVALID_SOCKET)
		{
		  OTCLIB_TRACER(MODULE) << "insock = " << insock << endl;

		  wakeupFds_[0] = insock;
		  wakeupFds_[1] = outsock;
		}
	      }
	    }
	  }
	}
      }
    }
  }

  int error = WSAGetLastError();
  OTCLIB_TRACER(MODULE) << "wsa error = " << error << endl;

  if (listensock != INVALID_SOCKET)
    closesocket(listensock);
  if (wakeupFds_[0] == INVALID_SOCKET && insock != INVALID_SOCKET)
    closesocket(insock);
  if (wakeupFds_[1] == INVALID_SOCKET && outsock != INVALID_SOCKET)
    closesocket(outsock);
#else
  if (pipe(signalFds_) == -1)
  {
    signalFds_[0] = INVALID_SOCKET;
    signalFds_[1] = INVALID_SOCKET;
  }
  else
  {
    fcntl(signalFds_[0],F_SETFD,FD_CLOEXEC);
    fcntl(signalFds_[1],F_SETFD,FD_CLOEXEC);
  }

#if defined(OSE_WITH_THREADS)
  if (pipe(wakeupFds_) == -1)
  {
    wakeupFds_[0] = INVALID_SOCKET;
    wakeupFds_[1] = INVALID_SOCKET;
  }
  else
  {
    fcntl(wakeupFds_[0],F_SETFD,FD_CLOEXEC);
    fcntl(wakeupFds_[1],F_SETFD,FD_CLOEXEC);
  }
#endif
#endif

  OTCLIB_TRACER(MODULE) << "signal[0] = " << signalFds_[0] << endl;
  OTCLIB_TRACER(MODULE) << "signal[1] = " << signalFds_[1] << endl;
  OTCLIB_TRACER(MODULE) << "wakeup[0] = " << wakeupFds_[0] << endl;
  OTCLIB_TRACER(MODULE) << "wakeup[1] = " << wakeupFds_[1] << endl;

#if defined(OSE_SYS_WIN32)
  if (wakeupFds_[0] == INVALID_SOCKET)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_JobQueue" << EOL;
    theLogger << "Method: OTC_JobQueue()" << EOL;
    theLogger << "Description: thread initialisation failed" << EOL;
    theLogger << "Result: threads may not unblock dispatcher" << EOL;
    theLogger << "Comment: has WSAStartup() function been called" << EOL;
    theLogger << flush;
  }
#else
#if defined(OSE_WITH_THREADS)
  if (signalFds_[0] == INVALID_SOCKET)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_JobQueue" << EOL;
    theLogger << "Method: OTC_JobQueue()" << EOL;
    theLogger << "Description: signal initialisation failed" << EOL;
    theLogger << "Result: signal handling may be delayed" << EOL;
    theLogger << flush;
  }

  if (wakeupFds_[0] == INVALID_SOCKET)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_JobQueue" << EOL;
    theLogger << "Method: OTC_JobQueue()" << EOL;
    theLogger << "Description: thread initialisation failed" << EOL;
    theLogger << "Result: threads may not unblock dispatcher" << EOL;
    theLogger << flush;
  }
#else
  if (signalFds_[0] == INVALID_SOCKET)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_JobQueue" << EOL;
    theLogger << "Method: OTC_JobQueue()" << EOL;
    theLogger << "Description: signal initialisation failed" << EOL;
    theLogger << "Result: signal handling may be delayed" << EOL;
    theLogger << flush;
  }
#endif
#endif

  nextFd_ = 0;
  maxFd_ = -1;
}

/* ------------------------------------------------------------------------- */
OTC_JobQueue::~OTC_JobQueue()
{
#if defined(OSE_SYS_WIN32)
  if (wakeupFds_[0] != INVALID_SOCKET)
  {
    closesocket(wakeupFds_[0]);
    closesocket(wakeupFds_[1]);
  }

  CloseHandle(wmutex_);
#else
  if (signalFds_[0] != INVALID_SOCKET)
  {
    close(signalFds_[0]);
    close(signalFds_[1]);
  }

#if defined(OSE_WITH_THREADS)
  if (wakeupFds_[0] != INVALID_SOCKET)
  {
    close(wakeupFds_[0]);
    close(wakeupFds_[1]);
  }
#endif
#endif

  delete idleJobs_;
  delete priorityJobs_;
  delete pendingIdleJobs_;
  delete [] pendingFdEvents_;
}

/* ------------------------------------------------------------------------- */
void OTC_JobQueue::add(OTC_Job* theJob, int theType)
{
  OTCLIB_ENSURE_FN((theJob != 0),
   "OTC_JobQueue::add(OTC_Job*, int)",
   "invalid job");

  qmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(qmutex_);

  if (theType == OTCLIB_IDLE_JOB)
    pendingIdleJobs_->add(theJob);
  else if (theType == OTCLIB_STANDARD_JOB)
    pendingStandardJobs_->add(theJob);
  else
    priorityJobs_->add(theJob);

  xxxMutex.release();

  qmutex_.unlock();

  if (theType == OTCLIB_IDLE_JOB)
    wakeup(OTCLIB_WAKEUP_IDLE_JOB);
  else if (theType == OTCLIB_STANDARD_JOB)
    wakeup(OTCLIB_WAKEUP_STANDARD_JOB);
  else
    wakeup(OTCLIB_WAKEUP_PRIORITY_JOB);
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTC_JobQueue::next(int theActions, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_JobQueue:next(int,int)");

  theActions = theActions & (OTCLIB_ALL_JOBS | OTCLIB_ALL_DONT_WAIT);
  if ((theActions & OTCLIB_ALL_JOBS) == 0)
    theActions |= OTCLIB_ALL_JOBS;

  OTC_Job* theJob = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

#if defined(OSE_SYS_WIN32)
    WaitForSingleObject(wmutex_,INFINITE);
#else
    wmutex_.lock();
#endif

    wakeupFlag_ = 1;

#if defined(OSE_SYS_WIN32)
    ReleaseMutex(wmutex_);
#else
    wmutex_.unlock();
#endif

  while (theJob == 0)
  {
    // Clear pipes of any known pending input.

    clearPipes();

    // Check for signals.

    if (theActions & OTCLIB_SIGNAL_JOBS)
    {
      theJob = OTCEV_Signal::pending();

      if (theJob != 0)
	break;
    }

    // Check for priority jobs.

    if (theActions & OTCLIB_PRIORITY_JOBS)
    {
      theJob = nextPriorityJob();

      if (theJob != 0)
	break;
    }

    // Check for alarms.

    if (theActions & OTCLIB_ALARM_JOBS)
    {
      theJob = OTCEV_Alarm::pending();

      if (theJob != 0)
	break;
    }

    // Check for io events which haven't yet been processed.

    if (theActions & OTCLIB_IO_JOBS)
    {
      while (nextFd_ <= maxFd_)
      {
        int theIndex = nextFd_++;
        int theFd = pendingFdEvents_[theIndex].fd;
        int theEvents = pendingFdEvents_[theIndex].events;

	if (theEvents != 0)
	{
          pendingFdEvents_[theIndex].fd = INVALID_SOCKET;
          pendingFdEvents_[theIndex].events = 0;
	  theJob = OTCEV_IOEvent::job(theFd,theEvents);

	  if (theJob != 0)
	    break;
	}
      }

      if (theJob != 0)
	break;
    }

    // Check for timeouts, but only if we have
    // standard jobs to process.

    if (theActions & OTCLIB_TIMEOUT_JOBS)
    {
      if (!isStandardJobsEmpty())
      {
	theJob = OTCEV_Timeout::pending();

	if (theJob != 0)
	  break;
      }
    }

    // Check if we have standard jobs to process.

    if (theActions & OTCLIB_STANDARD_JOBS)
    {
      theJob = nextStandardJob();

      if (theJob != 0)
	break;
    }

    // Check if we have idle jobs to process.

    if (theActions & OTCLIB_IDLE_JOBS)
    {
      theJob = nextIdleJob();

      if (theJob != 0)
	break;
    }

    // Check for timeouts, but only if we didn't
    // have standard jobs to process.

    if (theActions & OTCLIB_TIMEOUT_JOBS)
    {
      if (isStandardJobsEmpty())
      {
	theJob = OTCEV_Timeout::pending();

	if (theJob != 0)
	  break;
      }
    }

    // Fill up standard jobs, but don't execute any.

    if (theActions & OTCLIB_STANDARD_JOBS)
      updateStandardJobs();

    // Work out period of timeout.

    long alarmPeriod = -1;
    long timeoutPeriod = -1;
    long period = -1;

    if (theActions & OTCLIB_ALARM_JOBS)
      alarmPeriod = 1000 * OTCEV_Alarm::period();

    if (theActions & OTCLIB_TIMEOUT_JOBS)
      timeoutPeriod = OTCEV_Timeout::period();

    if (alarmPeriod < 0 && timeoutPeriod >= 0)
      period = timeoutPeriod;
    else if (timeoutPeriod < 0 && alarmPeriod >= 0)
      period = alarmPeriod;
    else if (alarmPeriod >= 0 && timeoutPeriod >= 0)
      period = (timeoutPeriod < alarmPeriod) ? timeoutPeriod : alarmPeriod;

    // Set timeout period to zero if we shouldn't wait.

    long rawPeriod = period;

    if (theActions & OTCLIB_DONT_WAIT)
      period = 0;

#if defined(HAVE_POLL)

    // Calculate timeout for <poll()>.

    int timeout = int(period);

    // Calculate io events we are interested in.

    static pollfd* fds = new pollfd[OTCEV_IOEvent::maxSubscriptions()];
    OTCLIB_ASSERT_M(fds != 0);

    int numFd = 0;
    int fdCnt = 0;

    if (theActions & OTCLIB_IO_JOBS)
    {
      fdCnt = OTCEV_IOEvent::subscriptions(pendingFdEvents_);
      for (int i=0; i<fdCnt; i++)
      {
	if (pendingFdEvents_[i].events != 0)
	{
	  fds[numFd].fd = pendingFdEvents_[i].fd;
	  fds[numFd].events = pendingFdEvents_[i].events;
	  fds[numFd].revents = 0;
	  numFd++;
	}
      }
    }

    int signalFd = INVALID_SOCKET;
    int wakeupFd = INVALID_SOCKET;

    if (theActions & OTCLIB_SIGNAL_JOBS)
    {
      signalFd = signalFds_[0];
      if (
       signalFd != INVALID_SOCKET &&
       OTCEV_Signal::signalsSubscribed() != 0
      )
      {
	fds[numFd].fd = signalFd;
	fds[numFd].events = POLLIN;
	fds[numFd].revents = 0;
	numFd++;

	OTCLIB_TRACER(MODULE) << "signal fd = " << signalFd << endl;
      }
    }

    wakeupFd = wakeupFds_[0];
    if (
     wakeupFd != INVALID_SOCKET &&
     (theActions & OTCLIB_THREADS_DONT_WAIT) == 0
    )
    {
      fds[numFd].fd = wakeupFd;
      fds[numFd].events = POLLIN;
      fds[numFd].revents = 0;
      numFd++;

      OTCLIB_TRACER(MODULE) << "wakeup fd = " << wakeupFd << endl;
    }

    // Do the <poll()> call.

    int res = 0;

    if (
     ((theActions & OTCLIB_IDLE_JOBS) && !isPendingIdleJobsEmpty()) ||
     ((theActions & OTCLIB_STANDARD_JOBS) && !isStandardJobsEmpty())
    )
    {
      if (numFd > 0)
	res = poll(fds,numFd,0);

      if (res == 0)
      {
	if (rawPeriod != 0)
	{
          if (theActions & OTCLIB_STANDARD_JOBS)
	  {
	    theJob = nextStandardJob();

	    if (theJob != 0)
	      break;
	  }

          if (theActions & OTCLIB_IDLE_JOBS)
	  {
	    updateIdleJobs();

	    theJob = nextIdleJob();

	    if (theJob != 0)
	      break;
	  }
	}
      }
    }

    res = 0;

    if (numFd > 0 || period != -1)
      res = poll(fds,numFd,timeout);
    else
      break;

    // Generate io events.

    if (res > 0)
    {
      nextFd_ = 0;
      maxFd_ = -1;

      for (int i=0; i<numFd; i++)
      {
	if (fds[i].revents != 0)
	{
	  if (fds[i].revents & POLLNVAL)
	  {
	    theJob = OTCEV_IOEvent::job(i,OTCLIB_POLLNVAL);

	    OTCEV_IOEvent::unsubscribeFd(fds[i].fd);

	    if (theJob != 0)
	      break;
	  }
	  else
	  {
	    pendingFdEvents_[nextFd_].fd = fds[i].fd;
	    pendingFdEvents_[nextFd_].events = fds[i].revents;
	    nextFd_++;
	  }
	}
      }

      if (nextFd_ != 0)
      {
        maxFd_ = nextFd_ - 1;
        nextFd_ = 0;
      }

      if (theJob != 0)
	break;
    }
    else if (res == 0)
    {
      if (theActions & OTCLIB_DONT_WAIT)
      {
	if (rawPeriod != 0)
	  break;
      }
    }

#else
#if defined(HAVE_SELECT)

    // Calculate timeout for <select()>.

    timeval timeout;
    timeval* timeoutp = 0;

    if (period != -1)
    {
      timeoutp = &timeout;
      timeout.tv_sec = period / 1000;
      timeout.tv_usec = 1000 * (period % 1000);
    }

    // Calculate ioevents we are interested in.

    int maxFd = -1;

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    int fdCnt = 0;

    if (theActions & OTCLIB_IO_JOBS)
    {
      fdCnt = OTCEV_IOEvent::subscriptions(pendingFdEvents_);
      for (int i=0; i<fdCnt; i++)
      {
        int theFd = pendingFdEvents_[i].fd;
        int theEvents = pendingFdEvents_[i].events;

        if (theFd > maxFd)
          maxFd = theFd;

	if (theEvents & OTCLIB_POLLIN)
	  FD_SET(theFd,&readfds);
	if (theEvents & OTCLIB_POLLOUT)
	  FD_SET(theFd,&writefds);
	if (theEvents & OTCLIB_POLLPRI)
	  FD_SET(theFd,&exceptfds);
      }
    }

    int width = maxFd + 1;
    int signalFd = INVALID_SOCKET;
    int wakeupFd = INVALID_SOCKET;

    if (theActions & OTCLIB_SIGNAL_JOBS)
    {
      signalFd = signalFds_[0];
      if (
       signalFd != INVALID_SOCKET &&
       OTCEV_Signal::signalsSubscribed() != 0
      )
      {
	FD_SET(signalFd,&readfds);
	if (signalFd >= width)
	  width = signalFd+1;

	OTCLIB_TRACER(MODULE) << "signal fd = " << signalFd << endl;
      }
    }

    wakeupFd = wakeupFds_[0];
    if (
     wakeupFd != INVALID_SOCKET &&
     (theActions & OTCLIB_THREADS_DONT_WAIT) == 0
    )
    {
      FD_SET(wakeupFd,&readfds);
      if (wakeupFd >= width)
	width = wakeupFd+1;

      OTCLIB_TRACER(MODULE) << "wakeup fd = " << wakeupFd << endl;
    }

    // Do the <select()> call.

    int res = 0;

    if (
     ((theActions & OTCLIB_IDLE_JOBS) && !isPendingIdleJobsEmpty()) ||
     ((theActions & OTCLIB_STANDARD_JOBS) && !isStandardJobsEmpty())
    )
    {
      timeval timeout;
      timeout.tv_sec = 0;
      timeout.tv_usec = 0;

      fd_set t_readfds;
      fd_set t_writefds;
      fd_set t_exceptfds;

      memcpy(&t_readfds,&readfds,sizeof(fd_set));
      memcpy(&t_writefds,&writefds,sizeof(fd_set));
      memcpy(&t_exceptfds,&exceptfds,sizeof(fd_set));

      if (fdCnt > 0 || signalFd != INVALID_SOCKET ||
       (wakeupFd != INVALID_SOCKET &&
       (theActions & OTCLIB_THREADS_DONT_WAIT) == 0))
      {
#if defined(hpux) && !defined(_FD_SET)
	res = select(width,(int*)&t_readfds,(int*)&t_writefds,
	 (int*)&t_exceptfds,&timeout);
#else
	res = select(width,&t_readfds,&t_writefds,&t_exceptfds,&timeout);
#endif

	int err = 0;

	if (res == -1)
	{
#if defined(OSE_SYS_WIN32)
	  err = WSAGetLastError();

	  if (err != WSAENOTSOCK && err != WSAEBADF
	   && err != WSANOTINITIALISED)
	  {
	    err = 0;
	  }
#else
	  err = errno;

	  if (err != EBADF)
	  {
	    err = 0;
	  }
#endif
	}

	// If we have a bad file descriptor, cycle through all file
	// descriptors, doing a select on each, such that we can find
	// which is the bad file descriptor and generate a OTCLIB_POLLNVAL
	// event for it. We can only assume that the recipient will take
	// that event and unsubscribe the file descriptor. If they do not
	// do this, the select() call will most likely loop. Note that
	// we can do select() just on input events as whether the type of
	// event is ready or not is not important but whether select() likes
	// the file descriptor.

	if (err != 0)
	{
	  OTCLIB_TRACER(MODULE) << "error = " << err << endl;

	  fd_set nullfds;
	  fd_set checkfds;

	  FD_ZERO(&nullfds);
	  FD_ZERO(&checkfds);

	  for (int i=0; i<fdCnt; i++)
	  {
	    int theFd = pendingFdEvents_[i].fd;
	    int theEvents = pendingFdEvents_[i].events;

	    OTCLIB_TRACER(MODULE) << "test fd = " << theFd << endl;

	    if (theEvents != 0)
	    {
	      FD_SET(theFd,&checkfds);

#if defined(hpux) && !defined(_FD_SET)
	      res = select(width,(int*)&checkfds,(int*)&nullfds,
	       (int*)&nullfds,&timeout);
#else
	      res = select(width,&checkfds,&nullfds,&nullfds,&timeout);
#endif

	      err = 0;

	      if (res == -1)
	      {
#if defined(OSE_SYS_WIN32)
		err = WSAGetLastError();

		if (err != WSAENOTSOCK && err != WSAEBADF
		 && err != WSANOTINITIALISED)
	        {
		  err = 0;
		}
#else
		err = errno;

		if (err != EBADF)
		{
		  err = 0;
		}
#endif
	      }

	      if (err != 0)
	      {
		OTCLIB_TRACER(MODULE) << "error (fd="
		 << theFd << ") = " << err << endl;

		OTCLIB_TRACER(MODULE) << "bad fd = " << theFd << endl;

		theJob = OTCEV_IOEvent::job(theFd,OTCLIB_POLLNVAL);

		OTCEV_IOEvent::unsubscribeFd(theFd);

		FD_CLR(theFd,&readfds);
		FD_CLR(theFd,&writefds);
		FD_CLR(theFd,&exceptfds);

		if (theJob != 0)
		  break;
	      }

	      FD_CLR(theFd,&checkfds);
	    }
	  }

	  if (theJob != 0)
	    break;

	  // If we get here and haven't found a bad file descriptor
	  // then we have a problem. All we can do is set result back
	  // to -1 and keep going and hope that everything is okay.

          res = -1;
	}
      }

      if (res == 0)
      {
	if (rawPeriod != 0)
	{
          if (theActions & OTCLIB_STANDARD_JOBS)
	  {
	    theJob = nextStandardJob();

	    if (theJob != 0)
	      break;
	  }

          if (theActions & OTCLIB_IDLE_JOBS)
	  {
	    updateIdleJobs();

	    theJob = nextIdleJob();

	    if (theJob != 0)
	      break;
	  }
	}
      }
    }

    res = 0;

    if (fdCnt > 0 || period != -1 || signalFd != INVALID_SOCKET ||
     (wakeupFd != INVALID_SOCKET &&
     (theActions & OTCLIB_THREADS_DONT_WAIT) == 0))
    {
      if (fdCnt > 0 || signalFd != INVALID_SOCKET ||
       (wakeupFd != INVALID_SOCKET &&
       (theActions & OTCLIB_THREADS_DONT_WAIT) == 0))
      {
#if defined(hpux) && !defined(_FD_SET)
	res = select(width,(int*)&readfds,(int*)&writefds,
	 (int*)&exceptfds,timeoutp);
#else
	res = select(width,&readfds,&writefds,&exceptfds,timeoutp);
#endif

	int err = 0;

	if (res == -1)
	{
#if defined(OSE_SYS_WIN32)
	  err = WSAGetLastError();

	  if (err != WSAENOTSOCK && err != WSAEBADF
	   && err != WSANOTINITIALISED)
	  {
	    err = 0;
	  }
#else
	  err = errno;

	  if (err != EBADF)
	  {
	    err = 0;
	  }
#endif
	}

	// If we have a bad file descriptor, cycle through all file
	// descriptors, doing a select on each, such that we can find
	// which is the bad file descriptor and generate a OTCLIB_POLLNVAL
	// event for it. We can only assume that the recipient will take
	// that event and unsubscribe the file descriptor. If they do not
	// do this, the select() call will most likely loop. Note that
	// we can do select() just on input events as whether the type of
	// event is ready or not is not important but whether select() likes
	// the file descriptor.

	if (err != 0)
	{
	  OTCLIB_TRACER(MODULE) << "error = " << err << endl;

	  fd_set nullfds;
	  fd_set checkfds;

	  FD_ZERO(&nullfds);
	  FD_ZERO(&checkfds);

	  for (int i=0; i<fdCnt; i++)
	  {
	    int theFd = pendingFdEvents_[i].fd;
	    int theEvents = pendingFdEvents_[i].events;

	    OTCLIB_TRACER(MODULE) << "test fd = " << theFd << endl;

	    if (theEvents != 0)
	    {
	      FD_SET(theFd,&checkfds);

#if defined(hpux) && !defined(_FD_SET)
	      res = select(width,(int*)&checkfds,(int*)&nullfds,
	       (int*)&nullfds,&timeout);
#else
	      res = select(width,&checkfds,&nullfds,&nullfds,&timeout);
#endif

	      err = 0;

	      if (res == -1)
	      {
#if defined(OSE_SYS_WIN32)
		err = WSAGetLastError();

		if (err != WSAENOTSOCK && err != WSAEBADF
		 && err != WSANOTINITIALISED)
	        {
		  err = 0;
		}
#else
		err = errno;

		if (err != EBADF)
		{
		  err = 0;
		}
#endif
	      }

	      if (err != 0)
	      {
		OTCLIB_TRACER(MODULE) << "error (fd="
		 << theFd << ") = " << err << endl;

		OTCLIB_TRACER(MODULE) << "bad fd = " << theFd << endl;

		theJob = OTCEV_IOEvent::job(theFd,OTCLIB_POLLNVAL);

		OTCEV_IOEvent::unsubscribeFd(theFd);

		FD_CLR(theFd,&readfds);
		FD_CLR(theFd,&writefds);
		FD_CLR(theFd,&exceptfds);

		if (theJob != 0)
		  break;
	      }

	      FD_CLR(theFd,&checkfds);
	    }
	  }

	  if (theJob != 0)
	    break;

	  // If we get here and haven't found a bad file descriptor
	  // then we have a problem. All we can do is set result back
	  // to -1 and keep going and hope that everything is okay.

          res = -1;
	}
      }
      else
      {
#if defined(OSE_SYS_WIN32)
	OTCLIB_TRACER(MODULE) << "sleep = " << period << endl;

	Sleep(period);
#else
	res = select(0,0,0,0,timeoutp);
#endif
      }
    }
    else
      break;

    // Generate io events.

    if (res > 0)
    {
      maxFd_ = -1;
      nextFd_ = 0;

      for (int i=0; i<fdCnt; i++)
      {
	int theFd = pendingFdEvents_[i].fd;
	int theEvents = 0;

	if (FD_ISSET(theFd,&readfds))
	  theEvents |= OTCLIB_POLLIN;
	if (FD_ISSET(theFd,&writefds))
	  theEvents |= OTCLIB_POLLOUT;
	if (FD_ISSET(theFd,&exceptfds))
	  theEvents |= OTCLIB_POLLPRI;

	if (theEvents != 0)
	{
          pendingFdEvents_[nextFd_].fd = theFd;
          pendingFdEvents_[nextFd_].events = theEvents;
          nextFd_++;
	}
      }

      if (nextFd_ != 0)
      {
        maxFd_ = nextFd_ - 1;
        nextFd_ = 0;
      }
    }
    else if (res == 0)
    {
      if (theActions & OTCLIB_DONT_WAIT)
      {
	if (rawPeriod != 0)
	  break;
      }
    }

#else

    OTCLIB_EXCEPTION("Dispatcher not supported");

#endif /* HAVE_SELECT */
#endif /* HAVE_POLL */

    // Has dispatcher been stopped.

    if (isStopped())
      break;
  }

#if defined(OSE_SYS_WIN32)
  WaitForSingleObject(wmutex_,INFINITE);
#else
  wmutex_.lock();
#endif

  wakeupFlag_ = 0;

#if defined(OSE_SYS_WIN32)
  ReleaseMutex(wmutex_);
#else
  wmutex_.unlock();
#endif

  xxxMutex.release();

  mutex_.unlock();

  return theJob;
}

/* ------------------------------------------------------------------------- */
int OTC_JobQueue::dispatch(int theActions, int theOptions)
{
  if (isWaiting())
    start();

  if (OTC_Task::isStopping())
    stop();

  if (isStopped())
    return -1;

  runUserSetup();

  if (isStopped())
    return -1;

  int theResult;
  theResult = 0;

  OTC_Job* theJob;

  theJob = next(theActions,theOptions);

  if (theJob == 0 && theActions == 0)
    stop();

  if (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();

    theResult = 1;
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
int OTC_JobQueue::run()
{
  return run(0);
}

/* ------------------------------------------------------------------------- */
int OTC_JobQueue::run(OTC_Condition* theCondition)
{
  int theResult = 1;

  while (theResult > 0)
  {
    theResult = dispatch();

    if (theResult > 0 && theCondition != 0)
    {
      theCondition->evaluate();
      if (theCondition->test())
	break;
    }
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_JobQueue::start()
{
  rmutex_.lock();

  if (state_ == OTCLIB_WAITING)
    state_ = OTCLIB_RUNNING;

  rmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_JobQueue::stop()
{
  rmutex_.lock();

  state_ = OTCLIB_STOPPED;

  rmutex_.unlock();

  wakeup(OTCLIB_WAKEUP_OTHER);
}

/* ------------------------------------------------------------------------- */
void OTC_JobQueue::reset()
{
  OTCLIB_ENSURE_FN((isStopped()),
   "OTC_JobQueue::reset()",
   "not in stopped state");

  rmutex_.lock();

  state_ = OTCLIB_WAITING;

  rmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_JobQueue::isWaiting()
{
  bool theResult;

  rmutex_.lock();

  theResult = (state_ == OTCLIB_WAITING);

  rmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
bool OTC_JobQueue::isRunning()
{
  bool theResult;

  rmutex_.lock();

  theResult = (state_ == OTCLIB_RUNNING);

  rmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
bool OTC_JobQueue::isStopped()
{
  bool theResult;

  rmutex_.lock();

  theResult = (state_ == OTCLIB_STOPPED);

  rmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_JobQueue::wakeup(int theType)
{
  // OTCLIB_MARKBLOCK(MODULE,"OTC_JobQueue::wakeup()");

  // OTCLIB_TRACER(MODULE) << "type = " << theType << endl;

#if defined(OSE_SYS_WIN32)
  WaitForSingleObject(wmutex_,INFINITE);

  if (wakeupFds_[1] != INVALID_SOCKET)
  {
    if (wakeupFlag_ != 0 && wakeupCount_ == 0)
    {
      int res = 0;
      int num = errno;
      res = send(wakeupFds_[1],"W",1,0);
      while (res == -1 && errno == EINTR)
	res = send(wakeupFds_[1],"W",1,0);
      errno = num;
      if (res != -1)
	wakeupCount_++;

      // OTCLIB_TRACER(MODULE) << "wakeup count = " << wakeupCount_ << endl;
    }
  }

  ReleaseMutex(wmutex_);
#else
  if (theType & OTCLIB_WAKEUP_SIGNAL)
  {
    OTC_SignalBlock sigblock;

    if (signalFds_[1] != INVALID_SOCKET)
    {
      if (signalCount_ == 0)
      {
	int res = 0;
	int num = errno;
	res = write(signalFds_[1],"S",1);
	while (res == -1 && errno == EINTR)
	  res = write(signalFds_[1],"S",1);
	errno = num;
	if (res != -1)
	  signalCount_++;
      }
    }
  }
#if defined(OSE_WITH_THREADS)
  else
  {
    wmutex_.lock();

    if (wakeupFds_[1] != INVALID_SOCKET)
    {
      if (wakeupFlag_ != 0 && wakeupCount_ == 0)
      {
	int res = 0;
	int num = errno;
	res = write(wakeupFds_[1],"W",1);
	while (res == -1 && errno == EINTR)
	  res = write(wakeupFds_[1],"W",1);
	errno = num;
	if (res != -1)
	  wakeupCount_++;

	// OTCLIB_TRACER(MODULE) << "wakeup count = " << wakeupCount_ << endl;
      }
    }

    wmutex_.unlock();
  }
#endif
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_JobQueue::clearPipes()
{
  // Clear signal and wakeup pipes.

  if (signalFds_[0] != INVALID_SOCKET)
  {
    OTC_SignalBlock sigblock;

    while (signalCount_ != 0)
    {
      int res = 0;
      char buf[1];
      res = read(signalFds_[0],buf,1);
      if (res != -1)
	signalCount_--;
      else
	break;
    }
  }

  if (wakeupFds_[0] != INVALID_SOCKET)
  {
#if defined(OSE_SYS_WIN32)
    WaitForSingleObject(wmutex_,INFINITE);
#else
    wmutex_.lock();
#endif

    while (wakeupCount_ != 0)
    {
      int res = 0;
      char buf[1];
#if defined(OSE_SYS_WIN32)
      res = recv(wakeupFds_[0],buf,1,0);
#else
      res = read(wakeupFds_[0],buf,1);
#endif
      if (res != -1)
	wakeupCount_--;
      else
	break;

      OTCLIB_TRACER(MODULE) << "wakeup count = " << wakeupCount_ << endl;
    }

#if defined(OSE_SYS_WIN32)
    ReleaseMutex(wmutex_);
#else
    wmutex_.unlock();
#endif
  }
}

/* ------------------------------------------------------------------------- */
bool OTC_JobQueue::isPriorityJobsEmpty()
{
  bool theResult;

  qmutex_.lock();

  theResult = priorityJobs_->isEmpty();

  qmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTC_JobQueue::nextPriorityJob()
{
  OTC_Job* theJob;

  qmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(qmutex_);

  theJob = priorityJobs_->next();

  xxxMutex.release();

  qmutex_.unlock();

  return theJob;
}

/* ------------------------------------------------------------------------- */
bool OTC_JobQueue::isStandardJobsEmpty()
{
  bool theResult;

  qmutex_.lock();

  theResult = standardJobs_->isEmpty();

  qmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTC_JobQueue::nextStandardJob()
{
  OTC_Job* theJob;

  qmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(qmutex_);

  theJob = standardJobs_->next();

  xxxMutex.release();

  qmutex_.unlock();

  return theJob;
}

/* ------------------------------------------------------------------------- */
bool OTC_JobQueue::isPendingStandardJobsEmpty()
{
  bool theResult;

  qmutex_.lock();

  theResult = pendingStandardJobs_->isEmpty();

  qmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_JobQueue::updateStandardJobs()
{
  OTC_Job* theJob;

  qmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(qmutex_);

  theJob = pendingStandardJobs_->next();
  while (theJob != 0)
  {
    standardJobs_->add(theJob);
    theJob = pendingStandardJobs_->next();
  }

  xxxMutex.release();

  qmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_JobQueue::isIdleJobsEmpty()
{
  bool theResult;

  qmutex_.lock();

  theResult = idleJobs_->isEmpty();

  qmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTC_JobQueue::nextIdleJob()
{
  OTC_Job* theJob;

  qmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(qmutex_);

  theJob = idleJobs_->next();

  xxxMutex.release();

  qmutex_.unlock();

  return theJob;
}

/* ------------------------------------------------------------------------- */
bool OTC_JobQueue::isPendingIdleJobsEmpty()
{
  bool theResult;

  qmutex_.lock();

  theResult = pendingIdleJobs_->isEmpty();

  qmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_JobQueue::updateIdleJobs()
{
  OTC_Job* theJob;

  qmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(qmutex_);

  theJob = pendingIdleJobs_->next();
  while (theJob != 0)
  {
    idleJobs_->add(theJob);
    theJob = pendingIdleJobs_->next();
  }

  xxxMutex.release();

  qmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
