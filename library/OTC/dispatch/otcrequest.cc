/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcrequest.cc
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
#pragma implementation "OTC/dispatch/request.hh"
#endif
#endif

#include <OTC/dispatch/request.hh>

#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/svpyload.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/strstream.h>

/* ------------------------------------------------------------------------- */
int OTCEV_Request::gTypeId_ = 0;
OTC_NRMutex OTCEV_Request::mutex_;

/* ------------------------------------------------------------------------- */
OTCEV_Request::~OTCEV_Request()
{
  envelope_->destroy();
  service_->destroy();

  if (content_ != 0)
    delete content_;
}

/* ------------------------------------------------------------------------- */
OTCEV_Request::OTCEV_Request(
 OTCEV_Envelope* theEnvelope,
 OTC_ServiceBinding* theService
)
  : envelope_(0),
    service_(0),
    content_(0)
{
  OTCLIB_ENSURE_FN((theEnvelope != 0),
   "OTCEV_Request::OTCEV_Request(OTCEV_Envelope*,OTC_ServiceBinding*)",
   "invalid envelope");
  OTCLIB_ENSURE_FN((theService != 0),
   "OTCEV_Request::OTCEV_Request(OTCEV_Envelope*,OTC_ServiceBinding*)",
   "invalid service description");

  envelope_ = (OTCEV_Envelope*)theEnvelope->clone();
  service_ = theService->clone();

  OTC_String theHeader(envelope_->header());

  istrstream ins1((char*)theHeader.string(),theHeader.length());

  OTC_String tmpLine;

  while (ins1.good())
  {
    tmpLine.readToDelim(ins1,EOL);
    if (!ins1.fail())
    {
      if (tmpLine.compare(0,"MethodName: ",12))
        methodName_ = tmpLine.after(11);
    }
  }

  // content_.decode(envelope_->message()->content());
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload const& OTCEV_Request::content() const
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Request* theSelf;
  theSelf = (OTCEV_Request*)this;

  if (theSelf->content_ == 0)
  {
    theSelf->content_ = new OTC_SVPayload();
    OTCLIB_ASSERT_M(theSelf->content_ != 0);

    theSelf->content_->decode(envelope_->message()->content());
  }

  xxxMutex.release();

  mutex_.unlock();

  return *content_;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Request::bounceRequest(
 OTC_ServiceBinding* theService,
 char const* theMethodName
)
{
  OTCLIB_ENSURE_FN((theService != 0),
   "void OTCEV_Request::bounceRequest(OTC_ServiceBinding*,char const*)",
   "invalid service description");

  if (theMethodName != 0)
  {
    // Change the method name when sent.

    OTC_String tmpContent(envelope_->message()->content());
    char const* tmpContentType = "text/xml";

    OTC_Capacity tmpCapacity(63);
    OTC_String theHeader(tmpCapacity);

    theHeader += "MethodName: ";
    theHeader += theMethodName;
    theHeader += EOL;

    OTCEV_Message* theMessage;
    theMessage = new OTCEV_Message(tmpContent,tmpContentType);
    OTCLIB_ASSERT_M(theMessage != 0);

    OTCEV_Envelope* theEnvelope;
    theEnvelope = new OTCEV_Envelope(theMessage,
     theService->serviceAddress(),envelope_->from(),"$S::REQUEST",
     theHeader,envelope_->conversationId(),envelope_->sequenceNumber());
    OTCLIB_ASSERT_M(theEnvelope != 0);

    OTC_String theConversationId(theEnvelope->conversationId());

    theEnvelope->queue(OTCLIB_STANDARD_JOB);
  }
  else
    envelope_->bounce(theService->serviceAddress(),OTCLIB_STANDARD_JOB);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Request::sendResponse(OTC_ROPayload const& theContent)
{
  OTC_String tmpContent(theContent.encode());
  char const* tmpContentType = "text/xml";

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(tmpContent,tmpContentType);
  OTCLIB_ASSERT_M(theMessage != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(theMessage,
   envelope_->from(),envelope_->to(),"$S::RESPONSE",0,
   envelope_->conversationId(),envelope_->sequenceNumber()+1);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  theEnvelope->queue(OTCLIB_STANDARD_JOB);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Request::sendFailure(
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
{
  OTC_SVPayload thePayload;
  thePayload["error"] <<= theError;
  thePayload["description"] <<= theDescription;
  thePayload["origin"] <<= theOrigin;
  thePayload["details"] <<= theDetails;

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(thePayload.encode());
  OTCLIB_ASSERT_M(theMessage != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(theMessage,
   envelope_->from(),envelope_->to(),"$S::FAILURE",0,
   envelope_->conversationId(),envelope_->sequenceNumber());
  OTCLIB_ASSERT_M(theEnvelope != 0);

  theEnvelope->queue(OTCLIB_STANDARD_JOB);
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Request::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Request::dump(ostream& outs) const
{
  outs << "<OTC> REQUEST - recipient = " << envelope_->to();
  outs << ", sender = " << envelope_->from();
  outs << ", id = " << envelope_->conversationId();
  outs << ", seq = " << envelope_->sequenceNumber();
  outs << ", methodName = " << methodName_;
  outs << ", contentType = " << envelope_->message()->contentType();
  outs << ", content = " << envelope_->message()->content();
}

/* ------------------------------------------------------------------------- */
