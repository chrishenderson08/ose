#ifndef ONS_AGENT_PNDGCNCL_HH
#define ONS_AGENT_PNDGCNCL_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/pndgcncl.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/refcnt/resource.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/pndgcncl.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_PendingCancellation : public OTC_Resource
    // = TITLE
    //     Holder of information about a pending request cancellation.
    //
    // = DESCRIPTION
    //     The <ONS_PendingCancellation> class holds information about a
    //     pending request cancellation.
{
  public:

			~ONS_PendingCancellation();

    // = INITIIALISATION

			ONS_PendingCancellation(
			 OTC_String const& theConversationId,
			 int theError,
			 char const* theDescription,
			 char const* theOrigin,
			 char const* theDetails
			);
				// Records cancellation of service request
				// with <theConversationId> where the reason
				// for cancellation is given by <theError>,
				// <theDescription>, <theOrigin> and
				// <theDetails>.

    // = DETAILS

    OTC_String const&	conversationId() const
				{ return conversationId_; }
				// Returns the conversation ID of the service
				// request being cancelled.

    int			error() const
				{ return error_; }
				// Returns the error number which will be
				// returned with the failure response to the
				// register failure callback if any.

    OTC_String const&	description() const
				{ return description_; }
				// Returns the error description which will
				// be returned with the failure response to
				// the register failure callback if any.

    OTC_String const&	origin() const
				{ return origin_; }
				// Returns the error origin which will
				// be returned with the failure response to
				// the register failure callback if any.

    OTC_String const&	details() const
				{ return details_; }
				// Returns any additional error details which
				// will be returned with the failure response
				// to the register failure callback if any.

  private:

  			ONS_PendingCancellation(ONS_PendingCancellation const&);
				// Do not define an implementation for this.

    ONS_PendingCancellation&	operator=(ONS_PendingCancellation const&);
				// Do not define an implementation for this.

    OTC_String		conversationId_;
    				// The conversation ID of the service request
				// being cancelled.

    int			error_;
    				// The error number to be returned with the
				// failure response.

    OTC_String		description_;
				// The error description to be returned with
				// the failure response.

    OTC_String		origin_;
				// The error origin to be returned with the
				// failure response.

    OTC_String		details_;
				// The addition error details to be returned
				// with the failure response.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<ONS_PendingCancellation*>
{
  public:

    static ONS_PendingCancellation*  add(ONS_PendingCancellation* theObject)
                                { theObject->reference(); return theObject; }

    static void         remove(ONS_PendingCancellation* theObject)
                                { theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_PNDGCNCL_HH */
