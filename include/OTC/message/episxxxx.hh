#ifndef OTC_MESSAGE_EPISXXXX_HH
#define OTC_MESSAGE_EPISXXXX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/episxxxx.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/episockt.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/episxxxx.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_LinkList;
class OTCEV_Envelope;

enum OTC_EPISMeSsAgEState
{
  OTCLIB_EPISMESSAGE_INACTIVE,
  OTCLIB_EPISMESSAGE_STARTING,
  OTCLIB_EPISMESSAGE_WAITING,
  OTCLIB_EPISMESSAGE_CONNECTED
};

class OSE_EXPORT OTC_EPISMeSsAgE : public OTC_EPISocket
    // = TITLE
    //     Handles transmission of message events between processes.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_EPISMeSsAgE> class handles, via a socket connection,
    //     the transmission of message events between processes. The class is
    //     used in pairs, one instance at either end of the socket
    //     connection. When an instance of the class is created, a local
    //     address is created encompassing an instance number, the process id
    //     and the machine name. The local address will be passed to the
    //     instance at the other end of the socket connection. That instance
    //     will register the local address as an agent address for its end.
    //     
    //     The class will send over the connection any message events it is
    //     passed for which the sequence number is <0>, or for which the head
    //     of the path in the destination address matches the address of the
    //     agent at the opposite end of the socket connection. Ie., matches
    //     the remote address.
    //
    //     When message events are being queued due to no connection
    //     existing, or due to a backlog of messages waiting to be written
    //     out, the message events will be filtered out at the time they
    //     come to be written out to the file descriptor. This is done
    //     to prevent writing out a message event which was originally
    //     addressed to the remote address but where the remote end had
    //     died and a new connection started, between the time of the initial
    //     request to send the message event and the time the message event
    //     came to be sent over the socket connection. If not filtered, it
    //     would more than likely be the case that the remote end would get a
    //     message it didn't know what to do with or wasn't expecting.
    //     
    //     When a message event is sent over the connection, the instance at
    //     the other end will massage the return address so as to extend the
    //     path to include the local address at the front. This will result in
    //     a reply being routed back along the same path it was sent. In the
    //     event of the message event being a reply, the destination address
    //     will also be massaged to drop the local address appearing as the
    //     initial path component.
    //     
    //     If multiple messages get queued up due to the file descriptor
    //     being blocked, when the file descriptor once becomes available
    //     again, they will not all be written out in one go. Instead,
    //     a maximum number will be written out and the control returned
    //     to the dispatcher. An event will be scheduled so that control
    //     will come back to this class to deliver the remainder. This is
    //     done so as to allow other agents to run when there becomes a
    //     backlog of messages. By default the maximum delivered in one
    //     go is <32>. This can be overridden using the environment
    //     variable <OTCLIB_MESSAGEBATCH>.
    //     
    // = NOTES
    //     If the file descriptor you provide is set for non blocking I/O,
    //     you MUST use the POSIX method for setting it to be non blocking.
    //     That is, you MUST use O_NONBLOCK. The behaviour of this class is
    //     undefined when a non blocking file descriptor is used and either
    //     the 4.2 BSD or SYS5 methods were used to define the file
    //     descriptor as being non blocking.
    //
    // = SEE ALSO
    //     <OTC_Event>, <OTCEV_Envelope>, <OTCEV_Message>, <OTC_EPISocket>
{
  public:

			~OTC_EPISMeSsAgE();

    // = CONSTRUCTION

			OTC_EPISMeSsAgE(OTC_EndPoint* theEndPoint);
				// Initialises this end of the link.
				// If the object is the server end, it will
				// always wait for the initial address setup
				// message before sending off its own address
				// setup message. That is, the client object
				// will initiate the exchange. Note that it
				// is still necessary to provide a file
				// descriptor to be used for the connection
				// via the <attach()> member function. The
				// <attach()> member function may be called
				// from a derived class constructor.

    // = QUERY

    int			fd() const
				{ return fd_; }
				// Returns the file descriptor on which
				// messages are being sent/received. Will
				// return <-1> if there is connection.
				// Note that return a value not equal
				// to <-1> does not indicate an active
				// connection as an active connection
				// could still be getting negotiated.

    // = CONNECTION

    void		attach(int theFd);
				// Defines that <theFd> should be used
				// as the socket connection. Will raise
				// an exception if there is already an
				// active connection. This function will
				// generate an event such that the class
				// will be called back so as to be able to
				// deliver the initial address control
				// messages and any queued messages waiting
				// for delivery.

    int			detach();
				// Detaches this object from the file
				// descriptor in use for the current active
				// connection. Raises an exception if this
				// object wasn't currently attached to any
				// file descriptor. Use <fd()> to check that
				// file descriptor is not <-1> before calling
				// this function. Note that this function
				// doesn't call <close()> on the file
				// descriptor. The caller should close
				// the file descriptor either just before or
				// after calling this function. Returns the
				// file descriptor which was being used.

    void                suspendInput();
                                // Will suspend handling of input.

    void                resumeInput();
                                // Will resume handling of input.

    // = EVENT CALLBACK

    void		handle(OTC_Event* theEvent);
				// Sends messages from this process over
				// the socket connection and reads messages
				// coming from the socket connection and
				// distributes them locally if possible.
				// If a failure of the connection occurs, the
				// <shutdown()> function will be called.
				// Prior to calling <shutdown()> this function
				// will unsubscribe from the file descriptor,
				// set the held file descriptor to <-1>,
				// remove the association between the remote
				// address and this class, and put this class
				// into a sleep state. If the <shutdown()>
				// function returns a value greater than or
				// equal to <0>, the value will be
				// interpreted as being a new file descriptor
				// to use for the connection and this class
				// will be woken up again. If the returned
				// value is <-1>, this function will assume
				// that the file descriptor has at the least
				// been closed by calling the system
				// <close()> function. In the event of <-1>
				// being returned this function will not do
				// anything else in case the derived class
				// <shutdown()> function deleted the instance
				// of the class.

    // = EVENT DELIVERY
    //     The following methods define in what manner events are delivered
    //     corresponding to the state of the connection. That is, that the
    //     connection has become active or inactive, or that congestion has
    //     occurred on the connection. By default, corresponding events
    //     notifying of the condition are distributed to any observers of
    //     the associated endpoint. These methods may be overridden in a
    //     derived class implementation if they should be dealt with some
    //     what differently in a specific implementation.

    virtual void	connectionActive();
				// Called when an initial attempt to setup
				// the connection succeeds. By default
				// generates an appropriate <OTCEV_Connection>
				// event to observers indicating the
				// connection has become active.

    virtual void	connectionInactive();
    				// Called when an active connection is closed
				// or is otherwise shutdown. By default
				// generates an appropriate <OTCEV_Connection>
				// event to observers indicating the
				// connection has become inactive.

    // The following methods all use <outgoingCount()> to determine the
    // number of blocks of data still to be written onto the socket and pass
    // that along with the congestion event to observers.

    virtual void	congestionBlocked();
    				// Called when no more data can be written
				// onto the connection. That is, an attempt
				// to write data onto the connection would
				// cause it to block. By default generates
				// an appropriate <OTCEV_Congestion> event
				// to observers.

    virtual void	congestionBacklog();
				// Called at different points while a
				// connection is still blocked to indicate
				// how much data is still to be written onto
				// the connection.

    virtual void	congestionCleared();
				// Called when a previously blocked
				// connection has cleared. That is, more data
				// can be written onto the connection.

    // = OUTGOING DATA

    void		queueOutgoing(OTCEV_Envelope* theEnvelope);
				// Adds message defined by <theEnvelope> to
				// queue of outgoing messages and triggers
				// sending of message if queue is currently
				// empty.

    // Following may be overridden in a derived class implementation if it
    // should be dealt with some what differently in a specific
    // implementation.

    virtual bool	formatOutgoing(
    			 OTCEV_Envelope* theEnvelope,
			 OTC_String& theBuffer
			);
				// Called when message contained within
				// <theEnvelope> needs to be formatted into
				// <theBuffer> for sending over the socket
				// connection. The message will be appended
				// to <theBuffer>. Will return <false> if a
				// message could not be constructed or <true>
				// if successful.

  private:

			OTC_EPISMeSsAgE(OTC_EPISMeSsAgE const&);
				// Do not define an implementation for this.

    OTC_EPISMeSsAgE&	operator=(OTC_EPISMeSsAgE const&);
				// Do not define an implementation for this.

    OTCEV_Envelope*	unpack_();
				// Unpacks the contents of <imessage_>.
				// Returns <0> if unsuccessful.

    void		clearOutgoing_();
    				// Clears the queue of pending outgoing
				// messages and resets congestion counts.

    void		messageQueued_();
				// Called when a message is queued in order
				// to do processing for congestion management.

    void		messageSent_();
				// Called when a message is sent in order to
				// do processing for congestion management.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    static char		gMagic_[4];
				// 4 byte prefix placed on messages
				// as a sanity check when reading them
				// in.

    static u_int	gBatch_;
				// Maximum number of messages to write
				// out in one go without going back to
				// the dispatcher.

    static u_int	gThreshold_;
				// Threshold count used in determining if a
				// backlog is arising.

    OTC_EPISMeSsAgEState	state_;
                                // The state of the tcp stream.

    int			fd_;
				// File descriptor for connection.

    OTC_String		remote_;
				// Remote address.

    OTC_LinkList*	outgoing_;
				// List of outgoing events waiting to
				// be delivered.

    size_t		outgoingCount_;
				// Count of how many outgoing messages
				// there currently are.

    u_int		backlogCount_;
				// Once a backlog has been detected, this
				// tracks how many messages sent and received
				// while backlogged.

    bool		waitingToWrite_;
				// Set to <true> if we are waiting for
				// notification that we can write something
				// out on the file descriptor. Only used when
				// the file descriptor is non blocking.

    int			iphase_;
				// What is currently happening with respect
				// to reading in of messages. Has value of
				// <0> if not doing anything. Has value <1>
				// if reading in message.

    int			ilength_;
				// Count of how much has yet to be read
				// in of message.

    OTC_String		imessage_;
				// This is where the message is read in.

    int			olength_;
				// Count of how much has yet to be sent
				// out of a message when dealing with
				// non blocking file descriptors.

    OTC_String		omessage_;
				// This is where the message to be
				// written out.

    bool		isuspended_;
				// Set to <true> if message input is
				// suspended.

    bool		stopping_;
				// Set to <true> if connection is to be
				// stopped once the output queue is
				// empty.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPISXXXX_HH */
