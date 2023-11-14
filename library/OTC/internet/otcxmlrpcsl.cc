/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otcxmlrpcsl.cc
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
#pragma implementation "OTC/internet/xmlrpcsl.hh"
#endif
#endif

#include <OTC/internet/xmlrpcsl.hh>

#include <OTC/dispatch/response.hh>
#include <OTC/dispatch/failure.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/text/osstream.hh>
#include <OTC/collctn/cbitset.hh>
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
    TAG = new OTC_TraceTag("OTC_XmlRpcServlet");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_XmlRpcServlet::~OTC_XmlRpcServlet()
{
  cleanup_();
}

/* ------------------------------------------------------------------------- */
OTC_XmlRpcServlet::OTC_XmlRpcServlet(
 OTC_HttpSession* theSession,
 OTC_ServiceBinding* theBinding
)
  : OTC_HttpServlet(theSession), binding_(0), broker_(0)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_XmlRpcServlet::OTC_XmlRpcServlet(...)");

  if (theBinding != 0)
    binding_ = theBinding->clone();
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcServlet::processRequest()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_XmlRpcServlet::processRequest()");

  // Determine if we have a service to send
  // the request to.

  if (binding_ == 0)
  {
    sendError(404,"No such service available.");
 
    return;
  }

  // Eliminate requests we can't handle.

  if (contentType() != "text/xml")
  {
    sendError(400,"Servlet can only handle text/xml.");

    return;
  }

  if (requestMethod() != "POST")
  {
    sendError(405,"Only POST method supported by servlet.");

    return;
  }

  if (contentLength() == -1)
  {
    sendError(400,"Content length must be specified.");

    return;
  }

  // Presize buffer into which full content
  // of request is read. Limit this to a
  // maximum value in case an attempt is
  // being made to crash the system.

  int const MAXIMUM = 32768;

  OTCLIB_TRACER(MODULE) << "content-length = " << contentLength() << endl;

  if (contentLength() < MAXIMUM)
    content_.capacity(contentLength());
  else
    content_.capacity(MAXIMUM);

  // The remainder of the processing is now
  // dependent on receiving actual content.
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcServlet::processContent(OTC_String const& theContent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_XmlRpcServlet::processContent()");

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

  OTCLIB_TRACER(MODULE) << content_ << endl << endl;

  istrstream theStream((char*)content_.string(),content_.length());

  OTC_Capacity theCapacity1(63);
  OTC_String theMethod(theCapacity1);

  OTC_Capacity theCapacity2(content_.length());
  OTC_String theWorkarea(theCapacity2);

  OTC_SVPayload theObject;

  if (decodeRequest_(theStream,theMethod,
   theObject,theWorkarea) == false)
  {
    OTCLIB_TRACER(MODULE) << "method=" << theMethod << endl << endl;
    OTCLIB_TRACER(MODULE) << theObject << endl;
    OTCLIB_TRACER(MODULE) << OTC_String::get(theStream,EOF) << endl;

    sendFailure_(54,"Request Decoding Error","netsvc.xmlrpc");

    return;
  }

  OTCLIB_TRACER(MODULE) << "method=" << theMethod << endl;
  OTCLIB_TRACER(MODULE) << theObject << endl;

  // Bind ourselves as an anonymous service
  // and send the request.

  broker_ = new OTC_SVBroker(this,"","",OTCLIB_SERVICE_HIDDEN);
  OTCLIB_ASSERT_M(broker_ != 0);

  conversationId_ = broker_->sendRequest(binding_,
   theMethod,theObject);

  // Subscribe to existance of the specific
  // service in case it disappears before
  // we get a response back.

  OTC_SVBroker::registry()->subscribeServiceAddress(
   id(),binding_->serviceAddress());
}

