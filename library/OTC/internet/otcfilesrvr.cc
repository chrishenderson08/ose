/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otcfilesrvr.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/filesrvr.hh"
#endif
#endif

#include <OTC/internet/filesrvr.hh>

#include <OTC/internet/filesrvl.hh>
#include <OTC/system/pathname.hh>

/* ------------------------------------------------------------------------- */
OTC_FileServer::~OTC_FileServer()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_FileServer::OTC_FileServer(char const* theDirectory)
  : directory_(theDirectory)
{
  map(".html","text/html");
  map(".htm","text/html");
  map(".txt","text/plain");
  map(".xml","text/xml");
}

/* ------------------------------------------------------------------------- */
OTC_HttpServlet* OTC_FileServer::servlet(OTC_HttpSession* theSession)
{
  OTC_String theFileName;
  OTC_String theFileType;

  theFileName = directory_;
  theFileName += '/';
  theFileName += theSession->servletPath();

  theFileType = "text/plain";

  int theIndex;
  theIndex = theFileName.rindex('.');
  if (theIndex != -1)
  {
    OTC_String theExtension;
    theExtension = theFileName.from(theIndex);

    if (theExtension.index('/') == -1)
    {
      if (mimeTypes_.contains(theExtension))
	theFileType = mimeTypes_.item(theExtension);
    }
  }

  OTC_HttpServlet* theServlet = 0;
  theServlet = new OTC_FileServlet(theSession,theFileName,theFileType);
  OTCLIB_ASSERT_M(theServlet != 0);

  return theServlet;
}

/* ------------------------------------------------------------------------- */
