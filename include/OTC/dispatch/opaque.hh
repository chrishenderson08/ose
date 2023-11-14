#ifndef OTC_DISPATCH_OPAQUE_HH
#define OTC_DISPATCH_OPAQUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/opaque.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2002-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/servbind.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/message.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/opaque.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_Opaque : public OTC_Event
    // = TITLE
    //     Class encapsulating an opaque message sent to a service agent.
    //
    // = DESCRIPTION
    //     The <OTCEV_Opaque> class is the event delivered to a class
    //     associated with <OTC_SVBroker> when another service send an
    //     opaque data message to that service. The <OTCEV_Opaque> class
    //     contains the actual data sent from the other service and the
    //     instance of <OTCEV_Envelope> used to deliver it.
    //     
    // = NOTES
    //     A service description object for the sender is not provided
    //     as it may be an anonynmous agent which has no service binding
    //     present.
    //     
    // = SEE ALSO
    //     <OTC_SVBroker>, <OTC_ServiceBinding>
{
  public:

			~OTCEV_Opaque();

    // = INITIALISATION

			OTCEV_Opaque(
			 OTCEV_Envelope* theEnvelope,
			 OTC_ServiceBinding* theService
			);
				// <theEnvelope> should contain the event
				// received as a request. <theService>
				// should be the service description for
				// the service to which the request was sent.
				// Note that this class will call <clone()>
				// on both <theEnvelope> and <theService>.
				// Thus, if appropriate, it is your
				// responsibility to destroy the originals.

    // = QUERY

    OTCEV_Envelope*	envelope() const
				{ return envelope_; }
				// Returns the envelope actually used to
				// carry the event from the other service.

    OTC_ServiceBinding*	service() const
				{ return service_; }
				// Returns the description of the service
				// to which the event was being sent.

    OTC_String const&	recipient() const
				{ return envelope_->to(); }
				// Returns the agent address to which the
				// request was being delivered.

    OTC_String const&	sender() const
				{ return envelope_->from(); }
				// Returns the agent address from whom the
				// request was received.

    OTC_String const&	conversationId() const
				{ return envelope_->conversationId(); }
				// Returns the conversation ID sent with
				// the event. This is the same as that
				// returned to the requesting service when it
				// sent the event.

    int			sequenceNumber() const
				{ return envelope_->sequenceNumber(); }
				// Returns the sequence number sent with
				// the event. For a request, this should
				// always be <0>.

    OTC_String const&	messageId() const
    				{ return messageId_; }
				// Returns the user supplied message ID
				// for identifying the message.

    OTC_String const&	description() const
    				{ return description_; }
				// Returns the user supplied description
				// of the message.

    OTC_String const&	content() const
      				{ return envelope_->message()->content(); }
    				// Returns the data sent with the request.

    OTC_String const&	contentType() const
      				{ return envelope_->message()->contentType(); }
    				// Returns the content type value associated
				// with the data which was sent.

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

			OTCEV_Opaque(OTCEV_Opaque const&);
				// Do not define an implementation for this.

    OTCEV_Opaque&	operator=(OTCEV_Opaque const&);
				// Do not define an implementation for this.

    static int          gTypeId_;
                                // Anchor for use as a type identifier.

    OTCEV_Envelope*	envelope_;
				// The envelope which carried the event.

    OTC_ServiceBinding*	service_;
				// The service to whom the request was
				// being sent.

    OTC_String		messageId_;
    				// The user supplied message ID for
				// identifying the message.

    OTC_String		description_;
    				// The user supplied description of the
				// message.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_OPAQUE_HH */
