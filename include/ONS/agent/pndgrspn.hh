#ifndef ONS_AGENT_PNDGRSPN_HH
#define ONS_AGENT_PNDGRSPN_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/pndgrspn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/eventcb.hh>
#include <OTC/dispatch/request.hh>
#include <OTC/refcnt/resource.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/pndgrspn.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_PendingResponse : public OTC_Resource
    // = TITLE
    //     Holder of information about an delayed service response.
    //
    // = DESCRIPTION
    //     The <ONS_PendingResponse> class holds information about a service
    //     request where the return of the actual service response has been
    //     delayed or suspended.
{
  public:

			~ONS_PendingResponse();

    // = INITIALISATION

			ONS_PendingResponse(
			 OTCEV_Request* theRequest,
			 ONS_EventCallback* theCallback
			);
				// Records that <theCallback> should be
				// executed when processing of <theRequest>
				// is eventually resumed.

    // = DETAILS

    OTCEV_Request*	request() const
				{ return request_; }
				// Returns the details of the original
				// service request.

    ONS_EventCallback*	callback() const
				{ return callback_; }
				// Returns the callback to be executed
				// when processing of the service request
				// is to be resumed.

  private:

  			ONS_PendingResponse(ONS_PendingResponse const&);
				// Do not define an implementation for this.

    ONS_PendingResponse&	operator=(ONS_PendingResponse const&);
				// Do not define an implementation for this.

    OTCEV_Request*	request_;
    				// The original service request.

    ONS_EventCallback*	callback_;
    				// The callback to execute when processing
				// of the service request is resumed.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<ONS_PendingResponse*>
{
  public:

    static ONS_PendingResponse*  add(ONS_PendingResponse* theObject)
                                { theObject->reference(); return theObject; }

    static void         remove(ONS_PendingResponse* theObject)
                                { theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_PNDGRSPN_HH */
