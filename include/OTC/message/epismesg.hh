#ifndef OTC_MESSAGE_EPISMESG_HH
#define OTC_MESSAGE_EPISMESG_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/epismesg.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/episstrm.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/epismesg.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_EPISMessage : public OTC_EPISStream
    // = TITLE
    //     Handles transmission of message events between processes.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_EPISMessage> class handles, via a socket connection,
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

			~OTC_EPISMessage();

    // = CONSTRUCTION

			OTC_EPISMessage(OTC_EndPoint* theEndPoint);
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

    // = INCOMING DATA
    //     Following may be overridden in a derived class implementation if
    //     it should be dealt with some what differently in a specific
    //     implementation.

    virtual bool	processIncoming(OTC_String& theBuffer);
    				// Called when a block of incoming data has
				// been read from the connection and is ready
				// to be processed. Any implementation should
				// consume what data it wants from the buffer
				// and leave in it what it doesn't want to
				// use at this time. If data isn't in expected
				// format <false> should be returned, which
				// will result in socket connection being
				// dropped. Otherwise <true> should be
				// returned if everything okay. By default
				// will deliver an envelope event containing
				// a message event which holds the data, to
				// any receiver bound to the local address of
				// the connection.

    // = OUTGOING DATA

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

			OTC_EPISMessage(OTC_EPISMessage const&);
				// Do not define an implementation for this.

    OTC_EPISMessage&	operator=(OTC_EPISMessage const&);
				// Do not define an implementation for this.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    static char		gMagic_[4];
				// 4 byte prefix placed on messages
				// as a sanity check when reading them
				// in.

    OTC_String		remote_;
				// Remote address.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPISMESG_HH */
