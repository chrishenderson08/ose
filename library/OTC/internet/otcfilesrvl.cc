/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otcfilesrvl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/filesrvl.hh"
#endif
#endif

#include <OTC/internet/filesrvl.hh>

#include <OTC/message/endpoint.hh>
#include <OTC/message/congest.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/system/stat.hh>

/* ------------------------------------------------------------------------- */
OTC_FileServlet::~OTC_FileServlet()
{
  if (fileSize_ != 0)
    fileStream_.close();

  OTCEV_Action::cancelAgent(id());
  endPoint()->removeObserver(id());
}

/* ------------------------------------------------------------------------- */
OTC_FileServlet::OTC_FileServlet(
 OTC_HttpSession* theSession,
 char const* theFileName,
 char const* theFileType
)
  : OTC_HttpServlet(theSession),
    fileName_(theFileName),
    fileType_(theFileType),
    fileSize_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_FileServlet::processRequest()
{
  // Eliminate requests we can't handle.

  if (requestMethod() != "GET")
  {
    sendError(405,"Only GET method supported by servlet.");

    return;
  }

  // Stat the file.

  OTC_Stat theFileStat;

  if (stat(fileName_.string(),theFileStat) == -1)
  {
    sendError(404);

    return;
  }

  if (theFileStat.isDirectory())
  {
    sendError(403);

    return;
  }

  if (!theFileStat.isRegularFile() || !theFileStat.isUserRead())
  {
    sendError(404);

    return;
  }

  // Make sure we can open the file.

  fileStream_.open(fileName_,ios::binary);

  if (fileStream_.fail())
  {
    sendError(404);

    return;
  }

  // Start the response.

  sendResponse(200);

  if (!fileType_.isEmpty())
    sendHeader("Content-Type",fileType_);

  fileSize_ = theFileStat.size();
  sendHeader("Content-Length",fileSize_);

  endHeaders();

  OTC_String theContent;

  // Send back file in parts so other stuff
  // in event system has chance to run, but
  // also to limit memory use and handle the
  // possibility that socket may block.

  theContent.readData(fileStream_,0x4000);

  if (!theContent.isEmpty())
    sendContent(theContent);

  if (!fileStream_.good())
  {
    fileSize_ = 0;
    fileStream_.close();

    endContent();

    return;
  }

  // Register with the endpoint so we know
  // when congestion occurs.

  endPoint()->addObserver(id());

  // Schedule idle action for sending of next
  // section of the file.

  OTCEV_Action::schedule(id(),OTCLIB_STANDARD_JOB);
}

/* ------------------------------------------------------------------------- */
void OTC_FileServlet::process(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTC_String theContent;

    theContent.readData(fileStream_,0x4000);

    if (!theContent.isEmpty())
      sendContent(theContent);

    if (!fileStream_.good())
    {
      fileSize_ = 0;
      fileStream_.close();

      endPoint()->removeObserver(id());

      endContent();

      return;
    }

    OTCEV_Action::schedule(id(),OTCLIB_STANDARD_JOB);
  }
  else if (theEvent->type() == OTCEV_Congestion::typeId())
  {
    OTCEV_Congestion* theCongestion;
    theCongestion = (OTCEV_Congestion*)theEvent;

    if (theCongestion->status() == OTCLIB_CONNECTION_BLOCKED)
      OTCEV_Action::cancelAgent(id());
    else if (theCongestion->status() == OTCLIB_CONNECTION_CLEARED)
      OTCEV_Action::schedule(id(),OTCLIB_STANDARD_JOB);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_FileServlet::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  process(theEvent);

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
