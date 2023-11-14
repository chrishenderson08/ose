/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/_uclient.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

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
      theAddress += "HEARTBEAT";

      theNewEvent->queue(theAddress,inBox_.name(),"BLIP","");

      OTCEV_Action::schedule(id(),OTCLIB_IDLE_JOB);

      timerId_ = 0;
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

  theLogger << "Client shutting down" << endl;

  OTCEV_Timeout::cancelAgent(id());
  OTCEV_Action::cancelAgent(id());
}

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -connect server [ -period period ]" << endl;
}

int main(int argc, char* argv[])
{
  OTC_Program::initialise(argc,argv);

  OTC_String theServerString;
  OTC_String thePeriodString;

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
    else
    {
      usage();
      return 1;
    }

    OTC_Program::shift(1);
  }

  if (theValidArgs < 1 || theValidArgs > 2)
  {
    usage();
    return 1;
  }

  if (theServerString.isEmpty())
  {
    usage();
    return 1;
  }

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  int thePeriod = 1000;

  if (theServerString[u_int(0)] != '/')
  {
    cerr << OTC_Program::basename() << ": invalid path" << endl;
    return 1;
  }

  if (!thePeriodString.isEmpty())
  {
    if (sscanf(thePeriodString,"%d",&thePeriod) != 1)
    {
      cerr << OTC_Program::basename() << ": invalid period" << endl;
      return 1;
    }
  }

  OTC_UnixClient* theClient;
  theClient = new OTC_UnixClient("MESSAGE",theServerString,1000);
  theClient->reference();

  theClient->start();

  OTC_Receiver* theReceiver;
  theReceiver = new OTC_Receiver(theClient,"HEARTBEAT");

  PRG_Producer theProducer(thePeriod);

  OTC_Dispatcher::run();

  delete theReceiver;

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
