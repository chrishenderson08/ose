#ifndef ONS_AGENT_AGNTEXTN_HH
#define ONS_AGENT_AGNTEXTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/agntextn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/agent.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_AgentExtension : public OTC_Resource, public ONS_CBObject
    // = TITLE
    //	   Extension for callbacks objects associated with an agent.
    //
    // = DESCRIPTION
    //     The <ONS_AgentExtension> class provides a means of separating out
    //     from an <ONS_Agent> derived class, any exported methods, callbacks
    //     and associated functionality. This means that it is possible to
    //     create modules of reusable functionality which can be associated
    //     with any derived instance of <ONS_Agent>.
    //
    // = NOTES
    //     Any derived instance of this class must be created using <operator
    //     new()> on the heap. Once a derived class calls any method which
    //     registers some form of callback, the associated agent class
    //     instance will manage ownership of the object and the object will
    //     be automatically deleted when the last callback has been removed.
    //     Thus, unless you wanted to reuse an object, there isn't actually a
    //     need to keep a pointer to the object once it has been setup nor
    //     explicitly delete it. If you do keep a pointer, you must use the
    //     reference counting management functions of the base class as
    //     appropriate.
    //     
    // = SEE ALSO
    //     <ONS_Agent>
{
    typedef ONS_AgentExtension	THIS;
    typedef OTC_Resource	COBJ;

  public:

    // = INITIALISATION

			ONS_AgentExtension(ONS_Agent* theAgent)
			 : agent_(theAgent) {}
				// Associates this callback object with
				// <theAgent>.

    // = TARGET AGENT

    ONS_Agent*		agent() const
    				{ return agent_; }
				// Returns a pointer to the associated
				// agent.

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

    // = PROCESS SIGNALS

    template<class T>
    void		subscribeSignal(
			 void (T::*theMethod)() const,
			 int theSignal
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SignalCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSignal_(
				   theCallback,theSignal);
				}
				// Registers the agent as being interested
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
				   ONS_SignalCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSignal_(
				   theCallback,theSignal);
				}
				// Registers the agent as being interested
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
				   ONS_SignalCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSignal_(
				   theCallback,theSignal);
				}
				// Registers the agent as being interested
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
				   ONS_SignalCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSignal_(
				   theCallback,theSignal);
				}
				// Registers the agent as being interested
				// in <theSignal>. Notifications will be made
				// by executing <theMethod>. There can only
				// be one callback registered at a time
				// against each type of signal.

    void		unsubscribeSignal(int theSignal)
    				{ agent_->unsubscribeSignal(theSignal); }
				// Cancels any interest by the agent in
				// notifications about <theSignal>.

    // = ALARM CLOCK

    template<class T>
    OTC_String		setAlarm(
			 void (T::*theMethod)() const,
			 time_t theTime,
			 char const* theAlarm=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_AlarmCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers the agent as being interested
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
				   ONS_AlarmCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers the agent as being interested
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
				   ONS_AlarmCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers the agent as being interested
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
				   ONS_AlarmCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers the agent as being interested
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
				   ONS_AlarmCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers the agent as being interested
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
				   ONS_AlarmCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->setAlarm_(theCallback,
				   theTime,theAlarm);
				}
				// Registers the agent as being interested
				// in <theAlarm> set to go off at <theTime>.
				// Notifications will be made by executing
				// <theMethod>. The value returned will be
				// <theAlarm>, or if <theAlarm> is <0> an
				// internally generated name used to identify
				// the alarm.

    void		cancelAlarm(char const* theAlarm)
    				{ agent_->cancelAlarm(theAlarm); }
				// Cancels any interest by the agent in
				// any notifications about <theAlarm>.

    bool		alarmPending(char const* theAlarm) const
    				{ return agent_->alarmPending(theAlarm); }
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
				   ONS_TimeoutCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers the agent as being interested
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
				   ONS_TimeoutCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers the agent as being interested
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
				   ONS_TimeoutCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers the agent as being interested
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
				   ONS_TimeoutCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers the agent as being interested
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
				   ONS_TimeoutCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers the agent as being interested
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
				   ONS_TimeoutCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->startTimer_(theCallback,
				   theDelay,theTimer);
				}
				// Registers the agent as being interested
				// in <theTimer> set to go off in <theDelay>
				// seconds. Notifications will be made by
				// executing <theMethod>. The value returned
				// will be <theTimer>, or if <theTimer> is
				// <0> an internally generated name used to
				// identify the timer.

    void		cancelTimer(char const* theTimer)
    				{ agent_->cancelTimer(theTimer); }
				// Cancels any interest by the agent in
				// any notifications about <theTimer>.

    bool		timerPending(char const* theTimer) const
    				{ return agent_->timerPending(theTimer); }
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
    //     First method is actually a special case version of scheduling a
    //     simple once off job style callback. In this case a dummy method
    //     is called. This particular function would be used when in starting
    //     up a derived callback object it was found that no callback
    //     registration was appropriate. The problem here is that the user of
    //     the callback object would expect that the callback object will be
    //     automatically deleted when no further callbacks are deregistered.
    //     If no callbacks are registered in the first place however, it
    //     will never be deleted. Thus a dummy action is registered and when
    //     that has triggered, the callback object will be destroyed.

    OTC_String		scheduleAction(OTC_JobType theType)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<COBJ,
				   THIS>::create(derivedObject<THIS,
				   THIS>(this),&THIS::dummyAction_);
				  return agent_->scheduleAction_(theCallback,
				   theType,0);
				}
				// Schedules a delayed callback of a dummy
				// method. How soon the callback occurs
				// depends on <theType> which determines what
				// type of job is scheduled with the
				// dispatcher to make the callback. The
				// possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>. The value returned
				// will be an internally generated name used
				// to identify the action.

    // Next set of methods provide main means of scheduling simple once off
    // job style callbacks. That is, the callback is scheduled at the same
    // time that the callback is registered. The callback is deregistered
    // automatically after being called.

    template<class T>
    OTC_String		scheduleAction(
			 void (T::*theMethod)() const,
			 OTC_JobType theType,
			 char const* theAction=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent_->scheduleAction_(theCallback,
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
				   ONS_ActionCallbackFactory<COBJ,
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
				   ONS_ActionCallbackFactory<COBJ,
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
				   ONS_ActionCallbackFactory<COBJ,
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
				   ONS_ActionCallbackFactory<COBJ,
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
				   ONS_ActionCallbackFactory<COBJ,
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
				   ONS_ActionCallbackFactory<COBJ,
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
			)
				{ agent_->triggerAction(theAction,theType); }
    				// Schedules the execution of the registered
				// callback with name <theAction>.
				// The possible values for <theType> are
				// <OTCLIB_PRIORITY_JOB>, OTCLIB_STANDARD_JOB>
				// and <OTCLIB_IDLE_JOB>.

    // Following provide a means of cancelling a CRON style callback or
    // wakeup callback. It will also cancel a simple job style callback
    // if the scheduled callback hasn't yet occurred.

    void		cancelAction(char const* theAction)
    				{ agent_->cancelAction(theAction); }
				// Cancels callback for <theAction>.

    // = SOCKET EVENTS

    template<class T>
    void		subscribeSocket(
			 void (T::*theMethod)() const,
			 int theSocket,
			 int theEvents=OTCLIB_POLLIN
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_SocketCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSocket_(
				   theCallback,theSocket,theEvents);
				}
				// Registers the agent as being interested
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
				   ONS_SocketCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSocket_(
				   theCallback,theSocket,theEvents);
				}
				// Registers the agent as being interested
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
				   ONS_SocketCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSocket_(
				   theCallback,theSocket,theEvents);
				}
				// Registers the agent as being interested
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
				   ONS_SocketCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSocket_(
				   theCallback,theSocket,theEvents);
				}
				// Registers the agent as being interested
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
				   ONS_SocketCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSocket_(
				   theCallback,theSocket,theEvents);
				}
				// Registers the agent as being interested
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
				   ONS_SocketCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent_->subscribeSocket_(
				   theCallback,theSocket,theEvents);
				}
				// Registers the agent as being interested
				// in <theEvents> on <theSocket>. Any
				// notifications will be made by executing
				// <theMethod>.

    void		unsubscribeSocket(int theSocket, int theEvents=~0)
    				{
				  agent_->unsubscribeSocket(
				   theSocket,theEvents);
				}
				// Cancels any interest by the agent in
				// <theEvents> on <theSocket>.

  private:

    			ONS_AgentExtension(ONS_AgentExtension const&);
				// Do not define an implementation for this.

    ONS_AgentExtension&	operator=(ONS_AgentExtension const&);
				// Do not define an implementation for this.

    ONS_Agent*		agent_;
   				// Pointer to the associated agent.

    void		dummyAction_() {}
    				// Dummy method doing nothing. Used as
				// action callback when polling action
				// is requested.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<ONS_AgentExtension*>
{
  public:

    static ONS_AgentExtension*	add(ONS_AgentExtension* theObject)
   				{ theObject->reference(); return theObject; }

    static void		remove(ONS_AgentExtension* theObject)
    				{ theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class ONS_CallbackObject<ONS_AgentExtension>
{
  public:

    static void		acquire(ONS_AgentExtension const* theObject)
    				{ theObject->reference(); }

    static void		release(ONS_AgentExtension const* theObject)
    				{ theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_AGNTEXTN_HH */
