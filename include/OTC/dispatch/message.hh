#ifndef OTC_DISPATCH_MESSAGE_HH
#define OTC_DISPATCH_MESSAGE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/message.hh
//
// = AUTHOR(S)
//     
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/hindex.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/message.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_Message : public OTC_Event
    // = TITLE
    //	   Event class encapsulating a message.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     The class <OTCEV_Message> encapsulates a message. Typically
    //     this will be a flattened event.
{
  public:

    // = CONSTRUCTION

			OTCEV_Message(
			 OTC_String const& theContent,
			 char const* theContentType=0,
			 char const* theEventType=0
			);
				// Creates a message to hold <theContent>. To
				// assist in unpacking of the message by
				// the recipient, a unique tag should be
				// provided which identifies the type of
				// content. This should be provided in
				// <theContentType>. If the message
				// corresponds to a flattened event,
				// <theEventType> should also be provided.
				// This will allow the flattened message to
				// be automatically reinflated at the
				// destination if a routine has been provided
				// for inflating that type of event.

    // = QUERY

    OTC_String const&	contentType() const
				{ return contentType_; }
				// Returns the content type.

    OTC_String const&	eventType() const
				{ return eventType_; }
				// Returns the event type.

    OTC_String const&	content() const
				{ return content_; }
				// Returns the message content.

    size_t		contentLength() const
				{ return content_.length(); }
				// Returns the length of the message content.

    // = MARSHALLING

    static void		callback(
			 OTC_String const& theType,
			 OTC_Event* (*theCallback)(OTCEV_Message*)
			);
				// Registers a callback function
				// <theCallback> as being able to convert a
				// message into <theType>. If <theCallback>
				// is <0>, the registration is cancelled.

    OTCEV_Message*	deflate() const;
				// Converts this event into a message. For
				// this event, this has the same affect as
				// calling <clone()> on an event.

    OTC_Event*		inflate(char const* theType=0) const;
				// Converts this message into an event. This
				// relies on a callack being supplied which
				// provides a means for converting the
				// message into the type of event which the
				// message had been created from. The value
				// <0> is returned if no routine had been
				// provided to inflate the message to the
				// necessary event type, or if the conversion
				// failed. By default, <theType> is
				// <0>. This indicates that the event type
				// specified in the message will be used to
				// determine which inflate function is to be
				// used. If <theType> is non zero, it
				// will be used instead of the event type
				// within the message, in order to work out
				// which inflate function to use.

    // = IDENTIFICATION

    void*		type() const;
				// Returns a value uniquely identifying
				// this type of event.

    static void*	typeId()
				{ return (void*)&gTypeId_; }
				// Returns a value uniquely identifying
				// this type of event.

    // = DEBUGGING

    void		dump(ostream& outs) const;
				// Dumps out the message.

  protected:

			~OTCEV_Message();

  private:

			OTCEV_Message(OTCEV_Message const&);
				// Do not define an implementation for this.

    OTCEV_Message&	operator=(OTCEV_Message const&);
				// Do not define an implementation for this.

    OTC_String		content_;
				// The message content.

    OTC_String		contentType_;
				// Type of content.

    OTC_String		eventType_;
				// Type of event.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static int		gTypeId_;
				// Global object, the address of which
				// identifies this type of event.

    static OTC_HIndex<OTC_String>*	gIndex_;
				// Index of types and the callback functions
				// for converting messages back into those
				// types.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_MESSAGE_HH */
