/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onspndgcncl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "ONS/agent/pndgcncl.hh"
#endif
#endif

#include <ONS/agent/pndgcncl.hh>

/* ------------------------------------------------------------------------- */
ONS_PendingCancellation::~ONS_PendingCancellation()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
ONS_PendingCancellation::ONS_PendingCancellation(
 OTC_String const& theConversationId,
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
  : conversationId_(theConversationId),
    error_(theError),
    description_(theDescription),
    origin_(theOrigin),
    details_(theDetails)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
