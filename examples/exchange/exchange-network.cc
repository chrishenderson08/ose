/*
// ============================================================================
//
// = FILENAME
//     exchange-network.cc
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
#include <OTC/message/inetlstn.hh>
#include <OTC/message/inetclnt.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/system/program.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>

#include <signal.h>

#include <OTC/stdlib/strstream.h>

/* ------------------------------------------------------------------------- */

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -listen port [ -connect server ] [ -domain name ]" << endl;
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

int main(int argc, char* argv[])
{
  // Save away program arguments.

  OTC_Program::initialise(argc,argv);

  // Decode arguments.

  OTC_String thePortString;
  OTC_Deque<OTC_String> theServers;
  OTC_String theDomainName;

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
    else if (OTC_Program::option(1) == "-domain")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
        break;

      theDomainName = OTC_Program::option(1);
    }

    OTC_Program::shift(1);
  }
  
  if (thePortString.isEmpty())
  {
    usage();
    return 1;
  }

  // Initialise dispatcher.

  OTC_Dispatcher::initialise();

  // Monitor signals for shutdown.

  OTCEV_Shutdown::monitor(SIGINT);
  OTCEV_Shutdown::monitor(SIGTERM);

  // Create server exchange.

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_SERVER,theDomainName);

  // Create the listener.

  int thePort;

  if (sscanf(thePortString,"%d",&thePort) != 1)
  {
    cerr << OTC_Program::basename() << ": invalid port number" << endl;
    return 1;
  }

  OTC_InetListener* theListener;
  theListener = new OTC_InetListener("MESSAGE",thePort);
  theListener->reference();

  theExchange.listen(theListener);

  if (theListener->fd() == -1)
  {
    cerr << OTC_Program::basename() << ": listener failed" << endl;
    return 1;
  }

  // Create clients to connect to other
  // exchanges in the exchange network. The
  // clients are destroyed through reference
  // counting by the exchange.

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

  // Monitor exchange connections.

  OTC_FNAgent fnAgent1(callback1);

  OTC_Exchange::registry()->subscribe(fnAgent1.id());

  // Run the dispatcher.

  OTC_Dispatcher::run();

  // Discard the listener.

  theListener->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
