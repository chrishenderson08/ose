#ifndef OTC_DISPATCH_GRPANNC_HH
#define OTC_DISPATCH_GRPANNC_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/grpannc.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/servannc.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/grpannc.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_GroupAnnouncement : public OTC_Event
    // = TITLE
    //     Event object for announcing service group changes.
    //
    // = SEE ALSO
    //     <OTC_Event>, <OTC_SVBroker>, <OTC_SVRegistry>
{
  public:

			~OTCEV_GroupAnnouncement();

    // = CONSTRUCTION

			OTCEV_GroupAnnouncement(
			 OTC_ServiceBinding* theServiceBinding,
			 OTC_String const& theServiceGroup,
			 OTC_ServiceStatus theStatus,
			 OTC_ServiceSubscription theSubscriptionType
			);
				// Creates an event object to notify that a
				// service has either joined or left a group.
				// the agent having <theServiceBinding>.
				// <theServiceGroup> is the name of the
				// group with notification of whether the
				// service has joined or left the group
				// given by <theStatus>. The possible
				// values are <OTCLIB_SERVICE_AVAILABLE> and
				// <OTCLIB_SERVICE_WITHDRAWN> for <theStatus>,
				// the first indicating a service has joined
				// a service group and the latter that it has
				// left. <theSubscriptionType> indicates the
				// type of subscription in place which is the
				// reason for this announcement being sent.
				// Note that this class calls <clone()> on
				// <theServiceBinding>. Thus, if appropriate
				// you should destroy the original.

			OTCEV_GroupAnnouncement(
			 OTC_ServiceBinding* theServiceBinding,
			 OTC_String const& theServiceGroup,
			 OTC_ServiceStatus theStatus,
			 OTC_ServiceSubscription theSubscriptionType,
			 OTC_String const& theSubscriptionName
			);
				// Creates an event object to notify that a
				// service has either joined or left a group.
				// the agent having <theServiceBinding>.
				// <theServiceGroup> is the name of the
				// group with notification of whether the
				// service has joined or left the group
				// given by <theStatus>. The possible
				// values are <OTCLIB_SERVICE_AVAILABLE> and
				// <OTCLIB_SERVICE_WITHDRAWN> for <theStatus>,
				// the first indicating a service has joined
				// a service group and the latter that it has
				// left. <theSubscriptionType> indicates the
				// type of subscription in place which is the
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

    OTC_String const&	serviceGroup() const
      				{ return serviceGroup_; }
    				// Returns the name of the service group.

    OTC_ServiceStatus	status() const
				{ return status_; }
				// Returns the type of notification being
				// given in respect of the service
				// joining or leaving the service group.

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

			OTCEV_GroupAnnouncement(
			 OTCEV_GroupAnnouncement const&
			);
				// Do not define an implementation for this.

    OTCEV_GroupAnnouncement&	operator=(OTCEV_GroupAnnouncement const&);
				// Do not define an implementation for this.

    static int		gTypeId_;
				// Anchor for use as a type identifier.

    OTC_ServiceBinding*	serviceBinding_;
				// The service binding.

    OTC_String		serviceGroup_;
    				// The service group to which the service
				// announcement applies.

    OTC_ServiceStatus	status_;
				// The type of notification.

    OTC_ServiceSubscription	subscriptionType_;
				// The type of subscription.

    OTC_String		subscriptionName_;
    				// The name of the subscription.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_GRPANNC_HH */
