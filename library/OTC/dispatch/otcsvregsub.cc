/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcsvregsub.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/svregsub.hh"
#endif
#endif

#include <OTC/dispatch/svregsub.hh>

/* ------------------------------------------------------------------------- */
OTC_SVRegistrySubscription::~OTC_SVRegistrySubscription()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistrySubscription::addSubscription(
 int theAgentId,
 OTC_String const& theName
)
{
  if (!subscriptions_.contains(theAgentId,theName))
    subscriptions_.add(theAgentId,theName);
}

/* ------------------------------------------------------------------------- */
