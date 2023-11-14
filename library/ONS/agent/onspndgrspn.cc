/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onspndgrspn.cc
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
#pragma implementation "ONS/agent/pndgrspn.hh"
#endif
#endif

#include <ONS/agent/pndgrspn.hh>

/* ------------------------------------------------------------------------- */
ONS_PendingResponse::~ONS_PendingResponse()
{
  request_->destroy();
  delete callback_;
}

/* ------------------------------------------------------------------------- */
ONS_PendingResponse::ONS_PendingResponse(
 OTCEV_Request* theRequest,
 ONS_EventCallback* theCallback
)
  : request_((OTCEV_Request*)theRequest->clone()),
    callback_(theCallback)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
