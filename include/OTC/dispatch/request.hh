#ifndef OTC_DISPATCH_REQUEST_HH
#define OTC_DISPATCH_REQUEST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/request.hh
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
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/request.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_Request : public OTC_Event
    // = TITLE
    //     Class encapsulating a request sent to a service agent.
    //
    // = DESCRIPTION
    //     The <OTCEV_Request> class is the event delivered to a class
    //     associated with <OTC_SVBroker> when another service send a request
    //     to that service. The <OTCEV_Request> class contains the actual
    //     request payload sent from the other service and the instance of
    //     <OTCEV_Envelope> used to deliver it.
    //
    // = NOTES
    //     A service description object for the sender is not provided
    //     as it may be an anonynmous agent which has no service binding
    //     present.
    //     
    // = SEE ALSO
    //     <OTC_SVBroker>, <OTCEV_Response>, <OTC_ServiceBinding>
{
  public:

			~OTCEV_Request();

    // = INITIALISATION

			OTCEV_Request(
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

    OTC_String const&	methodName() const
    				{ return methodName_; }
				// Returns the name of the method of the
				// service which the message is targeted at.

    OTC_ROPayload const&	content() const;
    				// Returns the data sent with the request.

    // = BOUNCE

    void		bounceRequest(
			 OTC_ServiceBinding* theService,
			 char const* theMethodName=0
			);
				// Bounces the request onto <theService>.
				// Used if service acted as front end onto a
				// series of backend instances across which
				// the work is shared. If <theMethodName>
				// is not <0>, the bounced request will now
				// use <theMethodName> instead of that which
				// the request originally held.

    // = REPLY

    void		sendResponse(OTC_ROPayload const& theContent);
				// Called to send back a response to the
				// agent from which the request was received.
				// The conversation ID will be the same as
				// that for this request but with the
				// sequence number one greater. The body
				// of the response is given by <theContent>.

    void		sendFailure(
			 int theError,
			 char const* theDescription,
			 char const* theOrigin=0,
			 char const* theDetails=0
			);
    				// Called to send back a failure event to
				// the agent from which the request was
				// received. The conversation ID and sequence
				// number will be the same as that for the
				// request. This method should be used when
				// the recipient was not able to process
				// the request. This might occur when the
				// method name wasn't supported or the
				// content could not be decoded. <theError>
				// should be an appropriate integer error
				// code corresponding to an error with
				// <theDescription>. <theOrigin> should
				// identify the source of the error.
				// Further information related to the error
				// can be given in <theDetails>.

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

			OTCEV_Request(OTCEV_Request const&);
				// Do not define an implementation for this.

    OTCEV_Request&	operator=(OTCEV_Request const&);
				// Do not define an implementation for this.

    static int          gTypeId_;
                                // Anchor for use as a type identifier.

    static OTC_NRMutex	mutex_;
    				// Thread lock on lazy evaulation of content.

    OTCEV_Envelope*	envelope_;
				// The envelope which carried the event.

    OTC_ServiceBinding*	service_;
				// The service to whom the request was
				// being sent.

    OTC_String		methodName_;
    				// The method name.

    OTC_SVPayload*	content_;
    				// The request data.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_REQUEST_HH */
