/*
// ============================================================================
//
// = FILENAME
//     exchange.cc
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
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/system/program.hh>

#include <signal.h>

#include <OTC/stdlib/strstream.h>

/* ------------------------------------------------------------------------- */

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -listen port [ -domain name ]" << endl;
}

int main(int argc, char* argv[])
{
  // Save away program arguments.

  OTC_Program::initialise(argc,argv);

  // Decode arguments.

  OTC_String thePortString;
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

  // Run the dispatcher.

  OTC_Dispatcher::run();

  // Discard the listener.

  theListener->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
