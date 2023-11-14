/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcevthread.cc
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
#pragma implementation "OTC/dispatch/evthread.hh"
#endif
#endif

#include <OTC/dispatch/evthread.hh>
#include <OTC/dispatch/evcllbck.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/collctn/link.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/logger.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_EventCallback : public OTC_Link
{
  public:

			OTC_EventCallback(
			 OTC_Event* theEvent,
			 OTC_EVCallback* theCallback
			)
			  : event_(theEvent), callback_(theCallback) {}

			~OTC_EventCallback();

    OTC_Event*		event() const
				{ return event_; }

    OTC_EVCallback*	callback() const
				{ return callback_; }

  private:

			OTC_EventCallback(OTC_EventCallback const&);
				// Do not define an implementation for this.

    OTC_EventCallback&	operator=(OTC_EventCallback const&);
				// Do not define an implementation for this.

    OTC_Event*		event_;

    OTC_EVCallback*	callback_;
};

OTC_EventCallback::~OTC_EventCallback()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EVThreadPool");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
#if defined(OSE_WITH_THREADS)
OTC_NRMutex OTC_EVThreadPool::mutex_;
bool OTC_EVThreadPool::initialised_ = false;
pthread_key_t OTC_EVThreadPool::context_;
#endif

/* ------------------------------------------------------------------------- */
OTC_EVThreadPool::~OTC_EVThreadPool()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVThreadPool::~OTC_EVThreadPool()");

#if defined(OSE_WITH_THREADS)
  if (threads_ != 0)
  {
    u_int i;
    void* theResult = 0;
    for (i=0; i<numThreads_; i++)
      pthread_join(threads_[i].thread,&theResult);

    delete [] threads_;
  }

  if (eventQueue_ != 0)
  {
    OTC_LinkIterator theIter = eventQueue_->items(OTCLIB_UNSAFE);
    theIter.resetFirst();
    while (theIter.isLink())
    {
      OTC_EventCallback* theLink;
      theLink = (OTC_EventCallback*)theIter.link();

      theLink->event()->destroy();
      theLink->callback()->destroy();

      theIter.next();
    }

    delete eventQueue_;
  }
#endif
}

/* ------------------------------------------------------------------------- */
OTC_EVThreadPool::OTC_EVThreadPool(
 u_int theNumThreads,
 u_int theQueueSize,
 u_int theThreshold
)
#if defined(OSE_WITH_THREADS)
  : threads_(0),
    numThreads_(theNumThreads),
    numActive_(theNumThreads),
    eventQueue_(0),
    eventCount_(0),
    queueSize_(theQueueSize),
    queueThreshold_(theThreshold),
    eventReady_(queueLock_),
    queueReady_(queueLock_),
    shutdownReady_(queueLock_),
    queueWaitingId_(0),
    queueId_(0),
    waiting_(0),
    suspended_(false),
    shutdown_(false),
    exited_(0)
#endif
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVThreadPool::OTC_EVThreadPool(u_int,u_int)");

