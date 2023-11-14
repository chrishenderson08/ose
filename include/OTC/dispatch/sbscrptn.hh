#ifndef OTC_DISPATCH_SBSCRPTN_HH
#define OTC_DISPATCH_SBSCRPTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/sbscrptn.hh
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
#pragma interface "OTC/dispatch/sbscrptn.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_SubscriptionStatus
{
  OTCLIB_SUBSCRIPTION_REQUESTED=0,
  OTCLIB_SUBSCRIPTION_WITHDRAWN=1
};

class OSE_EXPORT OTCEV_Subscription : public OTC_Event
    // = TITLE
    //     Notifies of subscription/unsubscription to a subject.
    //
    // = DESCRIPTION
    //     The <OTCEV_Subscription> class is the event delivered to an agent
    //     associated with <OTC_SVBroker>, when another instance of
    //     <OTC_SVBroker> subscribes or unsubscribes to a subject of the
    //     first.
    //     
    // = SEE ALSO
    //     <OTC_SVBroker>, <OTC_ServiceBinding>
{
  public:

			~OTCEV_Subscription();

    // = INITIALISATION

			OTCEV_Subscription(
			 OTC_ServiceBinding* thePublisher,
			 OTC_ServiceBinding* theSubscriber,
			 OTC_SubscriptionStatus theStatus,
			 char const* theSubject=0
			);
				// <thePublisher> should be the service
				// binding for the service agent publishing
				// events. <theSubscriber> should be the
				// service binding for the service handle of
				// the subscriber. <theStatus> indicates if
				// the subscription is being requested or
				// withdrawn. Appropriate values are
				// <OTCLIB_SUBSCRIPTION_REQUESTED> and
				// <OTCLIB_SUBSCRIPTION_WITHDRAWN>.
				// <theSubject> defines the subject of
				// interest to the subscriber. Note that
				// this class calls <clone()> on
				// <thePublisher> and <theSubscriber> and
				// thus if appropriate, it is your
				// responsibility to destroy the original.

    // = QUERY

    OTC_ServiceBinding*	publisher() const
				{ return publisher_; }
				// Returns the service binding of the
				// publisher.


    OTC_ServiceBinding*	subscriber() const
				{ return subscriber_; }
				// Returns the service binding of subscriber.

    OTC_SubscriptionStatus	status() const
				{ return status_; }
				// Returns <OTCLIB_SUBSCRIPTION_REQUESTED>
				// if subscription is requested and the
				// value <OTCLIB_SUBSCRIPTION_WITHDRAWN>
				// if the subscription is cancelled.

    OTC_String const&	subject() const
				{ return subject_; }
				// Returns the subject filter associated
				// with the subscription/unsubscription
				// request.

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

			OTCEV_Subscription(OTCEV_Subscription const&);
				// Do not define an implementation for this.

    OTCEV_Subscription&	operator=(OTCEV_Subscription const&);
				// Do not define an implementation for this.

    static int          gTypeId_;
                                // Anchor for use as a type identifier.

    OTC_ServiceBinding*	publisher_;
				// Service binding of the publisher.

    OTC_ServiceBinding*	subscriber_;
				// Service binding of the subscriber.

    OTC_SubscriptionStatus	status_;
				// Whether subscription has been added or
				// withdrawn.

    OTC_String		subject_;
				// The subject filter.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SBSCRPTN_HH */
