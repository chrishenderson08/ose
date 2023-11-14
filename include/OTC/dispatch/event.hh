#ifndef OTC_DISPATCH_EVENT_HH
#define OTC_DISPATCH_EVENT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/event.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/jobqueue.hh>
#include <OTC/dispatch/receiver.hh>
#include <OTC/text/string.hh>
#include <OTC/collctn/iterator.hh>
#include <OTC/memory/cmmnpool.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/atomic.hh>

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/event.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_EVAgent;
class OTCEV_Message;

class OSE_EXPORT OTC_Event
    // = TITLE
    //     Base class for any event object.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     <OTC_Event> is the base class for any event objects, which are
    //     to be delivered to an agent.
    //
    //     Space for derived classes will be allocated from <OTC_CommonPool>.
    //     
    // = SEE ALSO
    //     <OTC_CommonPool>, <OTC_EVAgent>, <OTC_Dispatcher>
{
  public:

    void*		operator new(size_t theSize)
				{ return OTC_CommonPool::allocate(theSize); }

    void		operator delete(void* theMem, size_t theSize)
				{ OTC_CommonPool::release(theMem,theSize); }

  public:

    // = IDENTIFICATION

    virtual void*	type() const = 0;
				// Must be redefined in a derived class
				// to return a <void*> which uniquely
				// identifies the event type. Normally,
				// a pointer to a static member variable
				// of a derived class would be used as the
				// return value.

    // = NOTIFICATION

    void		deliver(OTC_EVAgent* theAgent);
				// Deliver the event to <theAgent>
				// immediately. No attempt should be made to
				// use the event object after calling this
				// function, as the agent probably will
				// have destroyed the event object.

    void		deliver(int theAgentId);
				// Deliver the event to the agent with ID
				// <theAgentId> immediately. No attempt
				// should be made to use the event object
				// after calling this function, as the agent
				// probably will have destroyed the event
				// object.

    void		deliver(void (*theFunc)(OTC_Event*));
				// Deliver the event to <theFunc>
				// immediately. No attempt should be made to
				// use the event object after calling this
				// function, as the agent probably will have
				// destroyed the event object.

    OTC_String		deliver(
			 char const* theToAddress,
			 char const* theFromAddress,
			 char const* theSubject=0,
			 char const* theHeader=0
			);
				// Deliver the event to the receiver
				// identified by <theToAddress>.
				// <theFromAddress> should identify the
				// sender. An optional subject and header
				// can be supplied via the arguments
				// <theSubject>, and <theHeader>. An instance
				// of <OTCEV_Envelope> will be created to
				// encapsulate the event and the envelope
				// delivered. That is the recipient will
				// actually receive an event of type
				// <OTCEV_Envelope>. The value returned is
				// the conversation id generated within the
				// envelope. No attempt should be made to use
				// the event object after calling this
				// function, as the agent probably will have
				// destroyed the event object.

    void		queue(
			 int theAgentId,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Creates a job, and schedules the job with
				// the dispatcher for delivery of the event
				// at some later time, to the agent with ID
				// number <theAgentId>. <theType> defines
				// whether <theJob> is an idle job, standard
				// job, or priority job. The default value
				// for <theType> is <OTCLIB_STANDARD_JOB>. To
				// define <theJob> as being an idle job use
				// <OTCLIB_IDLE_JOB> and for a priority job,
				// use <OTCLIB_PRIORITY_JOB>.

    void		queue(
			 OTC_Iterator<int> theAgentIds,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Creates a job, and schedules the job with
				// the dispatcher for delivery of the event
				// at some later time, to the agents with IDs
				// given by the iterator <theAgentIds>. Note
				// that the iterator is not reset before it
				// is used. <theType> defines whether
				// <theJob> is an idle job, standard job, or
				// priority job. The default value for
				// <theType> is <OTCLIB_STANDARD_JOB>. To
				// define <theJob> as being an idle job use
				// <OTCLIB_IDLE_JOB> and for a priority job,
				// use <OTCLIB_PRIORITY_JOB>.

    void		queue(
			 OTC_EVAgent* theAgent,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Creates a job, and schedules the job with
				// the dispatcher for delivery of the event
				// at some later time, to <theAgent>
				// <theType> defines whether the job is an
				// idle job, standard job, or a priority job.
				// An idle job will only be delivered when
				// the dispatcher has nothing to do or would
				// block waiting for something to happen. The
				// default value for <theType> is
				// <OTCLIB_STANDARD_JOB>. To define <theJob>
				// as being an idle job use <OTCLIB_IDLE_JOB>
				// and for a priority job, use
				// <OTCLIB_PRIORITY_JOB>.

    void		queue(
			 void (*theFunc)(OTC_Event*),
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Creates a job, and schedules the job with
				// the dispatcher for delivery of the event
				// at some later time, to <theFunc>.
				// <theType> defines whether the job is an
				// idle job, standard job or a priority job.
				// An idle job will only be delivered when
				// the dispatcher has nothing to do or would
				// block waiting for something to happen. The
				// default value for <theType> is
				// <OTCLIB_STANDARD_JOB>. To define <theJob>
				// as being an idle job use <OTCLIB_IDLE_JOB>
				// and for a priority job, use
				// <OTCLIB_PRIORITY_JOB>.

    OTC_String		queue(
			 char const* theToAddress,
			 char const* theFromAddress=0,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Creates a job, and schedules the job with
				// the dispatcher for delivery of the event
				// at some later time, to the receiver
				// identified by <theToAddress>.
				// <theFromAddress> should identify the
				// sender. An instance of <OTCEV_Envelope>
				// will be created to encapsulate the event
				// and the envelope delivered. That is the
				// recipient will actually receive an event
				// of type <OTCEV_Envelope>. <theType>
				// defines whether the job is an idle job,
				// standard job, or a priority job. An idle
				// job will only be delivered when the
				// dispatcher has nothing to do or would
				// block waiting for something to happen. The
				// default value for <theType> is
				// <OTCLIB_STANDARD_JOB>. To define <theJob>
				// as being an idle job use <OTCLIB_IDLE_JOB>
				// and for a priority job, use
				// <OTCLIB_PRIORITY_JOB>. The value returned
				// is the conversation id generated within the
				// envelope.

    OTC_String		queue(
			 char const* theToAddress,
			 char const* theFromAddress,
			 char const* theSubject,
			 char const* theHeader,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Creates a job, and schedules the job with
				// the dispatcher for delivery of the event
				// at some later time, to the receiver
				// identified by <theToAddress>.
				// <theFromAddress> should identify the
				// sender. A subject and header can be
				// supplied via the arguments <theSubject>
				// and <theHeader>. An instance of
				// <OTCEV_Envelope> will be created to
				// encapsulate the event and the envelope
				// delivered. That is the recipient will
				// actually receive an event of type
				// <OTCEV_Envelope>. <theType> defines
				// whether the job is an idle job, standard
				// job or a priority job. An idle job will
				// only be delivered when the dispatcher has
				// nothing to do or would block waiting for
				// something to happen. The default value for
				// <theType> is <OTCLIB_STANDARD_JOB>. To
				// define <theJob> as being an idle job use
				// <OTCLIB_IDLE_JOB> and for a priority job,
				// use <OTCLIB_PRIORITY_JOB>. The value
				// returned is the conversation id generated
				// within the envelope.

    // = CLONING

    virtual OTC_Event*	clone();
				// Returns an instance of the event object,
				// which is substitutable for this event
				// object. By default, this function will
				// increment an internal reference count and
				// return a pointer to the same object. If an
				// event contains modifiable data, the derived
				// class should override this function to
				// create an actual copy of the event object
				// and return the copy.

    // = MARHSALLING

    virtual OTCEV_Message*	deflate() const;
				// Can be redefined in a derived class to
				// convert the event into a serialised form.
				// Default implementation returns <0>
				// indicating that event cannot be converted
				// into a serialised form.

    // = DESTRUCTION

    void		destroy();
				// Destroys the object, returning the space
				// it occupies back to the memory pool.
				// Actually, this function will only destroy
				// the event object if the internal reference
				// count, when decremented, reaches zero.

    // = DEBUGGING

    virtual void	dump(ostream& outs) const = 0;
				// Must be redefined in a derived class to
				// dump to <outs> a representation of the
				// event for debugging purposes. This
				// function will be called to dump out
				// information about an event if it has to be
				// discarded.

#if 0

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& outs,
				 OTC_Event const& theEvent
				)
				{ theEvent.dump(outs); return outs; }
				// Dumps a representation of <theEvent>
				// onto the stream <outs>.

#endif

  protected:

    // = NON DELIVERY

    virtual void	cancelSource(int theAgentId);
				// By default does nothing. Is called with
				// the ID agent <theAgentId>, when an event
				// is being discarded because the agent does
				// not exist. A derived event should override
				// this function to cancel the subscription
				// which caused the event to occur so that it
				// will not keep reoccuring.

  protected:

			OTC_Event()
			  : refCount_(1)
				{}

    virtual		~OTC_Event();

  private:

			OTC_Event(OTC_Event const&) {}
				// Do not define an implementation for this.

    OTC_Event&		operator=(OTC_Event const&);
				// Do not define an implementation for this.

    OTC_Atomic		refCount_;
				// The number of references to this event.

    static OTC_NRMutex	mutex_;
				// Lock for threads. Used to protect
				// reference counting code.
};

/* ------------------------------------------------------------------------- */

inline ostream& operator<<(ostream& outs, OTC_Event const& theEvent)
{ theEvent.dump(outs); return outs; }

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_EVENT_HH */
