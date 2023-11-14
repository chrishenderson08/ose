#ifndef OTC_DISPATCH_IOEVENT_HH
#define OTC_DISPATCH_IOEVENT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/ioevent.hh
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
#include <OTC/thread/mutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/ioevent.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_IOEventMask
{
  OTCLIB_POLLIN = 0x0001,
  OTCLIB_POLLPRI = 0x0002,
  OTCLIB_POLLOUT = 0x0004,
  OTCLIB_POLLERR = 0x0008,
  OTCLIB_POLLHUP = 0x0010,
  OTCLIB_POLLNVAL = 0x0020
};

class OTC_Job;
class OTC_LinkList;
class OTC_HashTable;
class OTC_IOAgentSubscription;

class OSE_EXPORT OTC_IOSubscription
    // = TITLE
    //     Records what events are of interest for a file descriptor.
{
  public:
    int			fd;
    int			events;
};

class OSE_EXPORT OTCEV_IOEvent : public OTC_Event
    // = TITLE
    //     Event object to notify of pending conditions on a file descriptor.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTCEV_IOEvent> is a derived version of <OTC_Event> specifically
    //     for notifying agents of pending conditions on a file descriptor.
    //
    // = NOTES
    //     There is a maximum number of subscriptions which this class will
    //     handle. On a Win32 system, that limit is the size of <FD_SETSIZE>.
    //     On a UNIX system, if <select()> is used, the limit will also be
    //     <FD_SETSIZE>. If <poll()> is used, the limit will be the same as
    //     the largest file descriptor the operating system supports. Note
    //     that on Win32 systems, the value of <FD_SETSIZE> is <64>. If you
    //     want to be able to subscribe to more file descriptors than that
    //     value, the library should be compiled with <FD_SETSIZE> being set
    //     to a larger value.
    //     
    // = SEE ALSO
    //     <OTC_Event>, <OTC_EVAgent>
{
  public:

			~OTCEV_IOEvent();

    // = CONSTRUCTION

			OTCEV_IOEvent(
			 int theFd,
			 int theEvents,
			 int theAgentId=0
			);
				// Creates an event object indicating that
				// <theEvents> are pending on file descriptor
				// <theFd>. If <theAgentId> is supplied, it
				// should be the agent ID of the agent which
				// had subscribed to this type of IO event.
				// When <theAgentId> is not <0>, the
				// subscription to the events this object
				// notifies of, will be suspended. The
				// subscription will be reinstated when
				// this event object has been destroyed.
				// This means that the event object must be
				// destroyed as soon as you have finished
				// handling the event. Failure to destroy
				// the event means the subscription will
				// not be reinstated and you will no longer
				// receive these types of events. If the
				// subscription had actually been removed
				// by the time this event is destroyed, the
				// subscription will not be reinstated.
				// While the subscription is suspended, no
				// other agent will be able to subscribe
				// to the same file descriptor. Another
				// agent can only subscribe to the same
				// file descriptor when there is no
				// subscription for the file descriptor.

    // = QUERY

    int			events() const
				{ return events_; }
				// Returns the events pending on the file
				// descriptor.

    int			fd() const
				{ return fd_; }
				// Returns the number of the file descriptor.

    static int		agent(int theFd);
				// Returns the ID of the agent subscribed to
				// <theFd> or <0> if no agent is subscribed
				// to that file descriptor.

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
    //     In the following functions, <theEvents> is a bit mask formed from
    //     a bitwise AND of the individual events that are of interest.
    //     Individual events which you may subscribe to are: input, output
    //     and priority input. These are selected using the variables:
    //     <OTCLIB_POLLIN>, <OTCLIB_POLLOUT> and <OTCLIB_POLLPRI>.

    static void		subscribe(int theAgentId, int theFd, int theEvents);
				// Registers that the agent with ID number
				// <theAgentId>, is interested in being
				// notified, when <theEvents> are pending on
				// the file descriptor <theFd>. If another
				// agent is already subscribed to <theFd>,
				// an exception is raised. If the agent is
				// already subscribed to the file descriptor,
				// the set of events will be added to those
				// already subscribed to.

    static void		unsubscribe(
			 int theAgentId,
			 int theFd,
			 int theEvents=~0
			);
				// Cancels interest by the agent with ID
				// number <theAgentId> in <theEvents> on the
				// file descriptor <theFd>. If <theEvents>
				// is <~0>, it cancels interest in any events
				// on that file descriptor by that agent.

    static void		unsubscribeFd(int theFd);
				// Cancels the subscription of any agent
				// subscribed to <theFd> has.

    static void		unsubscribeAgent(int theAgentId);
				// Cancels all interest that the agent with
				// ID number <theAgentId>, has in any file
				// descriptors.

    // = SCHEDULING

    static OTC_Job*	job(int theFd, int theEvents);
				// Returns a job for delivery to the agent
				// interested in <theFd> of <theEvents>.
				// Returns <0> if there is no subscription
				// to that file descriptor.

    static int		events(int theFd);
				// Returns the set of events on the file
				// descriptor <theFd>, in which any agents
				// are interested.

#if defined(OSE_SYS_UNIX)

    static int		maxFd()
				{
				  return globAgentSubscriptions_ == 0
				   ? -1 : globMaxFd_;
				}
				// Returns the highest numbered file
				// descriptor in which any agent has been
				// interested. Ie., it creaps up and never
				// comes down. Returns <-1> if there are no
				// agents interested in any file descriptors.
				// This function is only really usable on
				// UNIX where file descriptors are small
				// numbers with a known upper bound, thus it
				// is not available on Win32. In reality,
				// this is only kept at all so the
				// <OTK_JobQueue> class doesn't have to be
				// rewritten.

#endif

    static int		maxSubscriptions();
				// Returns the maximum number of file
				// descriptors which can be subscribed to.

    static int		numSubscriptions();
				// Returns the current number of
				// subscriptions.

    static int		subscriptions(OTC_IOSubscription* theSubscriptions);
				// Fills the array pointed to by
				// <theSubscriptions> with details of file
				// descriptors subscribed to and events of
				// interest. Details will appear at the start
				// of the array. The number of entries filled
				// is returned. Note that the array must
				// have length at least that given by
				// <maxSubscriptions()>.

  protected:

    // = NON DELIVERY

    void		cancelSource(int theAgentId);
				// Unsubscribes the agent with ID
				// <theAgentId> to which this event was to be
				// delivered, from all file descriptor
				// events. This function will be called when
				// the agent to which this event was to be
				// delivered does not exist.

  private:

			OTCEV_IOEvent(OTCEV_IOEvent const&);
				// Do not define an implementation for this.

    OTCEV_IOEvent&	operator=(OTCEV_IOEvent const&);
				// Do not define an implementation for this.

    static int		nfds_();
				// Returns the highest numbered file
				// descriptor the operating system can
				// manage.

    static bool		validFd_(int theFd);
				// Returns <true> if <theFd> is a valid file
				// descriptor.

    static void		suspendSubscription_(
			 int theAgentId,
			 int theFd,
			 int theEvents
			);
				// Suspends the subscription by <theAgentId>
				// to <theEvents> on <theFd>. Will have no
				// affect if the specified agent is not
				// subscribed to the file descriptor.

    static void		resumeSubscription_(
			 int theAgentId,
			 int theFd,
			 int theEvents
			);
				// Resumes the subscription by <theAgentId>
				// to <theEvents> on <theFd>. Will have no
				// affect if the specified agent is not
				// subscribed to the file descriptor.

    static OTC_Mutex	mutex_;
				// Lock for threads.

    static int		globMaxFd_;
				// Highest numbered file descriptor that an
				// agent is interested in. Only maintained
				// for backwards compatability on UNIX
				// systems.

    static OTC_LinkList*	globAgentSubscriptions_;
				// List of subscriptions by agents.

    static OTC_HashTable*	globSubscriptionIndex_;
				// Index into agent subscriptions by
				// file descriptor.

    static int		globTypeId_;
				// Anchor for use as type idenitifier.

    int			fd_;
				// File descriptor which event has occurred on.

    int			events_;
				// Events which occurred on file descriptor.

    int			agentId_;
				// Agent which was subscribe to the event.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_IOEVENT_HH */
