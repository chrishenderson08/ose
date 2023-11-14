/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcservannc.cc
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
#pragma implementation "OTC/dispatch/servannc.hh"
#endif
#endif

#include <OTC/dispatch/servannc.hh>

#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */
int OTCEV_ServiceAnnouncement::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_ServiceAnnouncement::~OTCEV_ServiceAnnouncement()
{
  serviceBinding_->destroy();
}

/* ------------------------------------------------------------------------- */
OTCEV_ServiceAnnouncement::OTCEV_ServiceAnnouncement(
 OTC_ServiceBinding* theServiceBinding,
 OTC_ServiceStatus theStatus,
 OTC_ServiceSubscription theSubscriptionType
)
  : serviceBinding_(0),
    status_(theStatus),
    subscriptionType_(theSubscriptionType)
{
  OTCLIB_ENSURE_FN((theServiceBinding != 0),
   "OTCEV_ServiceAnnouncement::OTCEV_ServiceAnnouncement(...)",
   "invalid service binding");

  serviceBinding_ = theServiceBinding->clone();
}

/* ------------------------------------------------------------------------- */
OTCEV_ServiceAnnouncement::OTCEV_ServiceAnnouncement(
 OTC_ServiceBinding* theServiceBinding,
 OTC_ServiceStatus theStatus,
 OTC_ServiceSubscription theSubscriptionType,
 OTC_String const& theSubscriptionName
)
  : serviceBinding_(0),
    status_(theStatus),
    subscriptionType_(theSubscriptionType),
    subscriptionName_(theSubscriptionName)
{
  OTCLIB_ENSURE_FN((theServiceBinding != 0),
   "OTCEV_ServiceAnnouncement::OTCEV_ServiceAnnouncement(...)",
   "invalid service binding");

  serviceBinding_ = theServiceBinding->clone();
}

/* ------------------------------------------------------------------------- */
void* OTCEV_ServiceAnnouncement::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_ServiceAnnouncement::dump(ostream& outs) const
{
  outs << "<OTC> SERVICE-ANNOUNCEMENT - ";
  outs << ", serviceName = " << serviceBinding_->serviceName();
  outs << ", serviceAudience = " << serviceBinding_->serviceAudience();
  outs << ", receiverName = " << serviceBinding_->receiverName();
  outs << ", processAddress = " << serviceBinding_->processAddress();
  outs << ", networkGroup = " << serviceBinding_->networkGroup();
  outs << ", agentAddress = " << serviceBinding_->agentAddress();
  outs << ", serviceAddress = " << serviceBinding_->serviceAddress();
  if (serviceBinding_->serviceLocation() == OTCLIB_SERVICE_LOCAL)
    outs << ", serviceLocation = LOCAL";
  else
    outs << ", serviceLocation = REMOTE";
  if (status_ == OTCLIB_SERVICE_AVAILABLE)
    outs << ", status = AVAILABLE";
  else
    outs << ", status = WITHDRAWN";
  if (subscriptionType_ == OTCLIB_SERVICE_NAME)
    outs << ", subscriptionType = NAME";
  else if (subscriptionType_ == OTCLIB_SERVICE_ADDRESS)
    outs << ", subscriptionType = ADDRESS";
  else if (subscriptionType_ == OTCLIB_SERVICE_REGISTRY)
    outs << ", subscriptionType = REGISTRY";
  else if (subscriptionType_ == OTCLIB_SERVICE_GROUP)
    outs << ", subscriptionType = GROUP";
  else
    outs << ", subscriptionType = UNKNOWN";

  outs << ", subscriptionName = " << subscriptionName_;
}

/* ------------------------------------------------------------------------- */
void OTCEV_ServiceAnnouncement::cancelSource(int theAgentId)
{
  OTC_SVBroker::registry()->unsubscribeAgent(theAgentId);

  if (OTC_Dispatcher::warningsEnabled())
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTCEV_ServiceAnnouncement" << EOL;
    theLogger << "Method: cancelSource()" << EOL;
    theLogger << "Description: specified agent doesn't exist" << EOL;
    theLogger << "Result: service subscription cancelled" << EOL;
    theLogger << "AgentId: " << theAgentId << EOL;
    theLogger << flush;
  }
}

/* ------------------------------------------------------------------------- */
