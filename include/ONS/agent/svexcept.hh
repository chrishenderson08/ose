#ifndef ONS_AGENT_SVEXCEPT_HH
#define ONS_AGENT_SVEXCEPT_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/svexcept.hh
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

/* ------------------------------------------------------------------------- */

int const ONSLIB_CLIENT_UNKNOWN_ERROR = 40;
int const ONSLIB_CLIENT_RESOURCE_UNAVAILABLE = 41;
int const ONSLIB_CLIENT_REQUEST_ENCODING_ERROR = 42;
int const ONSLIB_CLIENT_RESPONSE_HEADERS_INVALID = 43;
int const ONSLIB_CLIENT_RESPONSE_DECODING_ERROR = 44;
int const ONSLIB_CLIENT_SERVICE_ADDRESS_INVALID = 45;
int const ONSLIB_CLIENT_SERVICE_UNAVAILABLE = 46;
int const ONSLIB_CLIENT_REQUEST_TIMEOUT = 47;
int const ONSLIB_CLIENT_REQUEST_ABORTED = 48;

char const* const ONSLIB_CLIENT_UNKNOWN_ERROR_MESSAGE \
 = "Unknown Error";
char const* const ONSLIB_CLIENT_RESOURCE_UNAVAILABLE_MESSAGE \
 = "Resource Unavailable";
char const* const ONSLIB_CLIENT_REQUEST_ENCODING_ERROR_MESSAGE \
 = "Request Encoding Error";
char const* const ONSLIB_CLIENT_RESPONSE_HEADERS_INVALID_MESSAGE \
 = "Response Headers Invalid";
char const* const ONSLIB_CLIENT_RESPONSE_DECODING_ERROR_MESSAGE \
 = "Response Decoding Error";
char const* const ONSLIB_CLIENT_SERVICE_ADDRESS_INVALID_MESSAGE \
 = "Server Address Invalid";
char const* const ONSLIB_CLIENT_SERVICE_UNAVAILABLE_MESSAGE \
 = "Server Unavailable";
char const* const ONSLIB_CLIENT_REQUEST_TIMEOUT_MESSAGE \
 = "Request Timeout";
char const* const ONSLIB_CLIENT_REQUEST_ABORTED_MESSAGE \
 = "Request Aborted";

int const ONSLIB_SERVER_UNKNOWN_ERROR = 50;
int const ONSLIB_SERVER_RESOURCE_UNAVAILABLE = 51;
int const ONSLIB_SERVER_RESPONSE_ENCODING_ERROR = 52;
int const ONSLIB_SERVER_REQUEST_HEADERS_INVALID = 53;
int const ONSLIB_SERVER_REQUEST_DECODING_ERROR = 54;
int const ONSLIB_SERVER_METHOD_UNAVAILABLE = 55;
int const ONSLIB_SERVER_PARAMETERS_INCORRECT = 56;
int const ONSLIB_SERVER_APPLICATION_ERROR = 57;
int const ONSLIB_SERVER_RESPONSE_ABORTED = 58;

char const* const ONSLIB_SERVER_UNKNOWN_ERROR_MESSAGE \
 = "Unknown Error";
char const* const ONSLIB_SERVER_RESOURCE_UNAVAILABLE_MESSAGE \
 = "Resource Unavailable";
char const* const ONSLIB_SERVER_RESPONSE_ENCODING_ERROR_MESSAGE \
 = "Response Encoding Error";
char const* const ONSLIB_SERVER_REQUEST_HEADERS_INVALID_MESSAGE \
 = "Request Headers Invalid";
char const* const ONSLIB_SERVER_REQUEST_DECODING_ERROR_MESSAGE \
 = "Request Decoding Error";
char const* const ONSLIB_SERVER_METHOD_UNAVAILABLE_MESSAGE \
 = "Method Unavailable";
char const* const ONSLIB_SERVER_PARAMETERS_INCORRECT_MESSAGE \
 = "Parameters Incorrect";
char const* const ONSLIB_SERVER_APPLICATION_ERROR_MESSAGE \
 = "Application Error";
char const* const ONSLIB_SERVER_RESPONSE_ABORTED_MESSAGE \
 = "Response Aborted";

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_ServiceFailure
    // = TITLE
    //     Exception class to throw when a service request has an error.
    //
    // = DESCRIPTION
    //     The <ONS_ServiceFailure> class is the exception class which is
    //     throw when an error is generated within the handling of a service
    //     request. There is no need to throw this exception class
    //     specifically, as the <abortResponse()> method of <OTC_Service>
    //     will do this for you. It may be necessary however to catch this
    //     exception type and rethrow it other exceptions need to be caught.
    //
    // = SEE ALSO
    //     <OTC_Service>
{
  public:

    // = INITIALISATION

			ONS_ServiceFailure(
			 int theError,
			 char const* theDescription,
			 char const* theOrigin="",
			 char const* theDetails=""
			)
			  : error_(theError),
			    description_(theDescription),
			    origin_(theOrigin),
			    details_(theDetails)
				{}
				// Records details of the error to be
				// returned to a called in a failure
				// response.

			ONS_ServiceFailure(
			 ONS_ServiceFailure const& theFailure
			)
			  : error_(theFailure.error_),
			    description_(theFailure.description_),
			    origin_(theFailure.origin_),
			    details_(theFailure.details_)
				{}
				// Records details of the error to be
				// returned to a called in a failure
				// response.

    ONS_ServiceFailure&	operator=(
			 ONS_ServiceFailure const& theFailure
			)
			{
			  error_ = theFailure.error_;
			  description_ = theFailure.description_;
			  origin_ = theFailure.origin_;
			  details_ = theFailure.details_;
			  return *this;
			}
				// Updates details of the error to be
				// returned to a called in a failure response
				// to what is defined by <theFailure>.

    // = DETAILS

    int			error() const
				{ return error_; }
				// Returns the error number to be returned
				// in the failure response.

    OTC_String const&	description() const
				{ return description_; }
				// Returns the error description to be
				// returned in the failure response.

    OTC_String const&	origin() const
				{ return origin_; }
				// Returns the error origin to be returned in
				// the failure response.

    OTC_String const&	details() const
				{ return details_; }
				// Returns any addition error details to be
				// returned in the failure response.

  private:

    int			error_;
    				// The error number.

    OTC_String		description_;
    				// The error description.

    OTC_String		origin_;
    				// The error origin.

    OTC_String		details_;
    				// Any additional error details.
};

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_DelayedResponse
    // = TITLE
    //     Exception thrown internally when delaying a service response.
{
  // Nothing to see.
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_SVEXCEPT_HH */
