/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcresponse.cc
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
#pragma implementation "OTC/dispatch/response.hh"
#endif
#endif

#include <OTC/dispatch/response.hh>

#include <OTC/dispatch/message.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
int OTCEV_Response::gTypeId_ = 0;
OTC_NRMutex OTCEV_Response::mutex_;

/* ------------------------------------------------------------------------- */
OTCEV_Response::~OTCEV_Response()
{
  envelope_->destroy();

  if (content_ != 0)
    delete content_;
}

/* ------------------------------------------------------------------------- */
OTCEV_Response::OTCEV_Response(OTCEV_Envelope* theEnvelope)
  : envelope_(0),
    content_(0)
{
  OTCLIB_ENSURE_FN((theEnvelope != 0),
   "OTCEV_Response::OTCEV_Response(OTCEV_Envelope*)",
   "invalid envelope");

  envelope_ = (OTCEV_Envelope*)theEnvelope->clone();

  // content_.decode(envelope_->message()->content());
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload const&OTCEV_Response::content() const
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Response* theSelf;
  theSelf = (OTCEV_Response*)this;

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
void* OTCEV_Response::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Response::dump(ostream& outs) const
{
  outs << "<OTC> RESPONSE - recipient = " << envelope_->to();
  outs << ", sender = " << envelope_->from();
  outs << ", id = " << envelope_->conversationId();
  outs << ", seq = " << envelope_->sequenceNumber();
  outs << ", contentType = " << envelope_->message()->contentType();
  outs << ", content = " << envelope_->message()->content();
}

/* ------------------------------------------------------------------------- */
