/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otcechosrvl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/echosrvl.hh"
#endif
#endif

#include <OTC/internet/echosrvl.hh>

#include <OTC/text/osstream.hh>

/* ------------------------------------------------------------------------- */
OTC_EchoServlet::~OTC_EchoServlet()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EchoServlet::processRequest()
{
  OTC_String theResponse;
  OTC_OSStream theStream(theResponse,OTCLIB_BUFFERED);

  sendResponse(200);
  sendHeader("Content-Type","text/html");
  endHeaders();

  // Compose response.

  theStream << "<HTML>\n";
  theStream << "<HEAD>\n";
  theStream << "<TITLE>\n";
  theStream << "HTTP Echo Server\n";
  theStream << "</TITLE>\n";
  theStream << "</HEAD>\n";

  theStream << "<BODY>\n";

  theStream << "<H1>\n";
  theStream << "Servlet Details\n";
  theStream << "</H1>\n";

  theStream << "<P>\n";
  theStream << "<PRE>\n";

  theStream << "Server-Root: ";
  xmlEncode(theStream,serverRoot());
  theStream << endl;
  theStream << "Server-Path: ";
  xmlEncode(theStream,serverPath());
  theStream << endl;
  theStream << "Servlet-Path: ";
  xmlEncode(theStream,servletPath());
  theStream << endl;
  theStream << "Servlet-Info: ";
  xmlEncode(theStream,servletInfo());
  theStream << endl;

  theStream << "<H1>\n";
  theStream << "Session Details\n";
  theStream << "</H1>\n";

  theStream << "<P>\n";
  theStream << "<PRE>\n";

  theStream << "Session-Id: ";
  xmlEncode(theStream,sessionId());
  theStream << endl;
  theStream << "Session-Context: ";
  xmlEncode(theStream,sessionContext());
  theStream << endl;

  theStream << "</PRE>\n";
  theStream << "</P>\n";

  theStream << "<H1>\n";
  theStream << "Request Details\n";
  theStream << "</H1>\n";

  theStream << "<P>\n";
  theStream << "<PRE>\n";

  theStream << "URL: ";
  xmlEncode(theStream,url());
  theStream << endl;
  theStream << "Request-Method: ";
  xmlEncode(theStream,requestMethod());
  theStream << endl;
  theStream << "Server-Host: ";
  xmlEncode(theStream,serverHost());
  theStream << endl;
  theStream << "Server-Port: ";
  xmlEncode(theStream,serverPort());
  theStream << endl;
  theStream << "Request-Path: ";
  xmlEncode(theStream,requestPath());
  theStream << endl;
  theStream << "Query-String: ";
  xmlEncode(theStream,queryString());
  theStream << endl;
  theStream << "Protocol-Version: ";
  xmlEncode(theStream,protocolVersion());
  theStream << endl;
  theStream << "Client-Host: ";
  xmlEncode(theStream,clientHost());
  theStream << endl;
  theStream << "Client-Port: ";
  xmlEncode(theStream,clientPort());
  theStream << endl;

  if (!remoteUser().isEmpty())
  {
    theStream << "Remote-User: ";
    xmlEncode(theStream,remoteUser());
    theStream << endl;
  }

  theStream << "</PRE>\n";
  theStream << "</P>\n";

  theStream << "<H1>\n";
  theStream << "Raw Headers\n";
  theStream << "</H1>\n";

  theStream << "<P>\n";
  theStream << "<PRE>\n";

  xmlEncode(theStream,header());

  theStream << "</PRE>\n";
  theStream << "</P>\n";

  theStream << "<H1>\n";
  theStream << "Processed Headers\n";
  theStream << "</H1>\n";

  theStream << "<P>\n";
  theStream << "<PRE>\n";

  OTC_PairIterator<OTC_String,OTC_String> theHeaders;
  theHeaders = headers();

  while (theHeaders.isValid())
  {
    xmlEncode(theStream,theHeaders.key());
    theStream << ": ";
    xmlEncode(theStream,theHeaders.item());
    theStream << endl;

    theHeaders.next();
  }

  theStream << "</PRE>\n";
  theStream << "</P>\n";

  if (contentLength() > 0)
  {
    theStream << "<H1>\n";
    theStream << "Request Contents\n";
    theStream << "</H1>\n";

    theStream << "<P>\n";
    theStream << "<PRE>\n";

    theStream << flush;

    sendContent(theResponse);
  }
  else
  {
    theStream << "</BODY>\n";
    theStream << "</HTML>\n";

    theStream << flush;

    sendContent(theResponse);

    endContent();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_EchoServlet::processContent(OTC_String const& theContent)
{
  OTC_String theResponse;
  OTC_OSStream theStream(theResponse,OTCLIB_BUFFERED);

  xmlEncode(theStream,theContent,theContent.length());

  contentLength_ += theContent.length();

  if (contentLength_ >= contentLength())
  {
    theStream << "</PRE>\n";
    theStream << "</P>\n";

    theStream << "</BODY>\n";
    theStream << "</HTML>\n";

    theStream << flush;

    sendContent(theResponse);

    endContent();
  }
  else
  {
    theStream << flush;

    sendContent(theResponse);
  }
}

/* ------------------------------------------------------------------------- */
