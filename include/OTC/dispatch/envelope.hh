#ifndef OTC_DISPATCH_ENVELOPE_HH
#define OTC_DISPATCH_ENVELOPE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/envelope.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/event.hh>
#include <OTC/system/program.hh>
#include <OTC/thread/mutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/envelope.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_EnvelopeContent;

class OSE_EXPORT OTCEV_Envelope : public OTC_Event
    // = TITLE
    //	   Envelope class containing an event and where to send it.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     The class <OTCEV_Envelope> contains an event. The envelope
    //     records the destination address and optional return address to
    //     which any response to the event should be sent. The envelope
    //     holds a conversation id and a sequence number. The conversation
    //     id should be used to match a reply to a request. The sequence
    //     number should be used to indicate the specific message within in a
    //     sequence of messages exchanged between two agents corresponding to
    //     the same conversation. For example, the initial request should have
    //     a sequence number of <0>, the reply should be given a sequence
    //     number of <1>, a reply to the reply should be given a sequence
    //     number of <2> etc. If desired, a subject and header may also be
    //     supplied. For consistancy, the header should in the form of
    //     key/value pairs. The key being at the start of the line followed
    //     by a ':'. Some features of this class will rely on the header
    //     being in this form and will search for specific key values.
    //
    // = BEGIN<CODE>
    //       Field1: Value
    //       Field2: Value
    // = END<CODE>
    //
    //     The destination and return addresses consist of a list of
    //     receivers names separated by an "!", this list being interpreted
    //     as a path to follow. That is, when a envelope is being delivered,
    //     it will be passed off to the recevier whose name is the first in
    //     the list of receivers in the destination address. It is assumed
    //     that that recipient knows what to do, if anything, with any
    //     further receiver names listed in the destination address. The
    //     typical scenario is that a receiver might forward the envelope to
    //     the next receiver and so on. This addressing mechanism is
    //     therefore much like UUCP style addressing, where the address is a
    //     series of nodes through which the message should pass to get to
    //     its eventual recipient. The only exception to this behaviour is
    //     that if a "*" is found to be the next receiver name. In this case,
    //     the remainder of the address is ignored and instead, the header is
    //     checked for lines starting with the key "To: ". Following the colon
    //     should be a single space and then the valid destination address.
    //     An address can't have a new line embedded within it. Instead of
    //     being delivered to only one receiver, the envelope will instead be
    //     delivered to the first receiver listed in each of the destination
    //     addresses given in the "To: " lines. That is, you can have more
    //     than one "To: " line and thus more than one address. This therefore
    //     provides a way of sending to multiple destinations without you
    //     having to create separate envelopes for the event. Separate
    //     envelopes will instead be created for you, although, the same
    //     conversation ID and sequence number will be used. Any lines in the
    //     header besides the "To: " lines will also be preserved. The
    //     implementation will try and identify where multiple addresses
    //     share a common first receiver and use a "*" address to avoid
    //     creating separate envelopes when necessary. This is important
    //     where the first receiver might be on a different machine as it
    //     means that only one message and not multiple messages get sent
    //     over any interprocess communications medium. It does however mean
    //     that receivers potentially have to know to look for the "*" and
    //     treat it specially, interogating the "To:" fields in the header.
    //     Also be aware that there is more work in evaluating "*". However,
    //     for most cases the extra efficiency gained in not sending
    //     messages multiple times over a communications link should outway
    //     the extra time and memory overhead involved in the calculation.
    //
    // = NOTES
    //     To avoid too much manipulation of data structures, searching and
    //     memory allocations, the current handling of "*" to send to
    //     multiple recipients uses a very simplistic algorithm. That is,
    //     it will linearly search through "To: " addresses and for adjacent
    //     addresses with the same initial prefix, it will use one message.
    //     If the initial prefix changes, the outstanding message is sent.
    //     This means that if addresses are ordered, the minimum number
    //     of messages will be used as they travel through the network of
    //     processes. If address with the same prefix are interleaved
    //     amongst addresses with different prefixes, separate messages will
    //     be sent for addresses with the same prefix. In short, order
    //     your destination addresses. If using tree based structures for
    //     holding addresses, addresses should be in order to start with.
    //
    // = SEE ALSO
    //	   <OTC_Event>, <OTC_Receiver>, <OTCEV_Message>
{
  public:

    // = CONSTRUCTION

			OTCEV_Envelope(
			 OTC_Event* theEvent,
			 char const* theToAddress,
			 char const* theFromAddress,
			 char const* theConversationId=0,
			 int theSequenceNumber=0
			);
				// Creates an envelope to hold <theEvent>.
				// <theToAddress> is the path of the receiver
				// to which <theEvent> is to be delivered.
				// <theFromAddress> is the return path to
				// which any reply should be sent.
				// <theConversationId> should be a value to
				// uniquely identify this exchange of events
				// as being an instance of a conversation. If
				// <theConversationId> is <0> an internal
				// conversation id will be generated.
				// <theSequenceNumber> should identify the
				// specific message within a sequence of
				// exchanged messages corresponding to the
				// same conversation. The default value for
				// <theSequenceNumber> is <0>. In this
				// constructor, the subject and header
				// will be empty.

			OTCEV_Envelope(
			 OTC_Event* theEvent,
			 char const* theToAddress,
			 char const* theFromAddress,
			 char const* theSubject,
			 char const* theHeader,
			 char const* theConversationId=0,
			 int theSequenceNumber=0
			);
				// Creates an envelope to hold <theEvent>.
				// <theToAddress> is the path of the receiver
				// to which <theEvent> is to be delivered.
				// <theFromAddress> is the return path to
				// which any reply should be sent.
				// <theConversationId> should be a value to
				// uniquely identify this exchange of events
				// as being an instance of a conversation. If
				// <theConversationId> is <0> an internal
				// conversation id will be generated.
				// <theSequenceNumber> should identify the
				// specific message within a sequence of
				// exchanged messages corresponding to the
				// same conversation. The default value for
				// <theSequenceNumber> is <0>. In this
				// constructor, a subject and header can
				// be supplied via the arguments <theSubject>
				// and <theHeader>.

			OTCEV_Envelope(
			 OTCEV_Envelope* theEnvelope,
			 char const* theToAddress,
			 char const* theFromAddress
			);
				// Creates an envelope to hold the same event
				// as in <theEnvelope>. The same subject,
				// header, conversation id and sequence number
				// as is in <theEnvelope> will be used.
				// <theToAddress> and <theFromAddress> will
				// be used as the new destination and return
				// address. Using this constructor as opposed
				// to obtaining the event from an envelope
				// and using the constructor taking an event,
				// is that if the event is in its deflated
				// form, it will not be inflated but will be
				// used in its deflated form.

			OTCEV_Envelope(
			 OTCEV_Envelope* theEnvelope,
			 char const* theToAddress,
			 char const* theFromAddress,
			 char const* theSubject,
			 char const* theHeader,
			 char const* theConversationId,
			 int theSequenceNumber=0
			);
				// Creates an envelope to hold the same
				// event as in <theEnvelope>.
				// <theConversationId> should be a value to
				// uniquely identify this exchange of events
				// as being an instance of a conversation. If
				// <theConversationId> is <0> an internal
				// conversation id will be generated.
				// <theSequenceNumber> should identify the
				// specific message within a sequence of
				// exchanged messages corresponding to the
				// same conversation. The default value for
				// <theSequenceNumber> is <0>. <theToAddress>
				// and <theFromAddress> will be used as the
				// new destination and return address. Using
				// this constructor as opposed to obtaining
				// the event from an envelope and using the
				// constructor taking an event, is that if
				// the event is in its deflated form, it will
				// not be inflated but will be used in its
				// deflated form. A subject and header have
				// to be supplied via the arguments
				// <theSubject> and <theHeader> if if they
				// will be empty. All except the last
				// argument have to be explicitly set to
				// ambiguities in which constructor will
				// be used.

    // = ACCESS

    OTCEV_Message*	message() const;
				// Returns the event in its deflated message
				// form. If you need to keep the message
				// beyond the life time of the envelope you
				// will need to make a clone of it. If the
				// event only previously existed in its
				// inflated form, calling this function will
				// cause it to be deflated and cached.

    OTC_Event*		event() const;
				// Returns the event in its inflated form.
				// If you need to keep the event beyond the
				// life time of the envelope you will need to
				// make a clone of it. If the event only
				// previously existed in its deflated form,
				// calling this function will cause it to be
				// inflated and the inflated form cached. If
				// the event could not be inflated from its
				// deflated form, this function will return
				// the event in its deflated form instead.
				// To register how a deflated event
				// is inflated use <OTCEV_Message>.

    // = ADDRESSING

    OTC_String const&	to() const
				{ return to_; }
				// Returns the destination address.

    OTC_String const&	from() const
				{ return from_; }
				// Returns the return address. If no return
				// address was provided, the address will be
				// empty.

    // = ROUTING

    OTC_String		originalSender() const;
				// Returns the receiver name of the node
				// from which the envelope appeared to
				// originate. Ie., the tail component of the
				// from address. Will return an empty string
				// if there was no return address specified
				// by the last node from which the envelope
				// was sent.

    OTC_String		previousSender() const;
				// Returns the receiver name of the last node
				// through which the envelope last appeared
				// to traverse. Ie., the lead component of
				// the from address. Will return an empty
				// string if there was no return address
				// specified by the last node from which the
				// envelope was sent.

    OTC_String		currentReceiver() const;
				// Returns the receiver name of the current
				// intended recipient. Ie., the lead
				// component of the destination address.

    OTC_String		nextReceiver() const;
				// Returns the next receiver name to
				// which the event contained by the envelope
				// should be delivered if the envelope has
				// already arrived at its current intended
				// recipient. Ie., the contents of the
				// destination excluding the lead component
				// and anything after the component that
				// follows the lead component. Will return an
				// empty string if the event had reached its
				// last intended recipient.

    OTC_String		finalReceiver() const;
				// Returns the receiver name which is the
				// intended eventual recipient of the
				// envelope. Ie., the tail component of the
				// destination address.

    OTC_String		nextDestination() const;
				// Returns the next destination address to
				// which the event contained by the envelope
				// should be delivered if the envelope has
				// already arrived at its current intended
				// recipient. Ie., the contents of the
				// destination excluding the lead component.
				// Will return an empty string if the event
				// had reached its last intended recipient.

    // = CATEGORISATION

    OTC_String const&	subject() const
				{ return subject_; }
				// Returns the subject.

    OTC_String const&	header() const
				{ return header_; }
				// Returns the header.

    OTC_String const&	eventType() const;
				// Returns the event type. If the event
				// doesn't exist in its deflated form,
				// calling this function will result in an
				// attempt to convert the event to its
				// deflated form. If deflating of the event
				// fails, an empty string will be returned.

    // = SEQUENCING

    OTC_String const&	conversationId() const
				{ return conversationId_; }
				// Returns the conversation id.

    int			sequenceNumber() const
				{ return sequenceNumber_; }
				// Returns the sequence number.

    // = MESSAGING

    void		deliver();
				// Deliver this envelope now. You shouldn't
				// attempt to use the envelope or the event
				// it contains after calling this function
				// as it may have been destroyed. To get
				// a copy you can keep hold of, use the
				// <clone()> function. You will need to
				// destroy the clone yourself.

    void		queue(int theType=OTCLIB_STANDARD_JOB);
				// Queues this envelope for delivery to
				// its current intended recipient. The
				// default value for <theType> is
				// <OTCLIB_STANDARD_JOB>. To define <theJob>
				// as being an idle job use <OTCLIB_IDLE_JOB>
				// and for a priority job, use
				// <OTCLIB_PRIORITY_JOB>.

    void		bounce(
			 char const* theAddress,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Forwards the event held by this envelope
				// to <theAddress>. The from address is
				// left as it was such that a reply is not
				// routed back through this same point.
				// Although the same event is used,
				// the envelope isn't and you must still
				// destroy it. If <theAddress> is <0> or is
				// an empty string, the event will be
				// forwarded to the next recipient in the
				// current to address. The same subject,
				// header, conversation id and sequence number
				// are used. The default value for <theType>
				// is <OTCLIB_STANDARD_JOB>. To define
				// <theJob> as being an idle job use
				// <OTCLIB_IDLE_JOB> and for a priority job,
				// use <OTCLIB_PRIORITY_JOB>.

    void		bounce(int theType=OTCLIB_STANDARD_JOB)
				{ bounce(0,theType); }
				// Forwards the event held by this envelope
				// to the next intended recipient. The from
				// address is left as it was such that a
				// reply is not routed back through this same
				// point. Although the same event is used,
				// the envelope isn't and you must still
				// destroy it. The same subject, header,
				// conversation id and sequence number are
				// used. The default value for <theType> is
				// <OTCLIB_STANDARD_JOB>. To define <theJob>
				// as being an idle job use <OTCLIB_IDLE_JOB>
				// and for a priority job, use
				// <OTCLIB_PRIORITY_JOB>.

    void		forward(
			 char const* theAddress,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Forwards the event held by this envelope
				// to <theAddress>. The head of the current
				// to address will be moved to the head of
				// the from address, such that a reply would
				// be routed back through the same point.
				// Although the same event is used, the
				// envelope isn't and you must still destroy
				// it. If <theAddress> is <0> or is an empty
				// string, the event will be forwarded to the
				// next recipient in the current to address.
				// The same subject, header, conversation id
				// and sequence number are used. The default
				// value for <theType> is
				// <OTCLIB_STANDARD_JOB>. To define <theJob>
				// as being an idle job use <OTCLIB_IDLE_JOB>
				// and for a priority job, use
				// <OTCLIB_PRIORITY_JOB>.

    void		forward(int theType=OTCLIB_STANDARD_JOB)
				{ forward(0,theType); }
				// Forwards the event held by this envelope
				// to the next recipient in the current to
				// address. The head of the to address will
				// be moved to the head of the from address,
				// such that a reply would be routed back
				// through the same point. Although the same
				// event is used, the envelope isn't and you
				// must still destroy it. The same subject,
				// header, conversation id and sequence number
				// are used. The default value for <theType>
				// is <OTCLIB_STANDARD_JOB>. To define
				// <theJob> as being an idle job use
				// <OTCLIB_IDLE_JOB> and for a priority job,
				// use <OTCLIB_PRIORITY_JOB>.

    void		reply(
			 OTC_Event* theEvent,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Sends <theEvent> to the recipient given in
				// the from address of this envelope. The
				// current to address will be used as the new
				// from address. The same subject, header and
				// conversation id will be used, however, the
				// sequence number will be incremented by
				// one. Note that a new envelope is created,
				// thus you still need to destroy this
				// envelope. The default value for <theType>
				// is <OTCLIB_STANDARD_JOB>. To define
				// <theJob> as being an idle job use
				// <OTCLIB_IDLE_JOB> and for a priority job,
				// use <OTCLIB_PRIORITY_JOB>.

    void		reply(
			 OTC_Event* theEvent,
			 char const* theSubject,
			 char const* theHeader,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Sends <theEvent> to the recipient given in
				// the from address of this envelope. The
				// current to address will be used as the new
				// from address. The same transacion id will
				// be used, however, the sequence number will
				// be incremented by one. An alternate subject
				// and header can be provided via the
				// arguments <theSubject> and <theHeader>.
				// Note that a new envelope is created, thus
				// you still need to destroy this envelope.
				// The default value for <theType> is
				// <OTCLIB_STANDARD_JOB>. To define <theJob>
				// as being an idle job use <OTCLIB_IDLE_JOB>
				// and for a priority job, use
				// <OTCLIB_PRIORITY_JOB>.

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
				// Dumps out address information, message
				// id and contents of message event held by
				// the envelope.

    // = CLONING

    OTC_Event*		clone();
				// If cloning of the message/event results
				// in a separate object, will clone a copy
				// of the envelope and return it. Otherwise
				// returns this envelope.

  public:

    // Following are here purely to get rid of the warnings from the
    // Sun C++ compiler about methods in base class being hidden.

    void                deliver(OTC_EVAgent* theAgent)
				{ OTC_Event::deliver(theAgent); }

    void                deliver(int theAgentId)
				{ OTC_Event::deliver(theAgentId); }

    void                deliver(void (*theFunc)(OTC_Event*))
				{ OTC_Event::deliver(theFunc); }

    OTC_String          deliver(
                         char const* theToAddress,
                         char const* theFromAddress,
                         char const* theSubject=0,
                         char const* theHeader=0
                        )
				{
				  return OTC_Event::deliver(theToAddress,
				   theFromAddress,theSubject,theHeader);
				}

    void		queue(int theAgentId, int theType)
				{ OTC_Event::queue(theAgentId,theType); }

    void		queue(OTC_Iterator<int> theAgentIds, int theType)
				{ OTC_Event::queue(theAgentIds,theType); }

    void                queue(OTC_EVAgent* theAgent, int theType)
				{ OTC_Event::queue(theAgent,theType); }

    void                queue(void (*theFunc)(OTC_Event*), int theType)
				{ OTC_Event::queue(theFunc,theType); }

    OTC_String          queue(
                         char const* theToAddress,
                         char const* theFromAddress,
                         int theType
                        )
				{
				  return OTC_Event::queue(theToAddress,
				   theFromAddress,theType);
				}

    OTC_String          queue(
                         char const* theToAddress,
                         char const* theFromAddress,
                         char const* theSubject,
                         char const* theHeader,
                         int theType
                        )
				{
				  return OTC_Event::queue(theToAddress,
				   theFromAddress,theSubject,theHeader,
				   theType);
				}

  protected:

			~OTCEV_Envelope();

  private:

			OTCEV_Envelope(OTCEV_Envelope const&);
				// Do not define an implementation for this.

    OTCEV_Envelope&	operator=(OTCEV_Envelope const&);
				// Do not define an implementation for this.


    static OTC_Mutex	mutex_;
				// Mutex for threading.

    OTC_EnvelopeContent*	content_;
				// Holds the message and event, which ever
				// happen to exist. These are kept in a
				// separate object so that together they can
				// be reference counted. Means that if new
				// envelope is created against same event,
				// but message hasn't been created, when it
				// is, all can benefit and not just the
				// single envelope that created message from
				// event.

    OTC_String		to_;
				// Destination address.

    OTC_String		from_;
				// Optional return address.

    OTC_String		subject_;
				// Optional subject.

    OTC_String		header_;
				// Optional header.

    OTC_String		conversationId_;
				// Conversation id for this message.

    int			sequenceNumber_;
				// The sequence number within the conversation.

    static int		gTypeId_;
				// Global object, the address of which
				// identifies this type of event.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_ENVELOPE_HH */
