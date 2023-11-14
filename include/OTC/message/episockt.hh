#ifndef OTC_MESSAGE_EPISOCKT_HH
#define OTC_MESSAGE_EPISOCKT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/episockt.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/epintrfc.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/episockt.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_EPISocket : public OTC_EPInterface
    // = TITLE
    //	   Base class for socket based interfaces.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   The <OTC_EPISocket> class is a base class for endpoint interface
    //     objects which use a socket for transmission of data. This is
    //     abstracted into a base class to allow a substitute protocol to be
    //     inserted where it is necessary to work with a proprietary protocol
    //     at the other end of the connection and where a suitable mapping
    //     can be achieved.
    //     
    // = SEE ALSO
    //	   <OTC_EPInterface>, <OTC_InetClient>, <OTC_InetServer>,
    //     <OTC_UnixClient>, <OTC_UnixServer>, <OTC_EPISMessage>,
    //     <OTC_EPISStream>
{
  public:

			~OTC_EPISocket();

    // = CONSTRUCTION

			OTC_EPISocket(OTC_EndPoint* theEndPoint);
				// Initialises socket based interface object
				// for <theEndPoint>.

    // = QUERY

    virtual int		fd() const = 0;
				// Returns the file descriptor on which
				// messages are being sent/received. Will
				// return <-1> if there is connection.
				// Note that return a value not equal
				// to <-1> does not indicate an active
				// connection as an active connection
				// could still be getting negotiated.

    // = CONNECTION

    virtual void	attach(int theFd) = 0;
				// Defines that <theFd> should be used
				// as the socket connection. Will raise
				// an exception if there is already an
				// active connection. This function will
				// generate an event such that the class
				// will be called back so as to be able to
				// deliver the initial address control
				// messages and any queued messages waiting
				// for delivery.

    virtual int		detach() = 0;
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

    virtual void	suspendInput() = 0;
				// Will suspend handling of input.

    virtual void	resumeInput() = 0;
				// Will resume handling of input.

    // = INTERFACE

    static void		install(
			 OTC_String const& theInterface,
			 OTC_EPISocket* (*theCallback)(OTC_EndPoint*)
			);
				// Registers <theCallback> as callback for
				// creating interface object of type
				// <theInterface> for use over a socket
				// connection. Any existing callback is
				// silently replaced. Note that there are two
				// default interfaces provided with OSE,
				// these are "STREAM" for streaming message
				// contents over a socket and "MESSAGE" which
				// build up structured messages including
				// routing information and writes that to the
				// socket. Although, you can replace the
				// callback for these interface types you
				// should only do so if they provide the
				// equivalent functionality else the result
				// will be undefined. For only the inbuilt
				// interfaces, they will be restored if
				// you replace them but remove your version
				// there after.

    static OTC_EPISocket*	create(
			 OTC_String const& theInterface,
			 OTC_EndPoint* theEndPoint
			);
				// Searches for a callback corresponding to
				// <theInterface> and calls it with
				// <theEndPoint> to produce an instance of a
				// derived version of <OTC_EPISocket>. If no
				// such interface exists or the callback
				// returns <0>, an interface of type "STREAM"
				// will be created.

  private:

			OTC_EPISocket(OTC_EPISocket const&);
				// Do not define an implementation for this.

    OTC_EPISocket&	operator=(OTC_EPISocket const&);
				// Do not define an implementation for this.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    static OTC_HIndex<OTC_String>*      gIndex_;
                                // Index of types and the callback functions
                                // for creating that type of interface.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPISOCKT_HH */
