#ifndef OTC_DISPATCH_SERVANNC_HH
#define OTC_DISPATCH_SERVANNC_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/servannc.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/event.hh>
#include <OTC/dispatch/servbind.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/servannc.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_ServiceStatus
{
  OTCLIB_SERVICE_AVAILABLE=0,
  OTCLIB_SERVICE_WITHDRAWN=1
};

enum OTC_ServiceSubscription
{
  OTCLIB_SERVICE_NAME=0,
  OTCLIB_SERVICE_ADDRESS=1,
  OTCLIB_SERVICE_REGISTRY=2,
  OTCLIB_SERVICE_GROUP=3
};

class OSE_EXPORT OTCEV_ServiceAnnouncement : public OTC_Event
    // = TITLE
    //     Event object for announcing service availability.
    //
    // = SEE ALSO
    //     <OTC_Event>, <OTC_SVBroker>, <OTC_SVRegistry>
{
  public:

			~OTCEV_ServiceAnnouncement();

    // = CONSTRUCTION

			OTCEV_ServiceAnnouncement(
			 OTC_ServiceBinding* theServiceBinding,
			 OTC_ServiceStatus theStatus,
			 OTC_ServiceSubscription theSubscriptionType
			);
				// Creates an event object to notify that a
				// binding has been created or destroyed for
				// the agent having <theServiceBinding>.
				// The type of notification is given by
				// the argument <theStatus>. The possible
				// values are <OTCLIB_SERVICE_AVAILABLE> and
				// <OTCLIB_SERVICE_WITHDRAWN> for <theStatus>.
				// <theSubscriptionType> indicates the type
				// of subscription in place which is the
				// reason for this announcement being sent.
				// Note that this class calls <clone()> on
				// <theServiceBinding>. Thus, if appropriate
				// you should destroy the original.

			OTCEV_ServiceAnnouncement(
			 OTC_ServiceBinding* theServiceBinding,
			 OTC_ServiceStatus theStatus,
			 OTC_ServiceSubscription theSubscriptionType,
			 OTC_String const& theSubscriptionName
			);
				// Creates an event object to notify that a
				// binding has been created or destroyed for
				// the agent having <theServiceBinding>.
				// The type of notification is given by
				// the argument <theStatus>. The possible
				// values are <OTCLIB_SERVICE_AVAILABLE> and
				// <OTCLIB_SERVICE_WITHDRAWN> for <theStatus>.
				// <theSubscriptionType> indicates the type
				// of subscription in place which is the
				// reason for this announcement being sent.
				// Where multiple subscriptions are allowed
				// for a single agent under different names,
				// <theSubscriptionName> should identify the
				// specific description. Note that this class
				// calls <clone()> on <theServiceBinding>.
				// Thus, if appropriate you should destroy
				// the original.

    // = QUERY

    OTC_ServiceBinding*	serviceBinding() const
				{ return serviceBinding_; }
				// Returns the full service description.

    OTC_ServiceStatus	status() const
				{ return status_; }
				// Returns the type of notification being
				// given in respect of the service.

    OTC_ServiceSubscription	subscriptionType() const
				{ return subscriptionType_; }
				// Returns the type of subscription which
				// resulted in the announcement being
				// generated.

    OTC_String const&	subscriptionName() const
      				{ return subscriptionName_; }
    				// Returns the name marked against the
				// subscription, usually an empty string.

    // = IDENTIFICATION

    void*		type() const;
				// Returns a value that uniquely identifies
				// this type of event.

    static void*	typeId()
				{ return &gTypeId_; }
				// Returns a value that uniquely identifies
				// this type of event.

    // = DEBUGGING

    void		dump(ostream& outs) const;
				// Dumps info about the event onto <outs>.

  protected:

    // = NON DELIVERY

    void                cancelSource(int theAgentId);
				// Unsubscribes the agent with ID
				// <theAgentId> to which this event was to be
				// delivered, from all service announcements.
				// This function will be called when
                                // the agent to which this event was to be
                                // delivered does not exist.

  private:

			OTCEV_ServiceAnnouncement(
			 OTCEV_ServiceAnnouncement const&
			);
				// Do not define an implementation for this.

    OTCEV_ServiceAnnouncement&	operator=(OTCEV_ServiceAnnouncement const&);
				// Do not define an implementation for this.

    static int		gTypeId_;
				// Anchor for use as a type identifier.

    OTC_ServiceBinding*	serviceBinding_;
				// The service binding.

    OTC_ServiceStatus	status_;
				// The type of notification.

    OTC_ServiceSubscription	subscriptionType_;
				// The type of subscription.

    OTC_String		subscriptionName_;
    				// The name of the subscription.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SERVANNC_HH */
