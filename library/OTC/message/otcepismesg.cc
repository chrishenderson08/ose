/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcepismesg.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/epismesg.hh"
#endif
#endif

#include <OTC/message/epismesg.hh>

#include <OTC/message/connect.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#if defined(OSE_SYS_UNIX)
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#if defined(shutdown)
#undef shutdown
#endif
#endif

#if defined(OSE_SYS_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include <memory.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EPISMessage");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EPISMessage::mutex_;
char OTC_EPISMessage::gMagic_[4] = { '\252', '\252', '0', '3' };

/* ------------------------------------------------------------------------- */
OTC_EPISMessage::~OTC_EPISMessage()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::~OTC_EPISMessage()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_EPISMessage::OTC_EPISMessage(OTC_EndPoint* theEndPoint)
  : OTC_EPISStream(theEndPoint)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::OTC_EPISMessage()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EPISMessage::connectionActive()
{
  if (type() == OTCLIB_ENDPOINT_CLIENT)
  {
    OTCEV_Message* theMessage;
    theMessage = new OTCEV_Message("");

    OTCEV_Envelope* theEnvelope;
    theEnvelope = new OTCEV_Envelope(theMessage,"",
     localAddress(),"$EP::ANNOUNCE","",0,0);

    queueOutgoing(theEnvelope);
  }

  suspendOutput();
}

