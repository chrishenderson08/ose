/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcreport.cc
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
#pragma implementation "OTC/dispatch/report.hh"
#endif
#endif

#include <OTC/dispatch/report.hh>

#include <OTC/dispatch/message.hh>

#include <OTC/stdlib/strstream.h>

/* ------------------------------------------------------------------------- */
int OTCEV_Report::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Report::~OTCEV_Report()
{
  envelope_->destroy();
  publisher_->destroy();
  subscriber_->destroy();
}

/* ------------------------------------------------------------------------- */
OTCEV_Report::OTCEV_Report(
 OTCEV_Envelope* theEnvelope,
 OTC_ServiceBinding* thePublisher,
 OTC_ServiceBinding* theSubscriber
)
  : envelope_(0),
    publisher_(0),
    subscriber_(0)
{
  OTCLIB_ENSURE_FN((theEnvelope != 0),
   "OTCEV_Report::OTCEV_Report(...)",
   "invalid envelope");
  OTCLIB_ENSURE_FN((thePublisher != 0),
   "OTCEV_Report::OTCEV_Report(...)",
   "invalid publisher service description");
  OTCLIB_ENSURE_FN((theSubscriber != 0),
   "OTCEV_Report::OTCEV_Report(...)",
   "invalid subscriber service description");

  envelope_ = (OTCEV_Envelope*)theEnvelope->clone();
  publisher_ = thePublisher->clone();
  subscriber_ = theSubscriber->clone();

  OTC_String theHeader(envelope_->header());

  istrstream ins1((char*)theHeader.string(),theHeader.length());

  OTC_String tmpLine;

  while (ins1.good())
  {
    tmpLine.readToDelim(ins1,EOL);
    if (!ins1.fail())
    {
      if (tmpLine.compare(0,"SubjectName: ",13))
        subjectName_ = tmpLine.after(12);
      else if (tmpLine.compare(0,"SubjectPattern: ",16))
        subjectPattern_ = tmpLine.after(15);
    }
  }

  content_.decode(envelope_->message()->content());
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Report::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Report::dump(ostream& outs) const
{
  outs << "<OTC> REPORT - publisher = " << publisher_->serviceAddress();
  outs << ", id = " << envelope_->conversationId();
  outs << ", subjectName = " << subjectName_;
  outs << ", subjectPattern = " << subjectPattern_;
  outs << ", contentType = " << envelope_->message()->contentType();
  outs << ", content = " << envelope_->message()->content();
}

/* ------------------------------------------------------------------------- */
