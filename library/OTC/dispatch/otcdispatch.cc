/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcdispatch.cc
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
#pragma implementation "OTC/dispatch/dispatch.hh"
#endif
#endif

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(OSE_SYS_WIN32)
#include <windows.h>
#include <process.h>
#ifndef getpid
#define getpid _getpid
#endif
#endif

/* ------------------------------------------------------------------------- */
class OTC_DispatcherTask : public OTC_Task
{
  public:

  			OTC_DispatcherTask() {}

  protected:

    void		execute();

    void		shutdown();

    void		fail();
};

/* ------------------------------------------------------------------------- */
void OTC_DispatcherTask::execute()
{
  OTC_Dispatcher::run();
}

/* ------------------------------------------------------------------------- */
void OTC_DispatcherTask::shutdown()
{
  OTC_Dispatcher::stop();
}

/* ------------------------------------------------------------------------- */
void OTC_DispatcherTask::fail()
{
  OTC_LogStream theLogger;
  theLogger.setLevel(OTCLIB_LOG_WARNING);

  theLogger << "Unexpected exception occurred" << EOL;
  theLogger << "Class: OTC_DispatcherTask" << EOL;
  theLogger << "Method: fail()" << EOL;
  theLogger << "Description: exception occurred while executing job" << EOL;
  theLogger << "Result: the dispatcher has been stopped" << EOL;
  theLogger << flush;
}

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Dispatcher::mutex_;
bool OTC_Dispatcher::initialised_ = false;
bool OTC_Dispatcher::warnings_ = false;
OTC_JobQueue* OTC_Dispatcher::globJobQueue_ = 0;
long OTC_Dispatcher::processId_ = 0;
OTC_Task* OTC_Dispatcher::task_ = 0;

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::initialise(
 void (*theFunc)()
)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = new OTC_JobQueue(theFunc);
  OTCLIB_ASSERT_M(theJobQueue != 0);

  initialise(theJobQueue);
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::initialise(OTC_JobQueue* theJobQueue)
{
  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::initialise(OTC_JobQueue*, void (*)())",
   "invalid jobqueue");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCLIB_ENSURE_FN((globJobQueue_ == 0),
   "OTC_Dispatcher::initialise(OTC_JobQueue*, void (*)())",
   "dispatcher already initialised");

  globJobQueue_ = theJobQueue;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_JobQueue* OTC_Dispatcher::queue()
{
  OTC_JobQueue* theJobQueue;

  mutex_.lock();

  theJobQueue = globJobQueue_;

  mutex_.unlock();

  return theJobQueue;
}

/* ------------------------------------------------------------------------- */
int OTC_Dispatcher::dispatch(int theActions, int theOptions)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::dispatch(int,int)",
   "dispatcher not initialised");

  return theJobQueue->dispatch(theActions,theOptions);
}

/* ------------------------------------------------------------------------- */
int OTC_Dispatcher::run()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::run()",
   "dispatcher not initialised");

  processId_ = getpid();

  return theJobQueue->run();
}

/* ------------------------------------------------------------------------- */
int OTC_Dispatcher::run(OTC_Condition* theCondition)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::run()",
   "dispatcher not initialised");

  processId_ = getpid();

  return theJobQueue->run(theCondition);
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::stop()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::stop()",
   "dispatcher not initialised");

  theJobQueue->stop();
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::reset()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::reset()",
   "dispatcher not initialised");

  theJobQueue->reset();
}

/* ------------------------------------------------------------------------- */
bool OTC_Dispatcher::isWaiting()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isWaiting()",
   "dispatcher not initialised");

  return theJobQueue->isWaiting();
}

/* ------------------------------------------------------------------------- */
bool OTC_Dispatcher::isRunning()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isRunning()",
   "dispatcher not initialised");

  return theJobQueue->isRunning();
}

/* ------------------------------------------------------------------------- */
bool OTC_Dispatcher::isStopped()
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isStopped()",
   "dispatcher not initialised");

  return theJobQueue->isStopped();
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::wakeup(int theType)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isStopped()",
   "dispatcher not initialised");

  theJobQueue->wakeup(theType);
}

/* ------------------------------------------------------------------------- */
OTC_Task* OTC_Dispatcher::task()
{
  OTC_Task* theResult;

  mutex_.lock();

  if (task_ == 0)
  {
    task_ = new OTC_DispatcherTask();
    OTCLIB_ASSERT_M(task_ != 0);

    theResult = task_;
  }

  mutex_.unlock();

  return task_;
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::schedule(OTC_Job* theJob, int theType)
{
  OTC_JobQueue* theJobQueue;
  theJobQueue = queue();

  OTCLIB_ENSURE_FN((theJobQueue != 0),
   "OTC_Dispatcher::isStopped()",
   "dispatcher not initialised");

  theJobQueue->add(theJob,theType);
}

/* ------------------------------------------------------------------------- */
bool OTC_Dispatcher::warningsEnabled()
{
  bool theResult;

  mutex_.lock();

  if (initialised_ == false)
  {
#ifdef NDEBUG
    warnings_ = false;
    if (getenv("OTCLIB_EVENTWARNINGS") != 0)
      warnings_ = true;
#else
    warnings_ = true;
    if (getenv("OTCLIB_NOEVENTWARNINGS") != 0)
      warnings_ = false;
#endif

    initialised_ = true;
  }

  theResult = warnings_;

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::enableWarnings()
{
  warningsEnabled();

  mutex_.lock();

  warnings_ = true;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Dispatcher::disableWarnings()
{
  warningsEnabled();

  mutex_.lock();

  warnings_ = false;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
