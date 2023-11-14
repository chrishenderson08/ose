#ifndef OTC_DISPATCH_TIMEOUT_HH
#define OTC_DISPATCH_TIMEOUT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/timeout.hh
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
#include <OTC/text/string.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/timeout.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Job;

class OTC_TimeoutSubscription;

class OSE_EXPORT OTCEV_Timeout : public OTC_Event
    // = TITLE
    //     Event object to notify that a timer has expired.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTCEV_Timeout> is a derived version of <OTC_Event> specifically
    //     for notifying agents that a timer has expired.
    //
    // = NOTES
    //     A value of <0> will never be used as a timer ID.
    //
    // = SEE ALSO
    //     <OTC_EVAgent>, <OTC_Event>
{
  public:

			~OTCEV_Timeout();

    // = CONSTRUCTION

			OTCEV_Timeout(
			 int theTimerId,
			 OTC_String const& theDescription,
			 int theAgentData=-1
			)
			  : timerId_(theTimerId),
			    description_(theDescription),
			    agentData_(theAgentData)
				{}
				// Creates a timeout event object for
				// notification that the timer with ID number
				// <theTimerId> has expired. <theDescription>
				// descrives the purpose of the event and
				// might be used by the recipient to work out
				// what to do. <theAgentData> is optional
				// user data supplied by whoever requested
				// the timeout. The normal use for the agent
				// data is so that an ID for the timeout
				// which is relevant within the context of
				// the agent can be supplied.

			OTCEV_Timeout(int theTimerId, int theAgentData=-1)
			  : timerId_(theTimerId), agentData_(theAgentData)
				{}
				// Creates a timeout event object for
				// notification that the timer with ID number
				// <theTimerId> has expired. <theAgentData>
				// is optional user data supplied by whoever
				// requested the timeout. The normal use for
				// the agent data is so that an ID for the
				// timeout which is relevant within the
				// context of the agent can be supplied.
				// The event description will be set to an
				// empty string.

    // = QUERY

    int			timer() const
				{ return timerId_; }
				// Returns the ID number of the timer which
				// expired.

    OTC_String const&	description() const
				{ return description_; }
				// Returns the description of the event.

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

    static int		start(
			 int theAgentId,
			 long thePeriod,
			 char const* theDescription,
			 int theAgentData=-1
			);
				// Registers that the agent identified by ID
				// number <theAgentId> should be notified
				// when a time period of <thePeriod>
				// milliseconds has expired. A timer ID
				// number is returned. The timer ID number
				// can be used to cancel a timer using
				// <cancel()>. If a timer is already
				// scheduled to go off at the same time as
				// this timer will expire, this timer will
				// be expired after the existing timer.
				// <thePeriod> must be a positive value,
				// or an exception will be raised.
				// <theDescription> should describe the
				// purpose of the events so the recipient
				// might now what to do with it.
				// <theAgentData> is an optional data value
				// that can be supplied by whoever is
				// initiating the timeout. This might be used
				// to associate an timeout ID relevant to the
				// context of the agent with the timeout.
				// This would be used instead of the
				// automatically generated timeout ID
				// returned from this function. If not
				// supplied, the agent data will default to
				// <-1>.

    static int		start(
			 int theAgentId,
			 long thePeriod,
			 int theAgentData=-1
			)
				{
				  return start(theAgentId,
				   thePeriod,0,theAgentData);
				}
				// Registers that the agent identified by ID
				// number <theAgentId> should be notified
				// when a time period of <thePeriod>
				// milliseconds has expired. A timer ID
				// number is returned. The timer ID number
				// can be used to cancel a timer using
				// <cancel()>. If a timer is already
				// scheduled to go off at the same time as
				// this timer will expire, this timer will
				// be expired after the existing timer.
				// <thePeriod> must be a positive value,
				// or an exception will be raised.
				// <theAgentData> is an optional data value
				// that can be supplied by whoever is
				// initiating the timeout. This might be used
				// to associate an timeout ID relevant to the
				// context of the agent with the timeout. This
				// would be used instead of the automatically
				// generated timeout ID returned from this
				// function. If not supplied, the agent data
				// will default to <-1>.

    static void		cancel(int theTimerId);
				// Cancels the timer with ID number
				// <theTimerId>.

    static void		cancelAgent(int theAgentId);
				// Cancels any timers in which the agent
				// with ID number <theAgentId> is interested.

    static void         cancelAgent(
			 int theAgentId,
			 char const* theDescription,
			 int theAgentData
			);
                                // Cancels any timeouts which the agent with
                                // ID number <theAgentId> is waiting upon
				// with <theDescription> and where
				// <theAgentData> matches the agent data
				// originally supplied when the timeout was
				// started. Note that the default agent data
				// when not supplied was <-1>. Thus if you
				// mix calls where you do and don't supply
				// agent data and <-1> was a valid value in
				// what you supply, you might cancel timeouts
				// for which you hadn't supplied any agent
				// data.

    static void         cancelAgent(int theAgentId, int theAgentData)
				{ cancelAgent(theAgentId,0,theAgentData); }
                                // Cancels any timeouts which the agent with
                                // ID number <theAgentId> is waiting upon
                                // where <theAgentData> matches the agent
                                // data originally supplied when the timeout
                                // was started. Note that the default
                                // agent data when not supplied was <-1>.
                                // Thus if you mix calls where you do and
                                // don't supply agent data and <-1> was
                                // a valid value in what you supply, you
                                // might cancel timeouts for which you hadn't
                                // supplied any agent data.

    static bool		active(int theTimerId);
				// Returns <true> if the timer with ID number
				// <theTimerId> has not yet expired.

    // = SCHEDULING

    static long		period();
				// Returns <-1> if there are no timers
				// active, otherwise the number of
				// milliseconds until the next timer is due
				// to expire.

    static bool		isPending();
				// Returns <true> if there are pending
				// timeouts.

    static OTC_Job*	pending();
				// Returns a job for the next pending
				// timeout or <0> if no timeouts are
				// pending.

  private:

			OTCEV_Timeout(OTCEV_Timeout const&);
				// Do not define an implementation for this.

    OTCEV_Timeout&	operator=(OTCEV_Timeout const&);
				// Do not define an implementation for this.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static int		globTimerIdCount_;
				// Counter for allocating timer ID.

    static OTC_TimeoutSubscription*	globSubscriptions_;
				// List of subscriptions to timeouts.

    static int		globTypeId_;
				// Anchor for use as a type identifier.

    int			timerId_;
				// The timer which expired.

    OTC_String		description_;
				// The description of the event.

    int			agentData_;
				// The user supplied agent data.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_TIMEOUT_HH */
