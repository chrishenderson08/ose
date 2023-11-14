/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otccongest.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/congest.hh"
#endif
#endif

#include <OTC/message/congest.hh>

/* ------------------------------------------------------------------------- */
int OTCEV_Congestion::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Congestion::OTCEV_Congestion(
 OTC_String const& theLocalAddress,
 OTC_String const& theRemoteAddress,
 OTC_CongestionStatus theStatus,
 u_int thePending
)
  : localAddress_(theLocalAddress),
    remoteAddress_(theRemoteAddress),
    status_(theStatus),
    pending_(thePending)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Congestion::type() const
{
  return &gTypeId_;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Congestion::dump(ostream& outs) const
{
  outs << "<OTC> CONGESTION - ";
  outs << "localAddress = " << localAddress_ << ", ";
  outs << "remoteAddress = " << remoteAddress_ << ", ";

  if (status_ == OTCLIB_CONNECTION_BLOCKED)
    outs << "status = BLOCKED, ";
  else if (status_ == OTCLIB_CONNECTION_CLEARED)
    outs << "status = CLEARED, ";
  else
    outs << "status = BACKLOG, ";

  outs << "pending = " << pending_;
}

/* ------------------------------------------------------------------------- */
OTCEV_Congestion::~OTCEV_Congestion()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
