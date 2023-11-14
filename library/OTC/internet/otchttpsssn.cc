/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otchttpsssn.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/httpsssn.hh"
#endif
#endif

#include <OTC/internet/httpsssn.hh>

#include <OTC/internet/httpdaem.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/system/program.hh>
#include <OTC/text/osstream.hh>
#include <OTC/collctn/cbitset.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>
#include <OTC/stdlib/strstream.h>

#include <time.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_HttpSession");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_HttpSession::mutex_;
bool OTC_HttpSession::gInitialised_ = false;
int OTC_HttpSession::gReadTimeout_ = 0;
int OTC_HttpSession::gKeepAliveTimeout_ = 0;
char* OTC_HttpSession::gLogChannel_ = 0;

/* ------------------------------------------------------------------------- */
OTC_HttpSession::~OTC_HttpSession()
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::~OTC_HttpSession()");

  OTCEV_Action::cancelAgent(id());
  OTCEV_Timeout::cancelAgent(id());

  // Destroy the servlet if we still hold
  // a reference to it.

  if (servlet_ != 0)
    servlet_->destroy();

  // Stop endpoint just in case it was the
  // listener that was killed. Don't wait
  // for any queued messages to be sent.

  endPoint_->stop(0);

  endPoint_->unReference();
  endPoint_ = 0;
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::terminate()
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::terminate()");

  OTCEV_Action::cancelAgent(id());
  OTCEV_Timeout::cancelAgent(id());

  // Destroy the servlet if we still hold
  // a reference to it.

  if (servlet_ != 0)
  {
    servlet_->destroy();
    servlet_ = 0;
  }
}

