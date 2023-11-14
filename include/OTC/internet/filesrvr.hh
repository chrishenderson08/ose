#ifndef OTC_INTERNET_FILESRVR_HH
#define OTC_INTERNET_FILESRVR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/filesrvr.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsrvr.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/filesrvr.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_FileServer : public OTC_HttpServer
    // = TITLE
    //     Factory for creation of HTTP servlets to handle file requests.
    //
    // = DESCRIPTION
    //     The <OTC_FileServer> class is a HTTP server object factory for
    //     creating HTTP servlets to handle file requests. Only files
    //     located under a specified root directory will be able to be
    //     accessed. Note that there is no support for browsing of the
    //     contents of a directory. The functionality is only provided
    //     for special purpose use of applications to serve up files such
    //     as configuration files, log files etc. It is anticipated that
    //     the client should know which file it wants. If it doesn't you
    //     perhaps should use XML-RPC requests to some service which can
    //     provide a list of available files.
    //     
    // = SEE ALSO
    //     <OTC_HttpDaemon>, <OTC_FileServlet>, <OTC_HttpServer>
{
  public:

    virtual		~OTC_FileServer();

    // = INITIALISATION

			OTC_FileServer(char const* theDirectory);
				// Creates a HTTP server object which handles
				// file requests for files located under
				// <theDirectory>. <theDirectory> should be
				// an absolute path name. It it isn't, you
				// will have problems if the current working
				// directory of the process changes.

    // = DIRECTORY

    OTC_String const&	directory() const
      				{ return directory_; }
    				// Returns the name of the directory which
				// is the root of the files being served.

    // = FILE TYPES

    void		map(char const* theExtension, char const* theMimeType)
      				{
				  mimeTypes_.add(theExtension,
				   theMimeType,OTCLIB_REPLACE);
				}
    				// Adds a mapping which indicates that files
				// with <theExtension> are of <theMimeType>.
				// <theMimeType> is what will be passed back
				// in the content type field of the HTTP
				// response. If an entry for <theExtension>
				// already exists, it will be replaced.
				// Common filename extensions for HTML and
				// XML are already preloaded, but anything
				// else should be added. The extension must
				// be preceded by ".".

  protected:

    // = SERVLET CREATION

    OTC_HttpServlet*	servlet(OTC_HttpSession* theSession);
				// Returns a HTTP servlet to process the
				// current request being handled by
				// <theSession> assuming it is a file
				// request.

  private:

			OTC_FileServer(OTC_FileServer const&);
				// Do not define an implementation for this.

    OTC_FileServer&	operator=(OTC_FileServer const&);
				// Do not define an implementation for this.

    OTC_String		directory_;
    				// Location of root directory for file
				// serving.

    OTC_HMap<OTC_String,OTC_String>	mimeTypes_;
    				// Mapping of filename extensions to type.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_FILESRVR_HH */
