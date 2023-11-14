/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcconnect.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/connect.hh"
#endif
#endif

#include <OTC/message/connect.hh>

/* ------------------------------------------------------------------------- */
int OTCEV_Connection::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Connection::OTCEV_Connection(
 OTC_String const& theLocalAddress,
 OTC_String const& theRemoteAddress,
 OTC_String const& theLocalInterface,
 OTC_String const& theLocalProtocol,
 OTC_String const& theLocalTransport,
 OTC_String const& theRemoteTransport,
 OTC_ConnectionState theState
)
  : localAddress_(theLocalAddress),
    remoteAddress_(theRemoteAddress),
    localInterface_(theLocalInterface),
    localProtocol_(theLocalProtocol),
    localTransport_(theLocalTransport),
    remoteTransport_(theRemoteTransport),
    state_(theState)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTCEV_Connection::OTCEV_Connection(
 OTC_String const& theLocalAddress,
 OTC_String const& theRemoteAddress,
 OTC_String const& theLocalInterface,
 OTC_ConnectionState theState
)
  : localAddress_(theLocalAddress),
    remoteAddress_(theRemoteAddress),
    localInterface_(theLocalInterface),
    state_(theState)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTCEV_Connection::OTCEV_Connection(
 OTC_String const& theLocalAddress,
 OTC_String const& theRemoteAddress,
 OTC_ConnectionState theState
)
  : localAddress_(theLocalAddress),
    remoteAddress_(theRemoteAddress),
    state_(theState)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Connection::type() const
{
  return &gTypeId_;
}

/* ------------------------------------------------------------------------- */
void OTCEV_Connection::dump(ostream& outs) const
{
  outs << "<OTC> CONNECTION - ";
  outs << "localAddress = " << localAddress_ << ", ";
  outs << "remoteAddress = " << remoteAddress_ << ", ";
  outs << "localInterface = " << localInterface_ << ", ";
  outs << "localProtocol = " << localProtocol_ << ", ";
  outs << "localTransport = " << localTransport_ << ", ";
  outs << "remoteTransport = " << remoteTransport_ << ", ";

  if (state_ == OTCLIB_CONNECTION_ACTIVE)
    outs << "state = ACTIVE";
  else
    outs << "state = INACTIVE";
}

/* ------------------------------------------------------------------------- */
OTCEV_Connection::~OTCEV_Connection()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