/* ------------------------------------------------------------------------- */
OTC_ServiceBinding* OTC_XmlRpcServlet::service(char const* theName)
{
  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceName(theName);

  if (theBindings.isValid())
    return theBindings.item()->clone();

  return 0;
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcServlet::process(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_XmlRpcServlet::process(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << *theEvent << endl;

  if (theEvent->type() == OTCEV_Response::typeId())
  {
    OTCEV_Response* theResponse;
    theResponse = (OTCEV_Response*)theEvent;

    if (theResponse->conversationId() == conversationId_)
    {
      OTC_ROPayload theObject;

      theObject = theResponse->content();

      if (theObject.isValid())
      {
	// Make a guess at the required capacity of
	// the string for the payload when formatted
	// as XML-RPC. Use NET-RPC encoding as a
	// guide and add fifty percent since it can
	// be a bit more verbose.

	OTCEV_Message* tmpMessage;
	tmpMessage = theResponse->envelope()->message();

	size_t tmpCapacity = tmpMessage->contentLength();

	OTCLIB_TRACER(MODULE) << "content-length = " << tmpCapacity << endl;

	tmpCapacity = size_t(1.5*tmpCapacity);

	OTC_Capacity theCapacity(tmpCapacity);
	OTC_String theString(theCapacity);

	OTC_OSStream theStream(theString,OTCLIB_BUFFERED);

	if (encodeResponse_(theStream,theObject) != false)
	{
	  theStream << flush;

	  sendResponse(200);

	  sendHeader("Content-Type","text/xml");
	  sendHeader("Content-Length",theString.length());

	  endHeaders();

	  sendContent(theString);

	  endContent();

	  cleanup_();
	}
	else
	  sendFailure_(52,"Response Encoding Error","netsvc.xmlrpc");
      }
      else
	sendFailure_(52,"Response Encoding Error","netsvc.xmlrpc");
    }
  }
  else if (theEvent->type() == OTCEV_Failure::typeId())
  {
    OTCEV_Failure* theFailure;
    theFailure = (OTCEV_Failure*)theEvent;

    if (theFailure->conversationId() == conversationId_)
    {
      OTC_Capacity theCapacity(1024);
      OTC_String theString(theCapacity);

      OTC_OSStream theStream(theString,OTCLIB_BUFFERED);

      encodeFailure_(theStream,theFailure->error(),
       theFailure->description(),theFailure->origin(),
       theFailure->details());

      theStream << flush;

      sendResponse(200);

      sendHeader("Content-Type","text/xml");
      sendHeader("Content-Length",theString.length());

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

      sendFailure_(57,"Application Error","netsvc.xmlrpc",
       "Service was withdrawn before response received.");
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcServlet::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  process(theEvent);

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::decodeRequest_(
 istream& theStream,
 OTC_String& theMethod,
 OTC_RWPayload& theObject,
 OTC_String& theWorkarea
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_XmlRpcServlet::decodeRequest_()");

  theMethod.truncate();

  // Ensure that payload starts out as
  // an empty array so that empty array
  // gets passed back if no params.

  theObject <<= OTC_ROPayload::nullArray();

  // First find and skip xml document
  // header.

  theStream >> ws;

  if (!skipProlog_(theStream))
    return false;

  theStream >> ws;

  // Verify we have a request and work
  // out what the method name is.

  OTC_Capacity theCapacity(63);
  OTC_String theString(theCapacity);

  if (theStream.get() != '<')
    return false;

  if (!readName_(theStream,theString))
    return false;

  if (theString != "methodCall")
    return false;

  theStream >> ws;

  if (theStream.get() != '>')
    return false;

  theStream >> ws;

  if (theStream.get() != '<')
    return false;

  if (!readName_(theStream,theString))
    return false;

  if (theString != "methodName")
    return false;

  theStream >> ws;

  // Check for possibility of <methodName/>.

  if (theStream.peek() == '/')
  {
    // Can only be <methodName/>.

    theStream.ignore(1);

    if (theStream.get() != '>')
      return false;
  }
  else
  {
    // Can only be <methodName>.

    if (theStream.get() != '>')
      return false;

    if (!decodeString_(theStream,'<',theMethod))
      return false;

    if (theStream.get() != '<')
      return false;

    if (theStream.get() != '/')
      return false;

    if (!readName_(theStream,theString))
      return false;

    if (theString != "methodName")
      return false;

    theStream >> ws;

    if (theStream.get() != '>')
      return false;
  }

  // We now have to decode the params.
  // We have to cope with the fact that
  // there may be no <params>, that it
  // is empty and where it is empty that
  // it may use either <params></params>
  // or <params/> forms.

  theStream >> ws;

  if (theStream.get() != '<')
    return false;

  // Check here for possibility of having
  // </methodCall>.

  if (theStream.peek() == '/')
  {
    // No <params> possible, must be the
    // </methodCall>.

    theStream.ignore(1);

    if (!readName_(theStream,theString))
      return false;

    if (theString != "methodCall")
      return false;

    theStream >> ws;

    if (theStream.get() != '>')
      return false;

    theStream >> ws;

    if (!theStream.eof())
      return false;

    return true;
  }

  // Can only be <params> or <params/>.

  if (!readName_(theStream,theString))
    return false;

  if (theString != "params")
    return false;

  theStream >> ws;

  // Now have to check for whether
  // this might be <params/>.

  if (theStream.peek() == '/')
  {
    // Only possibility is <params/>.

    theStream.ignore(1);

    if (theStream.get() != '>')
      return false;
  }
  else
  {
    // Only possibility is <params>.

    if (theStream.get() != '>')
      return false;

    theStream >> ws;

    if (theStream.get() != '<')
      return false;

    // Now have to check whether this
    // might be </params>.

    if (theStream.peek() == '/')
    {
      // Only possibility is </params>.

      theStream.ignore(1);

      if (!readName_(theStream,theString))
	return false;

      if (theString != "params")
	return false;

      theStream >> ws;

      if (theStream.get() != '>')
	return false;
    }
    else
    {
      // Only possibility is <param>. Note that
      // not accepting <param/> as being valid as
      // should contain at least <value/>.

      if (!readName_(theStream,theString))
	return false;

      if (theString != "param")
	return false;

      theStream >> ws;

      if (theStream.get() != '>')
	return false;

      size_t theIndex = 0;

      while (1)
      {
	theStream >> ws;

	if (theStream.get() != '<')
	  return false;

	if (!readName_(theStream,theString))
	  return false;

	if (theString != "value")
	  return false;

	theStream >> ws;

	// Have to check whether this might
	// might be <value/>.

	if (theStream.peek() == '/')
	{
	  // Can only be <value/>.

	  theStream.ignore(1);

	  if (theStream.get() != '>')
	    return false;

	  theObject[theIndex++] <<= "";
	}
	else
	{
	  // Can only be <value>.

	  if (theStream.get() != '>')
	    return false;

	  if (!decodeValue_(theStream,theObject[theIndex++],theWorkarea))
	    return false;
	}

	theStream >> ws;

	if (theStream.get() != '<')
	  return false;

	if (theStream.get() != '/')
	  return false;

	if (!readName_(theStream,theString))
	  return false;

	if (theString != "param")
	  return false;

	theStream >> ws;

	if (theStream.get() != '>')
	  return false;

	theStream >> ws;

	if (theStream.get() != '<')
	  return false;

	if (theStream.peek() == '/')
	  break;

	if (!readName_(theStream,theString))
	  return false;

	if (theString != "param")
	  return false;

	theStream >> ws;

	if (theStream.get() != '>')
	  return false;
      }

      theStream.ignore(1);

      if (!readName_(theStream,theString))
	return false;

      if (theString != "params")
	return false;

      theStream >> ws;

      if (theStream.get() != '>')
	return false;
    }
  }

  // Make sure method call is closed
  // off correctly.

  theStream >> ws;

  if (theStream.get() != '<')
    return false;

  if (theStream.peek() != '/')
    return false;

  theStream.ignore(1);

  if (!readName_(theStream,theString))
    return false;

  if (theString != "methodCall")
    return false;

  theStream >> ws;

  if (theStream.get() != '>')
    return false;

  theStream >> ws;

  if (!theStream.eof())
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcServlet::encodeFailure_(
 ostream& theStream,
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
{
  theStream << "<?xml version=\"1.0\"?>\n";
  theStream << "<methodResponse>\n";
  theStream << "<fault>\n";
  theStream << "<value>\n";
  theStream << "<struct>\n";
  theStream << "<member>\n";
  theStream << "<name>faultCode</name>\n";
  theStream << "<value><int>";
  theStream << theError;
  theStream << "</int></value>\n";
  theStream << "</member>\n";
  theStream << "<member>\n";
  theStream << "<name>faultString</name>\n";
  theStream << "<value>";

  if (theOrigin != 0 && *theOrigin != EOS)
  {
    encodeString_(theStream,theOrigin);
    theStream << " -- ";
  }

  encodeString_(theStream,theDescription);

  if (theDetails != 0 && *theDetails != EOS)
  {
    theStream << "\n\n";
    encodeString_(theStream,theDetails);
  }

  theStream << "</value>\n";
  theStream << "</member>\n";
  theStream << "</struct>\n";
  theStream << "</value>\n";
  theStream << "</fault>\n";
  theStream << "</methodResponse>\n";
  theStream << flush;
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcServlet::sendFailure_(
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
{
  OTC_Capacity theCapacity(1024);
  OTC_String theString(theCapacity);

  OTC_OSStream theStream(theString,OTCLIB_BUFFERED);

  encodeFailure_(theStream,theError,
   theDescription,theOrigin,theDetails);

  theStream << flush;

  sendResponse(200);

  sendHeader("Content-Type","text/xml");
  sendHeader("Content-Length",theString.length());

  endHeaders();

  sendContent(theString);

  endContent();

  cleanup_();
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::encodeRequest_(
 ostream& theStream,
 char const* theMethod,
 OTC_ROPayload const& theObject
)
{
  theStream << "<?xml version=\"1.0\"?>\n";
  theStream << "<methodCall>\n";
  theStream << "<params>\n";

  if (theObject.nodeType() != "array")
    return false;

  OTC_ROPayload theReader;
  theReader = theObject.firstChild();

  while (theReader.isValid())
  {
    theStream << "<param>\n";

    if (!encodeObject_(theStream,theReader))
      return false;

    theStream << "</param>\n";

    theReader = theReader.nextSibling();
  }

  theStream << "</params>\n";
  theStream << "</methodCall>\n";
  theStream << flush;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::encodeResponse_(
 ostream& theStream,
 OTC_ROPayload const& theObject
)
{
  theStream << "<?xml version=\"1.0\"?>\n";
  theStream << "<methodResponse>\n";
  theStream << "<params>\n";
  theStream << "<param>\n";

  if (!encodeObject_(theStream,theObject))
    return false;

  theStream << "</param>\n";
  theStream << "</params>\n";
  theStream << "</methodResponse>\n";
  theStream << flush;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::encodeObject_(
 ostream& theStream,
 OTC_ROPayload const& theObject
)
{
  theStream << "<value>";

  if (theObject.nodeType() == "scalar")
  {
    if (theObject.property("type") == "xsd:integer" ||
     theObject.property("type") == "xsd:long" ||
     theObject.property("type") == "xsd:int" ||
     theObject.property("type") == "xsd:short" ||
     theObject.property("type") == "xsd:byte" ||
     theObject.property("type") == "xsd:unsignedLong" ||
     theObject.property("type") == "xsd:unsignedInt" ||
     theObject.property("type") == "xsd:unsignedShort" ||
     theObject.property("type") == "xsd:unsignedByte")
    {
      theStream << "<int>";
      encodeString_(theStream,theObject.value().string(),
       theObject.value().length());
      theStream << "</int>";
    }
    else if (theObject.property("type") == "xsd:double" ||
     theObject.property("type") == "xsd:float")
    {
      theStream << "<double>";
      encodeString_(theStream,theObject.value().string(),
       theObject.value().length());
      theStream << "</double>";
    }
    else if (theObject.property("type") == "xsd:boolean")
    {
      theStream << "<boolean>";
      if (theObject.value() == "false" || theObject.value() == "0")
	theStream << "0";
      else if (theObject.value() == "true" || theObject.value() == "1")
	theStream << "1";
      else
	return false;

      theStream << "</boolean>";
    }
    else if (theObject.property("type") == "xsd:base64Binary")
    {
      theStream << "<base64>\n";
      encodeBinary_(theStream,theObject.value().string(),
       theObject.value().length());
      theStream << "\n</base64>";
    }
    else if (theObject.property("type") == "xsd:dateTime")
    {
      theStream << "<dateTime.iso8601>";

      if (OTC_Time::isValidTime(theObject.value().string()))
      {
	OTC_Time theTime(theObject.value().string());
	OTC_String theString = theTime.asString("%Y%m%dT%H:%M:%S");

	encodeString_(theStream,theString.string(),theString.length());
      }
      else
	return false;

      theStream << "</dateTime.iso8601>";
    }
    // else if (theObject.property("type") == "")
    // {
    //   theStream << "<nil/>";
    // }
    else
    {
      theStream << "<string>";
      encodeString_(theStream,theObject.value().string(),
       theObject.value().length());
      theStream << "</string>";
    }
  }
  else if (theObject.nodeType() == "array")
  {
    theStream << "<array>\n";
    theStream << "<data>\n";

    OTC_ROPayload theReader;
    theReader = theObject.firstChild();

    while (theReader.isValid())
    {
      if (!encodeObject_(theStream,theReader))
	return false;

      theReader = theReader.nextSibling();
    }

    theStream << "</data>\n";
    theStream << "</array>";
  }
  else if (theObject.nodeType() == "index")
  {
    theStream << "<struct>\n";

    OTC_ROPayload theReader;
    theReader = theObject.firstChild();

    while (theReader.isValid())
    {
      theStream << "<member>\n";
      theStream << "<name>";
      encodeString_(theStream,
       theReader.nodeName().string(),
       theReader.nodeName().length());
      theStream << "</name>\n";

      if (!encodeObject_(theStream,theReader))
	return false;

      theStream << "</member>\n";

      theReader = theReader.nextSibling();
    }

    theStream << "</struct>";
  }
  else
    return false;

  theStream << "</value>\n";

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::skipProlog_(istream& theStream)
{
  theStream >> ws;

  if (theStream.get() != '<')
    return false;

  if (theStream.get() != '?')
    return false;

  OTC_String tmpString;

  if (!readName_(theStream,tmpString))
    return false;

  if (!tmpString.compare("xml",OTCLIB_IGNORECASE))
    return false;

  // XXX Should read all version info etc.

#if 0
  theStream >> ws;

  while (theStream.peek() != '?')
  {
     if (skipProperty_(theStream) == false)
       return false;

     theStream >> ws;
  }

  theStream.ignore(1);

  if (theStream.get() != '>')
    return false;
#endif

  if (!readToString_(theStream,"?>",tmpString))
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::skipProperty_(istream& theStream)
{
  OTC_Capacity theCapacity(63);
  OTC_String theName(theCapacity);

  theName.readToDelim(theStream,'=');

  if (theStream.fail())
    return false;

  theStream >> ws;

  OTC_String theValue(theCapacity);

  if (theStream.peek() == '"')
  {
    theStream.ignore(1);

    if (decodeString_(theStream,'"',theValue) == false)
      return false;
  }
  else if (theStream.peek() == '\'')
  {
    theStream.ignore(1);

    if (decodeString_(theStream,'\'',theValue) == false)
      return false;
  }
  else
    return false;

  theStream.ignore(1);

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::decodeValue_(
 istream& theStream,
 OTC_RWPayload theObject,
 OTC_String& theWorkarea
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_XmlRpcServlet::decodeValue_()");

  // Have to deal with case whereby
  // may be value with no type.

  OTC_Capacity theCapacity(63);

  OTC_String theString(theCapacity);
  OTC_String theElement(theCapacity);

  theWorkarea.truncate();

  if (!decodeString_(theStream,'<',theWorkarea))
    return false;

  if (theStream.get() != '<')
    return false;

  // Check for possibility of </value>.

  if (theStream.peek() == '/')
  {
    // Can only be </value>.

    theStream.ignore(1);

    if (!readName_(theStream,theString))
      return false;

    if (theString != "value")
      return false;

    theStream >> ws;

    if (theStream.get() != '>')
      return false;

    theObject.assign(theWorkarea,theWorkarea.length());

    return true;
  }
  else
  {
    if (!readName_(theStream,theElement))
      return false;

    theStream >> ws;

    // Check for possibility of an empty
    // tag.

    if (theStream.peek() == '/')
    {
      // Must be an empty tag.

      theStream.ignore(1);

      if (theStream.get() != '>')
	return false;

      if (theElement == "struct")
	theObject <<= OTC_ROPayload::nullIndex();
      else if (theElement == "i4" || theElement == "int")
	theObject.assign("","xsd:int");
      else if (theElement == "boolean")
	theObject.assign("","xsd:boolean");
      else if (theElement == "double")
	theObject.assign("","xsd:double");
      else if (theElement == "dateTime.iso8601")
	theObject.assign("","xsd:dateTime");
      else if (theElement == "base64")
	theObject.assign("","xsd:base64Binary");
      else if (theElement == "string")
	theObject.assign("","xsd:string");
      // else if (theElement == "nil")
      //   theObject <<= OTC_ROPayload::nullValue();
      else
	return false;
    }
    else
    {
      if (theStream.get() != '>')
	return false;

      if (theElement == "array")
      {
	theObject <<= OTC_ROPayload::nullArray();

	theStream >> ws;

	if (theStream.get() != '<')
	  return false;

	if (!readName_(theStream,theString))
	  return false;

	if (theString != "data")
	  return false;

	theStream >> ws;

	// Check for possibility of <data/>.

	if (theStream.peek() == '/')
	{
	  // Can only be <data/>

	  theStream.ignore(1);

	  if (theStream.get() != '>')
	    return false;
	}
	else
	{
	  // Can only be <data>.

	  if (theStream.get() != '>')
	    return false;

	  theStream >> ws;

	  if (theStream.get() != '<')
	    return false;

	  while (1)
	  {
	    // Check for possibility of </data>.

	    if (theStream.peek() == '/')
	    {
	      // Can only be </data>.

	      theStream.ignore(1);

	      if (!readName_(theStream,theString))
		return false;

	      if (theString != "data")
		return false;

	      theStream >> ws;

	      if (theStream.get() != '>')
		return false;

	      break;
	    }
	    else
	    {
	      // Can only be <value> or <value/>.

	      if (!readName_(theStream,theString))
		return false;

	      if (theString != "value")
		return false;

	      theStream >> ws;

	      // Check for possibility of <value/>.

	      if (theStream.peek() == '/')
	      {
		// Can only be <value/>.

		theStream.ignore(1);

		if (theStream.get() != '>')
		  return false;

		theObject[-1] <<= "";
	      }
	      else
	      {
		// Can only be <value>.

		if (theStream.get() != '>')
		  return false;

		if (!decodeValue_(theStream,theObject[-1],theWorkarea))
		  return false;
	      }
	    }

	    theStream >> ws;

	    if (theStream.get() != '<')
	      return false;
	  }
	}

	theStream >> ws;

	if (theStream.get() != '<')
	  return false;

	theString.readToDelim(theStream,'>');
	theString.rtrim();

	if (theString != "/array")
	  return false;
      }
      else if (theElement == "struct")
      {
	theObject <<= OTC_ROPayload::nullIndex();

	theStream >> ws;

	if (theStream.get() != '<')
	  return false;

	theString.readToDelim(theStream,'>');
	theString.rtrim();

	while (theString == "member")
	{
	  theStream >> ws;

	  if (theStream.get() != '<')
	    return false;

	  if (!readName_(theStream,theString))
	    return false;

	  if (theString != "name")
	    return false;

	  theStream >> ws;

	  OTC_String theMember(theCapacity);

	  // Check for possibility of <name/>

	  if (theStream.peek() == '/')
	  {
	    // Can only be <name/>.

	    theStream.ignore(1);

	    if (theStream.get() != '>')
	      return false;
	  }
	  else
	  {
	    // Can only be <name>.

	    if (theStream.get() != '>')
	      return false;

	    if (!decodeString_(theStream,'<',theMember))
	      return false;

	    theStream >> ws;

	    if (theStream.get() != '<')
	      return false;

	    theString.readToDelim(theStream,'>');
	    theString.rtrim();

	    if (theString != "/name")
	      return false;
	  }

	  theStream >> ws;

	  if (theStream.get() != '<')
	    return false;

	  if (!readName_(theStream,theString))
	    return false;

	  if (theString != "value")
	    return false;

	  theStream >> ws;

	  // Check for possibility of <value/>.

	  if (theStream.peek() == '/')
	  {
	    // Can only be <value/>.

	    theStream.ignore(1);

	    if (theStream.get() != '>')
	      return false;

	    theObject[theMember] <<= "";
	  }
	  else
	  {
	    // Can only be <value>.

	    if (theStream.get() != '>')
	      return false;

	    if (!decodeValue_(theStream,theObject[theMember],theWorkarea))
	      return false;
	  }

	  theStream >> ws;

	  if (theStream.get() != '<')
	    return false;

	  theString.readToDelim(theStream,'>');
	  theString.rtrim();

	  if (theString != "/member")
	    return false;

	  theStream >> ws;

	  if (theStream.get() != '<')
	    return false;

	  theString.readToDelim(theStream,'>');
	  theString.rtrim();
	}

	if (theString != "/struct")
	  return false;
      }
      else
      {
	theWorkarea.truncate();

	if (theElement == "base64")
	{
	  if (!decodeBinary_(theStream,'<',theWorkarea))
	    return false;
	}
	else if (theElement == "dateTime.iso8601")
	{
	  if (!decodeString_(theStream,'<',theWorkarea))
	    return false;

	  u_int theYear;
	  u_int theMonth;
	  u_int theDay;
	  u_int theHour;
	  u_int theMin;
	  u_int theSec;

	  char theNext;

	  if (theWorkarea.index(EOS) != -1)
	    return false;

	  if (sscanf(theWorkarea,"%4u%2u%2uT%2u:%2u:%2u%c",
	   &theYear,&theMonth,&theDay,&theHour,&theMin,&theSec,&theNext) == 6
	  )
	  {
	    char buf[64];

	    sprintf(buf,"%04u-%02u-%02uT%02u:%02u:%02u",
	     theYear,theMonth,theDay,theHour,theMin,theSec);

	    theWorkarea.assign(buf);
	  }
	  else
	    return false;
	}
	else
	{
	  if (!decodeString_(theStream,'<',theWorkarea))
	    return false;
	}

	if (theElement == "boolean")
	{
	  if (theWorkarea == "0")
	    theObject.assign("false");
	  else
	    theObject.assign("true");
	}
	else
	  theObject.assign(theWorkarea,theWorkarea.length());

	if (theElement == "i4" || theElement == "int")
	  theObject.setProperty("type","xsd:int");
	else if (theElement == "boolean")
	  theObject.setProperty("type","xsd:boolean");
	else if (theElement == "double")
	  theObject.setProperty("type","xsd:double");
	else if (theElement == "dateTime.iso8601")
	  theObject.setProperty("type","xsd:dateTime");
	else if (theElement == "base64")
	  theObject.setProperty("type","xsd:base64Binary");
	// else if (theElement == "nil")
	//   theObject.setProperty("type","");
	else if (theElement != "string")
	  return false;

	if (theStream.get() != '<')
	  return false;

	if (theStream.get() != '/')
	  return false;

	theString.readToDelim(theStream,'>');
	theString.rtrim();

	if (theString != theElement)
	  return false;
      }
    }
  }

  theStream >> ws;

  if (theStream.get() != '<')
    return false;

  theString.readToDelim(theStream,'>');
  theString.rtrim();

  if (theString != "/value")
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::readName_(
 istream& theStream,
 OTC_String& theResult
)
{
  theResult.truncate();

  int theInput;

  // NameChar ::= Letter | Digit | '.' | '-' | '_' | ':'
  // Name ::= (Letter | '_' | ':') (NameChar)*

  theInput = theStream.peek();

  if (theInput == EOF || (!isalpha(theInput) && !strchr("_:",theInput)))
    return false;

  theResult += theStream.get();

  theInput = theStream.peek();

  while (theInput != EOF && (isalnum(theInput) || strchr(".-_:",theInput)))
  {
    theResult += theStream.get();

    theInput = theStream.peek();
  }

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::readDigits_(
 istream& theStream,
 OTC_String& theResult
)
{
  theResult.truncate();

  int theInput;

  // Digits ::= [0-9]+

  theInput = theStream.peek();

  if (theInput == EOF || !isdigit(theInput))
    return false;

  theResult += theStream.get();

  theInput = theStream.peek();

  while (theInput != EOF && isdigit(theInput))
  {
    theResult += theStream.get();

    theInput = theStream.peek();
  }

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::readHexDigits_(
 istream& theStream,
 OTC_String& theResult
)
{
  theResult.truncate();

  int theInput;

  // HexDigits ::= [0-9][a-f][A-F]+

  theInput = theStream.peek();

  if (theInput == EOF || !isxdigit(theInput))
    return false;

  theResult += theStream.get();

  theInput = theStream.peek();

  while (theInput != EOF && isxdigit(theInput))
  {
    theResult += theStream.get();

    theInput = theStream.peek();
  }

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::readToChar_(
 istream& theStream,
 char theChar,
 OTC_String& theResult
)
{
  theResult.truncate();

  int theInput;

  theInput = theStream.peek();

  while (theInput != EOF && theInput != theChar)
  {
    theResult += theStream.get();

    theInput = theStream.peek();
  }

  if (theInput == EOF)
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::readToString_(
 istream& theStream,
 char const* theString,
 OTC_String& theResult
)
{
  if (theString == 0 || *theString == EOS)
    return false;

  int theInput;
  size_t theLength;
  OTC_String tmpString;

  theLength = strlen(theString);

  if (theLength == 1)
    return readToChar_(theStream,*theString,theResult);

  theResult.truncate();

  theInput = theStream.peek();

  while (theInput != EOF)
  {
    if (theInput == *theString)
    {
      // We might have a match. If we
      // don't match we have to cycle
      // data through the temporary
      // string while we still might
      // have part of the data which
      // constitutes the match.

      tmpString.truncate();

      OTC_String::read(tmpString,theStream,theLength);

      if (tmpString.length() != theLength)
	return false;

      while (1)
      {
	if (tmpString == theString)
	  return true;

	theResult += tmpString[size_t(0)];
	tmpString.remove(0,1);

	while (!tmpString.isEmpty())
	{
	  theInput = tmpString[size_t(0)];

	  if (theInput == *theString)
	    break;

	  theResult += theInput;
	  tmpString.remove(0,1);
	}

	if (tmpString.isEmpty())
	  break;

	OTC_String::read(tmpString,theStream,theLength-tmpString.length());

	if (tmpString.length() != theLength)
	  return false;
      }
    }
    else
      theResult += theStream.get();

    theInput = theStream.peek();
  }

  if (theInput == EOF)
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_XmlRpcServlet::decodeToChar_(
 istream& theStream,
 char theChar,
 OTC_String& theResult
)
{
  theResult.truncate();

  int theInput;
  OTC_String tmpString;

  theInput = theStream.peek();

  while (theInput != EOF && theInput != theChar)
  {
    theStream.ignore(1);

    if (theInput == '&')
    {
      // This should be a character or
      // entity reference.

      // Reference ::= EntityRef | CharRef
      // CharRef ::= '&#' [0-9]+ ';' | '&#x' [0-9a-fA-F]+ ';'
      // EntityRef ::= '&' Name ';'

      theInput = theStream.peek();

      if (theInput == '#')
      {
	// Character reference.

	theStream.ignore(1);

	theInput = theStream.peek();

	if (theInput == 'x')
	{
	  // Hexadecimal.

	  if (!readHexDigits_(theStream,tmpString))
	    return false;

	  if (theStream.peek() != ';')
	    return false;

	  int v1 = 0;
	  char v2 = 0;

	  if (sscanf(tmpString,"%x%c",&v1,&v2) != 1)
	    return false;

	  if (v1 > 0xFF)
	    return false;

	  theResult += char(v1);
	}
	else if (isdigit(theInput))
	{
	  // Numeric.

	  if (!readDigits_(theStream,tmpString))
	    return false;

	  if (theStream.peek() != ';')
	    return false;

	  int v1 = 0;
	  char v2 = 0;

	  if (sscanf(tmpString,"%u%c",&v1,&v2) != 1)
	    return false;

	  if (v1 > 0xFF)
	    return false;

	  theResult += char(v1);
	}
	else
	  return false;
      }
      else
      {
	// Entity reference.

	if (!readName_(theStream,tmpString))
	  return false;

	if (theStream.peek() != ';')
	  return false;

	if (tmpString == "amp")
	  theResult += '&';
	else if (tmpString == "lt")
	  theResult += '<';
	else if (tmpString == "gt")
	  theResult += '>';
	else if (tmpString == "apos")
	  theResult += '\'';
	else if (tmpString == "quot")
	  theResult += '"';
	else
	  return false;

	theStream.ignore(1);
      }
    }
    else
      theResult += theInput;

    theInput = theStream.get();
  }

  if (theInput == EOF)
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcServlet::encodeString_(
 ostream& theStream,
 char const* theString,
 size_t theLength
)
{
  static int init = 0;
  static OTC_CBitSet theChars;

  if (init == 0)
  {
    init = 1;
    theChars.set(128,128);
  }

  xmlEncode(theStream,theString,theLength,theChars);
}

/* ------------------------------------------------------------------------- */
void OTC_XmlRpcServlet::cleanup_()
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
