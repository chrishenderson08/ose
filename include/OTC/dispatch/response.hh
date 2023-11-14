#ifndef OTC_DISPATCH_RESPONSE_HH
#define OTC_DISPATCH_RESPONSE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/response.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/svpyload.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/response.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_Response : public OTC_Event
    // = TITLE
    //     Event class encapsulating a response to a service request.
    //
    // = DESCRIPTION
    //     The <OTCEV_Response> class is the event delivered to a class
    //     associated with <OTC_SVBroker> in response to a prior request
    //     to another instance of <OTC_SVBroker>. The <OTCEV_Response>
    //     class contains the actual event sent from the other service
    //     and the instance of <OTCEV_Envelope> used to deliver it.
    //
    // = NOTES
    //     No service descriptions are provided for either sender or receiver
    //     as either may be an anonymous agent which no service binding
    //     present. In a conversation involving multiple responses to a
    //     request, you should have remembered the conversation ID and save
    //     against that some knowledge of the participants in the
    //     conversation.
    //     
    // = SEE ALSO
    //     <OTC_SVBroker>, <OTCEV_Request>
{
  public:

			~OTCEV_Response();

    // = INITIALISATION

			OTCEV_Response(OTCEV_Envelope* theEnvelope);
				// <theEnvelope> should contain the
				// event received in response to the
				// request or prior response. Note that
				// this class will call <clone()> on
				// <theEnvelope>. Thus, if appropriate
				// you should destroy the original.

    // = QUERY

    OTCEV_Envelope*	envelope() const
				{ return envelope_; }
				// Returns the actual envelope event which
				// carried the event from the other service.

    OTC_String const&	recipient() const
				{ return envelope_->to(); }
				// Returns the agent address to which the
				// response as being delivered.

    OTC_String const&	sender() const
				{ return envelope_->from(); }
				// Returns the agent address from whom the
				// event was received.

    OTC_String const&	conversationId() const
				{ return envelope_->conversationId(); }
				// Returns the conversation ID sent with the
				// event. This should be the same as the
				// original request.

    int			sequenceNumber() const
				{ return envelope_->sequenceNumber(); }
				// Returns the sequence number sent with
				// the event. This should never be <0>,
				// only the original request should have
				// sequence number of <0>.

    OTC_ROPayload const&	content() const;
    				// Returns the data sent with the response.

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

			OTCEV_Response(OTCEV_Response const&);
				// Do not define an implementation for this.

    OTCEV_Response&	operator=(OTCEV_Response const&);
				// Do not define an implementation for this.

    static int          gTypeId_;
                                // Anchor for use as a type identifier.

    static OTC_NRMutex	mutex_;
    				// Thread lock on lazy evaulation of content.

    OTCEV_Envelope*	envelope_;
				// The envelope containing the received
				// event.

    OTC_SVPayload*	content_;
    				// The response data.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_RESPONSE_HH */
