#ifndef OTC_DISPATCH_ALARM_HH
#define OTC_DISPATCH_ALARM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/alarm.hh
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

#include <OTC/dispatch/event.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/alarm.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Job;

class OTC_AlarmSubscription;

class OSE_EXPORT OTCEV_Alarm : public OTC_Event
    // = TITLE
    //     Event object to notify of a clock reaching a particular time.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTCEV_Alarm> is a derived version of <OTC_Event> specifically for
    //     notifying agents that a clock has reached a particular time. The
    //     class also provides the interface for registration of interest in
    //     alarms by agents.
    //     
    // = NOTES
    //     Time is measured as the number of seconds since the epoch. In most
    //     systems the epoch is January 1, 1970.
    //     
    //     A value of <0> will never be used as an alarm ID.
    //
    // = SEE ALSO
    //     <OTC_Event>, <OTC_EVAgent>
{
  public:

			~OTCEV_Alarm();

    // = CONSTRUCTION

			OTCEV_Alarm(
			 int theAlarmId,
			 long theTime,
			 OTC_String const& theDescription,
			 int theAgentData=-1
			)
			  : alarmId_(theAlarmId),
			    time_(theTime),
			    description_(theDescription),
			    agentData_(theAgentData)
				{}
				// Creates an event object for the alarm with
				// ID number <theAlarmId>. <theTime> is the
				// time at which the alarm was triggered,
				// measured as the number of seconds since
				// the epoch. <theDescription> is the purpose
				// of the event and would be used by the
				// recipient to work out what to do.
				// <theAgentData> is optional user data
				// supplied by whoever requested the alarm.
				// The normal use for the agent data is so
				// that an ID for the alarm which is relevant
				// within the context of the agent can be
				// supplied.

			OTCEV_Alarm(
			 int theAlarmId,
			 long theTime,
			 int theAgentData=-1
			)
			  : alarmId_(theAlarmId),
			    time_(theTime),
			    agentData_(theAgentData)
				{}
				// Creates an event object for the alarm with
				// ID number <theAlarmId>. <theTime> is the
				// time at which the alarm was triggered,
				// measured as the number of seconds since
				// the epoch. <theAgentData> is optional user
				// data supplied by whoever requested the
				// alarm. The normal use for the agent data
				// is so that an ID for the alarm which is
				// relevant within the context of the agent
				// can be supplied.

    // = QUERY

    int			alarm() const
				{ return alarmId_; }
				// Returns the ID number of the alarm.

    long		time() const
				{ return time_; }
				// Returns the time at which the alarm was
				// scheduled to be triggered, measured as the
				// number of seconds since the epoch.

    OTC_String const&	description() const
				{ return description_; }
				// Returns the description of the event,
				// indicating what it is for.

    int			data() const
				{ return agentData_; }
                                // Returns the optional agent data supplied
                                // by whoever initiated the action.

    // = IDENTIFICATION

    void*		type() const;
				// Returns a value that uniquely identifies
				// this type of event.

    static void*	typeId()
				{ return &globTypeId_; }
				// Returns a value that uniquely identifies
				// this type of event.

    // = DEBUGGING

    void		dump(ostream& outs) const;
				// Dumps info about the event onto <outs>.

    // = SUBSCRIPTION

    static int		set(
			 int theAgentId,
			 long theTime,
			 char const* theDescription,
			 int theAgentData=-1
			);
				// Registers that the agent identified by ID
				// <theAgentId> is interested in being
				// notified when time reaches <theTime>
				// seconds since the epoch. An ID is returned
				// which can be used to cancel the alarm.
				// If an alarm is already registered to
				// expire at <theTime>, the new alarm will be
				// expired after the existing alarm.
				// <theDescription> provides the purpose of
				// the event and can be used by the recipient
				// to work out what to do. <theAgentData> is
				// an optional data value that can be
				// supplied by whoever is initiating the
				// alarm. This might be used to associate an
				// alarm ID relevant to the context of the
				// agent with the alarm. This would be used
				// instead of the automatically generated
				// alarm ID returned from this function. If
				// not supplied, the agent data will default
				// to <-1>.

    static int		set(int theAgentId, long theTime, int theAgentData=-1)
				{
				  return set(theAgentId,
				   theTime,0,theAgentData);
				}
				// Registers that the agent identified by ID
				// <theAgentId> is interested in being
				// notified when time reaches <theTime>
				// seconds since the epoch. An ID is returned
				// which can be used to cancel the alarm.
				// If an alarm is already registered to
				// expire at <theTime>, the new alarm will be
				// expired after the existing alarm.
				// <theAgentData> is an optional data value
                                // that can be supplied by whoever is
                                // initiating the alarm. This might be used
                                // to associate an alarm ID relevant to the
                                // context of the agent with the alarm. This
                                // would be used instead of the automatically
                                // generated alarm ID returned from this
                                // function. If not supplied, the agent data
                                // will default to <-1>.


    static void		cancel(int theAlarmId);
				// Cancels any interest in the alarm
				// with ID <theAlarmId>.

    static void		cancelAgent(int theAgentId);
				// Cancels all alarms that the agent with
				// ID <theAgentId> is interested in.

    static void         cancelAgent(
			 int theAgentId,
			 char const* theDescription,
			 int theAgentData
			);
                                // Cancels any alarms which the agent with
                                // ID number <theAgentId> is waiting upon
				// with <theDescription> and where
				// <theAgentData> matches the agent data
				// originally supplied when the alarm was
				// started. Note that the default agent data
				// when not supplied was <-1>. Thus if you
				// mix calls where you do and don't supply
				// agent data and <-1> was a valid value in
				// what you supply, you might cancel alarms
				// for which you hadn't supplied any agent
				// data.

    static void         cancelAgent(int theAgentId, int theAgentData)
				{ cancelAgent(theAgentId,0,theAgentData); }
                                // Cancels any alarms which the agent with
                                // ID number <theAgentId> is waiting upon
				// where the event description is empty and
                                // where <theAgentData> matches the agent
                                // data originally supplied when the alarm
                                // was started. Note that the default
                                // agent data when not supplied was <-1>.
                                // Thus if you mix calls where you do and
                                // don't supply agent data and <-1> was
                                // a valid value in what you supply, you
                                // might cancel alarms for which you hadn't
                                // supplied any agent data.

    static bool		active(int theAlarmId);
				// Returns <true> if the alarm with ID
				// <theAlarmId> has yet to be triggered.

    // = SCHEDULING

    static long		period();
				// Returns the time in seconds until the
				// next alarm is due to expire.

    static bool		isPending();
				// Returns <true> if there are pending
				// alarms.

    static OTC_Job*	pending();
				// Returns a job for the next pending
				// alarm or <0> if no alarms are pending.

  private:

			OTCEV_Alarm(OTCEV_Alarm const&);
				// Do not define an implementation for this.

    OTCEV_Alarm&	operator=(OTCEV_Alarm const&);
				// Do not define an implementation for this.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static int		globAlarmIdCount_;
				// Counter for allocation of alarm IDs.

    static OTC_AlarmSubscription*	globSubscriptions_;
				// List of agents interested in alarms.

    static int		globTypeId_;
				// Anchor for use as a type identifier.

    int			alarmId_;
				// ID of the alarm which was triggered.

    long		time_;
				// Time, in seconds since the epoch, at
				// which the alarm was triggered

    OTC_String		description_;
				// The description of the event.

    int			agentData_;
				// The user supplied agent data.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_ALARM_HH */
