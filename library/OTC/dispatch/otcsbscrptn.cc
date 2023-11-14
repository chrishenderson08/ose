/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcsbscrptn.cc
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
#pragma implementation "OTC/dispatch/sbscrptn.hh"
#endif
#endif

#include <OTC/dispatch/sbscrptn.hh>

/* ------------------------------------------------------------------------- */
int OTCEV_Subscription::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Subscription::~OTCEV_Subscription()
{
  publisher_->destroy();
  subscriber_->destroy();
}

/* ------------------------------------------------------------------------- */
OTCEV_Subscription::OTCEV_Subscription(
 OTC_ServiceBinding* thePublisher,
 OTC_ServiceBinding* theSubscriber,
 OTC_SubscriptionStatus theStatus,
 char const* theSubject
)
  : publisher_(0),
    subscriber_(0),
    status_(theStatus),
    subject_(theSubject)
{
  OTCLIB_ENSURE_FN((thePublisher != 0),
   "OTCEV_Subscription::OTCEV_Subscription(...)",
   "invalid publisher service description");
  OTCLIB_ENSURE_FN((theSubscriber != 0),
   "OTCEV_Subscription::OTCEV_Subscription(...)",
   "invalid subscriber service description");

  publisher_ = thePublisher->clone();
  subscriber_ = theSubscriber->clone();
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Subscription::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Subscription::dump(ostream& outs) const
{
  outs << "<OTC> SUBSCRIPTION - ";
  outs << ", pub/serviceName = " << publisher_->serviceName();
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
  if (status_ == OTCLIB_SUBSCRIPTION_REQUESTED)
    outs << ", status = REQUESTED";
  else
    outs << ", status = WITHDRAWN";
  outs << ", subject = " << subject_;
}

/* ------------------------------------------------------------------------- */
