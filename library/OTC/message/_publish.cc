/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/_publish.cc
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
#include <OTC/message/connect.hh>
#include <OTC/message/inetclnt.hh>
#include <OTC/message/unixclnt.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/system/program.hh>
#include <OTC/types/time.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class PRG_Producer : public OTC_EVAgent
{
  public:

			PRG_Producer(char const* theName="PRODUCER");

  protected:

    void                handle(OTC_Event* theEvent);

  private:

    OTC_SVBroker	broker_;
};

PRG_Producer::PRG_Producer(char const* theName)
  : broker_(this,theName)
{
  OTC_SVPayload theContent;
  theContent <<= OTC_Time::currentTime();

  broker_.joinGroup("SOURCE");

  broker_.publishReport("time",theContent,10);

  OTCEV_Timeout::start(id(),10000);
}

void PRG_Producer::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("PRG_Producer::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  tracer() << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTC_SVPayload theContent;
    theContent <<= OTC_Time::currentTime();

    broker_.publishReport("time",theContent,10);

    broker_.joinGroup("DUMMY");
    broker_.leaveGroup("DUMMY");

    OTCEV_Timeout::start(id(),10000);
  }

  theEvent->destroy();
}

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -connect server [ -group name ]" << endl;
}

int main(int argc, char* argv[])
{
  OTC_Program::initialise(argc,argv);

  OTC_String theServerString;
  OTC_String theGroupString;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::option(1) == "-connect")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
	break;

      theServerString = OTC_Program::option(1);
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

  if (theServerString.isEmpty())
  {
    usage();
    return 1;
  }

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  OTC_String theHostname;
  int thePort = 0;

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_CLIENT,theGroupString);

  OTC_EPClient* theClient;

  if (theServerString[u_int(0)] == '/')
  {
#if defined(OSE_SYS_UNIX) && !defined(OSE_SYS_CYGWIN32)
    OTC_UnixClient* tmpClient;
    tmpClient = new OTC_UnixClient("MESSAGE",theServerString,1000);
    tmpClient->reference();

    theClient = tmpClient;
#else
    cerr << OTC_Program::basename() << ": invalid server" << endl;
    return 1;
#endif
  }
  else
  {
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

    OTC_InetClient* tmpClient;
    tmpClient = new OTC_InetClient("MESSAGE",theHostname,thePort,1000);
    tmpClient->reference();

    theClient = tmpClient;
  }

  theExchange.connect(theClient);

  PRG_Producer theProducer1("MICKEY");
  //PRG_Producer theProducer2("MICKEY");

  //PRG_Producer theProducer3("MINNY");
  //PRG_Producer theProducer4("DAFFY");
  //PRG_Producer theProducer5("GOOFY");
  //PRG_Producer theProducer6("PLUTO");

  OTC_Dispatcher::run();

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
