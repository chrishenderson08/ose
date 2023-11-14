#ifndef OTC_DISPATCH_RECEIVER_HH
#define OTC_DISPATCH_RECEIVER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/receiver.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/jobqueue.hh>
#include <OTC/dispatch/joblist.hh>
#include <OTC/collctn/hindex.hh>
#include <OTC/text/string.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/receiver.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Event;
class OTC_EVAgent;

class OSE_EXPORT OTC_Receiver
    // = TITLE
    //     Binds a name to an agent or function for event delivery.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_Receiver> class binds a name to an agent or a function.
    //     This allows an event to be delivered to an agent by an abstract
    //     name rather than a numeric agent id. It also allows an event to be
    //     delivered to a standalone function.
    //     
    // = NOTES
    //     The name of the receiver is created as an instance of <OTC_Symbol>
    //     but then stored as an instance of <OTC_String>. The end result is
    //     that although stored as an instance of <OTC_String> it is still
    //     in the symbol database and identified as being a symbol. All this
    //     is to cut down on amount of space used up by receiver names.
    //
    //     Dont use "$" as a leading component of the receiver name. This
    //     character as the lead character is reserved for internal use
    //     within OSE. If you use it as the leading character, you may
    //     conflict with OSE code.
{
  public:

			~OTC_Receiver();

    // = INITIALISATION

			OTC_Receiver();
				// Creates a receiver which doesn't yet bind
				// a name to anything.

			OTC_Receiver(
			 OTC_EVAgent* theAgent,
			 char const* theName
			);
				// Binds <theName> to the <theAgent>.
				// If <theAgent> is <0>, <theName> nil or
				// empty, an exception is raised.

			OTC_Receiver(OTC_EVAgent* theAgent);
				// Creates an arbitrary name and binds it to
				// <theAgent>. If <theAgent> is <0> an
				// exception will be raised. The name will be
				// guaranteed not to already be in use.

			OTC_Receiver(
			 void (*theCallback)(OTC_Event*),
			 char const* theName
			);
				// Binds <theName> to <theCallback>. If
				// <theCallback> is a nil pointer and
				// <theName> is valid, or <theCallback> is
				// valid and <theName> nil or empty, an
				// exception is raised.

			OTC_Receiver(void (*theCallback)(OTC_Event*));
				// Creates an arbitrary name and binds it to
				// <theCallback>. If <theAgent> is <0> an
				// exception will be raised. The name will be
				// guaranteed not to already be in use.

    // = ADDRESS

    OTC_String const&	name() const
				{ return name_; }
				// Returns the name of the receiver.

    // = BINDING

    void		unbind();
				// Removes the binding this class describes.

    void		bind(OTC_EVAgent* theAgent, char const* theName);
				// Binds <theName> to <theAgent>. If
				// <theAgent> is <0> or <theName> nil or
				// empty, an exception is raised. If this
				// class already binds a name to an agent or
				// function, that binding will be dropped.

    void		bind(OTC_EVAgent* theAgent);
				// Creates an arbitrary name and binds it to
				// <theAgent>. If <theAgent> is <0> an
				// exception will be raised. The name will be
				// guaranteed not to already be in use. If
				// this class already binds a name to an
				// agent or function, that binding will be
				// dropped.

    void		bind(
			 void (*theCallback)(OTC_Event*),
			 char const* theName
			);
				// Binds <theName> to <theCallback>. If
				// <theAgent> is a nil pointer and <theName>
				// is valid, or <theCallback> is valid and
				// <theName> nil or empty, an exception is
				// raised. If this class already binds a name
				// to an agent, that binding will be dropped.

    void		bind(void (*theCallback)(OTC_Event*));
				// Creates an arbitrary name and binds it to
				// <theCallback>. If <theAgent> is <0> an
				// exception will be raised. The name will be
				// guaranteed not to already be in use. If
				// this class already binds a name to an
				// agent or function, that binding will be
				// dropped.

    // = DELIVERY

    static void		deliver(
			 char const* theName,
			 OTC_Event* theEvent
			);
				// Delivers <theEvent> to all recipients
				// with <theName>. This function takes
				// responsibility for destroying <theEvent>.

    static void		queue(
			 char const* theName,
			 OTC_Event* theEvent,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Queue <theEvent> for delivery to all
				// recipients with <theName>. This function
				// takes responsibility for destroying
				// <theEvent>.

    // = RECEIVERS
    //     The lookup operation is thread protected, but what you may do with
    //     the object returned isn't. Thus, iterating over receivers, using
    //     <next()> isn't advisable in a threaded environment unless all
    //     addition and deletion of receiver objects is done in the same
    //     thread.

    static OTC_Receiver const*	lookup(char const* theName=0);
				// Returns a pointer to the first receiver in
				// a chain of receivers which are bound with
				// <theName>. If <theName> is a nil pointer
				// or is empty or <theName> doesn't describe
				// any receivers, <0> is returned.


    OTC_Receiver const*	next() const
				{ return next_; }
				// Returns next receiver in chain.

  private:

			OTC_Receiver(OTC_Receiver const&);
				//  Do not define an implementation for this.

    OTC_Receiver&	operator=(OTC_Receiver const&);
				//  Do not define an implementation for this.

    static void		init_();
				// Creates index onto receivers.

    static void		fill_(
			 OTC_JobList* theJobList,
			 char const* theName,
			 OTC_Event* theEvent
			);
				// Fills <theJobList> with individual
				// jobs where each job sends <theEvent>
				// to a different receiver with <theName>.

    static void		proxy_(OTC_Event* theEvent);
				// Acts as proxy for messages, delivering
				// message immediately onto next receiver.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    static OTC_NRMutex  receiverMutex_;
                                // Special Lock for threads when creating
				// special receivers.

    static OTC_HIndex<OTC_String>*	gIndex_;
				// Index onto receiver.

    static OTC_Receiver*	gLocalReceiverInBox_;
				// Special receiver for capturing messages
				// send to "$LOCAL-RECEIVER". Message will
				// be forwarded to receiver which is next
				// in the address.

    static OTC_Receiver*	gReceiverInBox_;
				// Special receiver for capturing messages
				// send to "$RECEIVER". Message will
				// be forwarded to receiver which is next
				// in the address. This is the same as above,
				// but this same receiver name can be used
				// elsewhere as a means of broadcasting
				// to receiver in other processes.

    static OTC_Receiver*	gAgentInBox_;
				// Special receiver for capturing messages
				// send to "$AGENT". Message will be
				// delivered to agent with agent ID given by
				// next recipient. The destination address
				// will be preserved.

    int			agentId_;
				// ID of agent.

    void		(*callback_)(OTC_Event*);
				// Pointer to a callback function.

    OTC_String		name_;
				// Name bound to the agent.

    OTC_Receiver*	prev_;
				// Pointer to previous receiver.

    OTC_Receiver*	next_;
				// Pointer to next receiver.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_RECEIVER_HH */
