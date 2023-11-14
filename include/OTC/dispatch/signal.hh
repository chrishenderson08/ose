#ifndef OTC_DISPATCH_SIGNAL_HH
#define OTC_DISPATCH_SIGNAL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/signal.hh
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
#pragma interface "OTC/dispatch/signal.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Job;
class OTC_JobList;

class OTC_SignalSubscription;
class OTC_SignalInfo;

class OSE_EXPORT OTCEV_Signal : public OTC_Event
    // = TITLE
    //     Event object to notify that signal has occurred.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTCEV_Signal> is a derived version of <OTC_Event> specifically
    //     for notifying agents of the occurence of a UNIX signal. The
    //     class also provides the interface for registration of interest
    //     in signals by agents.
    //
    // = SEE ALSO
    //     <OTC_Event>, <OTC_EVAgent>
{
  public:

			~OTCEV_Signal();

    // = CONSTRUCTION

			OTCEV_Signal(int theSignal)
			  : signal_(theSignal)
				{}
				// Creates an event object for <theSignal>.

    // = QUERY

    int			signal() const
				{ return signal_; }
				// Returns the number of the signal that
				// occurred.

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

    // = SUBSCRIPTION

    static void		subscribe(int theAgentId, int theSignal);
				// Registers that the agent with ID number
				// <theAgentId> should be notified when
				// <theSignal> occurs.

    static void		unsubscribe(int theAgentId, int theSignal);
				// Forget that the agent with ID number
				// <theAgentId> is interested in being
				// notified about occurences of <theSignal>.

    static void		unsubscribeAgent(int theAgentId);
				// Forget that the agent identified by
				// <theAgentId> is interested in any signals.

    // = INITIALISATION

    static void		capacity(u_int theCapacity);
				// Sets the capacity of the array used to
				// hold information about pending signals.
				// Must be called before any signal
				// subscriptions are made. If called after
				// signal subscriptions have been made,
				// the call is ignored. If not called at all,
				// the default size of <64> is used. If
				// <theCapacity> is <0> an exception is
				// raised.

    // = SCHEDULING

    static u_int	signalsSubscribed();
				// Returns the number of different signals
				// for which there are subscriptions.

    static bool		isPending();
				// Returns <true> if there is a signal
				// waiting to be handled.

    static OTC_Job*	pending();
				// Returns a job for the next pending
				// signal or <0> if no pending signals
				// exist.

  protected:

    // = NON DELIVERY

    void		cancelSource(int theAgentId);
				// Unsubscribes the agent with ID
				// <theAgentId> to which this event was to be
				// delivered, from all further signals. This
				// function will be called when the agent to
				// which this event was to be delivred does
				// not exist.

  private:

			OTCEV_Signal(OTCEV_Signal const&);
				// Do not define an implementation for this.

    OTCEV_Signal&	operator=(OTCEV_Signal const&);
				// Do not define an implementation for this.

    static void		fill_(OTC_JobList* theJobQueue);
				// Adds jobs for any pending signals to
				// <theJobQueue>.

    static void		sighandler_(int theSignal);
				// Signal handler.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static OTC_SignalSubscription**	gSubscriptions_;
				// List of agents interested in signals.

    static OTC_SignalInfo*	gPendingSignals_;
				// Table of signals which have occurred
				// but which haven't been processed yet.

    static u_int	gPendingSize_;
				// Maximum number of queue signals which
				// can be kept. Actually, it will be more
				// than this as we compress signals and
				// keep a count of the number of times
				// they occur when the signal number
				// matches for sequential signals.

    static u_int	gNextAvailable_;
				// Next available slot in table of pending
				// signals.

    static bool		gSignalsLost_;
				// Set to <true> by signal handler if there
				// was no space in the table to record
				// information about a signal.

    static u_int	gSigCount_;
				// Number of different signals to which
				// agents are subscribed.

    static OTC_JobList*	gSignalJobs_;
				// Pending signal jobs.

    static int		gTypeId_;
				// Anchor for use a type identifier.

    int			signal_;
				// The signal which occurred.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SIGNAL_HH */
