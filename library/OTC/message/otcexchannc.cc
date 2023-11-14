/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcexchannc.cc
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
#pragma implementation "OTC/message/exchannc.hh"
#endif
#endif

#include <OTC/message/exchannc.hh>
#include <OTC/message/exchange.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>

/* ------------------------------------------------------------------------- */
int OTCEV_ExchangeAnnouncement::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_ExchangeAnnouncement::OTCEV_ExchangeAnnouncement(
 OTC_String const& theGroup,
 OTC_String const& theLocalAddress,
 OTC_String const& theRemoteAddress,
 OTC_String const& theRemoteProcess,
 OTC_ConnectionState theState
)
  : group_(theGroup),
    localAddress_(theLocalAddress),
    remoteAddress_(theRemoteAddress),
    remoteProcess_(theRemoteProcess),
    state_(theState)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_ExchangeAnnouncement::type() const
{
  return &gTypeId_;
}

/* ------------------------------------------------------------------------- */
void OTCEV_ExchangeAnnouncement::dump(ostream& outs) const
{
  outs << "<OTC> EXCHANGE-ANNOUNCEMENT - ";
  outs << "group = " << group_ << ", ";
  outs << "localAddress = " << localAddress_ << ", ";
  outs << "remoteAddress = " << remoteAddress_ << ", ";
  outs << "remoteProcess = " << remoteProcess_ << ", ";

  if (state_ == OTCLIB_CONNECTION_ACTIVE)
    outs << "state = ACTIVE";
  else
    outs << "state = INACTIVE";
}

/* ------------------------------------------------------------------------- */
OTCEV_ExchangeAnnouncement::~OTCEV_ExchangeAnnouncement()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTCEV_ExchangeAnnouncement::cancelSource(int theAgentId)
{
  OTC_Exchange::registry()->unsubscribe(theAgentId);

  if (OTC_Dispatcher::warningsEnabled())
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTCEV_ExchangeAnnouncement" << EOL;
    theLogger << "Method: cancelSource()" << EOL;
    theLogger << "Description: specified agent doesn't exist" << EOL;
    theLogger << "Result: exchange subscription cancelled" << EOL;
    theLogger << "AgentId: " << theAgentId << EOL;
    theLogger << flush;
  }
}

/* ------------------------------------------------------------------------- */
