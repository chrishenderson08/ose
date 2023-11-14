#ifndef ONS_AGENT_ACTVRQST_HH
#define ONS_AGENT_ACTVRQST_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/actvrqst.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/servbind.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/actvrqst.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_ActiveRequest : public OTC_Resource
    // = TITLE
    //     Holder of information about an active service request.
    //
    // = DESCRIPTION
    //     The <ONS_ActiveRequest> class holds information about an active
    //     service request against another service agent.
{
  public:

			~ONS_ActiveRequest();

    // = INITIALISATION

			ONS_ActiveRequest(
			 OTC_String const& theConversationId,
			 OTC_ServiceBinding* theServiceBinding,
			 OTC_String const& theMethodName
			);
				// Records request against <theMethodName> of
				// service agent with <theServiceBinding>
				// where the request had <theConversationId>.

    // = DETAILS

    OTC_String const&	conversationId() const
				{ return conversationId_; }
				// Returns the conversation ID of the request.

    OTC_ServiceBinding*	serviceBinding() const
				{ return serviceBinding_; }
				// Returns the service binding object of the
				// service agent against which the request
				// was made.

    OTC_String const&	methodName() const
				{ return methodName_; }
				// Returns the name of the method of the
				// service agent against which the request
				// was made.

  private:

  			ONS_ActiveRequest(ONS_ActiveRequest const&);
				// Do not define an implementation for this.

    ONS_ActiveRequest&	operator=(ONS_ActiveRequest const&);
				// Do not define an implementation for this.

    OTC_String		conversationId_;
    				// Conversation ID of the active request.

    OTC_ServiceBinding*	serviceBinding_;
    				// Service binding object of remote service
				// agent.

    OTC_String		methodName_;
    				// Name of the method which was called.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<ONS_ActiveRequest*>
{
  public:

    static ONS_ActiveRequest*  add(ONS_ActiveRequest* theObject)
                                { theObject->reference(); return theObject; }

    static void         remove(ONS_ActiveRequest* theObject)
                                { theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_ACTVRQST_HH */
