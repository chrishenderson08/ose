#ifndef ONS_AGENT_AGENT_HH
#define ONS_AGENT_AGENT_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/agent.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/cbobject.hh>
#include <ONS/agent/eventcb.hh>
#include <ONS/agent/shtdwncb.hh>
#include <ONS/agent/signalcb.hh>
#include <ONS/agent/alarmcb.hh>
#include <ONS/agent/timoutcb.hh>
#include <ONS/agent/actioncb.hh>
#include <ONS/agent/socketcb.hh>
#include <OTC/dispatch/startup.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/evhandle.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/agent.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum ONS_LifeTime
{
  ONSLIB_TRANSIENT=0,
  ONSLIB_PERSISTENT=1
};

class OSE_EXPORT ONS_Agent : public OTC_EVAgent, public ONS_CBObject
    // = TITLE
    //	   Event agent implementing callbacks to class member functions.
    //
    // = DESCRIPTION
    //     The <ONS_Agent> class extends the <OTC_EVAgent> class to add
    //     the ability to register specific member functions to be called in
    //     response to certain event types. In doing this it brings together
    //     the functionality of many of the different event classes, hiding
    //     their complexity in the process.
    //     
    //     The class also embeds the ability to optionally create multiple
    //     threads of control for events to be handled within. If threading is
    //     activated with only a single thread, provided that no external
    //     classes makes direct calls against member functions of the agent
    //     instance, including its derived class, and provided that the agent
    //     itself doesn't access data structures shared by other object
    //     instances which aren't internally thread protected, there is no
    //     specific requirement for code to implement thread locking itself.
    //     This is because it is gauranteed that only one event will be
    //     handled at a time. If multiple threads are however created, your
    //     derived class must do appropriate locking on access to shared
    //     data structures.
    //     
    // = NOTES
    //     If the library isn't compiled with thread support, enabling of
    //     thread support within the agent will have no effect and events
    //     will be processed within the context of the main dispatcher loop
    //     as would usually be the case. If this occurs and you implement a
    //     long running process in response to an event, that action will
    //     block the processing of any other events.
    //     
    //     In all cases where callbacks to member functions are being
    //     registered, they must be valid member functions of this class or
    //     derived class instances. If this condition isn't satisified, the
    //     code would typically not compile because of compile time checks
    //     embedded in the code to ensure that this is the case.
    //     
    //     When the methods for cancelling or unsubscribing to all events
    //     in one go are used, they will also stop events of that type
    //     destined for this agent which were subscribed directly using the
    //     lower level event class interfaces.
    //     
    // = SEE ALSO
    //     <OTC_EVAgent>, <OTCEV_Startup>, <OTCEV_Shutdown>, <OTCEV_Signal>,
    //     <OTCEV_Alarm>, <OTCEV_Timeout>, <OTCEV_Action>, <OTCEV_IOEvent>,
    //     <ONS_AgentExtension>
{
    friend class	ONS_AgentExtension;

    typedef ONS_Agent	THIS;

  public:

    // = DESTRUCTION

    			~ONS_Agent();
				// Unsubscribes from all event sources and
				// cancels and timers, alarms and other
				// pending callbacks.

    // = INITIALISATION

			ONS_Agent();
				// Allocates a new agent ID and adds this
				// object to the table of active agents. Note
				// that invoking the constructor does
				// not activate a separate thread of control
				// in which events are handled. To activate
				// the thread, use <activateThreading()>,
				// outside of the scope of the constructor.

    // = STARTUP TRIGGER

    void		scheduleStartup();
				// Schedules the delivery of a startup event
				// to this agent only. If a callback is
				// registered for startup, the callback will
				// be called. Should only really be called
				// once. This should occur after the full
				// agent has been instantiated. Ie. this
				// method should probably be called by the
				// same code as instantiated the agent. A
				// startup callback is the recommended way
				// to activate any thread associated with
				// the agent.

    // = EVENT THREADING
    //     The <activateThreading()> method should not be executed within the
    //     scope of a derived class constructor. Instead, it is best
    //     triggered by a startup method scheduled by the creator of the
    //     derived class instance.

    void		activateThreading(u_int theNumThreads=1);
                                // Initiates the creation of <theNumThreads>
				// in which all events are handled. The
				// default is to create a single thread. If
				// more than one thread is created, any
				// shared data structures in derived class
				// must be suitably protected by locks
				// against access by multiple threads. If
				// there is a single thread, locks can be
				// avoided so long as all access is mediated
				// through events. Prior to actually creating
				// the threads, this method will call the
				// member function <allocateContexts()> to
				// allow derived classes to allocate data
				// structures specific to each thread to be
				// created.

    bool		threadingEnabled() const;
				// Returns <true> if threading has been
				// activated.

    u_int		threadingContext() const;
				// Returns an ID identifying the calling
				// thread within the context of the
				// associated thread pool. If thread support
				// is not compiled in to the library or
				// threading has not been activated, <0> will
				// be returned. If the calling thread is not
				// from the associated thread pool, <0> will
				// again be returned. Otherwise, the thread
				// IDs will range form <1> up to the number
				// of threads.

    void		suspendThreading();
				// Suspends processing of any further events
				// by the threads associated with the agent.
				// While suspended, any events which arrive
				// will be queued up for later consumption.
				// This doesn't stop any threads which may
				// already be handling an event. If threading
				// is activated or thread support isn't
				// compiled into the library, this member
				// function has no affect and events will
				// continue to be processed.

    void		resumeThreading();
				// Resumes processing of events by the
				// threads associated with the agent.

    void		limitThreading(u_int theNumThreads);
				// Temporarily limits the number of threads
				// which are allowed to handle incoming
				// events to <theNumThreads>. This might be
				// used during an initialisation or shutdown
				// phase to set the number of threads to <1>
				// so that events are handled in the order
				// received until a state is reached where
				// having multiple threads running and where
				// the events may appear to be processed out
				// of order is okay. To resume all available
				// threads, provide the argument of <0>. If
				// you want to stop all threads for a while,
				// use <suspendThreading()> instead. Note
				// that if there are threads currently
				// handling events such that more threads are
				// active than <theNumThreads>, currently
				// running threads are not stopped. It just
				// means that the excess threads when they
				// finally finish with the event they are
				// handling will not necessarily grab the
				// next event if there are some available.
				// Instead, the excess threads will wait
				// until the number of active threads is
				// less than <theNumThreads>. This function
				// has no affect when there are no active
				// threads. That is, events will still be
				// handled.

    void		shutdownThreading();
				// Disables processing of any additional
				// events by the threads associated with the
				// agent, with the associated threads also
				// being killed off. If any of the threads
				// are currently processing an event, the
				// method will not return until they have all
				// finished. This method should not be called
				// by an associated thread itself as it will
				// block waiting for itself, but instead
				// should be called from the main dispatcher
				// thread. After this method has been called,
				// it should be quite safe to delete the
				// class instance even when threading is
				// being used.

  protected:

    // = THREAD CONTEXTS

    virtual void	allocateContexts(u_int theNumThreads);
				// This member function is called with
				// <theNumThreads> to be created just prior
				// to the actual threads being created.
				// The member function can be overridden in
				// a derived class to allocate any data
				// structures which need to be specific to
				// each active thread. When the threads
				// are running, the <threadingContext()>
				// member function can be used to identify
				// the thread and thereby access the data
				// structure specific to that thread. Any
				// derived class implementation of this
				// member function should always call as
				// its first step, the version of the
				// method in its immediate base class.

    // = CALLBACK TARGET
    //     Following method is used as a way of ensuring that the pointer
    //     to member function supplied as a callback is a member function of
    //     this class or a member function of a class which is a derived
    //     class of this class. The main check is done at compile time so
    //     your code will not compile if you have done the wrong thing.

    template<class BASE, class DERIVED>
    DERIVED*		derivedObject(BASE* p)
				{
				  DERIVED* o = 0;
				  if (0) { BASE* b = (DERIVED*)0; b++; }
				  try { o = dynamic_cast<DERIVED*>(p); }
				  catch (...) { /* dealt with later */ }
				  return o;
				}
				// Returns argument cast to a derived class
				// type. The types of each are determined by
				// template parameterisation.

  public:

    // = JOB EXECUTION

    void		scheduleJob(OTC_Job* theJob, OTC_JobType theType)
    				{ OTC_Dispatcher::schedule(theJob,theType); }
				// Schedules <theJob> with the dispatcher.
				// <theType> determines whether the job is
				// executed as a priority, standard or idle
				// job.

    // = AGENT STARTUP

    template<class T>
    void		subscribeStartup(void (T::*theMethod)() const)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_EventCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeStartup_(theCallback);
				}
				// Registers this agent as being interested
				// in startup notifications. Notifications
				// will be made by executing <theMethod>.
				// There can only be one callback registered
				// at a time for startup notifications and
				// it will be automatically deregistered once
				// the startup callback has been executed.

    template<class T>
    void		subscribeStartup(void (T::*theMethod)())
    				{
				  ONS_EventCallback* theCallback =
				   ONS_EventCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeStartup_(theCallback);
				}
				// Registers this agent as being interested
				// in startup notifications. Notifications
				// will be made by executing <theMethod>.
				// There can only be one callback registered
				// at a time for startup notifications.

    void		unsubscribeStartup();
				// Cancels any interest by this agent in the
				// startup notifications.

    // = AGENT SHUTDOWN
    //     Note that handling of shutdown events is done in the normal
    //     sequence of events. That is, if threading is used and there is a
    //     queue of events still to be dealt with, the shutdown event does
    //     not skip the queue. If a derived class is completely multithread
    //     safe and implements a means of flagging to a long running thread
    //     handler that shutdown has arrived, the <accept()> method could be
    //     overridden to check for <OTCEV_Shutdown> events. When encountered,
    //     the <handleShutdown()> method could be called with the event so
    //     that it is handled in the main dispatcher thread, thus skipping
    //     any queue of pending events.

    void		scheduleShutdown(double theDelay=0.0);
				// Schedules the shutdown procedure for
				// the whole process. If this function is
				// called when a shutdown is already in
				// progress, the subsequent request will be
				// ignored. Messages are output via the
				// logger to indicate each phase of the
				// shutdown. <theDelay> indicates the number
				// of seconds between the pending shutdown
				// notification and the final shutdown
				// notification. Note that it is not possible
				// to specific a delay of zero seconds. The
				// default value of <0.0> is used to indicate
				// the inbuilt delay should be used. The
				// inbuilt delay defaults to 1 second but can
				// be overridden by the environment variable
				// <OTCLIB_SHUTDOWNDELAY> to the number of
				// milliseconds to wait. Note though that
				// although the environment takes delay in
				// exact milliseconds, this member function
				// takes it as a floating point value
				// representing the number of seconds.

    void		suspendShutdown();
				// Causes any shutdown to be delayed. The
				// function increments by one a count
				// indicating parties wanting shutdown to be
				// delayed. Delaying of shutdown is useful to
				// allow agents within the event system to
				// perform some required action first. This
				// function might be called in response to
				// a pending shutdown notification, but may
				// be called before a shutdown has even been
				// initiated. If notifications have already
				// been sent out for final shutdown, the
				// call to this function will not stop the
				// shutdown.

    void		resumeShutdown();
				// Decrements by one the number of parties
				// wanting shutdown to be delayed. If this
				// reaches <0> and a shutdown was supposed
				// to have already have occurred, it will
				// happen immediately. If the shutdown was
				// pending, it will still happen at the
				// scheduled time. If no shutdown has yet
				// been scheduled, nothing at all will
				// happen.

    // Note that in subscribing to shutdown events, an automatic subscription
    // is registered for interest in program shutdown events as well as any
    // other shutdown events which may happen to be directly delivered to the
    // agent. Program shutdown events will have an associated description for
    // the event which is empty. If shutdown events are used for other
    // purposes, the associated description should be non empty and a
    // callback should be registered which can accept the description and act
    // differently if appropriate based on whether it is program shutdown or
    // some other type of shutdown. Note that to deliver a specific type of
    // shutdown event to another agent, the <OTCEV_Shutdown> event class will
    // need to be used explicitly.

    template<class T>
    void		subscribeShutdown(
			 void (T::*theMethod)(OTC_ShutdownNotification) const
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ShutdownCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeShutdown_(theCallback);
				}
				// Registers this agent as being interested
				// in shutdown notifications. Notifications
				// will be made by executing <theMethod>.
				// There can only be one callback registered
				// at a time for shutdown notifications.

    template<class T>
    void		subscribeShutdown(
			 void (T::*theMethod)(OTC_ShutdownNotification)
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ShutdownCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeShutdown_(theCallback);
				}
				// Registers this agent as being interested
				// in shutdown notifications. Notifications
				// will be made by executing <theMethod>.
				// There can only be one callback registered
				// at a time for shutdown notifications.

    template<class T>
    void		subscribeShutdown(
			 void (T::*theMethod)(OTC_ShutdownNotification,
			  char const*) const
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ShutdownCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeShutdown_(theCallback);
				}
				// Registers this agent as being interested
				// in shutdown notifications. Notifications
				// will be made by executing <theMethod>.
				// There can only be one callback registered
				// at a time for shutdown notifications.

    template<class T>
    void		subscribeShutdown(
			 void (T::*theMethod)(OTC_ShutdownNotification,
			  char const*)
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ShutdownCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeShutdown_(theCallback);
				}
				// Registers this agent as being interested
				// in shutdown notifications. Notifications
				// will be made by executing <theMethod>.
				// There can only be one callback registered
				// at a time for shutdown notifications.

    template<class T>
    void		subscribeShutdown(
			 void (T::*theMethod)(OTC_ShutdownNotification,
			  OTC_String const&) const
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ShutdownCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeShutdown_(theCallback);
				}
				// Registers this agent as being interested
				// in shutdown notifications. Notifications
				// will be made by executing <theMethod>.
				// There can only be one callback registered
				// at a time for shutdown notifications.

    template<class T>
    void		subscribeShutdown(
			 void (T::*theMethod)(OTC_ShutdownNotification,
			  OTC_String const&)
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ShutdownCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeShutdown_(theCallback);
				}
				// Registers this agent as being interested
				// in shutdown notifications. Notifications
				// will be made by executing <theMethod>.
				// There can only be one callback registered
				// at a time for shutdown notifications.

    void		unsubscribeShutdown();
				// Cancels any interest by this agent in the
				// shutdown notifications.

    // = PROCESS SIGNALS

    template<class T>
    void		subscribeSignal(
			 void (T::*theMethod)() const,
			 int theSignal
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SignalCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSignal_(theCallback,theSignal);
				}
				// Registers this agent as being interested
				// in <theSignal>. Notifications will be made
				// by executing <theMethod>. There can only
				// be one callback registered at a time
				// against each type of signal.

    template<class T>
    void		subscribeSignal(
			 void (T::*theMethod)(),
			 int theSignal
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SignalCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSignal_(theCallback,theSignal);
				}
				// Registers this agent as being interested
				// in <theSignal>. Notifications will be made
				// by executing <theMethod>. There can only
				// be one callback registered at a time
				// against each type of signal.

    template<class T>
    void		subscribeSignal(
			 void (T::*theMethod)(int) const,
			 int theSignal
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SignalCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSignal_(theCallback,theSignal);
				}
				// Registers this agent as being interested
				// in <theSignal>. Notifications will be made
				// by executing <theMethod>. There can only
				// be one callback registered at a time
				// against each type of signal.

    template<class T>
    void		subscribeSignal(
			 void (T::*theMethod)(int),
			 int theSignal
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SignalCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSignal_(theCallback,theSignal);
				}
				// Registers this agent as being interested
				// in <theSignal>. Notifications will be made
				// by executing <theMethod>. There can only
				// be one callback registered at a time
				// against each type of signal.

    void		unsubscribeSignal(int theSignal);
				// Cancels any interest by this agent in
				// notifications about <theSignal>.

    void		unsubscribeAllSignals();
				// Cancels any interest by this agent in
				// any notifications about signals.

    // = ALARM CLOCK

    template<class T>
    OTC_String		setAlarm(
			 void (T::*theMethod)() const,
			 time_t theTime,
			 char const* theAlarm=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_AlarmCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers this agent as being interested
				// in <theAlarm> set to go off at <theTime>.
				// Notifications will be made by executing
				// <theMethod>. The value returned will be
				// <theAlarm>, or if <theAlarm> is <0> an
				// internally generated name used to identify
				// the alarm.

    template<class T>
    OTC_String		setAlarm(
			 void (T::*theMethod)(),
			 time_t theTime,
			 char const* theAlarm=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_AlarmCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers this agent as being interested
				// in <theAlarm> set to go off at <theTime>.
				// Notifications will be made by executing
				// <theMethod>. The value returned will be
				// <theAlarm>, or if <theAlarm> is <0> an
				// internally generated name used to identify
				// the alarm.

    template<class T>
    OTC_String		setAlarm(
			 void (T::*theMethod)(char const*) const,
			 time_t theTime,
			 char const* theAlarm=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_AlarmCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers this agent as being interested
				// in <theAlarm> set to go off at <theTime>.
				// Notifications will be made by executing
				// <theMethod>. The value returned will be
				// <theAlarm>, or if <theAlarm> is <0> an
				// internally generated name used to identify
				// the alarm.

    template<class T>
    OTC_String		setAlarm(
			 void (T::*theMethod)(char const*),
			 time_t theTime,
			 char const* theAlarm=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_AlarmCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers this agent as being interested
				// in <theAlarm> set to go off at <theTime>.
				// Notifications will be made by executing
				// <theMethod>. The value returned will be
				// <theAlarm>, or if <theAlarm> is <0> an
				// internally generated name used to identify
				// the alarm.

    template<class T>
    OTC_String		setAlarm(
			 void (T::*theMethod)(OTC_String const&) const,
			 time_t theTime,
			 char const* theAlarm=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_AlarmCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers this agent as being interested
				// in <theAlarm> set to go off at <theTime>.
				// Notifications will be made by executing
				// <theMethod>. The value returned will be
				// <theAlarm>, or if <theAlarm> is <0> an
				// internally generated name used to identify
				// the alarm.

    template<class T>
    OTC_String		setAlarm(
			 void (T::*theMethod)(OTC_String const&),
			 time_t theTime,
			 char const* theAlarm=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_AlarmCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers this agent as being interested
				// in <theAlarm> set to go off at <theTime>.
				// Notifications will be made by executing
				// <theMethod>. The value returned will be
				// <theAlarm>, or if <theAlarm> is <0> an
				// internally generated name used to identify
				// the alarm.

    void		cancelAlarm(char const* theAlarm);
				// Cancels any interest by this agent in
				// any notifications about <theAlarm>.

    void		cancelAllAlarms();
				// Cancels any interest by this agent in
				// any notifications about alarms.

    bool		alarmPending(char const* theAlarm) const;
				// Returns <true> if <theAlarm> is still
				// pending.

    // = INTERVAL TIMER

    template<class T>
    OTC_String		startTimer(
			 void (T::*theMethod)() const,
			 double theDelay,
			 char const* theTimer=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_TimeoutCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers this agent as being interested
				// in <theTimer> set to go off in <theDelay>
				// seconds. Notifications will be made by
				// executing <theMethod>. The value returned
				// will be <theTimer>, or if <theTimer> is
				// <0> an internally generated name used to
				// identify the timer.

    template<class T>
    OTC_String		startTimer(
			 void (T::*theMethod)(),
			 double theDelay,
			 char const* theTimer=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_TimeoutCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers this agent as being interested
				// in <theTimer> set to go off in <theDelay>
				// seconds. Notifications will be made by
				// executing <theMethod>. The value returned
				// will be <theTimer>, or if <theTimer> is
				// <0> an internally generated name used to
				// identify the timer.

    template<class T>
    OTC_String		startTimer(
			 void (T::*theMethod)(char const*) const,
			 double theDelay,
			 char const* theTimer=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_TimeoutCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers this agent as being interested
				// in <theTimer> set to go off in <theDelay>
				// seconds. Notifications will be made by
				// executing <theMethod>. The value returned
				// will be <theTimer>, or if <theTimer> is
				// <0> an internally generated name used to
				// identify the timer.

    template<class T>
    OTC_String		startTimer(
			 void (T::*theMethod)(char const*),
			 double theDelay,
			 char const* theTimer=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_TimeoutCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers this agent as being interested
				// in <theTimer> set to go off in <theDelay>
				// seconds. Notifications will be made by
				// executing <theMethod>. The value returned
				// will be <theTimer>, or if <theTimer> is
				// <0> an internally generated name used to
				// identify the timer.

    template<class T>
    OTC_String		startTimer(
			 void (T::*theMethod)(OTC_String const&) const,
			 double theDelay,
			 char const* theTimer=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_TimeoutCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers this agent as being interested
				// in <theTimer> set to go off in <theDelay>
				// seconds. Notifications will be made by
				// executing <theMethod>. The value returned
				// will be <theTimer>, or if <theTimer> is
				// <0> an internally generated name used to
				// identify the timer.

    template<class T>
    OTC_String		startTimer(
			 void (T::*theMethod)(OTC_String const&),
			 double theDelay,
			 char const* theTimer=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_TimeoutCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers this agent as being interested
				// in <theTimer> set to go off in <theDelay>
				// seconds. Notifications will be made by
				// executing <theMethod>. The value returned
				// will be <theTimer>, or if <theTimer> is
				// <0> an internally generated name used to
				// identify the timer.

    void		cancelTimer(char const* theTimer);
				// Cancels any interest by this agent in
				// any notifications about <theTimer>.

    void		cancelAllTimers();
				// Cancels any interest by this agent in
				// any notifications about timers.

    bool		timerPending(char const* theTimer) const;
				// Returns <true> if <theTimer> is still
				// pending.

    // = CALLBACK ACTION
    //     Callback actions come in three types. There are simple once off
    //     job style callbacks, CRON style recurring callbacks and wakeup
    //     or synchronisation callbacks. Note that all types of delayed
    //     callbacks share the same namespace so instigating a new callback
    //     with a name the same as an existing one, regardless of type, will
    //     replace the previous one.
    //
    //     First set of methods provide means of scheduling simple once off
    //     job style callbacks. That is, the callback is scheduled at the same
    //     time that the callback is registered. The callback is deregistered
    //     automatically after being called.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)() const,
			 OTC_JobType theType,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theType,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How soon the callback occurs
				// depends on <theType> which determines what
				// type of job is scheduled with the
				// dispatcher to make the callback. The
				// possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(),
			 OTC_JobType theType,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theType,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How soon the callback occurs
				// depends on <theType> which determines what
				// type of job is scheduled with the
				// dispatcher to make the callback. The
				// possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(char const*) const,
			 OTC_JobType theType,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theType,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How soon the callback occurs
				// depends on <theType> which determines what
				// type of job is scheduled with the
				// dispatcher to make the callback. The
				// possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(char const*),
			 OTC_JobType theType,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theType,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How soon the callback occurs
				// depends on <theType> which determines what
				// type of job is scheduled with the
				// dispatcher to make the callback. The
				// possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(OTC_String const&) const,
			 OTC_JobType theType,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theType,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How soon the callback occurs
				// depends on <theType> which determines what
				// type of job is scheduled with the
				// dispatcher to make the callback. The
				// possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(OTC_String const&),
			 OTC_JobType theType,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theType,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How soon the callback occurs
				// depends on <theType> which determines what
				// type of job is scheduled with the
				// dispatcher to make the callback. The
				// possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the action.

    // Following methods provide means of specifying a periodic callback
    // using a CRON type time specification. The callback registration
    // persists beyond the callback being called the first time, since the
    // CRON specification may denote a recurring event. If it was the case
    // that the CRON specification denoted a once off event, the callback
    // would need to deregister itself by calling <cancelAction()>.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)() const,
			 char const* theSpecification,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theSpecification,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How often the callback occurs
				// depends on <theSpecification> which is
				// a CRON style time specification. The value
				// returned will be <theAction>, or if
				// <theAction> is <0> an internally generated
				// name used to identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(),
			 char const* theSpecification,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theSpecification,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How often the callback occurs
				// depends on <theSpecification> which is
				// a CRON style time specification. The value
				// returned will be <theAction>, or if
				// <theAction> is <0> an internally generated
				// name used to identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(char const*) const,
			 char const* theSpecification,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theSpecification,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How often the callback occurs
				// depends on <theSpecification> which is
				// a CRON style time specification. The value
				// returned will be <theAction>, or if
				// <theAction> is <0> an internally generated
				// name used to identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(char const*),
			 char const* theSpecification,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theSpecification,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How often the callback occurs
				// depends on <theSpecification> which is
				// a CRON style time specification. The value
				// returned will be <theAction>, or if
				// <theAction> is <0> an internally generated
				// name used to identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(OTC_String const&) const,
			 char const* theSpecification,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theSpecification,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How often the callback occurs
				// depends on <theSpecification> which is
				// a CRON style time specification. The value
				// returned will be <theAction>, or if
				// <theAction> is <0> an internally generated
				// name used to identify the action.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)(OTC_String const&),
			 char const* theSpecification,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return scheduleAction_(theCallback,
				   theSpecification,theAction);
				}
				// Schedules a delayed callback of
				// <theMethod>. How often the callback occurs
				// depends on <theSpecification> which is
				// a CRON style time specification. The value
				// returned will be <theAction>, or if
				// <theAction> is <0> an internally generated
				// name used to identify the action.

    // Wakeup or synchronisation callbacks are different to the other
    // callback actions in that a simple callback action schedules the
    // callback at the same time as the callback is registered. With a wakeup
    // callback however, the registration of the callback and the scheduling
    // of the calling of that callback are distinct in time. This is similar
    // to a CRON style callback, except that when a CRON style callback is
    // called is denoted by the callback specification. With the wakeup
    // callback the user has exact control over when they wish to trigger the
    // callback by calling the <triggerAction()> method. A wakeup callback
    // can therefore be used as a form of syncronisation. Wakeup callbacks
    // will be deregistered or not when executed, depending on how the
    // subscription is original setup.

    template<class T>
    OTC_String		subscribeAction(
			 void (T::*theMethod)() const,
			 ONS_LifeTime theLifeTime=ONSLIB_TRANSIENT,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeAction_(theCallback,
				   theLifeTime,theAction);
				}
				// Registers a delayed callback of
				// <theMethod>. <theMethod> will be called
				// when a wakeup event is scheduled with a
				// name of <theAction>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the callback registration is
				// used. In the case where <theAction> is
				// <0>, the value returned by the method
				// should instead be used to trigger the
				// delayed callback. If <theLifeTime> is
				// set to <ONSLIB_TRANSIENT>, the callback
				// will be deregistered when it is
				// subsequently executed. If <theLifeTime>
				// is <ONSLIB_PERSISTENT>, the registration
				// will be left as is and it will be
				// necessary to explicitly unsubscribe the
				// registration.

    template<class T>
    OTC_String		subscribeAction(
			 void (T::*theMethod)(),
			 ONS_LifeTime theLifeTime=ONSLIB_TRANSIENT,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeAction_(theCallback,
				   theLifeTime,theAction);
				}
				// Registers a delayed callback of
				// <theMethod>. <theMethod> will be called
				// when a wakeup event is scheduled with a
				// name of <theAction>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the callback registration is
				// used. In the case where <theAction> is
				// <0>, the value returned by the method
				// should instead be used to trigger the
				// delayed callback. If <theLifeTime> is
				// set to <ONSLIB_TRANSIENT>, the callback
				// will be deregistered when it is
				// subsequently executed. If <theLifeTime>
				// is <ONSLIB_PERSISTENT>, the registration
				// will be left as is and it will be
				// necessary to explicitly unsubscribe the
				// registration.

    template<class T>
    OTC_String		subscribeAction(
			 void (T::*theMethod)(char const*) const,
			 ONS_LifeTime theLifeTime=ONSLIB_TRANSIENT,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeAction_(theCallback,
				   theLifeTime,theAction);
				}
				// Registers a delayed callback of
				// <theMethod>. <theMethod> will be called
				// when a wakeup event is scheduled with a
				// name of <theAction>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the callback registration is
				// used. In the case where <theAction> is
				// <0>, the value returned by the method
				// should instead be used to trigger the
				// delayed callback. If <theLifeTime> is
				// set to <ONSLIB_TRANSIENT>, the callback
				// will be deregistered when it is
				// subsequently executed. If <theLifeTime>
				// is <ONSLIB_PERSISTENT>, the registration
				// will be left as is and it will be
				// necessary to explicitly unsubscribe the
				// registration.

    template<class T>
    OTC_String		subscribeAction(
			 void (T::*theMethod)(char const*),
			 ONS_LifeTime theLifeTime=ONSLIB_TRANSIENT,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeAction_(theCallback,
				   theLifeTime,theAction);
				}
				// Registers a delayed callback of
				// <theMethod>. <theMethod> will be called
				// when a wakeup event is scheduled with a
				// name of <theAction>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the callback registration is
				// used. In the case where <theAction> is
				// <0>, the value returned by the method
				// should instead be used to trigger the
				// delayed callback. If <theLifeTime> is
				// set to <ONSLIB_TRANSIENT>, the callback
				// will be deregistered when it is
				// subsequently executed. If <theLifeTime>
				// is <ONSLIB_PERSISTENT>, the registration
				// will be left as is and it will be
				// necessary to explicitly unsubscribe the
				// registration.

    template<class T>
    OTC_String		subscribeAction(
			 void (T::*theMethod)(OTC_String const&) const,
			 ONS_LifeTime theLifeTime=ONSLIB_TRANSIENT,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeAction_(theCallback,
				   theLifeTime,theAction);
				}
				// Registers a delayed callback of
				// <theMethod>. <theMethod> will be called
				// when a wakeup event is scheduled with a
				// name of <theAction>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the callback registration is
				// used. In the case where <theAction> is
				// <0>, the value returned by the method
				// should instead be used to trigger the
				// delayed callback. If <theLifeTime> is
				// set to <ONSLIB_TRANSIENT>, the callback
				// will be deregistered when it is
				// subsequently executed. If <theLifeTime>
				// is <ONSLIB_PERSISTENT>, the registration
				// will be left as is and it will be
				// necessary to explicitly unsubscribe the
				// registration.

    template<class T>
    OTC_String		subscribeAction(
			 void (T::*theMethod)(OTC_String const&),
			 ONS_LifeTime theLifeTime=ONSLIB_TRANSIENT,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeAction_(theCallback,
				   theLifeTime,theAction);
				}
				// Registers a delayed callback of
				// <theMethod>. <theMethod> will be called
				// when a wakeup event is scheduled with a
				// name of <theAction>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the callback registration is
				// used. In the case where <theAction> is
				// <0>, the value returned by the method
				// should instead be used to trigger the
				// delayed callback. If <theLifeTime> is
				// set to <ONSLIB_TRANSIENT>, the callback
				// will be deregistered when it is
				// subsequently executed. If <theLifeTime>
				// is <ONSLIB_PERSISTENT>, the registration
				// will be left as is and it will be
				// necessary to explicitly unsubscribe the
				// registration.

    // The following provides the mechanism for triggering a wakeup callback.
    // Whether or not the callback is automatically deregistered once called
    // is dependent on whether or not the lifetime of the subscription was
    // set to be transient or persistent at the time it was created. Note
    // that this method can also be used to trigger a CRON style callback at
    // a different time to when the CRON specification says it would be
    // called. In that case, the CRON style callback will not be deregistered.

    void		triggerAction(
    			 char const* theAction,
			 OTC_JobType theType
			);
    				// Schedules the execution of the registered
				// callback with name <theAction>.
				// The possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>.

    // Following provide a means of cancelling a CRON style callback or
    // wakeup callback. It will also cancel a simple job style callback
    // if the scheduled callback hasn't yet occurred.

    void		cancelAction(char const* theAction);
				// Cancels callback for <theAction>.

    void		cancelAllActions();
				// Cancels all callback actions.

    // = SOCKET EVENTS

    template<class T>
    void		subscribeSocket(
			 void (T::*theMethod)() const,
			 int theSocket,
			 int theEvents=OTCLIB_POLLIN
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SocketCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSocket_(theCallback,theSocket,
				   theEvents);
				}
				// Registers this agent as being interested
				// in <theEvents> on <theSocket>. Any
				// notifications will be made by executing
				// <theMethod>.

    template<class T>
    void		subscribeSocket(
			 void (T::*theMethod)(),
			 int theSocket,
			 int theEvents=OTCLIB_POLLIN
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SocketCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSocket_(theCallback,theSocket,
				   theEvents);
				}
				// Registers this agent as being interested
				// in <theEvents> on <theSocket>. Any
				// notifications will be made by executing
				// <theMethod>.

    template<class T>
    void		subscribeSocket(
			 void (T::*theMethod)(int) const,
			 int theSocket,
			 int theEvents=OTCLIB_POLLIN
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SocketCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSocket_(theCallback,theSocket,
				   theEvents);
				}
				// Registers this agent as being interested
				// in <theEvents> on <theSocket>. Any
				// notifications will be made by executing
				// <theMethod>.

    template<class T>
    void		subscribeSocket(
			 void (T::*theMethod)(int),
			 int theSocket,
			 int theEvents=OTCLIB_POLLIN
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SocketCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSocket_(theCallback,theSocket,
				   theEvents);
				}
				// Registers this agent as being interested
				// in <theEvents> on <theSocket>. Any
				// notifications will be made by executing
				// <theMethod>.

    template<class T>
    void		subscribeSocket(
			 void (T::*theMethod)(int,int) const,
			 int theSocket,
			 int theEvents=OTCLIB_POLLIN
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SocketCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSocket_(theCallback,theSocket,
				   theEvents);
				}
				// Registers this agent as being interested
				// in <theEvents> on <theSocket>. Any
				// notifications will be made by executing
				// <theMethod>.

    template<class T>
    void		subscribeSocket(
			 void (T::*theMethod)(int,int),
			 int theSocket,
			 int theEvents=OTCLIB_POLLIN
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SocketCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeSocket_(theCallback,theSocket,
				   theEvents);
				}
				// Registers this agent as being interested
				// in <theEvents> on <theSocket>. Any
				// notifications will be made by executing
				// <theMethod>.

    void		unsubscribeSocket(int theSocket, int theEvents=~0);
				// Cancels any interest by this agent in
				// <theEvents> on <theSocket>.

    void		unsubscribeAllSockets();
				// Cancels any interest by this agent in
				// any socket notifications.

  protected:

    // = EVENT HANDLING

    void		execute(OTC_Event* theEvent);
				// If threading is enabled, hands <theEvent>
				// off to the thread pool handler such that
				// <filter()> is called within the context
				// of a separate thread.

    void		filter(OTC_Event* theEvent);
				// Determines if <theEvent> corresponds to
				// a registered callback and if so triggers
				// the callback instead. Other events are
				// passed onto the base class <filter()>
				// member function.

    void		handle(OTC_Event* theEvent);
				// Destroys <theEvent>. Should be overridden
				// in a derived class to handle any event
				// not associated with a callback.

    // = CALLBACK TRIGGERS
    //     These methods could be called be a derived class which has
    //     overridden the <OTC_EVAgent> class <accept()> method so as to have
    //     certain events handled outside of the bounds of any thread. In
    //     doing so however, such a derived class would need to use thread
    //     locking on any data structures.

    virtual void	handleStartup(OTCEV_Startup* theStartup);
				// Called when <theStartup> event is
				// received. If a startup callback is
				// registered, it will be called. Note though
				// that it is suggested that a startup event
				// be what triggers the activation of
				// threading in the first place.

    virtual void	handleShutdown(OTCEV_Shutdown* theShutdown);
				// Called when <theShutdown> event is
				// received. If a shutdown callback is
				// registered, it will be called.

    virtual void	handleSignal(OTCEV_Signal* theSignal);
				// Called when <theSignal> is received.
				// If a signal callback is registered for
				// the particular signal, it will be
				// called.

    virtual void	handleAlarm(OTCEV_Alarm* theAlarm);
				// Called when <theAlarm> is received.
				// If an alarm callback is registered for
				// the particular alarm, it will be
				// called with the callback reference then
				// being destroyed.

    virtual void	handleTimeout(OTCEV_Timeout* theTimeout);
				// Called when <theTimeout> is received.
				// If a timer callback is registered for
				// the particular timer, it will be
				// called with the callback reference then
				// being destroyed.

    virtual void	handleAction(OTCEV_Action* theAction);
				// Called when <theAction> is received.
				// If an action callback is registered for
				// the particular action, it will be
				// called. If the action is a once off
				// callback, the callback reference will then
				// be destroyed.

    virtual void	handleSocket(OTCEV_IOEvent* theIOEvent);
				// Called when <theIOEvent> is received.
				// If a socket callback is registered for
				// the particular socket and event type on the
				// socket, it will be called.

  private:

    // = CALLBACK REGISTRATION

    void		subscribeStartup_(ONS_EventCallback* theCallback);
				// Registers this agent as being interested
				// in startup notifications. Notifications
				// will be made by executing <theCallback>.

    void		subscribeShutdown_(ONS_EventCallback* theCallback);
				// Registers this agent as being interested
				// in shutdown notifications. Notifications
				// will be made by executing <theCallback>.

    void		subscribeSignal_(
			 ONS_EventCallback* theCallback,
			 int theSignal
			);
				// Registers this agent as being interested
				// in <theSignal>. Notifications will be made
				// by executing <theCallback>.

    OTC_String		setAlarm_(
			 ONS_EventCallback* theCallback,
			 time_t theTime,
			 char const* theAlarm
			);
				// Registers this agent as being interested
				// in <theAlarm> set to go off at <theTime>.
				// Notifications will be made by executing
				// <theCallback>. Returns <theAlarm> or if
				// <theAlarm> is <0>, returns an internally
				// generated alarm name.

    OTC_String		startTimer_(
			 ONS_EventCallback* theCallback,
			 double theDelay,
			 char const* theTimer
			);
				// Registers this agent as being interested
				// in <theTimer> set to go off <theDelay>
				// seconds. Notifications will be made by
				// executing <theCallback>. Returns
				// <theTimer> or if <theTimer> is <0>,
				// returns an internally generated timer
				// name.

    OTC_String		scheduleAction_(
			 ONS_EventCallback* theCallback,
			 OTC_JobType theType,
			 char const* theAction
			);
				// Schedules a delayed action to call
				// <theCallback>. How soon the callback
				// occurs depends on <theType> which
				// determines what type of job is scheduled
				// with the dispatcher to make the callback.
				// The possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the action.

    OTC_String		scheduleAction_(
			 ONS_EventCallback* theCallback,
			 char const* theSpecification,
			 char const* theAction
			);
				// Schedules a delayed callback of
				// <theCallback>. How often the callback
				// occurs depends on <theSpecification> which
				// is a CRON style time specification. The
				// value returned will be <theAction>, or if
				// <theAction> is <0> an internally generated
				// name used to identify the action.

    OTC_String		subscribeAction_(
			 ONS_EventCallback* theCallback,
			 ONS_LifeTime theLifeTime,
			 char const* theAction
			);
				// Registers a delayed callback of
				// <theCallback>. <theMethod> will be called
				// when a wakeup event is scheduled with a
				// name of <theAction>. The value returned
				// will be <theAction>, or if <theAction> is
				// <0> an internally generated name used to
				// identify the callback registration is
				// used. In the case where <theAction> is
				// <0>, the value returned by the method
				// should instead be used to trigger the
				// delayed callback. If <theLifeTime> is
				// set to <ONSLIB_TRANSIENT>, the callback
				// will be deregistered when it is
				// subsequently executed. If <theLifeTime>
				// is <ONSLIB_PERSISTENT>, the registration
				// will be left as is and it will be necessary
				// to explicitly unsubscribe the registration.

    void		subscribeSocket_(
			 ONS_EventCallback* theCallback,
			 int theSocket,
			 int theEvents
			);
				// Registers this agent as being interested
				// in <theEvents> on <theSocket>. Notifications
				// will be made by executing <theCallback>.

  private:

    			ONS_Agent(ONS_Agent const&);
				// Do not define an implementation for this.

    ONS_Agent&		operator=(ONS_Agent const&);
				// Do not define an implementation for this.

    OTC_NRMutex		mutex_;
				// Mutex to protected access by threads.

    OTC_EVHandler*	thread_;
    				// Threaded event handler when active.

    OTC_HMap<OTC_String,ONS_EventCallback*>	agentCallbacks_;
				// Mapping from the subscription key to the
				// registered callback for that action.
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_AGENT_HH */
