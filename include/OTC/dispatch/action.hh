#ifndef OTC_DISPATCH_ACTION_HH
#define OTC_DISPATCH_ACTION_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/action.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/hashtble.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/thread/mutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/action.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_LinkList;
class OTC_ActionJob;
class OTC_ActionCronJob;
class OTC_EVAgent;

enum OTC_ActionType
{
  OTCLIB_ACTION_JOB,
  OTCLIB_ACTION_CRON
};

class OSE_EXPORT OTCEV_Action : public OTC_Event
    // = TITLE
    //     Event object to notify that it is time to perform some action.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTCEV_Action> is a derived version of <OTC_Event> for notifying
    //     agents that it should now perform whatever action that it has been
    //     waiting to perform. The event can be scheduled for delivery as
    //     a job on the dispatcher job queue immediately, or at a later time
    //     using a scheduling specification like that for the UNIX cron
    //     program.
    //
    //     The specification when scheduling in advance consists of five
    //     fields. The fields are separated by white space. The fields
    //     specify:
    //
    // = BEGIN<NOFILL>
    //       minute (0-59),
    //       hour (0-23),
    //       day of the month (1-31),
    //       month of the year (1-12),
    //       day of the week (0-6 with 0=Sunday).
    // = END<NOFILL>
    //
    //     A field may be an asterisk "*", which always stands for
    //     "first-last".
    //     
    //     Ranges of numbers are allowed. Ranges are two numbers separated
    //     with a hyphen. The specified range is inclusive. For example, 8-11
    //     for an "hours" entry specifies execution at hours 8, 9, 10 and 11.
    //     
    //     Lists are allowed. A list is a set of numbers (or ranges)
    //     separated by commas. Examples: "1,2,5,9", "0-4,8-12".
    //     
    //     Step values can be used in conjunction with ranges. Following a
    //     range with "/number" specifies skips of the number's value through
    //     the range. For example, "0-23/2" can be used in the hours field to
    //     specify command execution every other hour Steps are also
    //     permitted after an asterisk, so if you want to say "every two
    //     hours", just use "*/2".
    //     
    //     Names can also be used for the "month" and "day of week" fields.
    //     Use the first three letters of the particular day or month (lower
    //     case, or first letter only uppercase).
    //     
    //     Note: The day of a command's execution can be specified by two
    //     fields - day of month, and day of week. If both fields are
    //     restricted (ie, aren't *), the command will be run when either
    //     field matches the current time. For example, "30 4 1,15 * 5" would
    //     cause a command to be run at 4:30 am on the 1st and 15th of each
    //     month, plus every Friday.
    //
    // = EXAMPLE
    //     Following would deliver the event <OTCEV_Action> to the agent
    //     on the hour. It would continue to do this until the scheduled
    //     action was cancelled.
    //
    // = BEGIN<CODE>
    //       OTCEV_Action::schedule(theAgentId,"0 * * * *");
    // = END<CODE>
    //
    // = NOTES
    //     A value of <0> will never be used as an action ID. This value
    //     is returned however when a specification defining when actions
    //     are to be delivered is invalid.
    //     
    // = SEE ALSO
    //     <OTC_EVAgent>, <OTC_Event>
{
    friend class	OTC_ActionJob;

  public:

			~OTCEV_Action();

    // = CONSTRUCTION

			OTCEV_Action(
			 OTC_ActionType theActionType,
			 int theActionId,
			 OTC_String const& theDescription,
			 int theAgentData=-1
			)
			  : actionType_(theActionType),
			    actionId_(theActionId),
			    description_(theDescription),
			    agentData_(theAgentData)
				{}
				// Creates an event object for notifying an
				// agent that the action given by
				// <theActionId> can now be performed.
				// <theDescription> is provided by whoever
				// initiated the action as a means for
				// the recipient to know why they are
				// receving the event. <theAgentData> is an
				// optional value used so that an ID for the
				// action which is relevant within the
				// context of the agent can be supplied.
				// <theActionType> is provided to indicate
				// whether event was due to one of job
				// scheduling or as the result of a cron
				// specification.

			OTCEV_Action(
			 OTC_ActionType theActionType,
			 int theActionId,
			 int theAgentData=-1
			)
			  : actionType_(theActionType),
			    actionId_(theActionId),
			    agentData_(theAgentData)
				{}
				// Creates an event object for notifying an
				// agent that the action given by
				// <theActionId> can now be performed.
				// <theAgentData> is an optional value used
				// so that an ID for the action which is
				// relevant within the context of the agent
				// can be supplied. The description associated
				// with the event is set to be empty.
				// <theActionType> is provided to indicate
				// whether event was due to one of job
				// scheduling or as the result of a cron
				// specification.

    // = QUERY

    OTC_ActionType	actionType() const
				{ return actionType_; }
				// Returns <OTCLIB_ACTION_JOB> if a one of
				// schedule action, or <OTCLIB_ACTION_CRON>
				// if the result of a cron specification.

    int			action() const
				{ return actionId_; }
				// Returns the ID number of the action which
				// should now be performed.

    OTC_String const&	description() const
				{ return description_; }
				// Returns the description of the event
				// provided when it was scheduled.

    int			data() const
				{ return agentData_; }
				// Returns the optional agent data supplied
				// by whoever initiated the action.

    // = IDENTIFICATION

    void*		type() const;
				// Returns a value that uniquely identifies
				// this type of event.

    static void*	typeId()
				{ return &gTypeId_; }
				// Returns a value that uniquely identifies
				// this type of event.

    // = DEBUGGING

    void		dump(ostream& outs) const;
				// Dumps info about the event onto <outs>.

    // = SCHEDULING

    static int		schedule(
			 int theAgentId,
			 int theType,
			 char const* theDescription,
			 int theAgentData=-1
			);
				// Registers that the agent with ID number
				// <theAgentId> wishes to perform an action.
				// The identifier for the action is returned.
				// <theDescription> should define the
				// purpose of the event so the recipient
				// knows what to do. <theType> defines
				// whether <theJob> is an idle job, standard
				// job, or priority job. Because of the
				// member function overloadings, you have to
				// explicitly specify the ype of job. To
				// define <theJob> as being an idle job use
				// <OTCLIB_IDLE_JOB> and for a priority job,
				// use <OTCLIB_PRIORITY_JOB>. <theAgentData>
				// is an optional data value that can be
				// supplied by whoever is initiating the
				// action. This might be used to associate an
				// action ID relevant to the context of the
				// agent with the action. This would be used
				// instead of the automatically generated
				// action ID returned from this function. If
				// not supplied, the agent data will default
				// to <-1>.

    static int		schedule(
			 int theAgentId,
			 int theType=OTCLIB_STANDARD_JOB,
			 int theAgentData=-1
			)
				{
				  return schedule(theAgentId,
				   theType,0,theAgentData);
				}
				// Registers that the agent with ID number
				// <theAgentId> wishes to perform an action.
				// The identifier for the action is returned.
				// <theType> defines whether <theJob> is an
				// idle job, standard job, or priority job.
				// The default value for <theType> is
				// <OTCLIB_STANDARD_JOB>. To define <theJob>
				// as being an idle job use <OTCLIB_IDLE_JOB>
				// and for a priority job, use
				// <OTCLIB_PRIORITY_JOB>. <theAgentData>
				// is an optional data value that can be
				// supplied by whoever is initiating the
				// action. This might be used to associate
				// an action ID relevant to the context
				// of the agent with the action. This would
				// be used instead of the automatically
				// generated action ID returned from this
				// function. If not supplied, the agent
				// data will default to <-1>. The
				// description associated with the event
				// will be set to be an empty string.

    static int		schedule(
			 int theAgentId,
			 char const* theSpecification,
			 char const* theDescription,
			 int theAgentData=-1
			);
				// Registers that the agent with ID number
				// <theAgentId> wishes to perform a regular
				// action. <theDescription> gives the purpose
				// of the event so the recipient knows what
				// to do with it. When this action should
				// occur is dictated by <theSpecification>
				// which is of the form used to define when
				// jobs should be run by the UNIX cron
				// program. <theAgentData> is an optional
				// data value that can be supplied by whoever
				// is initiating the action. This might be
				// used to associate an action ID relevant to
				// the context of the agent with the action.
				// This would be used instead of the
				// automatically generated action ID returned
				// from this function. If not supplied, the
				// agent data will default to <-1>. The
				// identifier for the action is returned and
				// should be used to cancel it when required.
				// This will be necessary as request will not
				// be cancel the first time it occurs but
				// will keep occuring when ever the
				// requirements of the specification is met.
				// If the specification is invalid, a value
				// of <0> is returned and not subscription
				// will be recorded.

    static int		schedule(
			 int theAgentId,
			 char const* theSpecification,
			 int theAgentData=-1
			)
				{
				  return schedule(theAgentId,
				   theSpecification,0,theAgentData);
				}
				// Registers that the agent with ID number
				// <theAgentId> wishes to perform a regular
				// action. When this action should occur is
				// dictated by <theSpecification> which is
				// of the form used to define when jobs
				// should be run by the UNIX cron program.
				// <theAgentData> is an optional data value
				// that can be supplied by whoever is
				// initiating the action. This might be used
				// to associate an action ID relevant to the
				// context of the agent with the action. This
				// would be used instead of the automatically
				// generated action ID returned from this
				// function. If not supplied, the agent data
				// will default to <-1>. The identifier for
				// the action is returned and should be used
				// to cancel it when required. This will be
				// necessary as request will not be cancel
				// the first time it occurs but will keep
				// occuring when ever the requirements of the
				// specification is met. If the specification
				// is invalid, a value of <0> is returned
				// and not subscription will be recorded.
				// The description associated with the
				// event will be set to be an empty string.

    static void		cancel(int theActionId);
				// Cancels the action with ID number
				// <theActionId>.

    static void		cancelAgent(int theAgentId);
				// Cancels any actions which the agent with
				// ID number <theAgentId> is waiting upon.

    static void		cancelAgent(
			 int theAgentId,
			 char const* theDescription,
			 int theAgentData
			);
				// Cancels any actions which the agent with
				// ID number <theAgentId> is waiting upon
				// with <theDescription> and where
				// <theAgentData> matches the agent data
				// originally supplied when the action was
				// scheduled. Note that the default agent
				// data when not supplied was <-1>. Thus if
				// you mix calls where you do and don't
				// supply agent data and <-1> was a valid
				// value in what you supply, you might cancel
				// actions for which you hadn't supplied any
				// agent data.

    static void		cancelAgent(
			 int theAgentId,
			 char const* theDescription
			)
				{ cancelAgent(theAgentId,theDescription,-1); }
				// Cancels any actions which the agent with
				// ID number <theAgentId> is waiting upon
				// with <theDescription>.

    static void		cancelAgent(int theAgentId, int theAgentData)
				{ cancelAgent(theAgentId,0,theAgentData); }
				// Cancels any actions which the agent with
				// ID number <theAgentId> is waiting upon
				// where the event description is empty and
				// where <theAgentData> matches the agent
				// data originally supplied when the action
				// was scheduled. Note that the default
				// agent data when not supplied was <-1>.
				// Thus if you mix calls where you do and
				// don't supply agent data and <-1> was
				// a valid value in what you supply, you
				// might cancel actions for which you hadn't
				// supplied any agent data.

    static bool		active(int theActionId);
				// Returns <true> if the action with ID
				// number <theTimerId> has not yet been
				// performed.

    // = TESTING

    static bool		matches(char const* theDescription, time_t theTime);
				// Returns <true> if the cron type
				// specification given by <theDescription>
				// would execute at <theTime>.

  protected:

    // = NON DELIVERY

    void		cancelSource(int theAgentId);
				// Cancels any further actions to be sent
				// to <theAgentId>.

  private:

			OTCEV_Action(OTCEV_Action const&);
				// Do not define an implementation for this.

    OTCEV_Action&	operator=(OTCEV_Action const&);
				// Do not define an implementation for this.

    static OTC_ActionCronJob*	parse_(
			 int theAgentId,
			 char const* theSpecification,
			 int theAgentData
			);
				// Parses <theSpecification> and determines
				// when the instance of this class should
				// be delivered to the agent with ID of
				// <theAgentId>. If the specification is
				// valid returns a class instance detailing
				// the schedule, who to deliver the event
				// to and what <theAgentData> should be.
				// Returns <0> if <theSpecification> was
				// invalid.

    static void		handle_(OTC_Event* theEvent, int);
				// Call back for alarm events to trigger
				// execution of scheduled actions.

    static OTC_Mutex	mutex_;
				// Lock for threads.

    static int		gActionIdCount_;
				// Counter for allocating action ID.

    static OTC_LinkList*	gSubscriptions_;
				// List of actions pending.

    static OTC_LinkList*	gCronJobs_;
				// List of cron jobs.

    static OTC_HashTable*	gCronIndex_;
				// Index onto cron jobs by action ID.

    static OTC_EVAgent*	gAgent_;
				// Agent handling scheduling alarms.

    static time_t	gLastTime_;
				// Last time that we checked to see
				// if any cron jobs should be run.

    static int		gTypeId_;
				// Anchor for use as a type identifier.

    OTC_ActionType	actionType_;
				// Whether a single one of job or a cron job.

    int			actionId_;
				// The timer which expired.

    OTC_String		description_;
				// Describes what the event is for.

    int			agentData_;
				// User supplied agent data.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_ACTION_HH */