/* ------------------------------------------------------------------------- */
bool OTC_EPISMessage::processIncoming(OTC_String& theBuffer)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::processIncoming()");

  OTCLIB_TRACER(MODULE) << "buffer = " << theBuffer << endl;

  char const* tmpBuffer = theBuffer.buffer();
  size_t tmpLength = theBuffer.length();

  char const* tmpNextBuffer = tmpBuffer;
  size_t tmpNextLength = tmpLength;

  int const SZINT = sizeof(u_int);

  while (true)
  {
    // Check if input buffer is long enough so
    // as to contain magic header and length of
    // message field.

    if (tmpLength < (sizeof(gMagic_) + SZINT))
    {
      theBuffer.remove(0,tmpBuffer-theBuffer.buffer());

      return true;
    }

    // Check if magic header is valid for message.

    if (memcmp(tmpBuffer,gMagic_,sizeof(gMagic_)) != 0)
      return false;

    // Check if we have received all data which
    // corresponds to the length of the message.

    u_int theValue;
    size_t theLength;

    memcpy((char*)&theValue,tmpBuffer+sizeof(gMagic_),SZINT);
    theLength = ntohl(theValue);

    if (theLength > tmpLength-sizeof(gMagic_)-SZINT)
    {
      theBuffer.remove(0,tmpBuffer-theBuffer.buffer());

      return true;
    }

    // Determine start of next message.

    tmpNextBuffer += (sizeof(gMagic_) + SZINT + theLength);
    tmpNextLength -= (sizeof(gMagic_) + SZINT + theLength);

    OTCLIB_TRACER(MODULE) << "next = " << tmpNextLength << endl;

    // Move to actual start of message data.

    tmpBuffer += sizeof(gMagic_) + SZINT;

    // The protocol.

    memcpy((char*)&theValue,tmpBuffer,SZINT);

    u_int theProtocol = ntohl(theValue);

    OTCLIB_TRACER(MODULE) << "protocol = " << theProtocol << endl;

    if (theProtocol != 0)
      return false;

    tmpBuffer += SZINT;
    theLength -= SZINT;

    // The destination.

    OTC_String theToAddress(tmpBuffer);

    if ((theToAddress.length() + 1) > theLength)
      return false;

    tmpBuffer += theToAddress.length() + 1;
    theLength -= theToAddress.length() + 1;

    OTCLIB_TRACER(MODULE) << "to = " << theToAddress << endl;

    if (theLength == 0)
      return false;

    // The source.

    OTC_String theFromAddress(tmpBuffer);

    if ((theFromAddress.length() + 1) > theLength)
      return false;

    tmpBuffer += theFromAddress.length() + 1;
    theLength -= theFromAddress.length() + 1;

    OTCLIB_TRACER(MODULE) << "from = " << theFromAddress << endl;

    if (theLength == 0)
      return false;

    // The subject.

    OTC_String theSubject(tmpBuffer);

    if ((theSubject.length() + 1) > theLength)
      return false;

    tmpBuffer += theSubject.length() + 1;
    theLength -= theSubject.length() + 1;

    OTCLIB_TRACER(MODULE) << "subject = " << theSubject << endl;

    if (theLength == 0)
      return false;

    // The header.

    OTC_String theHeader(tmpBuffer);

    if ((theHeader.length() + 1) > theLength)
      return false;

    tmpBuffer += theHeader.length() + 1;
    theLength -= theHeader.length() + 1;

    OTCLIB_TRACER(MODULE) << "header = " << theHeader << endl;

    if (theLength == 0)
      return false;

    // The conversation id.

    OTC_String theConversationId(tmpBuffer);

    if ((theConversationId.length() + 1) > theLength)
      return false;

    tmpBuffer += theConversationId.length() + 1;
    theLength -= theConversationId.length() + 1;

    OTCLIB_TRACER(MODULE) << "conversationId = " << theConversationId << endl;

    if (theLength == 0)
      return false;

    // The sequence number.

    size_t tmpSequenceNumberLength = strlen(tmpBuffer);

    if ((tmpSequenceNumberLength + 1) > theLength)
      return false;

    int theSequenceNumber = 0;

    sscanf(tmpBuffer,"%d",&theSequenceNumber);

    OTCLIB_TRACER(MODULE) << "sequenceNumber = " << theSequenceNumber << endl;

    tmpBuffer += tmpSequenceNumberLength + 1;
    theLength -= tmpSequenceNumberLength + 1;

    if (theLength == 0)
      return false;

    // The content type.

    OTC_String theContentType(tmpBuffer);

    if ((theContentType.length() + 1) > theLength)
      return false;

    tmpBuffer += theContentType.length() + 1;
    theLength -= theContentType.length() + 1;

    OTCLIB_TRACER(MODULE) << "contentType = " << theContentType << endl;

    if (theLength == 0)
      return false;

    // The event type.

    OTC_String theEventType(tmpBuffer);

    if ((theEventType.length() + 1) > theLength)
      return false;

    tmpBuffer += theEventType.length() + 1;
    theLength -= theEventType.length() + 1;

    OTCLIB_TRACER(MODULE) << "eventType = " << theEventType << endl;

    // Now the actual message content.

    OTC_String theContent(tmpBuffer,theLength);

    OTCLIB_TRACER(MODULE) << "content = " << theContent << endl;

    // Check whether the envelope is actually
    // targeted at this endpoint.

    if (theToAddress.isEmpty())
    {
      if (theSubject == "$EP::ANNOUNCE")
      {
        // Deal with setup handshaking message.

	if (type() == OTCLIB_ENDPOINT_SERVER)
	{
	  OTCEV_Message* theMessage;
	  theMessage = new OTCEV_Message("");

	  OTCEV_Envelope* theEnvelope;
	  theEnvelope = new OTCEV_Envelope(theMessage,"",
	   localAddress(),"$EP::ANNOUNCE","",0,0);

	  queueOutgoing(theEnvelope);
	}

	bindRemoteAddress(theFromAddress);

	OTCEV_Connection* theEvent;
	theEvent = new OTCEV_Connection(localAddress(),remoteAddress(),
	 interface(),protocol(),localTransport(),remoteTransport(),
	 OTCLIB_CONNECTION_ACTIVE);
	OTCLIB_ASSERT_M(theEvent != 0);

	endPoint()->notifyObservers(theEvent,OTCLIB_PRIORITY_JOB);

	resumeOutput();
      }
    }
    else
    {
      // Construct message and envelope.

      OTCEV_Message* theEvent;
      theEvent = new OTCEV_Message(theContent,theContentType,theEventType);
      OTCLIB_ASSERT_M(theEvent != 0);

      OTCEV_Envelope* theEnvelope;
      theEnvelope = new OTCEV_Envelope(theEvent,theToAddress,theFromAddress,
       theSubject,theHeader,theConversationId,theSequenceNumber);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      // Queue it up for delivery.

      theEnvelope->queue(OTCLIB_STANDARD_JOB);
    }

    // Determine beginning of next possible
    // message in input or finish processing.

    if (tmpNextLength > 0)
    {
      tmpBuffer = tmpNextBuffer;
      tmpLength = tmpNextLength;
    }
    else
    {
      theBuffer.length(0);

      return true;
    }
  }

  return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_EPISMessage::formatOutgoing(
 OTCEV_Envelope* theEnvelope,
 OTC_String& theBuffer
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPISMessage::formatOutgoing()");

  OTCEV_Message* theMessage;
  theMessage = theEnvelope->message();

  if (theMessage == 0)
    return false;

  char tmpBuffer[32];

  int const SZINT = sizeof(u_int);

  // Adjust destination and return address
  // if required.

  OTC_String theToAddress;
  OTC_String theFromAddress;

  OTC_String theInBoxName;

  if (!theEnvelope->to().isEmpty())
  {
    theInBoxName = theEnvelope->currentReceiver();

    if (theInBoxName == remoteAddress())
    {
      theToAddress = theEnvelope->nextDestination();

      if (!theEnvelope->from().isEmpty())
	theFromAddress = localAddress() + "!" + theEnvelope->from();
    }
    else
    {
      theToAddress = theEnvelope->to();
      if (!theEnvelope->from().isEmpty())
	theFromAddress = localAddress() + "!" + theEnvelope->from();
    }
  }
  else
  {
    theFromAddress = theEnvelope->from();
  }

  // Add in magic 4 byte header header as a
  // sanity check.

  theBuffer.append(gMagic_,sizeof(gMagic_));

  // Leave space for message length.
  // Note that 32 bit systems say you use
  // u_long however on 64 bit systems you use
  // u_int. Assume that we never have to deal
  // with a 16 bit system and always use u_int
  // which is 32 bits on both 32 and 64 bit
  // systems which have so far been
  // encountered.

  theBuffer.append(EOS,SZINT);

  // Leave space for protocol field. We reserve
  // all zeros for the protocol field. Other
  // values should be used for the protocol
  // field for different header formats.

  theBuffer.append(EOS,SZINT);

  // Append destination address.

  theBuffer.append(theToAddress);
  theBuffer.append(EOS);

  // Append return address.

  theBuffer.append(theFromAddress);
  theBuffer.append(EOS);

  // Append subject.

  theBuffer.append(theEnvelope->subject());
  theBuffer.append(EOS);

  // Append header.

  theBuffer.append(theEnvelope->header());
  theBuffer.append(EOS);

  // Append conversation id.

  theBuffer.append(theEnvelope->conversationId());
  theBuffer.append(EOS);

  // Append sequence number.

  sprintf(tmpBuffer,"%d",theEnvelope->sequenceNumber());
  theBuffer.append(tmpBuffer);
  theBuffer.append(EOS);

  // Append content type.

  theBuffer.append(theMessage->contentType());
  theBuffer.append(EOS);

  // Append event type.

  theBuffer.append(theMessage->eventType());
  theBuffer.append(EOS);

  // Set message length field.

  u_int theHLength;
  u_int theNLength;

  theHLength = theBuffer.length() + theMessage->contentLength() -
   sizeof(gMagic_) - SZINT;
  theNLength = htonl(theHLength);

  theBuffer.replace(sizeof(gMagic_),SZINT,(char*)&theNLength,SZINT);

  theBuffer.append(theMessage->content(),theMessage->contentLength());

  OTCLIB_TRACER(MODULE) << "buffer = " << theBuffer << endl;

  return true;
}

/* ------------------------------------------------------------------------- */
