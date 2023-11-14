#ifndef OTC_DISPATCH_PBNTFCTN_HH
#define OTC_DISPATCH_PBNTFCTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/pbntfctn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/servannc.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/pbntfctn.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_PublisherNotification : public OTC_Event
    // = TITLE
    //     Notifies of publisher being available.
    //
    // = DESCRIPTION
    //     The <OTCEV_PublisherNotification> class is the event delivered to
    //     an agent associated with a <OTC_SVBroker>, when another instance
    //     of <OTC_SVBroker> comes into existance for which a pending
    //     service subscription matches. Similarly used to notify when a
    //     service dissappears.
    //     
    // = SEE ALSO
    //     <OTC_SVBroker>, <OTC_ServiceBinding>
{
  public:

			~OTCEV_PublisherNotification();

    // = INITIALISATION

			OTCEV_PublisherNotification(
			 OTC_ServiceBinding* thePublisher,
			 OTC_ServiceBinding* theSubscriber,
			 OTC_ServiceStatus theStatus
			);
				// <thePublisher> should be the service
				// description for the service agent
				// publishing events. <theSubscriber> should
				// be the service description for the service
				// handle of the subscriber. <theStatus>
				// indicates if the subscription is being
				// requested or withdrawn. Appropriate values
				// are <OTCLIB_SERVICE_AVAILABLE> and
				// <OTCLIB_SERVICE_WITHDRAWN>. Note that
				// this class calls <clone()> on
				// <thePublisher> and <theSubscriber> and
				// thus if appropriate, it is your
				// responsibility to destroy the original.

    // = QUERY

    OTC_ServiceBinding*	publisher() const
				{ return publisher_; }
				// Returns the service description of the
				// publisher.

    OTC_ServiceBinding*	subscriber() const
				{ return subscriber_; }
				// Returns the service description of the
				// service handle of the subscribing service
				// agent.

    OTC_ServiceStatus	status() const
				{ return status_; }
				// Returns <OTCLIB_SERVICE_AVAILABLE>
				// if publisher becomes available and the
				// value <OTCLIB_SERVICE_WITHDRAWN>
				// if the publisher no longer exists.

    // = IDENTIFICATION

    void*               type() const;
                                // Returns a value that uniquely identifies
                                // this type of event.

    static void*        typeId()
                                { return &gTypeId_; }
                                // Returns a value that uniquely identifies
                                // this type of event.

    // = DEBUGGING

    void                dump(ostream& outs) const;
                                // Dumps info about the event onto <outs>.

  private:

			OTCEV_PublisherNotification(
			 OTCEV_PublisherNotification const&
			);
				// Do not define an implementation for this.

    OTCEV_PublisherNotification&	operator=(
				 OTCEV_PublisherNotification const&
				);
				// Do not define an implementation for this.

    static int          gTypeId_;
                                // Anchor for use as a type identifier.

    OTC_ServiceBinding*	publisher_;
				// Service description of the publisher.

    OTC_ServiceBinding*	subscriber_;
				// Service description of the subscriber.

    OTC_ServiceStatus	status_;
				// Whether publisher is created or destroyed.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_PBNTFCTN_HH */
