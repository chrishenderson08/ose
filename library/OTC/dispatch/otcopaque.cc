/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcopaque.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2002-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/opaque.hh"
#endif
#endif

#include <OTC/dispatch/opaque.hh>

#include <OTC/dispatch/message.hh>

#include <OTC/stdlib/strstream.h>

/* ------------------------------------------------------------------------- */
int OTCEV_Opaque::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Opaque::~OTCEV_Opaque()
{
  envelope_->destroy();
  service_->destroy();
}

/* ------------------------------------------------------------------------- */
OTCEV_Opaque::OTCEV_Opaque(
 OTCEV_Envelope* theEnvelope,
 OTC_ServiceBinding* theService
)
  : envelope_(0),
    service_(0)
{
  OTCLIB_ENSURE_FN((theEnvelope != 0),
   "OTCEV_Opaque::OTCEV_Opaque(OTCEV_Envelope*,OTC_ServiceBinding*)",
   "invalid envelope");
  OTCLIB_ENSURE_FN((theService != 0),
   "OTCEV_Opaque::OTCEV_Opaque(OTCEV_Envelope*,OTC_ServiceBinding*)",
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
      if (tmpLine.compare(0,"MessageId: ",11))
        messageId_ = tmpLine.after(10);
      if (tmpLine.compare(0,"Description: ",13))
        description_ = tmpLine.after(12);
    }
  }
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Opaque::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Opaque::dump(ostream& outs) const
{
  outs << "<OTC> OPAQUE - recipient = " << envelope_->to();
  outs << ", sender = " << envelope_->from();
  outs << ", id = " << envelope_->conversationId();
  outs << ", seq = " << envelope_->sequenceNumber();
  outs << ", messageId = " << messageId_;
  outs << ", description = " << description_;
  outs << ", contentType = " << envelope_->message()->contentType();
  outs << ", content = " << envelope_->message()->content();
}

/* ------------------------------------------------------------------------- */
