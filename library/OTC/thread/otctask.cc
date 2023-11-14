/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/otctask.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/thread/task.hh"
#endif
#endif

#include <OTC/thread/task.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Task::mutex_;
bool OTC_Task::initialised_ = false;
#if defined(OSE_WITH_THREADS)
pthread_key_t OTC_Task::context_;
#endif

/* ------------------------------------------------------------------------- */
OTC_Task::~OTC_Task()
{
  // In case thread task is still running, stop
  // it and then wait for it to finish.

  stop();

  wait();
}

/* ------------------------------------------------------------------------- */
OTC_Task::OTC_Task()
  : state_(OTCLIB_TASK_STATE_UNKNOWN)
{
  mutex_.lock();

  if (initialised_ == false)
  {
#if defined(OSE_WITH_THREADS)
    pthread_key_create(&context_,0);
#endif

    initialised_ = true;
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_TaskState OTC_Task::state() const
{
  OTC_TaskState theResult;

  mutex_.lock();

  theResult = state_;

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Task::start()
{
#if defined(OSE_WITH_THREADS)
  mutex_.lock();

  if (state_ == OTCLIB_TASK_STATE_UNKNOWN)
  {
    state_ = OTCLIB_TASK_STATE_STARTING;

    pthread_create(&thread_,0,threadFunc_,(void*)this);
  }

  mutex_.unlock();
#else
  if (state_ == OTCLIB_TASK_STATE_UNKNOWN)
  {
    state_ = OTCLIB_TASK_STATE_STARTING;

    run_();
  }
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_Task::stop()
{
#if defined(OSE_WITH_THREADS)
  if (state_ == OTCLIB_TASK_STATE_RUNNING)
  {
    state_ = OTCLIB_TASK_STATE_STOPPING;

    shutdown();
  }
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_Task::wait()
{
#if defined(OSE_WITH_THREADS)
  void* theResult = 0;
  pthread_join(thread_,&theResult);
#endif
}

/* ------------------------------------------------------------------------- */
bool OTC_Task::isStopping()
{
#if defined(OSE_WITH_THREADS)
  bool theResult;
  theResult = false;

  mutex_.lock();

  if (initialised_ == false)
  {
    pthread_key_create(&context_,0);

    initialised_ = true;
  }

  OTC_TaskState* tmpState;
  tmpState = (OTC_TaskState*)pthread_getspecific(context_);

  if (tmpState != 0 && *tmpState == OTCLIB_TASK_STATE_STOPPING)
    theResult = true;

  mutex_.unlock();

  return theResult;
#else
  return false;
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_Task::abortTask()
{
  throw OTC_TaskAborted();
}

/* ------------------------------------------------------------------------- */
void OTC_Task::execute()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_Task::shutdown()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_Task::fail()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTC_Task::threadFunc_(void* theArg)
{
#if defined(OSE_WITH_THREADS)
  OTC_Task* theTask;
  theTask = (OTC_Task*)theArg;

  theTask->run_();

  pthread_exit(0);
#endif

  return 0;
}

/* ------------------------------------------------------------------------- */
void OTC_Task::run_()
{
#if defined(OSE_WITH_THREADS)
  pthread_setspecific(context_,(void*)&state_);
#endif

  try
  {
    mutex_.lock();

    state_ = OTCLIB_TASK_STATE_RUNNING;

    mutex_.unlock();

    execute();

    mutex_.lock();

    state_ = OTCLIB_TASK_STATE_STOPPED;

    mutex_.unlock();
  }
  catch (OTC_TaskAborted theException)
  {
    mutex_.lock();

    state_ = OTCLIB_TASK_STATE_STOPPED;

    mutex_.unlock();
  }
  catch (...)
  {
    mutex_.lock();

    state_ = OTCLIB_TASK_STATE_FAILED;

    mutex_.unlock();

    fail();
  }
}

/* ------------------------------------------------------------------------- */
