/*
// ============================================================================
//
// = FILENAME
//     netrpc-gateway.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/exchange.hh>
#include <OTC/message/inetclnt.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/internet/rpcgtway.hh>
#include <OTC/internet/httpdaem.hh>
#include <OTC/system/program.hh>
#include <OTC/types/time.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -connect server [ -domain name ]";
  cerr << " -listen port [ -group name ]" << endl;
}

int main(int argc, char* argv[])
{
  // Save away program arguments.

  OTC_Program::initialise(argc,argv);

  // Decode the arguments.

  OTC_String thePortString;
  OTC_String theServerString;
  OTC_String theDomainString;
  OTC_String theGroupString;

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

      theServerString = OTC_Program::option(1);
    }
    else if (OTC_Program::option(1) == "-domain")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
	break;

      theDomainString = OTC_Program::option(1);
    }
    else if (OTC_Program::option(1) == "-group")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
	break;

      theGroupString = OTC_Program::option(1);
    }
    else
    {
      usage();
      return 1;
    }

    OTC_Program::shift(1);
  }

  if (thePortString.isEmpty())
  {
    usage();
    return 1;
  }

  if (theServerString.isEmpty())
  {
    usage();
    return 1;
  }

  if (theGroupString.isUndefined())
    theGroupString = "PUBLIC";

  // Initialise the dispatcher.

  OTC_Dispatcher::initialise();

  // Monitor signals for shutdown.

  OTCEV_Shutdown::monitor(SIGINT);

  // Connect to the exchange process.

  OTC_String theHostname;
  int thePort = 0;

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_CLIENT,theDomainString);

  char const* theHostPort;
  theHostPort = theServerString.string();

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
  theClient = new OTC_InetClient("STREAM",theHostname,thePort,1000);
  theClient->reference();

  theExchange.connect(theClient);

  // Create HTTP daemon.

  if (sscanf(thePortString,"%d",&thePort) != 1)
  {
    cerr << OTC_Program::basename() << ": invalid port" << endl;
    return 1;
  }

  OTC_HttpDaemon theDaemon(thePort);

  OTC_RpcGateway theServer("*",OTCLIB_SERVICE_REGISTRY);
  theDaemon.attach("/",&theServer);

  theDaemon.start();

  // Run the dispatcher.

  OTC_Dispatcher::run();

  // Discard the client.

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
