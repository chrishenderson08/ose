/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onsactvrqst.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "ONS/agent/actvrqst.hh"
#endif
#endif

#include <ONS/agent/actvrqst.hh>

/* ------------------------------------------------------------------------- */
ONS_ActiveRequest::~ONS_ActiveRequest()
{
  serviceBinding_->destroy();
}

/* ------------------------------------------------------------------------- */
ONS_ActiveRequest::ONS_ActiveRequest(
 OTC_String const& theConversationId,
 OTC_ServiceBinding* theServiceBinding,
 OTC_String const& theMethodName
)
  : conversationId_(theConversationId),
    methodName_(theMethodName)
{
  serviceBinding_ = theServiceBinding->clone();
}

/* ------------------------------------------------------------------------- */
