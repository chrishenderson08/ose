#ifndef OTC_INTERNET_HTTPSRVL_HH
#define OTC_INTERNET_HTTPSRVL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/httpsrvl.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsssn.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/httpsrvl.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_HttpServlet
    // = TITLE
    //     Base class for object handling an individual HTTP request.
    //
    // = DESCRIPTION
    //     The <OTC_HttpServlet> class is a base class for an object which
    //     handles a single HTTP request. A derived version of this class
    //     will be created on demand by a HTTP server object supplied to the
    //     HTTP daemon.
    //
    //     To handle requests, you should load into the HTTP daemon object a
    //     HTTP server object for creating a derived version of this class
    //     for any requests falling under a specified URL path. In your
    //     derived class you must override the method <processRequest()> to
    //     handle the request. If the type of request you are handling
    //     contains content you will need to override the <processContent()>
    //     method to deal with that content.
    //     
    // = SEE ALSO
    //     <OTC_HttpSession>, <OTC_HttpDaemon>, <OTC_HttpServer>
{
    friend class	OTC_HttpSession;

  public:

    virtual		~OTC_HttpServlet();

    // = DESTRUCTION

    virtual void	destroy();
				// Can be overridden in a derived class to
				// effect some other means of destroying a
				// servlet. By default just calls delete on
				// itself.

    // = INITIALISATION

			OTC_HttpServlet(OTC_HttpSession* theSession);
				// <theSession> is the HTTP session object
				// which has parsed the HTTP request. This
				// must be initialised by a derived class
				// with the value supplied to the HTTP
				// servlet factory method contained in the
				// HTTP server object.

    // = CONNECTION

    OTC_EndPoint*	endPoint() const
      				{ return session_->endPoint(); }
				// Returns a pointer to the endpoint managing
				// the connection to the HTTP client.

  protected:

    // = REQUEST HANDLING

    virtual void	processRequest() = 0;
				// Must be overridden in a derived class to
				// process the request.

    virtual void	processContent(OTC_String const& theContent);
				// Can be overridden in a derived class to
				// deal with any content received after the
				// request header has been read and parsed.

  public:

    // = RAW REQUEST
    //     The following functions can be used by a derived version of the
    //     <processRequest()> member function and subsequently there after
    //     until the point that either the <endResponse()> or <sendError()>
    //     member functions are called at which point the information will be
    //     cleared.

    OTC_String const&	request() const
				{ return session_->request(); }
				// Returns the actual contents of the
				// request as it was received. This does
				// not include the content component.

    OTC_String const&	header() const
				{ return session_->header(); }
				// Returns the complete set of headers in
				// their raw form. This does not include
				// the request method line.

    // = REQUEST DETAILS
    //     The following functions can be used by a derived version of the
    //     <processRequest()> member function and subsequently there after
    //     until the point that either the <endContent()> or <sendError()>
    //     member functions are called at which point the information will be
    //     cleared.

    OTC_String const&	requestMethod() const
				{ return session_->requestMethod(); }
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
				{ return session_->url(); }
				// Returns the raw URL as it appears in the
				// request string. Except that "http://"
				// and host and port information will have
				// been added if it wasn't already included.

    OTC_String const&	protocolVersion() const
				{ return session_->protocolVersion(); }
				// Returns the version of the protocol used
				// by the request. Will be set to "HTTP/0.9"
				// if approproate even though for that
				// version of the client no version string is
				// provided.

    // Following return the various components of the URL once it has been
    // stripped apart and decoded. Note that the server host and port aren't
    // always given in the URL. In this case you might want to query the
    // "Host" header property. If specified this will be the server host
    // and port number separated by a colon.

    OTC_String const&	serverHost() const
				{ return session_->serverHost(); }
				// Returns the name of HTTP server given in
				// the URL if supplied.

    OTC_String const&	serverPort() const
				{ return session_->serverPort(); }
				// Returns the port used by the HTTP server
				// given in the URL if supplied.

    OTC_String const&	requestPath() const
				{ return session_->requestPath(); }
				// Returns the path component of the URL.
				// This has had any hex escapes decoded.
				// Note that the path is not normalised with
				// respect to use of ".." or ".", it is up to
				// the servlet handling the specific request
				// to do this if required.

    OTC_String const&	queryString() const
				{ return session_->queryString(); }
				// Returns the query string from the URL.
				// The query string has not been decoded,
				// and can contain hex escape sequences.
				// The string needs to be split before
				// you can decode it.

    // Following relate to the client making the request. They are derived
    // from the incoming socket information.

    OTC_String const&	clientHost() const
      				{ return session_->clientHost(); }
    				// Returns the name of the host from which
				// the client request came. The name will
				// be an IP address.

    OTC_String const&	clientPort() const
      				{ return session_->clientPort(); }
    				// Returns the port on the client host that
				// the request came from.

    // Following relate to the user defined by any authentication scheme.

    OTC_String const&	remoteUser() const
      				{ return session_->remoteUser(); }
    				// Returns the login id of the user when
				// an authentication scheme is used.

    // = REQUEST HEADERS
    //     The following functions can be used by a derived version of the
    //     <processRequest()> member function and subsequently there after
    //     until the point that either the <endContent()> or <sendError()>
    //     member functions are called at which point the information will be
    //     cleared.
    //     
    //     Note that the names of headers are squashed to lower case. When
    //     querying headers, you must use all lower case.

    OTC_PairIterator<OTC_String,OTC_String>	headers() const
				{ return session_->headers(); }
				// Returns an iterator over the headers. The
				// same header name can appear more than once
				// with different values.

    bool		containsHeader(char const* theName) const
				{ return session_->containsHeader(theName); }
				// Returns <true> if header with <theName> is
				// included. <theName> must always be lower
				// case.

    OTC_String const&	header(char const* theName) const
				{ return session_->header(theName); }
				// Returns the value associated with a header
				// with <theName>. <theName> must always be
				// lower case. An exception will be raised if
				// no such header existed. If a header
				// appeared multiple times, its values
				// will be combined, with a comma separating
				// each value.

    int			contentLength() const
      				{ return session_->contentLength(); }
    				// If no content length is specified in
				// the header, returns <-1>, else the
				// content length specified in the header
				// is returned.

    OTC_String const&	contentType() const
      				{ return session_->contentType(); }
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
      				{ return session_->serverRoot(); }
    				// Returns a full URL identifying the
				// root of the HTTP server object.

    OTC_String const&	serverPath() const
      				{ return session_->serverPath(); }
    				// Returns the path component of the URL
				// identifying the HTTP server object.

    OTC_String const&	servletPath() const
      				{ return session_->servletPath(); }
				// Returns the path of the servlet below
				// the HTTP server object.

    OTC_String const&	servletInfo() const
      				{ return session_->servletInfo(); }
    				// Returns additional path information
				// falling below the actual servlet object.

    // = USER SESSION
    //     Following store information about the user session the request
    //     is a part of.

    OTC_String const&	sessionId() const
      				{ return session_->sessionId(); }
    				// Returns a value identifying some sort
				// of user session.

    OTC_String const&	sessionContext() const
      				{ return session_->sessionContext(); }
				// Returns a valud giving context information
				// in regard to the session ID.

    // = BUILDING/SENDING A RESPONSE
    //     The following functions are provided for sending back a response.
    //     The member function <sendResponse()> must be called first,
    //     followed by any headers. The member function <endHeaders()> must
    //     be called prior to any content being sent. The member function
    //     <endContent()> last.

    void		sendResponse(
    			 u_int theStatus,
			 char const* theMessage=0
			)
      				{
				  session_->sendResponse(
				   theStatus,theMessage);
				}
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
			)
      				{ session_->sendHeader(theKeyWord,theValue); }
				// Call this to output a header with
				// <theKeyWord> and <theValue>. <theValue>
				// should not contain an embedded newline and
				// should otherwise follow rules for HTTP
				// response headers.

    void		sendHeader(
    			 char const* theKeyWord,
			 int theValue
			)
      				{ session_->sendHeader(theKeyWord,theValue); }
				// Call this to output a header with
				// <theKeyWord> and <theValue>, where
				// <theValue> is an integer.

    void		endHeaders()
      				{ session_->endHeaders(); }
				// You must call this to indicate there are
				// no more headers. This must be called even
				// if no headers were sent.

    void		sendContent(OTC_String const& theContent)
      				{ session_->sendContent(theContent); }
				// Sends <theContent> out onto the connection.
				// Note that you will need to have added a
				// header describing the content length and
				// type yourself if you want them.

    void		endContent()
      				{ session_->endContent(); }
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

    void		sendError(u_int theStatus, char const* theContent=0)
      				{ session_->sendError(theStatus,theContent); }
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

    void		sendComplete(OTC_String const& theResponse)
      				{ session_->sendComplete(theResponse); }
    				// This function takes a preformatted
				// response and sends it back to the HTTP
				// client. Ie., <theResponse> should
				// contain a correct status line, headers
				// and any appropriate content. No checking
				// of the format of <theResponse> is made.
				// This function will cause the connection
				// to be closed once the response is sent.

    // = SHUTTING DOWN CONNECTION

    void		shutdown(int theDelay=-1)
      				{ session_->shutdown(theDelay); }
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

     // = STATUS CODES

    static char const*	statusText(u_int theStatus)
      				{
				  return OTC_HttpSession::statusText(
				   theStatus);
				}
				// Returns a string description of most
				// common status codes.

    // = ENCODING/DECODING

    static bool		xmlDecode(
			 istream& theStream,
			 int theDelim,
			 OTC_String& theResult
			)
      				{
				  return OTC_HttpSession::xmlDecode(
				   theStream,theDelim,theResult);
				}
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
      				{
				  return OTC_HttpSession::xmlDecode(
				   theStream,EOF,theResult);
				}
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
			)
      				{
				  OTC_HttpSession::xmlEncode(theStream,
				   theString,theLength,theChars);
				}
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
				  OTC_HttpSession::xmlEncode(theStream,
				   theString,theString?strlen(theString):0,
				   theChars);
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
			)
      				{
				  OTC_HttpSession::xmlEncode(
				   theStream,theString,theLength);
				}
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
			)
      				{
				  return OTC_HttpSession::b64Decode(
				   theStream,theDelim,theResult);
				}
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
      				{
				  return OTC_HttpSession::b64Decode(
				   theStream,EOF,theResult);
				}
				// Reads characters from <theStream> undoing
				// any base64 encoding until the end of the
				// stream is reached. The result is appended
				// to <theResult> with the function returning
				// <true> if the conversion was successful.

    static void		b64Encode(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength
			)
      				{
				  OTC_HttpSession::b64Encode(
				   theStream,theString,theLength);
				}
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
			)
      				{
				  return OTC_HttpSession::urlDecode(
				   theString,theResult);
				}
				// Converts '%' prefixed hex encodings in
				// <theString>, as used in URL path, back to
				// the characters they represent, appending
				// them onto <theResult>.

    static OTC_String	urlEncode(
			 char const* theString,
			 char const* theExclusions=0
			)
      				{
				  return OTC_HttpSession::urlEncode(
				   theString,theExclusions);
				}
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
  private:

			OTC_HttpServlet(OTC_HttpServlet const&);
				// Do not define an implementation for this.

    OTC_HttpServlet&	operator=(OTC_HttpServlet const&);
				// Do not define an implementation for this.

    OTC_HttpSession*	session_;
    				// Associated HTTP session object.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_HTTPSRVL_HH */
