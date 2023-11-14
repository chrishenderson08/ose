/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/_exchange.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/exchange.hh>
#include <OTC/message/congest.hh>
#include <OTC/message/inetlstn.hh>
#include <OTC/message/unixlstn.hh>
#include <OTC/message/inetclnt.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/system/program.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/logger.hh>

#include <signal.h>

#if defined(OSE_SYS_UNIX)
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include <OTC/stdlib/strstream.h>

/* ------------------------------------------------------------------------- */

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -listen port -connect server [ -group name ]" << endl;
}

void callback1(OTC_Event* theEvent, int)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_ExchangeAnnouncement::typeId())
  {
    OTCEV_ExchangeAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ExchangeAnnouncement*)theEvent;

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_NOTICE);

    if (theAnnouncement->state() == OTCLIB_CONNECTION_ACTIVE)
      theLogger << "ACTIVE";
    else
      theLogger << "INACTIVE";

    theLogger << " " << theAnnouncement->remoteProcess();
    theLogger << " (" << theAnnouncement->remoteAddress() << ")";
    theLogger << flush;
  }

  theEvent->destroy();
}

void callback2(OTC_Event* theEvent, int theAgentId)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Congestion::typeId())
  {
    OTCEV_Congestion* theCongestion;
    theCongestion = (OTCEV_Congestion*)theEvent;

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_NOTICE);

    if (theCongestion->status() == OTCLIB_CONNECTION_BLOCKED)
      theLogger << "BLOCKED ";
    else if (theCongestion->status() == OTCLIB_CONNECTION_CLEARED)
      theLogger << "CLEARED ";
    else
      theLogger << "BACKLOG ";

    theLogger << theCongestion->remoteAddress();
    theLogger << " " << theCongestion->pending();
    theLogger << flush;
  }
}

int main(int argc, char* argv[])
{
  OTC_Program::initialise(argc,argv);

  OTC_String thePortString;
  OTC_Deque<OTC_String> theServers;
  OTC_String theGroupName;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::option(1) == "-listen")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
        break;

      thePortString = OTC_Program::option(1);
    }
    else if (OTC_Program::option(1) == "-connect")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
        break;

      theServers.addLast(OTC_Program::option(1));
    }
    else if (OTC_Program::option(1) == "-group")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
        break;

      theGroupName = OTC_Program::option(1);
    }

    OTC_Program::shift(1);
  }
  
  if (thePortString.isEmpty())
  {
    usage();
    return 1;
  }

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);
  OTCEV_Shutdown::monitor(SIGTERM);

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_SERVER,theGroupName);

  OTC_EPListener* theListener = 0;

  if (thePortString[u_int(0)] == '/')
  {
#if defined(OSE_SYS_UNIX) && !defined(OSE_SYS_CYGWIN32)
    OTC_UnixListener* tmpListener;
    tmpListener = new OTC_UnixListener("MESSAGE",thePortString);
    tmpListener->reference();

    theExchange.listen(tmpListener);

    if (tmpListener->fd() == -1)
    {
      cerr << OTC_Program::basename() << ": listener failed" << endl;
      return 1;
    }

    theListener = tmpListener;
#else
    cerr << OTC_Program::basename() << ": invalid port number" << endl;
    return 1;
#endif
  }
  else
  {
    int thePort;

    if (sscanf(thePortString,"%d",&thePort) != 1)
    {
      cerr << OTC_Program::basename() << ": invalid port number" << endl;
      return 1;
    }

    OTC_InetListener* tmpListener;
    tmpListener = new OTC_InetListener("MESSAGE",thePort);
    tmpListener->reference();

    theExchange.listen(tmpListener);

    if (tmpListener->fd() == -1)
    {
      cerr << OTC_Program::basename() << ": listener failed" << endl;
      return 1;
    }

    theListener = tmpListener;
  }

  OTC_Iterator<OTC_String> theIter;
  theIter = theServers.items();
  while (theIter.isValid())
  {
    OTC_String theHostname;
    int thePort = 0;

    char const* theHostPort;
    theHostPort = theIter.item().string();

    while (*theHostPort != EOS && *theHostPort != ':')
      theHostname += *theHostPort++;

    if (*theHostPort == ':')
      theHostPort++;

    if (sscanf(theHostPort,"%d",&thePort) != 1)
      thePort = 0;

    if (theHostname.isEmpty() || thePort == 0)
    {
      cerr << OTC_Program::basename() << ": invalid server" << endl;
      return 1;
    }

    OTC_InetClient* theClient;
    theClient = new OTC_InetClient("MESSAGE",theHostname,thePort,1000);

    theExchange.connect(theClient);

    theIter.next();
  }

  OTC_FNAgent fnAgent1(callback1);

  OTC_Exchange::registry()->subscribe(fnAgent1.id());

  OTC_FNAgent fnAgent2(callback2);

  theListener->addObserver(fnAgent2.id());

  OTC_LogStream theLogger;
  theLogger.setLevel(OTCLIB_LOG_NOTICE);

  theLogger << "Running dispatcher";
  theLogger << flush;

  OTC_Dispatcher::run();

  theListener->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
