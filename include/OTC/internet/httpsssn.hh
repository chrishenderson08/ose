#ifndef OTC_INTERNET_HTTPSSSN_HH
#define OTC_INTERNET_HTTPSSSN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/httpsssn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetlstn.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/cbitset.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/httpsssn.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_HttpSessionState
{
  OTCLIB_HTTPSESSION_HEADER,
  OTCLIB_HTTPSESSION_WAITING,
  OTCLIB_HTTPSESSION_RESPONSE
};

class OTC_HttpDaemon;
class OTC_HttpServlet;
class OTC_HttpServer;

class OSE_EXPORT OTC_HttpSession : public OTC_Resource, public OTC_EVAgent
    // = TITLE
    //     HTTP session object.
    //
    // = DESCRIPTION
    //     The <OTC_HttpSession> class manages a single HTTP session. This
    //     class performs the job of reading and parsing any requests which
    //     are received over a connection. An appropriate HTTP server object
    //     is then found which manages that portion of the URL namespace and
    //     a servlet created to process the specific request.
    //
    // = SEE ALSO
    //     <OTC_HttpDaemon>, <OTC_HttpServlet>
{
    friend class	OTC_HttpServlet;

  public:

			~OTC_HttpSession();

    // = INITIALISATION

			OTC_HttpSession(
			 OTC_HttpDaemon* theDaemon,
			 OTC_EndPoint* theEndPoint
			);
				// <theDaemon> is the associated HTTP daemon.
				// <theEndPoint> is the connection from which
				// requests are received and to which
				// responses are sent.

    // = CONNECTION

    OTC_HttpDaemon*	daemon() const
      				{ return daemon_; }
    				// Returns a pointer to the daemon which
				// spawned this HTTP server object.

    OTC_EndPoint*	endPoint() const
      				{ return endPoint_; }
    				// Returns a pointer to the endpoint which
				// is managing the connection to the HTTP
				// client.

    // = RAW REQUEST

    OTC_String const&	request() const
				{ return request_; }
				// Returns the actual contents of the
				// request as it was received. This does
				// not include the content component.

    OTC_String const&	header() const
				{ return header_; }
				// Returns the complete set of headers in
				// their raw form. This does not include
				// the request method line.

    // = REQUEST DETAILS

    OTC_String const&	requestMethod() const
				{ return requestMethod_; }
				// Returns the type of request method. For
				// example "GET" or "POST". Note that the
				// base class doesn't filter out non standard
				// types in case you want to experiment with
				// other types of requests. Thus, you need to
				// determine if a request is valid and if not
				// send a <400> error. If you don't implement
				// a valid type of request, you should send
				// back a <401> error.

    OTC_String const&	url() const
				{ return url_; }
				// Returns the raw URL as it appears in the
				// request string. Except that "http://"
				// and host and port information will have
				// been added if it wasn't already included.

    OTC_String const&	protocolVersion() const
				{ return protocolVersion_; }
				// Returns the version of the protocol used
				// by the request. Will be set to "HTTP/0.9"
				// if appropriate even though for that
				// version of the client no version string is
				// provided.

    // Following return the various components of the URL once it has been
    // stripped apart and decoded. Note that the server host and port aren't
    // always given in the URL. In this case an attempt is made to determine
    // the server host and port from the "Host" header property. If that does
    // not exist, the hostname of the machine and the port the daemon was
    // listening on are used.

    OTC_String const&	serverHost() const
				{ return serverHost_; }
				// Returns the host name of HTTP server.

    OTC_String const&	serverPort() const
				{ return serverPort_; }
				// Returns the port used by the HTTP server.

    OTC_String const&	requestPath() const
				{ return requestPath_; }
				// Returns the path component of the URL.
				// This has had any hex escapes decoded.
				// The path will also be normalised with
				// occurrences of "." and ".." being removed
				// as appropriate.

    OTC_String const&	queryString() const
				{ return queryString_; }
				// Returns the query string from the URL.
				// The query string has not been decoded,
				// and can contain hex escape sequences.
				// The string needs to be split before
				// you can decode it.

    // Following relate to the client making the request. They are derived
    // from the incoming socket information.

    OTC_String const&	clientHost() const
      				{ return clientHost_; }
    				// Returns the name of the host from which
				// the client request came. The name will
				// be an IP address.

    OTC_String const&	clientPort() const
      				{ return clientPort_; }
    				// Returns the port on the client host that
				// the request came from.

    // Following relate to the user defined by any authentication scheme.

    OTC_String const&	remoteUser() const
      				{ return remoteUser_; }
    				// Returns the login id of the user when
				// an authentication scheme is used.

    // = REQUEST HEADERS
    //     Note that the names of headers are squashed to lower case.
    //     When querying headers, you must use all lower case.

    OTC_PairIterator<OTC_String,OTC_String>	headers() const
				{ return headers_.pairs(); }
				// Returns an iterator over the headers. The
				// same header name can appear more than once
				// with different values.

    bool		containsHeader(char const* theName) const
				{ return headers_.contains(theName); }
				// Returns <true> if header with <theName> is
				// included. <theName> must always be lower
				// case.

    OTC_String const&	header(char const* theName) const
				{ return headers_.item(theName); }
				// Returns the value associated with a header
				// with <theName>. <theName> must always
				// be lower case. An exception will be raised
				// if no such header existed. If a header
				// appeared multiple times, its values
				// will be combined, with a comma separating
				// each value.

    int			contentLength() const
      				{ return contentLength_; }
    				// If no content length is specified in
				// the header, returns <-1>, else the
				// content length specified in the header
				// is returned.

    OTC_String const&	contentType() const;
    				// Returns the content type specified in
				// the header. If there are multiple
				// occurences of the content type header
				// field, the last is used. If the field
				// is not defined in the header an empty
				// string is returned which is marked as
				// undefined. Ie., <isUndefined()> returns
				// <true>.

    // = HTTP SERVER/SERVLET
    //     Following relate to the HTTP server object and servlet object
    //     which has been determined as being the target of the request.

    OTC_String const&	serverRoot() const
      				{ return serverRoot_; }
    				// Returns a full URL identifying the
				// root of the HTTP server object.

    OTC_String const&	serverPath() const
      				{ return serverPath_; }
    				// Returns the path component of the URL
				// identifying the HTTP server object.

    OTC_String const&	servletPath() const
      				{ return servletPath_; }
				// Returns the path of the servlet below
				// the HTTP server object.

    OTC_String const&	servletInfo() const
      				{ return servletInfo_; }
    				// Returns additional path information
				// falling below the actual servlet object.
				// This isn't actually set by this class
				// but along with the servlet path might
				// be set and adjusted by the HTTP server
				// object if necessary.

    void		bindServlet(
			 char const* theServletPath,
			 char const* theServletInfo
			)
      				{
				  servletPath_ = theServletPath;
				  servletInfo_ = theServletInfo;
				}
				// Allows the servlet path and servlet info
				// to be adjusted by the HTTP server object
				// if additional path information is supplied
				// beyond that identifying the actual
				// servlet.

    // = USER SESSION
    //     Following store information about the user session the request
    //     is a part of. These aren't set by this class but may be set by
    //     the HTTP server object as part of some mechanism which manages
    //     user sessions or authentication.

    OTC_String const&	sessionId() const
      				{ return sessionId_; }
    				// Returns a value identifying some sort
				// of user session.

    OTC_String const&	sessionContext() const
      				{ return sessionContext_; }
				// Returns a value giving context information
				// in regard to the session ID.

    void		bindSession(
			 char const* theSessionId,
			 char const* theSessionContext=0
			)
      				{
				  sessionId_ = theSessionId;
				  sessionContext_ = theSessionContext;
				}
    				// Allows the session ID and context to be
				// set by the HTTP server object.

  protected:

    // = BUILDING/SENDING A RESPONSE
    //     The following functions are provided for sending back a response.
    //     The member function <sendResponse()> must be called first,
    //     followed by any headers. The member function <endHeaders()> must
    //     be called prior to any content being sent. The member function
    //     <endContent()> last.

    void		sendResponse(
    			 u_int theStatus,
			 char const* theMessage=0
			);
				// You must call this to begin the response.
				// <theStatus> is the HTTP status code and
				// <theMessage> an optional description for
				// that status code. If <theMessage> is not
				// provided an internal table of descriptions
				// for the HTTP codes will be queried to get
				// an appropriate message.

    void		sendHeader(
    			 char const* theKeyWord,
			 char const* theValue
			);
				// Call this to output a header with
				// <theKeyWord> and <theValue>. <theValue>
				// should not contain an embedded newline and
				// should otherwise follow rules for HTTP
				// response headers.

    void		sendHeader(
    			 char const* theKeyWord,
			 int theValue
			);
				// Call this to output a header with
				// <theKeyWord> and <theValue>, where
				// <theValue> is an integer.

    void		endHeaders();
				// You must call this to indicate there are
				// no more headers. This must be called even
				// if no headers were sent. This function
				// will automatically output a server
				// identification string via the header field.
				// If a servlet were handling the request
				// the server identification string will
				// incorporate the servlet name.

    void		sendContent(OTC_String const& theContent);
				// Sends <theContent> out onto the connection.
				// Note that you will need to have added a
				// header describing the content length and
				// type yourself if you want them.

    void		endContent();
				// You must call this to indicate you have
				// finished sending the content of the
				// response. If no content length was
				// provided or HTTP client didn't request
				// connection be kept alive, connection will
				// be closed as soon as response has been
				// written out on connection. If content
				// length was supplied for response and keep
				// alive was requested by HTTP client,
				// connection will be kept open waiting for
				// any subsequent request or until the HTTP
				// client closes the connection.

    // = SENDING AN ERROR RESPONSE

    void		sendError(u_int theStatus, char const* theContent=0);
				// This function is a short cut for the above
				// functions when wishing to send back an
				// error response. <theStatus> should be the
				// server error status code. <theContent>
				// should be the body you wish to be placed
				// into the HTML document which is sent back.
				// Ie., the HEAD of the document is created
				// for you as well as the title. You only
				// need supply the BODY of the message. You
				// can use any HTML which is valid within
				// the bounds of the BODY tags. This function
				// will cause the connection to be closed
				// once the response is sent.

    // = SENDING A COMPLETE RESPONSE

    void		sendComplete(OTC_String const& theResponse);
    				// This function takes a preformatted
				// response and sends it back to the HTTP
				// client. Ie., <theResponse> should
				// contain a correct status line, headers
				// and any appropriate content. No checking
				// of the format of <theResponse> is made.
				// This function will cause the connection
				// to be closed once the response is sent.

    // = SHUTTING DOWN CONNECTION

    void		shutdown(int theDelay=-1);
				// If you want to forcibly shutdown the
				// connection at any point of handling a
				// request you can call this function.
				// If <theDelay> is <-1>, the session object
				// will wait for any data to be flushed out
				// first. If <theDelay> is <0>, the
				// connection will be closed straight away.
				// For any other positive value of <theDelay>
				// a maxmimum time corresponding to
				// <theDelay> milliseconds will be allowed to
				// expire before closing connection to allow
				// any response to be flushed out. If the
				// response is flushed out within that time,
				// the connection will be closed straight
				// away.

  public:

     // = STATUS CODES

    static char const*	statusText(u_int theStatus);
				// Returns a string description of most
				// common status codes.

    // = ENCODING/DECODING

    static bool		xmlDecode(
			 istream& theStream,
			 int theDelim,
			 OTC_String& theResult
			);
				// Reads characters from <theStream> undoing
				// any XML escapes using "&" escape sequence
				// until <theDelim> is encountered.
				// <theDelim> will be left in the stream. The
				// result is appended to <theResult> with the
				// function returning <true> if the
				// conversion was successful.

    static bool		xmlDecode(
			 istream& theStream,
			 OTC_String& theResult
			)
      				{ return xmlDecode(theStream,EOF,theResult); }
				// Reads characters from <theStream> undoing
				// any XML escapes using "&" escape sequence
				// until the end of the stream is reached.
				// The result is appended to <theResult> with
				// the function returning <true> if the
				// conversion was successful.

    static void		xmlEncode(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength,
			 OTC_CBitSet const& theChars
			);
				// Outputs the first <theLength> characters
				// of <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data using
				// "&" escape sequences. If you want to
				// have additional characters encoded, define
				// them in <theChars>.

    static void		xmlEncode(
			 ostream& theStream,
			 char const* theString,
			 OTC_CBitSet const& theChars
			)
      				{
				  xmlEncode(theStream,theString,
				   theString?strlen(theString):0,theChars);
				}
				// Outputs <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data using
				// "&" escape sequences. If you want to
				// have additional characters encoded, define
				// them in <theChars>.

    static void		xmlEncode(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength
			);
				// Outputs the first <theLength> characters
				// of <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data using
				// "&" escape sequences.

    static void		xmlEncode(
			 ostream& theStream,
			 char const* theString
			)
      				{
				  xmlEncode(theStream,theString,
				   theString?strlen(theString):0);
				}
				// Outputs <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data using
				// "&" escape sequences.

    static bool		b64Decode(
			 istream& theStream,
			 int theDelim,
			 OTC_String& theResult
			);
				// Reads characters from <theStream> undoing
				// any base64 encoding until <theDelim> is
				// encountered. <theDelim> will be left in
				// the stream. The result is appended to
				// <theResult> with the function returning
				// <true> if the conversion was successful.

    static bool		b64Decode(
			 istream& theStream,
			 OTC_String& theResult
			)
      				{ return b64Decode(theStream,EOF,theResult); }
				// Reads characters from <theStream> undoing
				// any base64 encoding until the end of the
				// stream is reached. The result is appended
				// to <theResult> with the function returning
				// <true> if the conversion was successful.

    static void		b64Encode(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength
			);
				// Outputs the first <theLength> characters
				// of <theString> onto <theStream>. The
				// characters are encoded in base64 format.

    static void		b64Encode(
			 ostream& theStream,
			 char const* theString
			)
      				{
				  b64Encode(theStream,theString,
				   theString?strlen(theString):0);
				}
				// Outputs <theString> onto <theStream>. The
				// characters are encoded in base64 format.

    static bool		urlDecode(
			 char const* theString,
			 OTC_String& theResult
			);
				// Converts '%' prefixed hex encodings in
				// <theString>, as used in URL path, back to
				// the characters they represent, appending
				// them onto <theResult>.

    static OTC_String	urlEncode(
			 char const* theString,
			 char const* theExclusions=0
			);
				// Encodes characters in <theString> deemed
				// as illegal in a URL into a hex escape
				// sequence. Legal characters are letters,
				// numbers and those is "$-_.+!*'()". There
				// are other characters used in URLs but
				// which have a special meaning. These
				// characters are "=;/#?:%&+". These will be
				// encoded unless they are listed in
				// <theExlusions>. You might typically want
				// to list "/" in <theExclusions> so that the
				// directory structure is preserved.

    // = REQUEST PATH

    static OTC_String	resolvePath(char const* thePath);
    				// Analyses <thePath> eliminating redundant
				// instances of cojoined directory seperator
				// symbols, ie., '/', and eliminates any
				// instances of ".." and ".", adjusting the
				// path as appropriate and returning the
				// result. If the resultant path resolves to
				// a directory above the root directory or
				// the path didn't start with a slash, an
				// empty string is returned.

    // = TERMINATION

    void		terminate();
				// Destroys any active servlet. This is
				// called by the HTTP daemon when the
				// connection is closed by the client.

  protected:

    // = EVENT CALLBACKS

    void		process(OTC_Event* theEvent);
				// Deals with any data received from the
				// connection, initiating parsing of the
				// request and triggering the member function
				// <processRequest()> when the full details
				// of the request has been received.

    void		handle(OTC_Event* theEvent);
                                // Calls <process()> to handle <theEvent> and
                                // then destroys the event. If you override
                                // this member function in a derived class,
                                // you must call <process()> at the start of
                                // your <handle()> method and subsequently
                                // destroy the event.

  private:

			OTC_HttpSession(OTC_HttpSession const&);
				// Do not define an implementation for this.

    OTC_HttpSession&	operator=(OTC_HttpSession const&);
				// Do not define an implementation for this.

    void		scanInput_();
				// If still waiting for header, scans for
				// the end of that header and when found
				// triggers parsing of input to get header
				// details and content out of it if content
				// length defined. If no content length
				// defined and have read in request header
				// passes content on.

    void		parseInput_(OTC_String const& theHeader);
				// Parses <theHeader> extracting the request
				// method type, URL information etc, plus the
				// header fields. If authorisation fields
				// are provided will attempt to authenticate
				// the user. If there is still content to
				// be read in, delays processing of request
				// until all content is read in.

    void		queueMessage_(OTC_String const& theContent);
    				// Queues a message containing content to
				// be sent back to the client. Will increment
				// the count of number of bytes sent.

    void		sendAuthFailure_();
    				// Sends HTTP 401 error with WWW-Authenticate
				// header requesting "Basic" authorisation.

    static OTC_NRMutex	mutex_;
                                // Lock for threads.

    static bool		gInitialised_;
    				// Flag to see if initialisation of global
				// configuration variables has been done.

    static int		gReadTimeout_;
    				// Maximum time to wait for a request to
				// come through from client.

    static int		gKeepAliveTimeout_;
    				// How long to keep alive a connection
				// when persistent connection requested.

    static char*	gLogChannel_;
    				// Channel to log to when logging enabled.

    OTC_HttpDaemon*	daemon_;
    				// The associated HTTP daemon.

    OTC_EndPoint*	endPoint_;
				// The endpoint for the connection over
				// which the request was received and to
				// which the response is to be sent.

    OTC_Receiver	inBox_;
				// Binds local address of endpoint to this
				// agent so that data received over the
				// connection is delivered to this agent.

    OTC_HttpSessionState	state_;
				// What state we are in regarding processing
				// of the request.

    int			closeActionId_;
				// Action ID for event notifiying that
				// request has been processed.

    int			readTimerId_;
				// Timer ID for read timeout.

    int			keepAliveTimerId_;
				// Timer ID for keep alive timeout.

    OTC_String		ibuffer_;
				// The buffer into which the request is
				// accumulated while being handled.

    OTC_String		request_;
				// Original content of request.

    OTC_String		requestMethod_;
				// Type of request method.

    OTC_String		url_;
				// The original URL.

    OTC_String		protocolType_;
				// Type of protocol.

    OTC_String		serverHost_;
				// HTTP server host if provided.

    OTC_String		serverPort_;
				// HTTP server port if provided.

    OTC_String		requestPath_;
				// The path component of the URL.

    OTC_String		queryString_;
				// The decoded query string.

    OTC_String		protocolVersion_;
				// Version of the protocol.

    OTC_String		clientHost_;
				// Name of host client connection came from.

    OTC_String		clientPort_;
				// Port on client host where connection came
				// from.

    OTC_String		remoteUser_;
    				// Login id of the remote user.

    OTC_String		header_;
				// Raw header string.

    OTC_HMap<OTC_String,OTC_String>	headers_;
				// The request headers.

    OTC_String		content_;
				// The request content.

    bool		keepAlive_;
				// Whether the client requested
				// the connection be kept alive.

    bool		sessionKeepAlive_;
				// Whether a valid content length field has
				// been sent back and thus client keep alive
				// can be honoured.

    int			contentLength_;
				// The expected request content length
				// when not all received in first lot of
				// data received from the connection. Will
				// be <-1> if no content length specified.

    OTC_HttpServlet*	servlet_;
    				// HTTP servlet handling the current request.

    OTC_String		serverRoot_;
    				// The full URL identifying the root of the
				// HTTP server object.

    OTC_String		serverPath_;
    				// The path component of the URL identifying
				// the HTTP server object.

    OTC_String		servletPath_;
    				// The path of the servlet below its HTTP
				// server object.

    // Following are placeholders which can be used by the HTTP server
    // and servlet objects.

    OTC_String		servletInfo_;
    				// Additional path information related to
				// the servlet.

    OTC_String		sessionId_;
    				// Value identifying a specific user session.

    OTC_String		sessionContext_;
    				// Value identifying what scope the session
				// ID is valid within.

    int			responseStatus_;
    				// The status assigned to the response
				// sent back to the client.

    u_int		responseLength_;
    				// Count of the number of bytes sent back
				// to the client.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<OTC_HttpSession*>
{
  public:

    static OTC_HttpSession*	add(OTC_HttpSession* theObject)
				{ theObject->reference(); return theObject; }

    static void		remove(OTC_HttpSession* theObject)
				{ theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_HTTPSSSN_HH */
