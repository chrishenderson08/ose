#ifndef OTC_MESSAGE_EPINTRFC_HH
#define OTC_MESSAGE_EPINTRFC_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/epintrfc.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/endpoint.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/epintrfc.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_EPInterface : public OTC_EVAgent
    // = TITLE
    //     Base class for endpoint interface object.
    //
    // = TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_EPInterface> class is used as a base class for classes
    //     which implement a specific protocol over some medium to transfer
    //     data between two endpoints.
    //
    // = SEE ALSO
    //     <OTC_EndPoint>
{
  public:

			~OTC_EPInterface();

    // = ENDPOINT

    OTC_EndPoint*	endPoint() const
				{ return endPoint_; }
				// Returns the endpoint for which this
				// class is handling messages.

    OTC_EndPointType	type() const
				{ return endPoint_->type(); }
				// Returns whether the endpoint is acting as
				// a client or a server by returning either
				// the value <OTCLIB_ENDPOINT_CLIENT> or the
				// value <OTCLIB_ENDPOINT_SERVER> as
				// appropriate.

    OTC_String const&	interface() const
      				{ return endPoint_->interface(); }
				// Returns the interface being used over
				// the transport layer.

    OTC_String const&	protocol() const
      				{ return endPoint_->protocol(); }
				// Returns the type of transport layer
				// being used for the link.

    OTC_String const&	localAddress() const
				{ return endPoint_->localAddress(); }
				// Returns local address assigned to the
				// endpoint.

    OTC_String const&	remoteAddress() const
				{ return endPoint_->remoteAddress(); }
				// Returns remote address assigned to the
				// endpoint. That is, the address of the
				// endpoint at the other end of the
				// connection of which this endpoint is one
				// end. Returns an empty string if no remote
				// address is known because of a connection
				// not having been made. In other words, if
				// this function returns an empty string, the
				// connection that this object is one end of,
				// is not active. Between two separate
				// connections, this address will most likely
				// change.

    OTC_String		localTransport() const
    				{ return endPoint_->localTransport(); }
				// Returns the local address corresponding to
				// the transport layer address.

    OTC_String		remoteTransport() const
    				{ return endPoint_->remoteTransport(); }
				// Returns the remote address corresponding
				// to the transport layer address.

    bool		isConnected() const
				{ return endPoint_->isConnected(); }
				// Returns <true> if this endpoint is one end
				// of an active connection. Whether the
				// connection is active is deduced from
				// whether the remote address is not empty.

  protected:

    // = INITIALISATION

			OTC_EPInterface(OTC_EndPoint* theEndPoint);
				// Initialises class to handle messages being
				// sent via <theEndPoint>.

    // = REMOTE ADDRESS

    void		bindRemoteAddress(char const* theRemoteAddress)
				{
				  endPoint_->bindRemoteAddress(
				   theRemoteAddress);
				}
				// Binds the remote address for the endpoint.
				// Calling this function effectively says
				// that this endpoint is now one end of an
				// active connection. Raises an exception if
				// <theRemoteAddress> is <0> or contains only
				// whitespace or is empty.

    void		unbindRemoteAddress()
				{ endPoint_->unbindRemoteAddress(); }
				// Unbinds the remote address for the
				// endpoint. Calling this function
				// effectively says that this endpoint is no
				// longer one end of an active connection.

    // = INTERNAL RESTART

    void*		shutdown(void* theHandle, int theError)
				{
				  return endPoint_->shutdown(
				   theHandle,theError);
				}
				// Calls <OTC_EndPoint::shutdown()>. Would be
				// redefined in a derived class of
				// <OTC_EndPoint> to perform whatever actions
				// should be performed in the event of loss
				// of the connection being used. <shutdown()>
				// will be called with a handle to some data
				// relevant to the existing connection.
				// <theError> will be set to <0> if the
				// remote end shutdown normally, or a copy of
				// <errno> if an error occured. It is the job
				// of the derived class to at least close
				// down and reclaim any resources relevant to
				// the existing connection. The derived class
				// function may create a new connection and
				// pass back a pointer to relevant data for
				// the new connection. If this is not done,
				// <0> should be returned. In the event that
				// no new connection was created, the derived
				// class may delete itself.

  private:

			OTC_EPInterface(OTC_EPInterface const&);
				// Do not define an implementation for this.

    OTC_EPInterface&	operator=(OTC_EPInterface const&);
				// Do not define an implementation for this.

    OTC_EndPoint*	endPoint_;
				// The endpoint for which messages are being
				// handled.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPINTRFC_HH */
