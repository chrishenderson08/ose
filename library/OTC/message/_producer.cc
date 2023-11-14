/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/_producer.cc
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
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/system/program.hh>
#include <OTC/types/time.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class PRG_Producer : public OTC_EVAgent
{
  public:

			~PRG_Producer()
				{ OTCEV_Shutdown::unsubscribe(id()); }

			PRG_Producer(int thePeriod);

    void                handle(OTC_Event* theEvent);

    void		shutdown();

  private:

    int			period_;

    OTC_Receiver	inBox_;

    int			timerId_;
};

PRG_Producer::PRG_Producer(int thePeriod)
{
  period_ = thePeriod;
  inBox_.bind(this,"HEART");

  timerId_ = OTCEV_Timeout::start(id(),period_);

  OTCEV_Shutdown::subscribe(id());
}

void PRG_Producer::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Shutdown::typeId())
  {
    OTCEV_Shutdown* theShutdown;
    theShutdown = (OTCEV_Shutdown*)theEvent;

    if (theShutdown->category() == OTCLIB_SHUTDOWN_PENDING)
      shutdown();
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Timeout* theTimeout;
    theTimeout = (OTCEV_Timeout*)theEvent;

    if (theTimeout->timer() == timerId_)
    {
      OTC_Time theTime;

      cout << theTime.asString() << endl;

      OTCEV_Message* theNewEvent;
      theNewEvent = new OTCEV_Message(theTime.asString());

      OTC_String theAddress;

      theAddress += "$RECEIVER!HEARTBEAT";

      theNewEvent->queue(theAddress,inBox_.name(),"BLIP","");

      OTCEV_Action::schedule(id(),OTCLIB_IDLE_JOB);
    }
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    timerId_ = OTCEV_Timeout::start(id(),period_);
  }

  theEvent->destroy();
}

void PRG_Producer::shutdown()
{
  OTC_LogStream theLogger;
  theLogger.setLevel(OTCLIB_LOG_ALERT);

  theLogger << "Producer shutting down" << endl;

  OTCEV_Timeout::cancelAgent(id());
  OTCEV_Action::cancelAgent(id());
}

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -connect server [ -period period -group name ]" << endl;
}

int main(int argc, char* argv[])
{
  OTC_Program::initialise(argc,argv);

  OTC_String theServerString;
  OTC_String thePeriodString;
  OTC_String theGroupName;

  int theValidArgs = 0;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::option(1) == "-connect")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
	break;

      theServerString = OTC_Program::option(1);

      theValidArgs++;
    }
    else if (OTC_Program::option(1) == "-period")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
	break;

      thePeriodString = OTC_Program::option(1);

      theValidArgs++;
    }
    else if (OTC_Program::option(1) == "-group")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
	break;

      theGroupName = OTC_Program::option(1);

      theValidArgs++;
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

  int thePeriod = 1000;

  if (!thePeriodString.isEmpty())
  {
    if (sscanf(thePeriodString,"%d",&thePeriod) != 1)
    {
      cerr << OTC_Program::basename() << ": invalid period" << endl;
      return 1;
    }
  }

  OTC_String theHostname;
  int thePort = 0;

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_CLIENT,theGroupName);

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

  PRG_Producer theProducer(thePeriod);

  OTC_Dispatcher::run();

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
