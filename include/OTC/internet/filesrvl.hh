#ifndef OTC_INTERNET_FILESRVL_HH
#define OTC_INTERNET_FILESRVL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/filesrvl.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsrvl.hh>
#include <OTC/dispatch/evagent.hh>

#include <OTC/stdlib/fstream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/filesrvl.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_FileServlet : public OTC_HttpServlet, public OTC_EVAgent
    // = TITLE
    //      Servlet which sends back results of a file request.
    //
    // = CLASS TYPE
    //      Concrete
    //
    // = DESCRIPTION
    //     The <OTC_FileServlet> class is a HTTP servlet which send back
    //     the results of a file request.
    //
    // = SEE ALSO
    //     <OTC_FileServer>, <OTC_HttpServlet>
{
  public:

    			~OTC_FileServlet();

    // = INITIALISATION

    			OTC_FileServlet(
			 OTC_HttpSession* theSession,
			 char const* theFileName,
			 char const* theFileType=0
			);
				// Creates a HTTP servlet to handle the file
				// request. <theSession> is the HTTP
				// session object which has parsed the HTTP
				// request. This must be supplied by the
				// factory method contained in the HTTP
				// server object. <theFileName> is the
				// location of the file to be sent back with
				// <theFileType> describing the contents.
				// The latter is sent back as the content
				// type in the HTTP response.

    // = DETAILS

    OTC_String const&	fileName() const
      				{ return fileName_; }
    				// Returns the name of the file.

    OTC_String const&	fileType() const
      				{ return fileType_; }
    				// Returns the type of file.

    size_t		fileSize() const
      				{ return fileSize_; }
    				// Returns the size of the file.

  protected:

    // = REQUEST PROCESSING

    void		processRequest();
    				// Decodes the request and either flags an
				// error or causes requested file to be
				// returned.

    // = EVENT HANDLING

    void		process(OTC_Event* theEvent);
    				// Looks for congestion events on connection
				// to HTTP client so that sending of data
				// from the file can be throttled.

    void		handle(OTC_Event* theEvent);
    				// Calls <process()> then destroys <theEvent>.

  private:

    			OTC_FileServlet(OTC_FileServlet const&);
				// Do not define an implementation for this.

    OTC_FileServlet&	operator=(OTC_FileServlet const&);
				// Do not define an implementation for this.

    OTC_String		fileName_;
    				// The name of the file.

    OTC_String		fileType_;
    				// The type of file.

    off_t		fileSize_;
    				// The size of the file.

    ifstream		fileStream_;
    				// Input stream for file.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_FILESRVL_HH */
