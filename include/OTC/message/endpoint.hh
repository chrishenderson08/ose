#ifndef OTC_MESSAGE_ENDPOINT_HH
#define OTC_MESSAGE_ENDPOINT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/endpoint.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/receiver.hh>
#include <OTC/dispatch/subject.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/text/string.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/hset.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/endpoint.hh"
#endif
#endif

#ifdef interface
#undef interface
#endif

/* ------------------------------------------------------------------------- */

enum OTC_EndPointType
{
  OTCLIB_ENDPOINT_CLIENT = 0,
  OTCLIB_ENDPOINT_SERVER = 1
};

class OTC_EPInterface;

class OSE_EXPORT OTC_EndPoint : public OTC_Resource,
 public OTC_EVAgent, public OTC_Subject
    // = TITLE
    //     Endpoint for messaging link between two processes.
    //
    // = TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_EndPoint> class is used as a base class for classes which
    //     implement an endpoint for a messaging link between two processes.
    //     Specific implementations for different endpoints should not derive
    //     from this class but should instead derive from either of the
    //     classes <OTC_EPServer> or <OTC_EPClient>. The <OTC_EndPoint> class
    //     derives from <OTC_Subject> and thus you can indicate that you want
    //     to be notified of changes in the state of the connection.
    //     
    // = SEE ALSO
    //     <OTC_EPServer>, <OTC_EPClient>, <OTC_EPInterface>, <OTC_Subject>
{
    friend class	OTC_EPInterface;

  public:

    // = ENDPOINT

    OTC_EndPointType	type() const
      				{ return type_; }
				// Returns whether this endpoint is acting as
				// a client or a server by returning either
				// the value <OTCLIB_ENDPOINT_CLIENT> or the
				// value <OTCLIB_ENDPOINT_SERVER> as
				// appropriate.

    OTC_String const&	interface() const
      				{ return interface_; }
				// Returns the interface being used over
				// the transport layer.

    OTC_String const&	protocol() const
      				{ return protocol_; }
				// Returns the type of transport layer
				// being used for the link.

    // = ADDRESSING

    // The following addresses are those used for event or message delivery
    // within the event framework.

    OTC_String const&	localAddress() const
				{ return localAddress_; }
				// Returns local address assigned to this
				// endpoint.

    OTC_String const&	remoteAddress() const
				{ return inBox_.name(); }
				// Returns the remote address of this
				// endpoint. That is, the virtual address of
				// the endpoint at the other end of the
				// connection of which this endpoint is one
				// end. Returns an empty string if no remote
				// address is known because of a connection
				// not being in a state which anything can be
				// sent.

    // The following are those related to the underlying transport layer
    // used to create the link. These are not used in event or message
    // delivery with the event framework and are present for informational
    // value only. One might use the remote transport layer address as
    // a means of rejecting connections from specific hosts when this end
    // point is a server. The values returned should only be regarded as
    // being valid while the endpoint is actually connected.

    virtual OTC_String	localTransport() const = 0;
				// Returns the local address corresponding to
				// the transport layer address.

    virtual OTC_String	remoteTransport() const = 0;
				// Returns the remote address corresponding
				// to the transport layer address.

    // = STATUS

    bool		isConnected() const
				{ return !inBox_.name().isEmpty(); }
				// Returns <true> if this endpoint is one end
				// of an active connection. Whether the
				// connection is active is deduced from
				// whether the remote address is not empty.

    // = CONNECTION

    virtual void	start();
				// If the endpoint is a client and it has
				// lost its connection and reverted to
				// an inactive state, it can be forced to
				// reconnect by calling this function.
				// Will have no effect if the endpoint was
				// a server or if the client was in the
				// process of connecting or was connected.
				// The implementation of this method
				// must be provided by a derived class.

    virtual void	stop(int theDelay=0) = 0;
				// Will shutdown an active connection.
				// Connections are permitted to wait until
				// any pending messages are written out, but
				// the maxmimum amount of time it should wait
				// is given by <theDelay>. <theDelay> is
				// given as milliseconds, with <-1> meaning
				// that the connection should wait until all
				// messages are written out. Note that if the
				// endpoint is a client and the derived class
				// provides a means of automatically
				// restarting the connection, this should
				// also stop it from restarting. The
				// implementation of this method must be
				// provided by a derived class.

    // The following relate to the handling of input from the other end
    // of a connection. There is no way to ask that output to the connection
    // be suspended. The purpose of these functions is specifically to
    // cause input to be throttled so that pending messages can be output
    // without having to deal with any more messages caused by further
    // input. The intent is that message input be throttled when shutting
    // down a process so that pending messages can be handled. Note that
    // if a connection becomes inactive, the suspended status will be
    // removed. Further, input from the connection will only be suspended
    // once the connection has become fully active.

    virtual void	suspendInput();
				// Will suspend handling of input.
				// The implementation of this method
				// must be provided by a derived class.

    virtual void	resumeInput();
				// Will resume handling of input.
				// The implementation of this method
				// must be provided by a derived class.

    // Note that only the following are truly safe to use in a multithread
    // environment. They will call the above functions on each endpoint
    // inside a mutex lock. A derived class should not cause itself to be
    // deleted in response to one of the above functions. If it does it
    // will hang the thread, trying to lock the mutex it already has, but
    // for which recursive locking is not allowed. This is why there is
    // not a <stopAll()> function as for server objects in all probability
    // they will be deleted immediately.

    static void		suspendAllInput();
				// Suspend handling of input for all
				// endpoints.

    static void		resumeAllInput();
				// Resume handling of input for all
				// endpoints.

    // = ENDPOINTS
    //     The operation of these functions is thread safe, however,
    //     accessing the data after the call isn't unless you know
    //     that new endpoints will not be created or existing endpoints
    //     destroyed while using the result.

    static OTC_EndPoint*	local(OTC_String const& theLocalAddress);
				// Returns a pointer to the endpoint with
				// <theLocalAddress>. If no such endpoint
				// exists, <0> will be returned.

    static OTC_EndPoint*	remote(OTC_String const& theRemoteAddress);
				// Returns a pointer to the endpoint with
				// <theRemoteAddress>. If no such endpoint
				// exists, <0> will be returned.

    static OTC_Iterator<OTC_EndPoint*>	endPoints();
				// Returns an iterator over all endpoints.

  protected:

			~OTC_EndPoint();

    // = INITIALISATION

			OTC_EndPoint(
			 OTC_EndPointType theType,
			 char const* theInterface,
			 char const* theProtocol
			);
				// Generates a local address for this
				// endpoint. <theType> should specify
				// whether this endpoint is acting as
				// a client or server. <theProtocol>
				// should identify the type of transport
				// layer being used. <theInterface> the
				// type of interface used to communicate
				// with that transport layer.

    // Note that the interface handler object must be created on the free
    // store as this class will take ownership of it and will delete it when
    // this class is destroyed.

    void		attach(OTC_EPInterface* theInterface);
				// Set the interface handler for this
				// endpoint.

    // = REMOTE ADDRESS

    void		bindRemoteAddress(char const* theRemoteAddress);
				// Binds the remote address for this
				// endpoint. Calling this function
				// effectively says that this endpoint is now
				// one end of an active connection. Raises an
				// exception if <theRemoteAddress> is <0> or
				// contains only whitespace or is empty.

    void		unbindRemoteAddress();
				// Unbinds the remote address for this
				// endpoint. Calling this function
				// effectively says that this endpoint is no
				// longer one end of an active connection.

    // = EVENT CALLBACKS

    void		accept(OTC_Event* theEvent);
				// Looks for <OTCEV_Envelope> events
				// and delivers them to the interface
				// object. The envelope events should be
				// the messages to be sent over the
				// connection. Other events are passed down
				// to the base class <accept()> function.

    // = INTERNAL RESTART

    virtual void*	shutdown(void* theHandle, int theError) = 0;
				// Must be redefined in a derived class to
				// perform whatever actions should be
				// performed in the event of loss of the
				// connection being used. <shutdown()> will be
				// called with a handle to some data relevant
				// to the existing connection. <theError>
				// will be set to <0> if the remote end
				// shutdown normally, or a copy of <errno> if
				// an error occured. It is the job of the
				// derived class to at least close down and
				// reclaim any resources relevant to the
				// existing connection. The derived class
				// function may create a new connection and
				// pass back a pointer to relevant data for
				// the new connection. If this is not done,
				// <0> should be returned. In the event that
				// no new connection was created, the derived
				// class may delete itself. Exactly what
				// <theHandle> points to is specific to
				// the type of connection.

    // = INTERFACE

    OTC_EPInterface*	handler() const
				{ return handler_; }
				// Returns pointer to interface handler for
				// this endpoint connection.

  private:

			OTC_EndPoint(OTC_EndPoint const&);
				// Do not define an implementation for this.

    OTC_EndPoint&	operator=(OTC_EndPoint const&);
				// Do not define an implementation for this.

    void		init_();
				// Initialise process identifier.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    static OTC_HMap<OTC_String,OTC_EndPoint*>*	gLocal_;
				// Index onto endpoints by local address.

    static OTC_HMap<OTC_String,OTC_EndPoint*>*	gRemote_;
				// Index onto endpoints by remote address.

    OTC_EndPointType	type_;
    				// The type of endpoint.

    OTC_String		interface_;
    				// Handler for transport layer.

    OTC_String		protocol_;
    				// Transport layer protocol.

    OTC_String		localAddress_;
				// Local address of this endpoint.

    OTC_Receiver	inBox_;
				// Inbox for routing of messages to
				// remote address through this endpoint.

    OTC_EPInterface*	handler_;
				// The protocol handler for the specific
				// type of message link between the
				// endpoints.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_ENDPOINT_HH */
