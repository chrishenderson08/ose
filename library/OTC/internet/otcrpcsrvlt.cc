/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otcrpcsrvlt.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/rpcsrvlt.hh"
#endif
#endif

#include <OTC/internet/rpcsrvlt.hh>

#include <OTC/dispatch/response.hh>
#include <OTC/dispatch/failure.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/text/osstream.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#include <OTC/stdlib/strstream.h>

#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_RpcServlet");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_RpcServlet::~OTC_RpcServlet()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_RpcServlet::~OTC_RpcServlet()");

  OTCLIB_TRACER(MODULE) << "id = " << id() << endl;

  cleanup_();
}

/* ------------------------------------------------------------------------- */
OTC_RpcServlet::OTC_RpcServlet(
 OTC_HttpSession* theSession,
 OTC_ServiceBinding* theBinding
)
  : OTC_HttpServlet(theSession), binding_(0), broker_(0)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_RpcServlet::OTC_RpcServlet(...)");

  OTCLIB_TRACER(MODULE) << "id = " << id() << endl;

  if (theBinding != 0)
    binding_ = theBinding->clone();
}

/* ------------------------------------------------------------------------- */
void OTC_RpcServlet::processRequest()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_RpcServlet::processRequest()");

  // Determine if we have a service to send
  // the request to.

  if (binding_ == 0)
  {
    sendError(404,"No such service available.");

    return;
  }

  // Eliminate requests we can't handle.

  if (contentType() != "application/x-rpc")
  {
    sendError(400,"Only support application/x-rpc requests.");

    return;
  }

  if (requestMethod() != "POST")
  {
    sendError(405,"Request must use POST.");

    return;
  }

  if (contentLength() == -1)
  {
    sendError(400,"Content length must be specified.");

    return;
  }

  // Check some details specifc to X-RPC.

  if (!containsHeader("x-rpc-version"))
  {
    sendError(400,"No version number specified.");

    return;
  }

  if (header("x-rpc-version") != "1.1")
  {
    sendError(400,"Unsupported version.");

    return;
  }

  // Look for request type.

  if (!containsHeader("x-rpc-content-type"))
  {
    sendError(400,"No x-rpc content type indicator.");

    return;
  }
    
  if (header("x-rpc-content-type") != "request")
  {
    sendError(400,"Unexpected x-rpc content type.");

    return;
  }

  // Look for request method and relay it.

  if (!containsHeader("x-rpc-request-method"))
  {
    sendError(400,"No request method specified.");

    return;
  }

  requestMethod_ = header("x-rpc-request-method");
  requestMethod_.trim();

  if (requestMethod_.isEmpty())
  {
    sendError(400,"Invalid request method.");

    return;
  }

  // Presize buffer into which full content
  // of request is read. Limit this to a
  // maximum value in case an attempt is
  // being made to crash the system.

  int const MAXIMUM = 32768;

  if (contentLength() < MAXIMUM)
    content_.capacity(contentLength());
  else
    content_.capacity(MAXIMUM);

  // The remainder of the processing is now
  // dependent on receiving actual content.
}

/* ------------------------------------------------------------------------- */
void OTC_RpcServlet::processContent(OTC_String const& theContent)
{
  // Ignore any extra content beyond that
  // specified by content length.

  if (int(content_.length()) >= contentLength())
    return;

  content_ += theContent;

  // Wait for more if don't yet have what
  // is specified by content length.

  if (int(content_.length()) < contentLength())
    return;

  // Attempt to decode request and send it
  // on its way if everything is okay.

  OTC_SVPayload theObject;
  if (!theObject.decode(content_))
  {
    sendError(400,"Request has invalid format.");

    return;
  }

  broker_ = new OTC_SVBroker(this,"","",OTCLIB_SERVICE_HIDDEN);
  OTCLIB_ASSERT_M(broker_ != 0);

  conversationId_ = broker_->sendRequest(binding_,
   requestMethod_,theObject);

  // Subscribe to existance of the specific
  // service in case it disappears before
  // we get a response back.

  OTC_SVBroker::registry()->subscribeServiceAddress(
   id(),binding_->serviceAddress());
}

/* ------------------------------------------------------------------------- */
OTC_ServiceBinding* OTC_RpcServlet::service(char const* theName)
{
  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceName(theName);

  if (theBindings.isValid())
    return theBindings.item()->clone();

  return 0;
}

/* ------------------------------------------------------------------------- */
void OTC_RpcServlet::process(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_RpcServlet::process(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << *theEvent << endl;

  if (theEvent->type() == OTCEV_Response::typeId())
  {
    OTCEV_Response* theResponse;
    theResponse = (OTCEV_Response*)theEvent;

    if (theResponse->conversationId() == conversationId_)
    {
      OTC_String theString;
      theString = theResponse->envelope()->message()->content();

      sendResponse(200);

      sendHeader("Content-Type","application/x-rpc");
      sendHeader("Content-Length",theString.length());
      sendHeader("X-RPC-Version","1.1");
      sendHeader("X-RPC-Content-Type","response");

      endHeaders();

      sendContent(theString);

      endContent();

      cleanup_();
    }
  }
  else if (theEvent->type() == OTCEV_Failure::typeId())
  {
    OTCEV_Failure* theFailure;
    theFailure = (OTCEV_Failure*)theEvent;

    if (theFailure->conversationId() == conversationId_)
    {
      OTC_SVPayload thePayload;
      thePayload["error"] <<= theFailure->error();
      thePayload["description"] <<= theFailure->description();
      thePayload["origin"] <<= theFailure->origin();
      thePayload["details"] <<= theFailure->details();

      OTC_String theString;
      theString = thePayload.encode();

      sendResponse(200);

      sendHeader("Content-Type","application/x-rpc");
      sendHeader("Content-Length",theString.length());
      sendHeader("X-RPC-Version","1.1");
      sendHeader("X-RPC-Content-Type","failure");

      endHeaders();

      sendContent(theString);

      endContent();

      cleanup_();
    }
  }
  else if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ServiceAnnouncement*)theEvent;

    if (theAnnouncement->status() == OTCLIB_SERVICE_WITHDRAWN)
    {
      // Service has disappeared before we got back
      // a response.

      char const* theDetails;
      theDetails = "Service was withdrawn before response received.";

      OTC_SVPayload thePayload;
      thePayload["error"] <<= 57;
      thePayload["description"] <<= "Application Error";
      thePayload["origin"] <<= "netsvc.servlet";
      thePayload["details"] <<= theDetails;

      OTC_String theString;
      theString = thePayload.encode();

      sendResponse(200);

      sendHeader("Content-Type","application/x-rpc");
      sendHeader("Content-Length",theString.length());
      sendHeader("X-RPC-Version","1.1");
      sendHeader("X-RPC-Content-Type","failure");

      endHeaders();

      sendContent(theString);

      endContent();

      cleanup_();
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_RpcServlet::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  process(theEvent);

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_RpcServlet::cleanup_()
{
  if (binding_ != 0)
  {
    OTC_SVBroker::registry()->unsubscribeServiceAddress(
     id(),binding_->serviceAddress());

    binding_->destroy();

    binding_ = 0;
  }

  if (broker_ != 0)
  {
    delete broker_;

    broker_ = 0;
  }
}

/* ------------------------------------------------------------------------- */
