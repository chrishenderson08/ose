#ifndef OTC_DISPATCH_SUBJECT_HH
#define OTC_DISPATCH_SUBJECT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/subject.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/thread/rwmutex.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/dispatch/jobqueue.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/hashtble.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/subject.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Subject
    // = TITLE
    //     Base class for an observerable object.
    //
    // = DESCRIPTION
    //     The <OTC_Subject> class is a base class for any class which
    //     other classes may wish to observe. For example, interested
    //     classes may be notified when the state of the subject has changed.
    //     Notification of the type of change is done by distributing an
    //     event class to interested parties.
    //
    // = NOTES
    //     Note that each instance of <OTC_Subject> is independently thread
    //     safe. You do not need to perform external locking when using the
    //     class.
    //     
    // = SEE ALSO
    //     <OTC_Event>
{
  public:

    virtual		~OTC_Subject();

    // = INITIALISATION

			OTC_Subject();
				// Initially indicates no interest in
				// this subject.

    // = SUBSCRIPTION

    void		addObserver(
			 int theAgentId,
			 char const* thePattern=0
			);
				// Events with subject matching <thePattern>
				// should be sent to the agent identified by
				// <theAgentId>. <thePattern> is actually a
				// glob style pattern and will be matched as
				// such. Note that if <thePattern> is <0>,
				// any events will be sent to <theAgentId>.
				// Ie., <thePattern> equal to <0> is the
				// same as it being <"*">.

    void		removeObserver(
			 int theAgentId,
			 char const* thePattern=0
			);
				// Events with subject matching <thePattern>
				// should no longer be sent to the agent
				// identified by <theAgentId>.

    void		removeAllObservers();
				// None of the current observers will be
				// regarded as being interested in this
				// object.

    bool		observersExist() const;
				// Returns <true> if any observers have been
				// registered against this object.

    bool		observersExist(char const* theSubject) const;
				// Returns <true> if any observers have been
				// registered against this object which have
				// defined a pattern which would match
				// <theSubject>.

    // = NOTIFICATION

    void		notifyObservers(
			 OTC_Event* theEvent,
			 char const* theSubject,
			 int theType=OTCLIB_STANDARD_JOB
			) const;
				// Broadcasts <theEvent> to observers
				// interested in <theSubject>. By default the
				// event is delivered as a standard job. This
				// can be overridden by setting <theType>.
				// Note that if <theSubject> is <0>,
				// <theEvent> is only delivered to observers
				// interested in any anything.

    void		notifyObservers(
			 OTC_Event* theEvent,
			 int theType=OTCLIB_STANDARD_JOB
			) const
				{ notifyObservers(theEvent,0,theType); }
				// Broadcasts <theEvent> to observers
				// interested in anything. By default the
				// event is delivered as a standard job. This
				// can be overridden by setting <theType>.

  private:

    OTC_RWMutex		mutex_;
				// Mutex for accessing lists of observers.
				// This is on a per instance basis and not
				// shared.

    OTC_LinkList*	observers_;
				// List of observers.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SUBJECT_HH */
