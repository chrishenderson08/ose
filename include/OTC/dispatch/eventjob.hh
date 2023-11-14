#ifndef OTC_DISPATCH_EVENTJOB_HH
#define OTC_DISPATCH_EVENTJOB_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/eventjob.hh
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

#include <OTC/dispatch/job.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/eventjob.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Event;

class OSE_EXPORT OTC_EventJob : public OTC_Job
    // = TITLE
    //     Holds an event and some target to which it should be delivered.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_EventJob> holds an event, and either the ID number of an
    //     agent to which it is to be delivered, or a function to which it
    //     should be delivered. Instances of the class are created by the
    //     <OTC_Event> class and scheduled with the dispatcher for later
    //     execution.
    //     
    // = SEE ALSO
    //     <OTC_JobQueue>, <OTC_Dispatcher>, <OTC_EVAgent>, <OTC_Event>
{
  public:

			~OTC_EventJob();

    // = INITIALISATION

			OTC_EventJob(int theAgentId, OTC_Event* theEvent);
				// Creates an object to hold <theEvent> and
				// <theAgentId>, the ID number of the agent
				// to which the event is to be delivered.

			OTC_EventJob(
			 void (*theFunc)(OTC_Event*),
			 OTC_Event* theEvent
			);
				// Creates an object to hold <theEvent> and
				// <theFunc>, a function to which the event
				// should be delivered.

    // = EXECUTION

    void		execute();
				// Asks the event to deliver itself to
				// the specified target.

  private:

			OTC_EventJob(OTC_EventJob const&);
				// Do not define an implementation for this.

    OTC_EventJob&	operator=(OTC_EventJob const&);
				// Do not define an implementation for this.

    int			agentId_;
				// The ID number of the agent to which
				// the event should be delivered.

    void		(*func_)(OTC_Event*);
				// The function to which the event should
				// be delivered.

    OTC_Event*		event_;
				// The event.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_EVENTJOB_HH */
