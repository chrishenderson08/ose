/*
// ============================================================================
//
// = FILENAME
//     service.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/service.hh>
#include <OTC/message/exchange.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/system/program.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class Service : public ONS_Service
{
  public:

    			Service();

    void		startup();

    void		publish();

    double		multiply(double theValue1, double theValue2);

    void		wait(double thePeriod);

    void		echo(OTC_ROPayload thePayload);
};

Service::Service()
  : ONS_Service("service","*",OTCLIB_SERVICE_HIDDEN)
{
  subscribeStartup(&Service::startup);
}

void Service::startup()
{
  exportMethod(&Service::multiply,"multiply");
  exportMethod(&Service::wait,"wait");
  exportMethod(&Service::echo,"echo");

  announceAvailability();

  publishReport("creationTime",OTC_Time::currentTime(),-1);

  startTimer(&Service::publish,1.0);
}

void Service::publish()
{
  publishReport("currentTime",OTC_Time::currentTime(),-1);

  startTimer(&Service::publish,1.0);
}

double Service::multiply(double theValue1, double theValue2)
{
  return theValue1 * theValue2;
}

void Service::wait(double thePeriod)
{
  startTimer(&ONS_Service::resumeResponse,thePeriod,conversationId());
  suspendResponse();
}

void Service::echo(OTC_ROPayload thePayload)
{
  returnResponse(thePayload);
}

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -connect server [ -domain name ]" << endl;
}

int main(int argc, char* argv[])
{
  // Save away program arguments.

  OTC_Program::initialise(argc,argv);

  // Decode the arguments.

  OTC_String theServerString;
  OTC_String theDomainName;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::option(1) == "-connect")
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

      theDomainName = OTC_Program::option(1);
    }
    else
    {
      usage();
      return 1;
    }

    OTC_Program::shift(1);
  }

  if (theServerString.isEmpty())
  {
    usage();
    return 1;
  }

  // Initialise the dispatcher.

  OTC_Dispatcher::initialise();

  // Monitor signals for shutdown.

  OTCEV_Shutdown::monitor(SIGINT);

  // Connect to the exchange process.

  OTC_String theHostname;
  int thePort = 0;

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_CLIENT,theDomainName);

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

  theExchange.connect(theHostname,thePort,1000);

  // Create publisher.

  Service theService;
  theService.scheduleStartup();

  // Run the dispatcher.

  OTC_Dispatcher::run();

  return 0;
}

/* ------------------------------------------------------------------------- */
