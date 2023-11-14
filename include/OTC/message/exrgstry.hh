#ifndef OTC_MESSAGE_EXRGSTRY_HH
#define OTC_MESSAGE_EXRGSTRY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/exrgstry.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/subject.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/exrgstry.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

/* ------------------------------------------------------------------------- */

class OTC_Exchange;

class OSE_EXPORT OTC_EXRegistry
    // = TITLE
    //     Holds information about exchanges present in the process.
    //
    // = DESCRIPTION
    //     The <OTC_EXRegistry> keeps track of what exchanges are active in
    //     the process and provides the ability to subscribe to information
    //     regarding changes in the state of connections managed by the
    //     exchanges. There is only ever one instance of this class. That
    //     instance if accessable through <OTC_Exchange>.
    //
    // = SEE ALSO
    //     <OTC_Exchange>
{
    friend class	OTC_Exchange;

  protected:

			~OTC_EXRegistry();

    // = INITIALISATION

			OTC_EXRegistry();
				// Creates an empty registry.

    // = EXCHANGES

    void		add(OTC_Exchange* theExchange);
				// Adds a new exchange to the registry.
				// Raises an exception if the group name
				// is the same for an existing exchange.

    void		remove(char const* theGroup);
				// Removes record of exchange from registry
				// with <theGroup>.

    // = NOTIFICATION

    void		notifyObservers(
    			 OTC_Event* theEvent,
			 int theType=OTCLIB_STANDARD_JOB
			)
				{
				  subscribers_.notifyObservers(
				   theEvent,theType);
				}
				// Delivers <theEvent> to subscribers.
				// Events would be exchange announcements.
				// By default the event is delivered as
				// a standard job. This can be overridden
				// by setting <theType>.

  public:

    // = LOOKUP

    OTC_Exchange*	lookup(char const* theGroup);
				// Returns the exchange for <theGroup>
				// or <0> if none exists.


    // = SUBSCRIPTIONS

    void		subscribe(int theAgentId);
				// Adds agent with <theAgentId> as being
				// interested in announcements about any
				// exchanges.

    void		unsubscribe(int theAgentId);
				// Removes agent with <theAgentId> as being
				// interested in announcements about any
				// exchanges.

  private:

			OTC_EXRegistry(OTC_EXRegistry const&);
				// Do not define an implementation for this.

    OTC_EXRegistry&	operator=(OTC_EXRegistry const&);
				// Do not define an implementation for this.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    OTC_Subject		subscribers_;
				// Subscribers to changes in exchanges.

    OTC_HMap<OTC_String,OTC_Exchange*>	exchanges_;
				// Set of active exchanges.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EXRGSTRY_HH */
