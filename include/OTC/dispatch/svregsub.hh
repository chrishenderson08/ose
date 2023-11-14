#ifndef OTC_DISPATCH_SVREGSUB_HH
#define OTC_DISPATCH_SVREGSUB_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/svregsub.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2002-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/tmanymap.hh>
#include <OTC/refcnt/resource.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/svregsub.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_SVRegistrySubscription : public OTC_Resource
    // = TITLE
    //     Holds the subscriptions against a service address.
    //
    // = DESCRIPTION
    //     The <OTC_SVRegistrySubscription> class holds the named agent
    //     subscriptions against service addresses within the service
    //     registry.
    //
    // = NOTES
    //     There is one instance of this class for each service address
    //     to which any subscription exists. The name associated with a
    //     subscription is not the service address but a name which the
    //     agent can identify a subscription when multiple subscriptions
    //     exist from the same agent.
{
  public:

    			~OTC_SVRegistrySubscription();

    			OTC_SVRegistrySubscription() {}

    // = SUBSCRIPTIONS

    bool		containsSubscription(
			 int theAgentId,
			 OTC_String const& theName
			) const
      				{
				  return subscriptions_.contains(
				   theAgentId,theName);
				}
				// Returns <true> if agent identified by
				// <theAgentId> holds a subscription of
				// <theName>.

    void		addSubscription(
			 int theAgentId,
			 OTC_String const& theName
			);
				// Adds subscription of <theName> for agent
				// with <theAgentId>.

    void		removeSubscription(
			 int theAgentId,
			 OTC_String const& theName
			)
    				{
				  subscriptions_.remove(theAgentId,
				   theName,OTCLIB_UNSAFE);
				}
				// Removes the subscription of <theName>
				// for agent with <theAgentId>.

    void		removeSubscription(int theAgentId)
    				{
				  subscriptions_.removeKey(
				   theAgentId,OTCLIB_UNSAFE);
				}
				// Removes all subscriptions helds by agent
				// with <theAgentId>.

    u_int		numSubscriptions() const
      				{ return subscriptions_.population(); }
				// Returns the total number of subscriptions
				// which exist.

    u_int		numSubscriptions(int theAgentId) const
      				{
				  return subscriptions_.frequencyKey(
				    theAgentId);
				}
				// Returns the number of subscriptions held
				// by the agent with <theAgentId>.

    OTC_PairIterator<int,OTC_String>	subscriptions() const
      				{ return subscriptions_.pairs(); }
				// Returns an iterator over all subscriptions.

  private:

    			OTC_SVRegistrySubscription(
			 OTC_SVRegistrySubscription const&);
				// Do not define an implementation for this.

    OTC_SVRegistrySubscription&	operator=(
			 OTC_SVRegistrySubscription const&);
				// Do not define an implementation for this.

    OTC_TManyMap<int,OTC_String>	subscriptions_;
				// Maps agent to subscriptions held by
				// that agent.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<OTC_SVRegistrySubscription*>
{
  public:

    static OTC_SVRegistrySubscription*	add(OTC_SVRegistrySubscription* theItem)
				{ theItem->reference(); return theItem; }

    static void		remove(OTC_SVRegistrySubscription* theItem)
				{ theItem->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SVREGSUB_HH */
