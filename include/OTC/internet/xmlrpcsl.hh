#ifndef OTC_INTERNET_XMLRPCSL_HH
#define OTC_INTERNET_XMLRPCSL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/xmlrpcsl.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsrvl.hh>
#include <OTC/dispatch/svbroker.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/xmlrpcsl.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_XmlRpcServlet
 : public OTC_HttpServlet, public OTC_EVAgent
    // = TITLE
    //      Servlet translating RPC requests into service requests.
    //
    // = CLASS TYPE
    //      Concrete
    //
    // = DESCRIPTION
    //     The <OTC_XmlRpcServlet> class is a HTTP servlet which acts as a
    //     gateway, translating RPC requests into service requests which
    //     are then delivered to a service. Any response form the service is
    //     subsequently translated back into the appropriate response for
    //     the RPC and returned to the HTTP client. At this time only an
    //     XML-RPC style request is implemented.
    //
    // = NOTES
    //     In decoding the XML, it can't be known how much memory may need to
    //     be preallocated in a buffer to hold the content of an XML element.
    //     As a consequence, a initial buffer size of approximately 64K is
    //     created into which XML content is decoded in order to determine
    //     the actual length of the data. If an application consistently
    //     handles requests where XML content can be larger than this, the
    //     performance of the program will be suboptimal as it will be
    //     necessary to resize the buffer in stages until all data is read.
    //     If this is occuring you should set the environment variable
    //     <OTCLIB_PAYLOADINPUT> to a larger value to encompass the largest
    //     possible size of content in any one XML element.
{
  public:

    			~OTC_XmlRpcServlet();

    // = INITIALISATION

    			OTC_XmlRpcServlet(
			 OTC_HttpSession* theSession,
			 OTC_ServiceBinding* theBinding=0
			);
				// Creates a HTTP servlet to handle an
				// RPC request. <theSession> is the HTTP
				// session object which has parsed the HTTP
				// request. This must be supplied by the
				// factory method contained in the HTTP
				// server object. <theBinding> is the service
				// binding of the service to which the
				// RPC request should be passed. If
				// <theBinding> is <0>, a HTTP response will
				// instead be sent back indicating the
				// target URL was invalid.

  protected:

    // = REQUEST PROCESSING

    void		processRequest();
    				// Decodes the request and either flags an
				// error.

    void		processContent(OTC_String const& theContent);
    				// Accumulates the content of the request
				// and when complete forwards request
				// onto the appropriate service.

    // = TARGET SERVICE

    virtual OTC_ServiceBinding*	service(char const* theName);
				// This function is be called by the method
				// <processRequest()> with <theName> equal to
				// the servlet path supplied to the
				// constructor. This method should analyse
				// <theName> to determine what service the
				// request is being targeted at. By default
				// the method simply looks up the service
				// registry and uses the first service having
				// <theName> that it finds. You can overload
				// this method in a derived class to use
				// some other means to identify which service
				// to use. This would be appropriate given
				// that not all services may be able to
				// respond to requests conforming to what
				// can be represented in an RPC. The instance
				// of <OTC_ServiceBinding> returned by a
				// derived class must have no owner, or if it
				// has, the derived method must call <clone()>
				// on it. This is because <processRequest()>
				// will call <destroy()> on the returned
				// object when it has finished with it.

    // = EVENT HANDLING

    void		process(OTC_Event* theEvent);
    				// Deals with the response received from a
				// service, translating it back into the
				// appropriate RPC format and returning it.

    void		handle(OTC_Event* theEvent);
    				// Calls <process()> then destroys <theEvent>.

  private:

    static bool		decodeRequest_(
			 istream& theStream,
			 OTC_String& theMethod,
			 OTC_RWPayload& theObject,
			 OTC_String& theWorkarea
			);
    				// Decodes the actual request converting
				// int into a service request. <theWorkarea>
				// should be a preallocated working buffer
				// estimated to be large enough to hold the
				// content of any element. If it isn't large
				// enough it will be increased in size as
				// necessary but by having it large enough to
				// start with can avoid reallocations and
				// memory copying.

    static void		encodeFailure_(
			 ostream& theStream,
			 int theError,
			 char const* theDescription="",
			 char const* theOrigin="",
			 char const* theDetails=""
			);
    				// Encodes an XML-RPC failure response.

    void		sendFailure_(
			 int theError,
			 char const* theDescription="",
			 char const* theOrigin="",
			 char const* theDetails=""
			);
    				// Sends back an XML-RPC failure response.

    static bool		encodeRequest_(
			 ostream& theStream,
			 char const* theMethod,
			 OTC_ROPayload const& theObject
			);
    				// Encodes an XML-RPC request. Not used.

    static bool		encodeResponse_(
			 ostream& theStream,
			 OTC_ROPayload const& theObject
			);
    				// Encodes an XML-RPC response.

    static bool		encodeObject_(
			 ostream& theStream,
			 OTC_ROPayload const& theObject
			);
    				// Converts a service response into an
				// XML-RPC value.

  private:

    static bool		skipProlog_(istream& theStream);
    				// Skips the XML document prolog.

    static bool		skipProperty_(istream& theStream);
    				// Skips an XML element attribute.

    static bool		decodeValue_(
			 istream& theStream,
			 OTC_RWPayload theObject,
			 OTC_String& theWorkarea
			);
				// Decodes an XML-RPC value. <theWorkarea>
				// should be a preallocated working buffer
				// estimated to be large enough to hold the
				// content of any element. If it isn't large
				// enough it will be increased in size as
				// necessary but by having it large enough to
				// start with can avoid reallocations and
				// memory copying.

    // All functions read from <theStream> with data returned via
    // <theResult>. The function returns <true> if successful and <false>
    // otherwise. A value of <false> will only be returned when an eof
    // condition is encountered if the function was expecting to read up
    // until some delimiter. The stream is not nessarily checked for failure
    // conditions in all circumstances. If expecting to read up to a
    // specified delimiter, a failure of the stream will be noticed and
    // <false> returned. In the case where a delimiter is not specified, a
    // failure of the stream will simply stop the reading but not result in
    // <false> being returned.

    static bool		readName_(
			 istream& theStream,
			 OTC_String& theResult
			);
    				// Reads in a name.

    static bool		readDigits_(
			 istream& theStream,
			 OTC_String& theResult
			);
    				// Reads in string of decimal digits.

    static bool		readHexDigits_(
			 istream& theStream,
			 OTC_String& theResult
			);
    				// Reads in string of hexadecimal digits.

    // When decoding input up until some delimiter, the delimiter is always
    // left in the stream input.

    static bool		readToChar_(
			 istream& theStream,
			 char theChar,
			 OTC_String& theResult
			);
				// Reads in characters without any decoding
				// until <theChar> is encountered in the
				// input stream.

    static bool		readToString_(
			 istream& theStream,
			 char const* theString,
			 OTC_String& theResult
			);
				// Reads in characters without any decoding
				// until <theString> is encountered in the
				// input stream. Will immediately return
				// <false> if <theString> is <0> or an empty
				// string. It isn't possible for the
				// delimiter string to contain a null
				// character.

    static bool		decodeToChar_(
			 istream& theStream,
			 char theChar,
			 OTC_String& theResult
			);
				// Reads in characters until <theChar> is
				// encountered, decoding character and
				// entity references in the process. Note
				// that only inbuilt entity references are
				// supported.

    // Following wrap up standard XML and b64 encoding/decoding functions.
    // The functions for encoding strings will also ensure that any not
    // ascii text is encoded as well.

    static bool		decodeString_(
			 istream& theStream,
			 int theDelim,
			 OTC_String& theResult
			)
      				{
				  return xmlDecode(
				   theStream,theDelim,theResult);
				}
				// Reads characters from <theStream> undoing
				// any XML escapes using "&" escape sequence
				// until <theDelim> is encountered.
				// <theDelim> will be left in the stream. The
				// result is appended to <theResult> with the
				// function returning <true> if the
				// conversion was successful.

    static void		encodeString_(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength
			);
				// Outputs the first <theLength> characters
				// of <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data using
				// "&" escape sequences. Also, any non
				// ascii characters are also escaped.

    static void		encodeString_(
			 ostream& theStream,
			 char const* theString
			)
      				{
				  encodeString_(theStream,theString,
				   theString?strlen(theString):0);
				}
				// Outputs <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data using
				// "&" escape sequences. Also, any non
				// ascii characters are also escaped.

    static bool		decodeBinary_(
			 istream& theStream,
			 int theDelim,
			 OTC_String& theResult
			)
      				{
				  return b64Decode(
				   theStream,theDelim,theResult);
				}
				// Reads characters from <theStream> undoing
				// any base64 encoding until <theDelim> is
				// encountered. <theDelim> will be left in
				// the stream. The result is appended to
				// <theResult> with the function returning
				// <true> if the conversion was successful.

    static void		encodeBinary_(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength
			)
      				{ b64Encode(theStream,theString,theLength); }
				// Outputs the first <theLength> characters
				// of <theString> onto <theStream>. The
				// characters are encoded in base64 format.

  private:

    			OTC_XmlRpcServlet(OTC_XmlRpcServlet const&);
				// Do not define an implementation for this.

    OTC_XmlRpcServlet&	operator=(OTC_XmlRpcServlet const&);
				// Do not define an implementation for this.

    void		cleanup_();
				// Removes any subscriptions on service
				// registry and destroys the service broker.

    OTC_ServiceBinding*	binding_;
    				// Service binding of target service.

    OTC_SVBroker*	broker_;
    				// Broker through which requests are sent
				// and subsequently received.

    OTC_String		content_;
    				// Accumulated content of the request.

    OTC_String		conversationId_;
    				// Conversation ID of request.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_XMLRPCSL_HH */
