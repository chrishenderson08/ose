#ifndef OTC_DISPATCH_REPORT_HH
#define OTC_DISPATCH_REPORT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/report.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/servbind.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/svpyload.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/report.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_Report : public OTC_Event
    // = TITLE
    //     Class encapsulating a publication from a service.
    //
    // = TITLE
    //     The <OTCEV_Report> class is the event delivered to a class
    //     derived from <OTC_SVBroker> when a publication has been received
    //     from another service to which it is subscribed. The
    //     <OTCEV_Report> class contains the actual event published by
    //     the subscribed service, as well as the envelope which carried the
    //     event.
    //
    // = SEE ALSO
    //     <OTC_SVBroker>, <OTCEV_Request>, <OTC_ServiceBinding>
{
  public:

			~OTCEV_Report();

    // = INITIALISATION

			OTCEV_Report(
			 OTCEV_Envelope* theEnvelope,
			 OTC_ServiceBinding* thePublisher,
			 OTC_ServiceBinding* theSubscriber
			);
				// <theEnvelope> should contain the event
				// received as a publication. <thePublisher>
				// should be the service description for the
				// service which published the event.
				// <theSubscriber> should be the service
				// description for the subscriber handle of
				// the subscribed service agent. Note that
				// this class will call <clone()> on
				// <theEnvelope>, <thePublisher> and
				// <theSubscriber>. hus, if appropriate, it
				// is your responsibility to destroy the
				// original.

    // = QUERY

    OTCEV_Envelope*	envelope() const
				{ return envelope_; }
                                // Returns the envelope actually used to
                                // carry the event from the other service.

    OTC_ServiceBinding*	publisher() const
				{ return publisher_; }
				// Returns the description of the service
				// which published the event.

    OTC_ServiceBinding*	subscriber() const
				{ return subscriber_; }
				// Returns the description of the service
				// handled for the subscribed service agent.

    OTC_String const&	subjectName() const
				{ return subjectName_; }
				// Returns the subject against which the
				// event was published. This is not the
				// same as the subject for the envelope.

    OTC_String const&	subjectPattern() const
				{ return subjectPattern_; }
				// Returns the subject pattern, ie., the
				// subscription, which matched the subject
				// name. This will return an empty string
				// with notional undefined value if this
				// was a directed report for which there
				// was no active subscription.

    OTC_ROPayload const&	content() const
      				{ return content_; }
    				// Returns the data sent with the report.

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

			OTCEV_Report(OTCEV_Report const&);
				// Do not define an implementation for this.

    OTCEV_Report&	operator=(OTCEV_Report const&);
				// Do not define an implementation for this.

    static int          gTypeId_;
                                // Anchor for use as a type identifier.

    OTCEV_Envelope*	envelope_;
				// The envelope which carried the event.

    OTC_ServiceBinding*	publisher_;
				// The description of the service that
				// published the event.

    OTC_ServiceBinding*	subscriber_;
				// The description of the service handle
				// for the subscribed service agent.

    OTC_String		subjectName_;
				// Subject against which event was published.

    OTC_String		subjectPattern_;
				// Subject pattern which matched the subject
				// name.

    OTC_SVPayload	content_;
    				// The report data.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_REPORT_HH */
