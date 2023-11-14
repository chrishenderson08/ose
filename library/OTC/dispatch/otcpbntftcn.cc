/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcpbntfctn.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/pbntfctn.hh"
#endif
#endif

#include <OTC/dispatch/pbntfctn.hh>

/* ------------------------------------------------------------------------- */
int OTCEV_PublisherNotification::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_PublisherNotification::~OTCEV_PublisherNotification()
{
  publisher_->destroy();
  subscriber_->destroy();
}

/* ------------------------------------------------------------------------- */
OTCEV_PublisherNotification::OTCEV_PublisherNotification(
 OTC_ServiceBinding* thePublisher,
 OTC_ServiceBinding* theSubscriber,
 OTC_ServiceStatus theStatus
)
  : publisher_(0),
    subscriber_(0),
    status_(theStatus)
{
  OTCLIB_ENSURE_FN((thePublisher != 0),
   "OTCEV_PublisherNotification::OTCEV_PublisherNotification(...)",
   "invalid publisher service description");
  OTCLIB_ENSURE_FN((theSubscriber != 0),
   "OTCEV_PublisherNotification::OTCEV_PublisherNotification(...)",
   "invalid subscriber service description");

  publisher_ = thePublisher->clone();
  subscriber_ = theSubscriber->clone();
}

/* ------------------------------------------------------------------------- */
void* OTCEV_PublisherNotification::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_PublisherNotification::dump(ostream& outs) const
{
  outs << "<OTC> PUBLISHER-NOTIFICATION - ";
  outs << "pub/serviceName = " << publisher_->serviceName();
  outs << ", pub/serviceAudience = " << publisher_->serviceAudience();
  outs << ", pub/receiverName = " << publisher_->receiverName();
  outs << ", pub/processAddress = " << publisher_->processAddress();
  outs << ", pub/networkGroup = " << publisher_->networkGroup();
  outs << ", pub/agentAddress = " << publisher_->agentAddress();
  outs << ", pub/serviceAddress = " << publisher_->serviceAddress();
  if (publisher_->serviceLocation() == OTCLIB_SERVICE_LOCAL)
    outs << ", pub/serviceLocation = LOCAL";
  else
    outs << ", pub/serviceLocation = REMOTE";
  outs << ", sub/serviceName = " << subscriber_->serviceName();
  outs << ", sub/serviceAudience = " << subscriber_->serviceAudience();
  outs << ", sub/receiverName = " << subscriber_->receiverName();
  outs << ", sub/processAddress = " << subscriber_->processAddress();
  outs << ", sub/networkGroup = " << subscriber_->networkGroup();
  outs << ", sub/agentAddress = " << subscriber_->agentAddress();
  outs << ", sub/serviceAddress = " << subscriber_->serviceAddress();
  if (subscriber_->serviceLocation() == OTCLIB_SERVICE_LOCAL)
    outs << ", sub/serviceLocation = LOCAL";
  else
    outs << ", sub/serviceLocation = REMOTE";
  if (status_ == OTCLIB_SERVICE_AVAILABLE)
    outs << ", status = AVAILABLE";
  else
    outs << ", status = WITHDRAWN";
}

/* ------------------------------------------------------------------------- */