#if defined(OSE_WITH_THREADS)
  if (numThreads_ != 0)
  {
    OTCLIB_TRACER(MODULE) << "threads = " << numThreads_ << endl;

    eventQueue_ = new OTC_LinkList;
    OTCLIB_ASSERT_M(eventQueue_ != 0);
  }
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::startup()
{
#if defined(OSE_WITH_THREADS)

  mutex_.lock();

  if (initialised_ == false)
  {
    pthread_key_create(&context_,0);

    initialised_ = true;
  }

  mutex_.unlock();

  // Grab mutex on queue. This is actually mutex
  // on all the associated counters etc as well.

  queueLock_.lock();

  // This ensures that lock is released if an
  // an exception occurs while we are doing this.

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(queueLock_);

  // Only create threads if they haven't
  // previously been started and some are
  // required.

  if (numThreads_ != 0 && threads_ == 0)
  {
    OTCLIB_TRACER(MODULE) << "threads = " << numThreads_ << endl;

    threads_ = new OTC_EVThreadHandle[numThreads_];
    OTCLIB_ASSERT_M(threads_);

    // Go through and create each thread.
    // The threads are recorded in an
    // array but no use of made of that
    // as yet.

    for (u_int i=0; i<numThreads_; i++)
    {
      // Arrange for threadFunc_() to be called
      // within thread. Give the thread a pointer
      // to this class.

      threads_[i].threadId = i + 1;
      threads_[i].threadState = OTCLIB_TASK_STATE_STARTING;
      threads_[i].threadPool = this;
 
      int theResult = -1;
      theResult = pthread_create(&threads_[i].thread,
       0,threadFunc_,(void*)&threads_[i]);

      OTCLIB_TRACER(MODULE) << "create thread " << i << " = ";
      OTCLIB_TRACER(MODULE) << theResult << endl;
    }
  }

  // No longer have to worry about exceptions.
 
  xxxMutex.release();

  // Release mutex on queue.

  queueLock_.unlock();
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::shutdown()
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_EVThreadPool::shutdown()");

#if defined(OSE_WITH_THREADS)
  // Suspend further processing of events.

  suspend();

  // Grab the queue mutex.

  queueLock_.lock();

  // Update thread state so that threads which
  // check status will shutdown.

  for (u_int i=0; i<numThreads_; i++)
  {
    if (threads_[i].threadState == OTCLIB_TASK_STATE_RUNNING)
      threads_[i].threadState = OTCLIB_TASK_STATE_STOPPING;
  }

  // If not already shutdown, set shutdown flag
  // and wakeup all threads waiting on events
  // so they will check flag and then exit.
  // That is, events waiting in the tight while
  // loop in the thread function.

  if (shutdown_ == false)
  {
    shutdown_ = true;

    eventReady_.broadcast();

    OTCLIB_TRACER(MODULE) << "threads = " << numThreads_ << endl;
    OTCLIB_TRACER(MODULE) << "waiting = " << waiting_ << endl;
    OTCLIB_TRACER(MODULE) << "exited = " << exited_ << endl;

    while (exited_ != numThreads_)
      shutdownReady_.wait();
  }

  // Unlock the queue mutex.

  queueLock_.unlock();
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::suspend()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVThreadPool::suspend()");

#if defined(OSE_WITH_THREADS)
  // Grab the queue mutex.

  queueLock_.lock();

  // If not already suspend or shutdown, set
  // the flag notifying of suspension.

  if (shutdown_ == false && suspended_ == false)
    suspended_ = true;

  // Unlock the queue mutex.

  queueLock_.unlock();
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::resume()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVThreadPool::suspend()");

#if defined(OSE_WITH_THREADS)
  queueLock_.lock();

  if (shutdown_ == false && suspended_ != false)
  {
    suspended_ = false;
    if (eventCount_ != 0)
      eventReady_.broadcast();
  }

  queueLock_.unlock();
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::limit(u_int theNumThreads)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVThreadPool::limit(u_int)");

#if defined(OSE_WITH_THREADS)
  queueLock_.lock();

  if (shutdown_ == false)
  {
    u_int oldNumActive = numActive_;

    numActive_ = theNumThreads;

    if (numActive_ > numThreads_ || numActive_ == 0)
      numActive_ = numThreads_;

    if (numActive_ > oldNumActive && eventCount_ != 0)
      eventReady_.broadcast();

    OTCLIB_TRACER(MODULE) << "limit = " << numActive_ << endl;
  }

  queueLock_.unlock();
#endif
}

