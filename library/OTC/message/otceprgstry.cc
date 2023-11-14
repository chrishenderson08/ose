/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otceprgstry.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/eprgstry.hh"
#endif
#endif

#include <OTC/message/eprgstry.hh>
#include <OTC/message/exchange.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EPRegistry");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_EPRegistry::~OTC_EPRegistry()
{
  endPoint_->unReference();
}

/* ------------------------------------------------------------------------- */
OTC_EPRegistry::OTC_EPRegistry(
 OTC_EndPoint* theEndPoint,
 char const* theExchangeGroup
)
  : endPoint_(theEndPoint),
    exchangeGroup_(theExchangeGroup),
    exchangeType_(OTCLIB_EXCHANGE_UNKNOWN)
{
  endPoint_->reference();
  exchangeName_ = OTC_Exchange::fullName(theExchangeGroup);
}

/* ------------------------------------------------------------------------- */
void OTC_EPRegistry::setup(
 OTC_ExchangeType theType,
 OTC_String const& theRemoteProcess
)
{
  exchangeType_ = theType;
  remoteProcess_ = theRemoteProcess;
  remoteAddress_ = endPoint_->remoteAddress();
}

/* ------------------------------------------------------------------------- */
void OTC_EPRegistry::reset()
{
  if (exchangeType_ != OTCLIB_EXCHANGE_UNKNOWN)
    exchangeType_ = OTCLIB_EXCHANGE_UNKNOWN;
}

/* ------------------------------------------------------------------------- */
void OTC_EPRegistry::shutdown()
{
  endPoint_->stop();
  exchangeType_ = OTCLIB_EXCHANGE_SHUTDOWN;
}

/* ------------------------------------------------------------------------- */
void OTC_EPRegistry::addService(OTC_ServiceBinding* theBinding)
{
  OTCLIB_ENSURE_FN((theBinding != 0),
   "OTC_EPRegistry::addService(OTC_ServiceBinding*)",
   "invalid service description");

  OTC_String tmpServiceAddress;
  tmpServiceAddress = theBinding->serviceAddress();

  OTCLIB_TRACER(MODULE) << "add = " << tmpServiceAddress << endl;

  services_.add(tmpServiceAddress,theBinding,OTCLIB_REPLACE);
}

/* ------------------------------------------------------------------------- */
void OTC_EPRegistry::removeService(OTC_String const& theServiceAddress)
{
  OTCLIB_TRACER(MODULE) << "remove = " << theServiceAddress << endl;

  serviceGroups_.removeItem(theServiceAddress,OTCLIB_UNSAFE);
  services_.remove(theServiceAddress,OTCLIB_UNSAFE);
}

/* ------------------------------------------------------------------------- */
