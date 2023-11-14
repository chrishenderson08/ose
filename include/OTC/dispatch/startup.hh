#ifndef OTC_DISPATCH_STARTUP_HH
#define OTC_DISPATCH_STARTUP_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/startup.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/event.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/startup.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_Startup : public OTC_Event
    // = TITLE
    //     Event object to notify agent it can start.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTCEV_Startup> is a derived version of <OTC_Event> specifically
    //     for notifying agents that they can start. The intent is that an
    //     instance of this event would be scheduled for delivery to an agent
    //     as the last thing in the agents constructor. This allows for the
    //     final part of any initialisation to be done outside of the
    //     constructor call sequence. To be able to do this is important in a
    //     multithreaded system where starting of threads from the
    //     constructor may cause access to incompletely initialised parts of
    //     a class to occur. If an agent is using <OTC_EVHandler> or any
    //     derived classes of <OTC_EVHandler>, it must cause an instance of
    //     <OTCEV_Startup> to be delivered to itself. The <OTCEV_Startup>
    //     event might be detected by a derived version of the <accept()>
    //     member function of <OTC_EVAgent> and result in a start up member
    //     function of the class being called as well as the <startup()>
    //     member function of any thread pools. Note that if you use
    //     <OTCEV_Startup>, you should also probably subscribe to
    //     <OTCEV_Shutdown> as a means of shutting down thread pools.
    //     
    // = SEE ALSO
    //     <OTC_Event>, <OTC_EVAgent>, <OTC_EVHandler>, <OTC_EVShutdown>
{
  public:

			~OTCEV_Startup();

    // = CONSTRUCTION

			OTCEV_Startup()
				{}
				// Creates an event object notifying an
				// agent that it should finalise any
				// initialisation.

    // = SCHEDULING

    static void		schedule(int theAgentId);
				// Schedules a job for the delivery of a
				// startup event to the agent identified by
				// ID of <theAgentId>. Job used will be a
				// priority job.

    // = DELIVERY

    static void		deliverImmediately(int theAgentId);
    				// Immediately creates a startup event and
				// delivers it to the agent identified by
				// ID of <theAgentId>.

    // = IDENTIFICATION

    void*		type() const;
				// Returns a value that uniquely identifies
				// this type of event.

    static void*	typeId()
				{ return &gTypeId_; }
				// Returns a value that uniquely identifies
				// this type of event.

    // = DEBUGGING

    void		dump(ostream& outs) const;
				// Dumps info about the event onto <outs>.

  private:

			OTCEV_Startup(OTCEV_Startup const&);
				// Do not define an implementation for this.

    OTCEV_Startup&	operator=(OTCEV_Startup const&);
				// Do not define an implementation for this.

    static int		gTypeId_;
				// Anchor for use as a type identifier.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_STARTUP_HH */
