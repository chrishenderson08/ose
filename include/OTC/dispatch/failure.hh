#ifndef OTC_DISPATCH_FAILURE_HH
#define OTC_DISPATCH_FAILURE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/failure.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/message.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/failure.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_Failure : public OTC_Event
    // = TITLE
    //     Event class encapsulating a failure response to a request.
    //
    // = DESCRIPTION
    //     The <OTCEV_Failure> class is the event delivered to a class
    //     associated with <OTC_SVBroker> in response to a prior request
    //     to another instance of <OTC_SVBroker> when a failure of some sort
    //     occurs.
    //     
    // = NOTES
    //     In any conversation you should have remembered the conversation ID
    //     and saved against that some knowledge of the participants in the
    //     conversation.
    //     
    // = SEE ALSO
    //     <OTC_SVBroker>, <OTCEV_Request>
{
  public:

			~OTCEV_Failure();

    // = INITIALISATION

			OTCEV_Failure(OTCEV_Envelope* theEnvelope);
				// <theEnvelope> should contain the event
				// received in failure response to the
				// request. This class does not keep hold
				// of any reference to <theEnvelope>, so
				// it is the callers responsibility to
				// destroy it.

			OTCEV_Failure(
			 char const* theConversationId,
			 int theError,
			 char const* theDescription,
			 char const* theOrigin,
			 char const* theDetails
			);
				// Permits creation of a failure object
				// independent of an actual message event
				// signifying a failure. <theConversationId>
				// should still identify the request the
				// failure pertains to. <theError>,
				// <theDescription>, <theOrigin> and
				// <theDetails> should describe the failure.

    // = QUERY

    OTC_String const&	conversationId() const
				{ return conversationId_; }
				// Returns the conversation ID for the
				// original request.

    int			error() const
      				{ return error_; }
    				// Returns the error code identifying the
				// type of failure.

    OTC_String const&	description() const
    				{ return description_; }
				// Returns the description of the failure.

    OTC_String const&	origin() const
      				{ return origin_; }
				// Returns a string which identifies who
				// generated the error. This can also
				// act as a namespace qualifier such that
				// meaning can be applied to an error code.

    OTC_String const&	details() const
    				{ return details_; }
				// Returns additional details regarding
				// the failure.

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

			OTCEV_Failure(OTCEV_Failure const&);
				// Do not define an implementation for this.

    OTCEV_Failure&	operator=(OTCEV_Failure const&);
				// Do not define an implementation for this.

    static int          gTypeId_;
                                // Anchor for use as a type identifier.

    OTC_String		conversationId_;
				// The conversation ID from the original
				// request.

    int			error_;
    				// The error code.

    OTC_String		description_;
    				// The description of the error.

    OTC_String		origin_;
    				// The origin of the failure.

    OTC_String		details_;
    				// More details relating to the error.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_FAILURE_HH */
