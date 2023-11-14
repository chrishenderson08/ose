/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcfailure.cc
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
#pragma implementation "OTC/dispatch/failure.hh"
#endif
#endif

#include <OTC/dispatch/failure.hh>

#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/svpyload.hh>

/* ------------------------------------------------------------------------- */
int OTCEV_Failure::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Failure::~OTCEV_Failure()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTCEV_Failure::OTCEV_Failure(OTCEV_Envelope* theEnvelope)
  : error_(-1)
{
  OTCLIB_ENSURE_FN((theEnvelope != 0),
   "OTCEV_Failure::OTCEV_Failure(OTCEV_Envelope*)",
   "invalid envelope");

  conversationId_ = theEnvelope->conversationId();

  OTC_SVPayload theContent;
  theContent.decode(theEnvelope->message()->content());

  bool theStatus;
  theStatus = true;

  (theContent["error"],theStatus) >>= error_;
  (theContent["description"],theStatus) >>= description_;
  (theContent["origin"],theStatus) >>= origin_;
  (theContent["details"],theStatus) >>= details_;

  OTCLIB_ENSURE_FN_W((theStatus != false),
   "OTCEV_Failure::OTCEV_Failure(...)",
   "Malformed failure notification.");
}

/* ------------------------------------------------------------------------- */
OTCEV_Failure::OTCEV_Failure(
 char const* theConversationId,
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
  : conversationId_(theConversationId),
    error_(theError),
    description_(theDescription),
    origin_(theOrigin),
    details_(theDetails)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Failure::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Failure::dump(ostream& outs) const
{
  outs << "<OTC> FAILURE - id = " << conversationId_;
  outs << ", error = " << error_;
  outs << ", description = " << description_;
  outs << ", origin = " << origin_;
  outs << ", details = " << details_;
}

/* ------------------------------------------------------------------------- */