/* ------------------------------------------------------------------------- */
u_int OTC_EVThreadPool::threadId() const
{
#if defined(OSE_WITH_THREADS)
  OTC_EVThreadHandle* theContext;
  theContext = (OTC_EVThreadHandle*)pthread_getspecific(context_);

  if (theContext == 0)
    return 0;

  if (theContext->threadPool != this)
    return 0;

  return theContext->threadId;
#else
  return 0;
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::process(
 OTC_Event* theEvent,
 OTC_EVCallback* theCallback
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_EVThreadPool::process(OTC_Event*,OTC_EVCallback*)");

  OTCLIB_ENSURE_FN((theEvent != 0),
   "OTC_EVThreadPool::process(OTC_Event*,OTC_EVCallback*)",
   "Invalid event");
  OTCLIB_ENSURE_FN((theCallback != 0),
   "OTC_EVThreadPool::process(OTC_Event*,OTC_EVCallback*)",
   "Invalid callback");

  OTCLIB_TRACER(MODULE) << *theEvent << endl;

#if defined(OSE_WITH_THREADS)
  queue_(theEvent,theCallback);
#else
  execute_(theEvent,theCallback);
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::throttle(
 OTC_Event* theEvent,
 OTC_EVCallback* theCallback
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_EVThreadPool::throttle(OTC_Event*,OTC_EVCallback*)");

#if defined(OSE_WITH_THREADS)
  OTCLIB_ENSURE_FN((queueThreshold_ != 0 || queueSize_ != 0),
   "OTC_EVThreadPool::throttle(OTC_Event*,OTC_EVCallback*)",
   "threshold or queue size not non zero");

  queueLock_.lock();

  u_int waitId = queueId_++;

  OTCLIB_TRACER(MODULE) << "wait on " << waitId << endl;
  OTCLIB_TRACER(MODULE) << "current " << queueWaitingId_ << endl;
  OTCLIB_TRACER(MODULE) << "event count " << eventCount_ << endl;
  OTCLIB_TRACER(MODULE) << "threshold " << queueThreshold_ << endl;
  OTCLIB_TRACER(MODULE) << "queue size " << queueSize_ << endl;

  while (eventCount_ >= queueSize_ ||
   (eventCount_ < queueSize_ && waitId != queueWaitingId_))
  {
    queueReady_.wait();
  }

  OTCLIB_TRACER(MODULE) << "wait done " << waitId << endl;

  queueWaitingId_++;

  if (queueId_ != queueWaitingId_)
    eventReady_.broadcast();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(queueLock_);

  OTC_EventCallback* theLink;
  theLink = new OTC_EventCallback(theEvent,theCallback);
  OTCLIB_ASSERT_M(theLink != 0);

  eventQueue_->addLast(theLink);
  eventCount_++;

  OTCLIB_TRACER(MODULE) << "event count = " << eventCount_ << endl;

  if (shutdown_ == false && suspended_ == false)
  {
    OTCLIB_TRACER(MODULE) << "signal thread" << endl;

    eventReady_.signal();
  }

  xxxMutex.release();
  
  queueLock_.unlock();
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::execute_(
 OTC_Event* theEvent,
 OTC_EVCallback* theCallback
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_EVThreadPool::execute_(OTC_Event*,OTC_EVCallback*)");

  theCallback->execute(theEvent);
  theCallback->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_EVThreadPool::queue_(OTC_Event* theEvent, OTC_EVCallback* theCallback)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_EVThreadPool::queue_(OTC_Event*,OTC_EVCallback*)");

#if defined(OSE_WITH_THREADS)
  // If threads exist we need to queue the
  // event and notify waiting threads, else it
  // gets executed straight away.

  if (numThreads_ != 0)
  {
    bool isOverloaded = false;

    queueLock_.lock();

    OTCLIB_TRACER(MODULE) << "event count = " << eventCount_ << endl;
    OTCLIB_TRACER(MODULE) << "threshold = " << queueThreshold_ << endl;
    OTCLIB_TRACER(MODULE) << "queue size = " << queueSize_ << endl;

    // Where a queue size or threshold has been
    // defined, if we go over this value, the
    // queue is regarded as being overloaded.
    // Deal with this down further.

    if (queueThreshold_ != 0 && eventCount_ >= queueThreshold_)
      isOverloaded = true;
    else if (queueSize_ != 0 && eventCount_ >= queueSize_)
      isOverloaded = true;

    // Queue is not overloaded, add the event
    // to the queue and notify a single thread
    // that an event is ready.

    if (isOverloaded == false)
    {
      // Ensure mutex is unlocked if an exception
      // occurs.

      OTC_MutexReaper<OTC_NRMutex> xxxMutex;
      xxxMutex.grab(queueLock_);

      // Create record of event and callback
      // and add it to end of the queue.
      // Increment count of number of queued
      // events.

      OTC_EventCallback* theLink;
      theLink = new OTC_EventCallback(theEvent,theCallback);
      OTCLIB_ASSERT_M(theLink != 0);

      eventQueue_->addLast(theLink);
      eventCount_++;

      // Don't have to worry about exceptions now.

      xxxMutex.release();

      OTCLIB_TRACER(MODULE) << "event count = " << eventCount_ << endl;

      // Notify a single event.

      if (shutdown_ == false && suspended_ == false)
      {
	OTCLIB_TRACER(MODULE) << "signal thread" << endl;

	eventReady_.signal();
      }
    }

    // Unlock the queue mutex.

    queueLock_.unlock();

    // If the queue was overloaded, call throttle().
    // If only threshold had been reached, throttle()
    // will add event onto end of queue. If queue
    // size has been exceeded it will block until
    // it can add event to the end of the queue.

    if (isOverloaded != false)
      throttle(theEvent,theCallback);
  }
  else
  {
    // No threads so just execute it straight away.

    execute_(theEvent,theCallback);
  }
#else
  execute_(theEvent,theCallback);
#endif
}

#if defined(OSE_WITH_THREADS)
/* ------------------------------------------------------------------------- */
void* OTC_EVThreadPool::threadFunc_(void* theArg)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVThreadPool::threadFunc_(void*)");

  if (theArg == 0)
    return 0;

  OTC_EVThreadHandle* theContext;
  theContext = (OTC_EVThreadHandle*)theArg;

  OTC_EVThreadPool* theHandler;
  theHandler = theContext->threadPool;

  theHandler->queueLock_.lock();

  theContext->threadState = OTCLIB_TASK_STATE_RUNNING;

  theHandler->queueLock_.unlock();

  pthread_setspecific(context_,(void*)theContext);
  pthread_setspecific(OTC_Task::context_,(void*)&theContext->threadState);

  while (1)
  {
    // Grab mutex on queue.

    theHandler->queueLock_.lock();

    // Increment counter for how many threads are
    // waiting for an event.

    theHandler->waiting_++;

    OTCLIB_TRACER(MODULE) << "waiting = ";
    OTCLIB_TRACER(MODULE) << theHandler->waiting_ << endl;

    // Wait on the condition variable mutex
    // linked to the queue mutex until there is
    // an event to be processed or until shutdown
    // is notified. Note that there are two
    // special cases to check for. These are
    // suspension of event processing. That is,
    // if all event processing is suspended
    // don't do anything even if there are events
    // ready. Also, the number of threads active
    // at any one point can be limited. In this
    // case, even if there are events ready,
    // don't process any if the temporary limit
    // on the number of active threads has been
    // reached. Note that when the condition
    // variable is waited upon, it will
    // automatically unlock the queue mutex so
    // that other threads can lock it to check
    // for events or so events can be queued.
    // When the wait is over, the queue mutex
    // will be automatically locked prior to
    // wait() returning. The wait is done in a
    // while loop as the wait can be broken but
    // because another thread make have got in
    // first and taken the event which is ready,
    // the condition may again not be satisified.
    // Thus check is done again and again until
    // we know we have something ready.

    while (
     theHandler->shutdown_ == false &&
     (theHandler->suspended_ != false ||
     theHandler->numThreads_ - theHandler->waiting_ > theHandler->numActive_ ||
     theHandler->eventCount_ == 0))
    {
      theHandler->eventReady_.wait();
    }

    // If the threads are marked for shutdown,
    // increment count of number of threads which
    // have exited, signal anyone waiting on
    // condition variable notifying of shutdown
    // so they are woken up, unlock the queue
    // mutex and then exit the thread.

    if (theHandler->shutdown_ != false)
    {
      theHandler->exited_++;

      theHandler->shutdownReady_.signal();

      if (theContext->threadState != OTCLIB_TASK_STATE_FAILED)
	theContext->threadState = OTCLIB_TASK_STATE_STOPPED;

      theHandler->queueLock_.unlock();

      OTCLIB_TRACER(MODULE) << "exit thread" << endl;

      pthread_exit(0);

      return 0;
    }

    OTCLIB_TRACER(MODULE) << "waiting = ";
    OTCLIB_TRACER(MODULE) << theHandler->waiting_ << endl;

    // There is an event ready and we have the
    // lock on the queue mutex. Decrement
    // count indicating how many threads are
    // waiting for events.

    theHandler->waiting_--;

    // Get the event and callback from the first
    // item in the queue. Have to do this as
    // first step before removing item from
    // queue as removal causes destruction of
    // the information.

    OTC_EventCallback* theLink;
    theLink = (OTC_EventCallback*)theHandler->eventQueue_->first();

    OTC_Event* theEvent;
    OTC_EVCallback* theCallback;

    theEvent = theLink->event();
    theCallback = theLink->callback();

    // Now remove the item from the queue, thus
    // causing it to be destroyed. Also decrement
    // count of how many events are still in the
    // queue.

    theHandler->eventQueue_->removeFirst();
    theHandler->eventCount_--;

    // If a queue size had been defined and
    // removal of the first item in the queue
    // mean't the queue was no longer full,
    // notify any thread which may have been
    // blocked waiting to put an event on
    // the queue.

    if (theHandler->queueSize_ != 0)
    {
      if (theHandler->eventCount_ == theHandler->queueSize_-1)
      {
	OTCLIB_TRACER(MODULE) << "pending ";
	OTCLIB_TRACER(MODULE) << theHandler->queueWaitingId_ << endl;

	theHandler->queueReady_.broadcast();
      }
    }

    // Unlock the queue mutex.

    theHandler->queueLock_.unlock();

    // Execute the callback with the event.
    // After which we go back to waiting on
    // the next event or shutdown.

    try
    {
      theHandler->execute_(theEvent,theCallback);
    }
    catch (OTC_TaskAborted theException)
    {
      // Can get here for two reasons. The thread
      // pool was shutdown and the thread was
      // checking the task state and aborted the
      // task, or the thread simply decided to
      // abort the task. In both cases just go back
      // to start of loop, because if the thread
      // pool was shutdown it will stop anyway. If
      // the thread pool hasn't been shutdown, the
      // next event will be processed.

      // Nothing to do.
    }
    catch (...)
    {
      // If an unexpected exception has occured,
      // shutdown the thread pool to limit damage.
      // This is better than letting it run off the
      // end of the thread, which will likely cause
      // even bigger problems with resource use.

      theHandler->queueLock_.lock();

      theContext->threadState = OTCLIB_TASK_STATE_FAILED;

      theHandler->queueLock_.unlock();

      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_CRITICAL);

      theLogger << "Unexpected exception occurred" << EOL;
      theLogger << "Class: OTC_EVThreadPool" << EOL;
      theLogger << "Method: threadFunc_()" << EOL;
      theLogger << "Description: exception in callback was not caught" << EOL;
      theLogger << "Result: thread pool will be shutdown immediately" << EOL;
      theLogger << flush;

      theHandler->shutdown();
    }
  }

  // Should never get here.

  return 0;
}
#endif

/* ------------------------------------------------------------------------- */