/* ------------------------------------------------------------------------- */
OTC_HttpSession::OTC_HttpSession(
 OTC_HttpDaemon* theDaemon,
 OTC_EndPoint* theEndPoint
)
  : daemon_(theDaemon),
    endPoint_(0),
    state_(OTCLIB_HTTPSESSION_HEADER),
    closeActionId_(0),
    readTimerId_(0),
    keepAliveTimerId_(0),
    keepAlive_(false),
    sessionKeepAlive_(false),
    contentLength_(-1),
    servlet_(0),
    responseStatus_(-1),
    responseLength_(0)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::OTC_HttpSession(OTC_HttpDaemon*,OTC_EndPoint*)");

  OTCLIB_ENSURE_FN((theDaemon != 0),
   "OTC_HttpSession::OTC_HttpSession(OTC_HttpDaemon*,OTC_EndPoint*)",
   "invalid HTTP daemon");
  OTCLIB_ENSURE_FN((theEndPoint != 0),
   "OTC_HttpSession::OTC_HttpSession(OTC_HttpDaemon*,OTC_EndPoint*)",
   "invalid endpoint");

  endPoint_ = theEndPoint;
  endPoint_->reference();

  inBox_.bind(this,endPoint_->localAddress());

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gInitialised_ == 0)
  {
    char const* theValue;

    theValue = getenv("OTCLIB_HTTPREADTIMEOUT");

    if (theValue != 0)
      gReadTimeout_ = 1000*atoi(theValue);

    if (gReadTimeout_ <= 0)
      gReadTimeout_ = 60000;

    theValue = getenv("OTCLIB_HTTPKEEPALIVETIMEOUT");

    if (theValue != 0)
      gKeepAliveTimeout_ = 1000*atoi(theValue);

    if (gKeepAliveTimeout_ < 0 || theValue == 0)
      gKeepAliveTimeout_ = 5000;

    OTCLIB_TRACER(MODULE) << "read-timeout = "
     << gReadTimeout_ << endl;
    OTCLIB_TRACER(MODULE) << "keep-alive-timeout = "
     << gKeepAliveTimeout_ << endl;

    theValue = getenv("OTCLIB_HTTPLOGCHANNEL");

    if (theValue != 0)
    {
      gLogChannel_ = new char[strlen(theValue)+1];
      OTCLIB_ASSERT_M(gLogChannel_ != 0);

      strcpy(gLogChannel_,theValue);

      OTCLIB_TRACER(MODULE) << "log-channel = "
       << gLogChannel_ << endl;
    }
    else
      gLogChannel_ = 0;

    gInitialised_ = true;
  }

  xxxMutex.release();

  mutex_.unlock();

  readTimerId_ = OTCEV_Timeout::start(id(),gReadTimeout_);

  clientHost_ = endPoint_->remoteTransport();

  int theIndex;
  theIndex = clientHost_.index(':');

  if (theIndex != -1)
  {
    clientPort_ = clientHost_.after(theIndex);
    clientHost_ = clientHost_.before(theIndex);
  }
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_HttpSession::contentType() const
{
  if (headers_.contains("content-type"))
    return headers_.item("content-type");

  return OTC_String::undefinedString();
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::process(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::process(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;
  OTCLIB_TRACER(MODULE) << "state = " << int(state_) << endl;

  if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction;
    theAction = (OTCEV_Action*)theEvent;

    if (closeActionId_ == theAction->action())
    {
      closeActionId_ = 0;

      if (state_ == OTCLIB_HTTPSESSION_HEADER)
      {
	OTCLIB_TRACER(MODULE) << "keepAlive = " << keepAlive_ << endl;

	if (keepAlive_ == false)
	{
	  shutdown();
	}
	else
	{
	  if (ibuffer_.isEmpty())
	  {
	    if (keepAliveTimerId_ == 0)
	    {
	      keepAliveTimerId_ = OTCEV_Timeout::start(
	       id(),gKeepAliveTimeout_);
	    }
	  }
	  else
	    scanInput_();
	}
      }
    }
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Timeout* theTimeout;
    theTimeout = (OTCEV_Timeout*)theEvent;

    if (theTimeout->timer() == readTimerId_ ||
     theTimeout->timer() == keepAliveTimerId_)
    {
      shutdown();
    }
  }
  else if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->currentReceiver() == inBox_.name())
    {
      if (readTimerId_ != 0)
      {
	OTCEV_Timeout::cancel(readTimerId_);
	readTimerId_ = 0;
      }

      if (keepAliveTimerId_ != 0)
      {
	OTCEV_Timeout::cancel(keepAliveTimerId_);
	keepAliveTimerId_ = 0;
      }

      if (theEnvelope->message() != 0)
	ibuffer_ += theEnvelope->message()->content();

      scanInput_();
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  process(theEvent);

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::shutdown(int theDelay)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::shutdown(int)");

  OTCLIB_TRACER(MODULE) << "delay = " << theDelay << endl;

  endPoint_->stop(theDelay);
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::scanInput_()
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::scanInput_()");

  if (state_ == OTCLIB_HTTPSESSION_HEADER)
  {
    // Still looking for end of header.

    int theIndex;
    theIndex = ibuffer_.index("\r\n\r\n");

    if (theIndex != -1)
    {
      // Remove the header from input buffer. If
      // there is still data in the input buffer,
      // it may correspond to the body of a POST
      // request. The POST case will be dealt with
      // in <parseInput_()> and more of the
      // input buffer used up if necessary.

      OTC_String theRequest;
      theRequest = ibuffer_.before(theIndex+4);

      ibuffer_.remove(0,theIndex+4);

      parseInput_(theRequest);
    }
    else
    {
      if (readTimerId_ == 0)
	readTimerId_ = OTCEV_Timeout::start(id(),gReadTimeout_);
    }
  }
  else
  {
    if (!ibuffer_.isEmpty())
    {
      OTC_String theContent;
      theContent = ibuffer_;

      ibuffer_.truncate();

      servlet_->processContent(theContent);
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::parseInput_(OTC_String const& theRequest)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::parseInput_(OTC_String const&)");

  OTCLIB_TRACER(MODULE) << "request = " << theRequest << endl;

  int theIndex;
  OTC_String theInput;

  OTC_CBitSet theSpace(" \t");

  // Keep original request for direct query by
  // user if required.

  request_ = theRequest;

  // Make a copy as it is easier to work by
  // removing each line as we process it.

  OTC_String theBuffer = theRequest;

  // Process request method. Need to extract
  // request type, URL, query string, protocol
  // and server hostname if it exists.

  theIndex = theBuffer.index("\r\n");

  if (theIndex == -1)
  {
    sendError(500,"An internal server error has occurred.");

    return;
  }

  theInput = theBuffer.before(theIndex);
  theBuffer.remove(0,theIndex+2);

  // If connection is being kept alive, subsequent
  // request may be preceeded by "\r\n". Thus remove
  // any blank lines if this is a subsequent request.

  OTCLIB_TRACER(MODULE) << "keep-alive = " << keepAlive_ << endl;

  if (keepAlive_ != false && theInput.isEmpty())
  {
    while (theInput.isEmpty() && !theBuffer.isEmpty())
    {
      theIndex = theBuffer.index("\r\n");

      if (theIndex == -1)
	break;

      theInput = theBuffer.before(theIndex);
      theBuffer.remove(0,theIndex+2);

      OTCLIB_TRACER(MODULE) << "input = " << theInput << endl;
    }
  }

  // Extract request method type.

  theIndex = theInput.index(theSpace);

  if (theIndex == -1)
  {
    sendError(400,"Your request has bad syntax.");

    return;
  }

  requestMethod_ = theInput.before(theIndex);

  theInput.remove(0,theIndex+1);
  theInput.ltrim(theSpace);

  OTCLIB_TRACER(MODULE) << "request-method = " << requestMethod_ << endl;

  // Extract URL. If the URL is the last thing
  // in the string, we are dealing with a client
  // which uses protocol version "HTTP/0.9".

  theIndex = theInput.index(theSpace);

  if (theIndex == -1)
  {
    url_ = theInput;

    if (url_.isEmpty())
    {
      sendError(400,"Your request has bad syntax.");

      return;
    }
    else
    {
      protocolVersion_ = "HTTP/0.9";
    }
  }
  else
  {
    url_ = theInput.before(theIndex);

    theInput.remove(0,theIndex);
    theInput.ltrim(theSpace);

    // Extract protocol. If we get an empty
    // protocol string set it to "HTTP/0.9".

    theIndex = theInput.index(theSpace);

    if (theIndex == -1)
    {
      protocolVersion_ = theInput;
      protocolVersion_.ltrim(theSpace);

      if (protocolVersion_.isEmpty())
	protocolVersion_ = "HTTP/0.9";
    }
    else
    {
      protocolVersion_ = theInput.before(theIndex);
      protocolVersion_.ltrim(theSpace);
    }
  }

  OTCLIB_TRACER(MODULE) << "url = " << url_ << endl;
  OTCLIB_TRACER(MODULE) << "protocol-version = " << protocolVersion_ << endl;

  // Different protocols have different
  // conventions for the URL format. Check for
  // different protocols explicitly in case
  // this changes over time. Bail out if
  // it isn't a type of protocol we handle.

  if (protocolVersion_.index("HTTP/") == 0)
  {
    requestPath_ = url_;

    // Deal with case whereby in HTTP/1.1 and
    // later versions, the URL can contain HTTP
    // URL prefix and server host name. The host
    // information needs to be removed and error
    // generated when earlier versions of
    // protocol contain that information. Try
    // to make this generic so can pick up other
    // protocol types as well for experimentation.

    if (protocolVersion_ == "HTTP/0.9" || protocolVersion_ == "HTTP/1.0")
    {
      if (requestPath_.index("http://") == 0)
      {
	sendError(400,"Your request has bad syntax.");

	return;
      }

      protocolType_ = "http";
    }
    else
    {
      theIndex = requestPath_.index("://");

      if (theIndex != -1)
      {
	protocolType_ = requestPath_.before(theIndex);

	requestPath_.remove(0,theIndex+3);

	theIndex = requestPath_.index('/');

	if (theIndex == -1)
	{
	  sendError(400,"Your request has bad syntax.");

	  return;
	}

	serverHost_ = requestPath_.before(theIndex);
	requestPath_.remove(0,theIndex);

	theIndex = serverHost_.index(':');

	if (theIndex != -1)
	{
	  serverPort_ = serverHost_.after(theIndex);
	  serverHost_ = serverHost_.before(theIndex);
	}
      }
      else
      {
	protocolType_ = "http";
      }
    }

    OTCLIB_TRACER(MODULE) << "protocol-type = " << protocolType_ << endl;
    OTCLIB_TRACER(MODULE) << "server-host = " << serverHost_ << endl;
    OTCLIB_TRACER(MODULE) << "server-port = " << serverPort_ << endl;

    // Extract any query string present in
    // encoded URL.

    theIndex = requestPath_.index('?');

    if (theIndex != -1)
    {
      queryString_ = requestPath_.after(theIndex);
      requestPath_ = requestPath_.before(theIndex);
    }

    // Decode the URL. Ie., convert any %
    // escape sequences.

    theInput.truncate();

    if (urlDecode(requestPath_,theInput) == false)
    {
      sendError(400,"Your request has bad syntax.");

      return;
    }

    requestPath_ = theInput;

    // The resulting URL must start with a
    // leading '/'.

    if (requestPath_[size_t(0)] != '/')
    {
      sendError(400,"Your request has bad syntax.");

      return;
    }
  }
  else
  {
    sendError(501,"Protocol version not supported.");

    return;
  }

  // Resolve the path.

  requestPath_ = resolvePath(requestPath_);

  if (requestPath_.isEmpty())
  {
    sendError(403,"Invalid request path location.");

    return;
  }

  OTCLIB_TRACER(MODULE) << "request-path = " << requestPath_ << endl;
  OTCLIB_TRACER(MODULE) << "query-string = " << queryString_ << endl;

  // Finished dealing with the request method,
  // now have to deal with any headers.

  header_ = theBuffer;

  OTC_String theHeader;

  while (!theBuffer.isEmpty())
  {
    theIndex = theBuffer.index("\r\n");

    if (theIndex == -1)
      break;

    theInput = theBuffer.before(theIndex);
    theBuffer.remove(0,theIndex+2);

    if (theInput.length() > 0)
    {
      if (!theHeader.isEmpty() &&
       (theInput[size_t(0)] == ' ' || theInput[size_t(0)] == '\t'))
      {
	// Continuation line.

	theInput.trim();

	OTC_String theValue;
	theValue = headers_.item(theHeader);
	theValue += "\n";
	theValue += theInput;

	headers_.add(theHeader,theValue,OTCLIB_REPLACE);

	continue;
      }
    }

    theHeader.truncate();
    theIndex = theInput.index(':');

    // Ignore any lines which aren't valid fields.
    // Probably should be more rigorous here and
    // flag an error when something isn't correct.

    if (theIndex != -1)
    {
      theHeader = theInput.before(theIndex);
      theHeader.lower();

      theHeader.rtrim();

      theInput = theInput.after(theIndex);

      theInput.trim();

      OTCLIB_TRACER(MODULE) << "header = " << theHeader << endl;
      OTCLIB_TRACER(MODULE) << "value = " << theInput << endl;

      if (headers_.contains(theHeader))
      {
	// Multiple headers.

	OTC_String theValue;
	theValue = headers_.item(theHeader);
	theValue += ", ";
	theValue += theInput;

	headers_.add(theHeader,theValue,OTCLIB_REPLACE);
      }
      else
	headers_.add(theHeader,theInput);
    }
  }

  // Fill out server host and port fields
  // as necessary.

  if (serverHost_.isEmpty())
  {
    if (headers_.contains("host"))
    {
      serverHost_ = headers_.item("host");
      serverHost_.trim();

      theIndex = serverHost_.index(':');

      if (theIndex != -1)
      {
	serverPort_ = serverHost_.after(theIndex);
	serverHost_ = serverHost_.before(theIndex);
      }
    }
  }

  if (serverHost_.isEmpty())
    serverHost_ = OTC_Program::hostname();

  if (serverPort_.isEmpty())
  {
    char buf[32];
    sprintf(buf,"%d",daemon_->port());

    serverPort_ = buf;
  }

  // Fix up the URL to include "http://" and
  // host and port information if appropriate.

  OTC_String theDaemonRoot;

  theDaemonRoot += "http://";
  theDaemonRoot += serverHost_;
  if (serverPort_ != "80")
  {
    theDaemonRoot += ":";
    theDaemonRoot += serverPort_;
  }

  if (requestPath_.index("http://") != 0)
    url_.prepend(theDaemonRoot);

  // Look for a request to keep alive
  // connection. This is give by the value
  // "keep-alive" in "connection" header. Note
  // that value must be checked with case
  // insensitive match.

  if (headers_.contains("connection"))
  {
    theInput = headers_.item("connection");

    OTCLIB_TRACER(MODULE) << "connection = " << theInput << endl;

    theInput.lower();

    if (theInput == "keep-alive")
      keepAlive_ = true;

    OTCLIB_TRACER(MODULE) << "keep-alive = " << keepAlive_ << endl;
  }

  sessionKeepAlive_ = false;

  // Obtain the HTTP server for this
  // request as need it for authentication.

  OTCLIB_ASSERT(servlet_ == 0);

  OTC_HttpServer* theServer = 0;

  OTC_String theServerPath;
  OTC_String theServletPath;

  theServerPath = requestPath_;
  theServerPath.rtrim('/');

  if (theServerPath.isEmpty())
    theServerPath = "/";

  while (!theServerPath.isEmpty())
  {
    theServer = daemon_->server_(theServerPath);

    OTCLIB_TRACER(MODULE) << "server = " << (void*)theServer << endl;

    if (theServer != 0)
    {
      theServletPath = requestPath_.from(theServerPath.length());
      theServletPath.ltrim('/');

      break;
    }

    if (theServerPath == "/")
      break;

    theIndex = theServerPath.rindex('/');

    if (theIndex == -1)
      break;

    theServerPath = theServerPath.before(theIndex);

    theServerPath.rtrim('/');

    if (theServerPath.isEmpty())
      theServerPath = "/";
  }

  if (theServer == 0)
  {
    sendError(404,"No server loaded to handle request.");

    return;
  }

  // Check for "authorization", then do
  // authentication.

  remoteUser_.truncate();

  if (headers_.contains("authorization"))
  {
    theInput = headers_.item("authorization");

    OTCLIB_TRACER(MODULE) << "authorization = " << theInput << endl;

    if (theInput.index("Basic ") == 0)
    {
      // Deal with basic authorisation.

      theInput.remove(0,6);
      theInput.trim();

      istrstream theStream((char*)theInput.string());
      OTC_String theAuthorisation;

      if (b64Decode(theStream,theAuthorisation) == false)
      {
	// Bad authentication string.

	sendAuthFailure_();

	return;
      }

      OTCLIB_TRACER(MODULE) << "decode = " << theAuthorisation << endl;

      theIndex = theAuthorisation.index(':');

      if (theIndex != -1)
      {
	OTC_String theLogin = theAuthorisation.before(theIndex);
	OTC_String thePassword = theAuthorisation.after(theIndex);

	if (theServer->authorise(theLogin,thePassword) == false)
	{
	  // Authentication not accepted.

	  sendAuthFailure_();

	  return;
	}

	remoteUser_ = theLogin;
      }
      else
      {
	// Bad authentication string.

	sendAuthFailure_();

	return;
      }
    }
    else
    {
      // Unknown encoding for authorisation
      // information.

      sendError(501,"Authorization type not implemented.");

      return;
    }
  }
  else
  {
    // Authenticate for no login/passwd.

    if (theServer->authorise("","") == false)
    {
      // Authentication not accepted.

      sendAuthFailure_();

      return;
    }
  }

  // Authentication succeeded, now create
  // the servlet. Fix up server to give
  // valid URL.

  if (theServerPath != "/")
    serverRoot_ = theDaemonRoot + theServerPath;
  else
    serverRoot_ = theDaemonRoot;

  serverPath_ = theServerPath;
  servletPath_ = theServletPath;
  servletInfo_.truncate();
  sessionId_.truncate();
  sessionContext_.truncate();

  servlet_ = theServer->servlet(this);

  if (servlet_ == 0)
  {
    sendError(404,"No servlet loaded to handle request.");

    return;
  }

  // Now check "content-length". If we find
  // this, need to read that many extra
  // characters from input buffer.

  if (headers_.contains("content-length"))
  {
    theInput = headers_.item("content-length");
    theInput.trim();

    theIndex = -1;

    char* theNext;

    theIndex = strtol(theInput,&theNext,10);

    OTCLIB_TRACER(MODULE) << "content-length = " << theIndex << endl;

    if (*theNext == EOS && theIndex >= 0)
    {
      contentLength_ = theIndex;
    }
    else
    {
      sendError(400,"Invalid content length header.");

      return;
    }
  }
  else
  {
    keepAlive_ = false;

    OTCLIB_TRACER(MODULE) << "keep-alive = " << keepAlive_ << endl;
  }

  // Have complete request.

  state_ = OTCLIB_HTTPSESSION_WAITING;

  // Process the request if we get this far.

  servlet_->processRequest();

  // Pass on any content. Only do this if
  // response hasn't been completed.

  if (state_ != OTCLIB_HTTPSESSION_HEADER)
  {
    if (!ibuffer_.isEmpty())
    {
      OTC_String theContent;
      theContent = ibuffer_;

      ibuffer_.truncate();

      servlet_->processContent(theContent);
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::sendResponse(
 u_int theStatus,
 char const* theMessage
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::sendResponse(u_int,char const*)");

  if (state_ != OTCLIB_HTTPSESSION_WAITING)
    return;

  state_ = OTCLIB_HTTPSESSION_RESPONSE;

  responseStatus_ = theStatus;

  if (!protocolVersion_.isEmpty() && protocolVersion_ != "HTTP/0.9")
  {
    OTC_String theResponse;
    OTC_OSStream theStream(theResponse,OTCLIB_BUFFERED);

    if (theMessage == 0)
      theMessage = statusText(theStatus);

    theStream << "HTTP/1.0" << " " << theStatus;
    theStream << " " << theMessage << "\r\n";

    theStream << flush;

    OTCLIB_TRACER(MODULE) << "response = " << theResponse << flush;

    queueMessage_(theResponse);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::sendHeader(
 char const* theKeyWord,
 char const* theValue
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::sendHeader(char const*,char const*)");

  if (state_ != OTCLIB_HTTPSESSION_RESPONSE)
    return;

  if (theKeyWord == 0)
    theKeyWord = "";
  if (theValue == 0)
    theValue = "";

  OTCLIB_TRACER(MODULE) << "keyword = " << theKeyWord << endl;
  OTCLIB_TRACER(MODULE) << "value = " << theValue << endl;

  if (*theKeyWord == EOS)
    return;

  OTC_String tmpKeyWord;
  
  tmpKeyWord = theKeyWord;

  tmpKeyWord.trim();
  tmpKeyWord.lower();

  if (tmpKeyWord == "content-length")
  {
    int theLength = -1;

    sscanf(tmpKeyWord.string(),"%d",&theLength);

    if (theLength >= 0)
      sessionKeepAlive_ = true;
  }

  if (!protocolVersion_.isEmpty() && protocolVersion_ != "HTTP/0.9")
  {
    OTC_String theResponse;
    OTC_OSStream theStream(theResponse,OTCLIB_BUFFERED);

    theStream << theKeyWord << ": " << theValue << "\r\n";

    theStream << flush;

    OTCLIB_TRACER(MODULE) << "response = " << theResponse << flush;

    queueMessage_(theResponse);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::sendHeader(
 char const* theKeyWord,
 int theValue
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::sendHeader(char const*,int)");

  if (state_ != OTCLIB_HTTPSESSION_RESPONSE)
    return;

  if (theKeyWord == 0)
    theKeyWord = "";

  OTCLIB_TRACER(MODULE) << "keyword = " << theKeyWord << endl;
  OTCLIB_TRACER(MODULE) << "value = " << theValue << endl;

  if (*theKeyWord == EOS)
    return;

  OTC_String tmpKeyWord;
  
  tmpKeyWord = theKeyWord;

  tmpKeyWord.trim();
  tmpKeyWord.lower();

  if (tmpKeyWord == "content-length")
  {
    if (theValue >= 0)
      sessionKeepAlive_ = true;
  }

  if (!protocolVersion_.isEmpty() && protocolVersion_ != "HTTP/0.9")
  {
    OTC_String theResponse;
    OTC_OSStream theStream(theResponse,OTCLIB_BUFFERED);

    theStream << theKeyWord << ": " << theValue << "\r\n";

    theStream << flush;

    OTCLIB_TRACER(MODULE) << "response = " << theResponse << flush;

    queueMessage_(theResponse);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::endHeaders()
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::endHeaders()");

  if (state_ != OTCLIB_HTTPSESSION_RESPONSE)
    return;

  if (sessionKeepAlive_ == false)
    keepAlive_ = false;

  sessionKeepAlive_ = false;

  if (!protocolVersion_.isEmpty() && protocolVersion_ != "HTTP/0.9")
  {
    OTC_String theResponse;
    OTC_OSStream theStream(theResponse,OTCLIB_BUFFERED);

    theStream << "Server: OSE/";
    theStream << OSE_MAJOR_RELEASE << "." << OSE_MINOR_RELEASE;
    theStream << "\r\n";

    if (keepAlive_ == false)
      theStream << "Connection: Close\r\n";
    else
      theStream << "Connection: Keep-Alive\r\n";

    theStream << "\r\n";

    theStream << flush;

    OTCLIB_TRACER(MODULE) << "response = " << theResponse << flush;

    queueMessage_(theResponse);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::sendContent(OTC_String const& theContent)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::sendContent(OTC_String const&)");

  if (state_ != OTCLIB_HTTPSESSION_RESPONSE)
    return;

  OTCLIB_TRACER(MODULE) << "content = " << theContent << endl;

  queueMessage_(theContent);
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::endContent()
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::endContent()");

  // Log details of the request:

  if (responseStatus_ != -1 && gLogChannel_ != 0)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_INFO);
    theLogger.setChannel(gLogChannel_);

    theLogger << clientHost_ << " ";

    theLogger << "-" << " ";

    if (!remoteUser_.isEmpty())
      theLogger << remoteUser_ << " ";
    else
      theLogger << "-" << " ";

#if defined(OSE_SYS_CYGWIN32)
    time_t theClock = ::time(0);
#if defined(_REENTRANT)
    tm theTimeStruct;
    gmtime_r(&theClock,&theTimeStruct);
    tm* theTime = &theTimeStruct;
#else
    tm* theTime = gmtime(&theClock);
#endif

    char buf[128];
    char const* theFormat = "%d/%b/%Y:%H:%M:%S GMT";
    strftime(buf,sizeof(buf),theFormat,theTime);
#else
    time_t theClock = ::time(0);
#if defined(_REENTRANT)
    tm theTimeStruct;
    localtime_r(&theClock,&theTimeStruct);
    tm* theTime = &theTimeStruct;
#else
    tm* theTime = localtime(&theClock);
#endif

    char buf[128];
    char const* theFormat = "%d/%h/%Y:%T %Z";
    strftime(buf,sizeof(buf),theFormat,theTime);
#endif

    theLogger << "[" << buf << "] ";

    theLogger << "\"" << requestMethod_ << " ";
    theLogger << url_ << " " << protocolVersion_ << "\" ";

    theLogger << responseStatus_ << " ";

    theLogger << responseLength_ << flush;
  }

  // Wipe out attributes we saved away
  // relating to the request.

  responseStatus_ = -1;
  responseLength_ = 0;

  request_.truncate();
  requestMethod_.truncate();
  url_.truncate();
  protocolType_.truncate();
  serverHost_.truncate();
  serverPort_.truncate();
  requestPath_.truncate();
  queryString_.truncate();
  protocolVersion_.truncate();
  header_.truncate();
  content_.truncate();

  headers_.removeAll();

  if (servlet_ != 0)
  {
    servlet_->destroy();
    servlet_ = 0;
  }

  contentLength_ = -1;

  serverRoot_.truncate();
  serverPath_.truncate();
  servletPath_.truncate();
  servletInfo_.truncate();
  sessionId_.truncate();
  sessionContext_.truncate();

  // Reset state.

  state_ = OTCLIB_HTTPSESSION_HEADER;

  // Cancel timers.

  if (readTimerId_ != 0)
  {
    OTCEV_Timeout::cancel(readTimerId_);
    readTimerId_ = 0;
  }

  if (keepAliveTimerId_ != 0)
  {
    OTCEV_Timeout::cancel(keepAliveTimerId_);
    keepAliveTimerId_ = 0;
  }

  // Use action to notify that we have finished
  // so that if connection is marked for keep
  // alive and there is another request
  // waiting, we process it out of dispatcher
  // and not user code.

  if (closeActionId_ == 0)
    closeActionId_ = OTCEV_Action::schedule(id(),OTCLIB_STANDARD_JOB);
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::sendError(u_int theStatus, char const* theContent)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::sendError(u_int,char const*)");

  OTC_String theResponse;
  OTC_OSStream theStream(theResponse,OTCLIB_BUFFERED);

  // Force into state where we can send a response.

  state_ = OTCLIB_HTTPSESSION_WAITING;

  keepAlive_ = false;

  // Compose message.

  theStream << "<HTML>\n";
  theStream << "<HEAD>\n";
  theStream << "<TITLE>\n";
  theStream << theStatus << " " << statusText(theStatus) << "\n";
  theStream << "</TITLE>\n";
  theStream << "</HEAD>\n";

  theStream << "<BODY>\n";
  
  theStream << "<H2>\n";
  theStream << theStatus << " " << statusText(theStatus) << "\n";
  theStream << "</H2>\n";

  if (theContent != 0)
    theStream << theContent << "\n";

  theStream << "</BODY>\n";
  theStream << "</HTML>\n";

  theStream << flush;

  sendResponse(theStatus);

  sendHeader("Content-Type","text/html");
  sendHeader("Content-Length",theResponse.length());

  endHeaders();

  sendContent(theResponse);

  endContent();
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::sendComplete(OTC_String const& theResponse)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::sendComplete(OTC_String const&)");

  // Don't use <sendResponse()> as the
  // provided response should include the
  // status line and headers as well as
  // content. Only send the content and
  // then call <endContent()> to finish
  // things up.

  if (state_ != OTCLIB_HTTPSESSION_WAITING)
    return;

  // Need to extract the status for the
  // purpose of doing logging.

  int theIndex;
  theIndex = theResponse.index(' ');

  if (theIndex != -1)
    responseStatus_ = atoi(&theResponse.string()[theIndex]);

  OTCLIB_TRACER(MODULE) << "response = " << theResponse << endl;

  queueMessage_(theResponse);

  keepAlive_ = false;

  endContent();
}

/* ------------------------------------------------------------------------- */
char const* OTC_HttpSession::statusText(u_int theStatus)
{
  char const* theText = 0;

  switch (theStatus)
  {
    case 100: { theText = "Continue"; break; }
    case 101: { theText = "Switch Protocols"; break; }
    case 200: { theText = "OK"; break; }
    case 201: { theText = "Created"; break; }
    case 202: { theText = "Accepted"; break; }
    case 203: { theText = "Non-Authoritative Information"; break; }
    case 204: { theText = "No Content"; break; }
    case 205: { theText = "Reset Content"; break; }
    case 206: { theText = "Partial Content"; break; }
    case 301: { theText = "Moved Permanently"; break; }
    case 302: { theText = "Found"; break; }
    case 303: { theText = "See Other"; break; }
    case 304: { theText = "Not Modified"; break; }
    case 305: { theText = "Use Proxy"; break; }
    case 307: { theText = "Temporary Redirect"; break; }
    case 400: { theText = "Bad Request"; break; }
    case 401: { theText = "Unauthorized"; break; }
    case 402: { theText = "Payment Required"; break; }
    case 403: { theText = "Forbidden"; break; }
    case 404: { theText = "Not Found"; break; }
    case 405: { theText = "Method Not Allowed"; break; }
    case 406: { theText = "Not Acceptable"; break; }
    case 407: { theText = "Proxy Authentication Required"; break; }
    case 408: { theText = "Request Time-out"; break; }
    case 409: { theText = "Conflict"; break; }
    case 410: { theText = "Gone"; break; }
    case 411: { theText = "Length Required"; break; }
    case 412: { theText = "Precondition Failed"; break; }
    case 413: { theText = "Request Entity Too Large"; break; }
    case 414: { theText = "Request-URI Too Large"; break; }
    case 415: { theText = "Unsupported Media Type"; break; }
    case 416: { theText = "Requested Range Not Satisfiable"; break; }
    case 417: { theText = "Exception Failed"; break; }
    case 500: { theText = "Internal Server Error"; break; }
    case 501: { theText = "Not Implemented"; break; }
    case 502: { theText = "Bad Gateway"; break; }
    case 503: { theText = "Service Unavailable"; break; }
    case 504: { theText = "Gateway Time-out"; break; }
    case 505: { theText = "HTTP Version Not Supported"; break; }
    default: { theText = "Unknown Status"; break; }
  }

  return theText;
}

/* ------------------------------------------------------------------------- */
bool OTC_HttpSession::xmlDecode(
 istream& theStream,
 int theDelim,
 OTC_String& theString
)
{
  int theInput;

  OTC_Capacity theCapacity(15);
  OTC_String theEntity(theCapacity);

  char theBuffer[1024];
  size_t theActive = 0;

  theInput = theStream.get();

  while (theStream.good())
  {
    if (theInput == theDelim)
    {
      theStream.putback(theInput);

      if (theActive != 0)
	theString.append(theBuffer,theActive);

      return true;
    }

    if (theInput == '&')
    {
      // This should be an XML escape.

      theEntity = OTC_String::get(theStream,';');

      if (!theStream.good())
	return false;

      theStream.ignore(1);

      if (theEntity.isEmpty())
	return false;

      if (theEntity[size_t(0)] == '#')
      {
	// Character reference.

	if (theEntity.length() > 2)
	{
	  if (theEntity[size_t(1)] == 'x')
	  {
	    // Hexadecimal.

	    int v1 = 0;
	    char v2 = 0;

	    if (sscanf(&theEntity[size_t(2)],"%x%c",&v1,&v2) != 1)
	      return false;

	    if (v1 > 0xFF)
	      return false;

	    // theString += char(v1);

	    theBuffer[theActive++] = char(v1);

	    if (theActive == sizeof(theBuffer))
	    {
	      theString.append(theBuffer,theActive);
	      theActive = 0;
	    }
	  }
	  else
	  {
	    // Numeric.

	    int v1 = 0;
	    char v2 = 0;

	    if (sscanf(&theEntity[size_t(1)],"%u%c",&v1,&v2) != 1)
	      return false;

	    if (v1 > 0xFF)
	      return false;

	    // theString += char(v1);

	    theBuffer[theActive++] = char(v1);

	    if (theActive == sizeof(theBuffer))
	    {
	      theString.append(theBuffer,theActive);
	      theActive = 0;
	    }
	  }
        }
	else
	  return false;
      }
      else
      {
	// Entity reference.

	if (theEntity == "amp")
	{
	  // theString += '&';
	  theBuffer[theActive++] = '&';
	}
	else if (theEntity == "lt")
	{
	  // theString += '<';
	  theBuffer[theActive++] = '<';
	}
	else if (theEntity == "gt")
	{
	  // theString += '>';
	  theBuffer[theActive++] = '>';
	}
	else if (theEntity == "apos")
	{
	  // theString += '\'';
	  theBuffer[theActive++] = '\'';
	}
	else if (theEntity == "quot")
	{
	  // theString += '"';
	  theBuffer[theActive++] = '"';
	}
	else
	  return false;

	if (theActive == sizeof(theBuffer))
	{
	  theString.append(theBuffer,theActive);
	  theActive = 0;
	}
      }
    }
    else
    {
      // theString += theInput;

      theBuffer[theActive++] = theInput;

      if (theActive == sizeof(theBuffer))
      {
	theString.append(theBuffer,theActive);
	theActive = 0;
      }
    }

    theInput = theStream.get();
  }

  if (theDelim == EOF && theInput == EOF)
  {
    if (theActive != 0)
      theString.append(theBuffer,theActive);

    return true;
  }

  return false;
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::xmlEncode(
 ostream& theStream,
 char const* theString,
 size_t theLength,
 OTC_CBitSet const& theChars
)
{
  char const* theStart;
  size_t theActive = 0;

  theStart = theString;

  for (size_t i=0; i<theLength; i++)
  {
    char c = theString[i];

    switch (c)
    {
      case '<':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&lt;";
	break;
      }

      case '>':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&gt;";
	break;
      }

      case '&':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&amp;";
	break;
      }

      case '"':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&quot;";
	break;
      }

      case '\'':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&apos;";
	break;
      }

      default:
      {
	if (theChars.test(u_char(c)))
	{
	  if (theActive != 0)
	  {
	    theStream.write(theStart,theActive);
	    theActive = 0;
	  }

	  theStart = &theString[i+1];

	  char buf[16];
	  sprintf(buf,"%X",size_t(u_char(c)));

	  theStream << "&#x";
	  theStream << buf;
	  theStream << ";";
	}
        else
	{
	  // theStream << c;

	  theActive++;
	}
      }
    }
  }

  if (theActive != 0)
    theStream.write(theStart,theActive);
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::xmlEncode(
 ostream& theStream,
 char const* theString,
 size_t theLength
)
{
  OTC_CBitSet theChars;
  xmlEncode(theStream,theString,theLength,theChars);
}

/* ------------------------------------------------------------------------- */
static char b64Encode_table[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
};

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::b64Encode(
 ostream& theStream,
 char const* theString,
 size_t theLength
)
{
  u_char const* tmpString;
  tmpString = (u_char const*)theString;
  size_t theCount = 0;

  while (theLength > 2)
  {
    theStream << b64Encode_table[tmpString[0]>>2];
    theStream << b64Encode_table[((tmpString[0]&0x03)<<4)+(tmpString[1]>>4)];
    theStream << b64Encode_table[((tmpString[1]&0x0f)<<2)+(tmpString[2]>>6)];
    theStream << b64Encode_table[tmpString[2]&0x3f];

    tmpString += 3;
    theLength -= 3;

    theCount += 3;

    if ((theCount % 57) == 0)
      theStream << "\n";
  }

  if (theLength != 0)
  {
    theStream << b64Encode_table[tmpString[0]>>2];

    if (theLength > 1)
    {
      theStream << b64Encode_table[((tmpString[0]&0x03)<<4)+(tmpString[1]>>4)];
      theStream << b64Encode_table[(tmpString[1]&0x0f)<<2];
      theStream << '=';
    }
    else
    {
      theStream << b64Encode_table[(tmpString[0]&0x03)<<4];
      theStream << '=';
      theStream << '=';
    }

    theCount += theLength;

    if ((theCount % 57) == 0)
      theStream << "\n";
  }

  if ((theCount % 57) != 0)
    theStream << "\n";
}

/* ------------------------------------------------------------------------- */
/* Base-64 decoding.  This represents binary data as printable ASCII
** characters.  Three 8-bit binary bytes are turned into four 6-bit
** values, like so:
**
**   [11111111]  [22222222]  [33333333]
**
**   [111111] [112222] [222233] [333333]
**
** Then the 6-bit values are represented using the characters "A-Za-z0-9+/".
*/

static int b64Decode_table[256] = {
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
  52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
  -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
  15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
  -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
  41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
};

/* Do base-64 decoding on a string.  Ignore any non-base64 bytes.
** Return the actual number of bytes generated.  The decoded size will
** be at most 3/4 the size of the encoded, and may be smaller if there
** are padding characters (blanks, newlines).
*/

bool OTC_HttpSession::b64Decode(
 istream& theStream,
 int theDelim,
 OTC_String& theResult
)
{
  int d = 0;
  int prev_d = 0;
  unsigned char c = 0;

  int thePhase = 0;

  int theInput;

  theStream >> ws;

  theInput = theStream.get();

  while (theStream.good())
  {
    if (theInput == theDelim)
    {
      theStream.putback(theDelim);

      if (thePhase == 0)
	return true;

      return false;
    }
    else if (isspace(theInput))
    {
      if (thePhase == 0)
      {
	theStream >> ws;
	theInput = theStream.get();

	if (theInput == theDelim)
	{
	  theStream.putback(theDelim);

	  return true;
	}

        // return false;
      }
    }

    if (theInput != '=')
    {
      d = b64Decode_table[theInput];

      if (d != -1)
      {
	switch (thePhase)
	{
	  case 0:
	  {
	    thePhase++;

	    break;
	  }

	  case 1:
	  {
	    c = ((prev_d << 2) | ((d & 0x30) >> 4));

	    theResult += c;

	    thePhase++;

	    break;
	  }

	  case 2:
	  {
	    c = (((prev_d & 0x0f) << 4) | ((d & 0x3c) >> 2));

	    theResult += c;

	    thePhase++;

	    break;
	  }

	  case 3:
	  {
	    c = (((prev_d & 0x03) << 6) | d);

	    theResult += c;

	    thePhase = 0;

	    break;
	  }
	}
   
	prev_d = d;
      }
      else
      {
	theStream.putback(theInput);

	return false;
      }
    }
    else
    {
      switch (thePhase)
      {
	case 2:
	{
	  if ((prev_d & 0x0f) != 0)
            return false;

	  theInput = theStream.get();

	  if (theInput != '=')
	    return false;

	  theInput = theStream.peek();

	  if (isspace(theInput))
	  {
	    theStream >> ws;
	    theInput = theStream.peek();
	  }

	  if (theInput != theDelim)
	    return false;

	  return true;
	}

	case 3:
	{
	  if ((prev_d & 0x03) != 0)
            return false;

	  theInput = theStream.peek();

	  if (isspace(theInput))
	  {
	    theStream >> ws;
	    theInput = theStream.peek();
	  }

	  if (theInput != theDelim)
	    return false;

	  return true;
	}

	default:
	{
	  return false;
	}
      }
    }

    theInput = theStream.get();
  }

  if (theStream.fail())
    return false;

  // Must always find delimiter.

  if (theDelim == EOF && theInput == EOF && thePhase == 0)
    return true;

  return false;
}

/* ------------------------------------------------------------------------- */
static int tohex(char c)
{
  if (c >= '0' && c <= '9')
      return c - '0';

  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  return 0;
}

bool OTC_HttpSession::urlDecode(
 char const* theString,
 OTC_String& theResult
)
{
  if (theString == 0)
    return false;

  OTC_CBitSet theHexDigits("0123456789abcdefABCDEF");

  while (theString[0] != EOS)
  {
    if (theString[0] == '%')
    {
      if (theString[0] == '%' &&
       theString[1] != EOS && theHexDigits.test(theString[1]) &&
       theString[2] != EOS && theHexDigits.test(theString[2]))
      {
	theResult += char(tohex(theString[1])*16 + tohex(theString[2]));
	theString += 2;
      }
      else
	return false;
    }
    else
      theResult += *theString;

    theString++;
  }

  return true;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_HttpSession::urlEncode(
 char const* theString,
 char const* theExceptions
)
{
  if (theString == 0)
    return OTC_String::undefinedString();

  OTC_String theResult;

  OTC_CBitSet theSet("$_@.-");

  theSet |= OTC_CBitSet(theExceptions);

  while (*theString != EOS)
  {
    if (!isalnum(*theString) && !theSet.test(*theString))
    {
      char buf[3];

      sprintf(buf,"%02x",*theString);

      theResult += "%";
      theResult += buf;
    }
    else
    {
      theResult += *theString;
    }

    theString++;
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_HttpSession::resolvePath(char const* thePath)
{
  if (thePath == 0 || *thePath == EOS || *thePath != '/')
    return OTC_String::nullString();

  OTC_String theResult;
  OTC_String theString;

  while (*thePath != EOS)
  {
    theResult += '/';

    while (*thePath == '/')
      thePath++;

    if (*thePath == EOS)
      break;

    theString.truncate();

    while (*thePath != EOS && *thePath != '/')
      theString += *thePath++;

    if (theString != ".")
    {
      if (theString == "..")
      {
	theResult.rtrim('/');

	int theIndex;

	theIndex = theResult.rindex('/');

	if (theIndex == -1)
	  return OTC_String::nullString();

	theResult.truncate(theIndex);
      }
      else
	theResult += theString;
    }
    else
      theResult.rtrim('/');
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::queueMessage_(OTC_String const& theString)
{
  if (theString.length() == 0)
    return;

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(theString);
  OTCLIB_ASSERT_M(theMessage != 0);

  theMessage->queue(endPoint_->remoteAddress(),
   endPoint_->localAddress(),OTCLIB_STANDARD_JOB);

  responseLength_ += theString.length();
}

/* ------------------------------------------------------------------------- */
void OTC_HttpSession::sendAuthFailure_()
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_HttpSession::sendAuthFailure()");

  OTC_String theResponse;
  OTC_OSStream theStream(theResponse,OTCLIB_BUFFERED);

  // Force into state where we can send a response.

  state_ = OTCLIB_HTTPSESSION_WAITING;

  keepAlive_ = false;

  // Compose message.

  u_int theStatus = 401;
  char const* theContent = "Authorization Failed.";

  theStream << "<HTML>\n";
  theStream << "<HEAD>\n";
  theStream << "<TITLE>\n";
  theStream << theStatus << " " << statusText(theStatus) << "\n";
  theStream << "</TITLE>\n";
  theStream << "</HEAD>\n";

  theStream << "<BODY>\n";
  
  theStream << "<H2>\n";
  theStream << theStatus << " " << statusText(theStatus) << "\n";
  theStream << "</H2>\n";

  if (theContent != 0)
    theStream << theContent << "\n";

  theStream << "</BODY>\n";
  theStream << "</HTML>\n";

  theStream << flush;

  sendResponse(theStatus);

  sendHeader("Content-Type","text/html");
  sendHeader("Content-Length",theResponse.length());
  sendHeader("WWW-Authenticate","Basic realm=\"\"");

  endHeaders();

  sendContent(theResponse);

  endContent();
}

/* ------------------------------------------------------------------------- */
