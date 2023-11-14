/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/_siclient.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetclnt.hh>
#include <OTC/message/connect.hh>
#include <OTC/message/congest.hh>
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

class PRG_Client : public OTC_EVAgent
{
  public:

			~PRG_Client() {}

			PRG_Client(char const* theLocalAddress, int thePeriod)
			  : period_(thePeriod)
				{ inBox_.bind(this,theLocalAddress); }

    void                handle(OTC_Event* theEvent);

  private:

    OTC_Receiver	inBox_;

    OTC_String		remoteAddress_;

    int			period_;
};

void PRG_Client::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    cout << *theEvent << endl;

    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    if (theConnection->state() == OTCLIB_CONNECTION_ACTIVE)
    {
      remoteAddress_ = theConnection->remoteAddress();

      OTCEV_Message* theMessage;
      theMessage = new OTCEV_Message("HELLO");
      OTCLIB_ASSERT_M(theMessage != 0);

      theMessage->queue(remoteAddress_,inBox_.name());

      OTCEV_Timeout::start(id(),period_);
    }
    else
    {
      OTCEV_Timeout::cancelAgent(id());
    }
  }
  else if (theEvent->type() == OTCEV_Congestion::typeId())
  {
    cout << *theEvent << endl;
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Message* theMessage;
    theMessage = new OTCEV_Message("HELLO");
    OTCLIB_ASSERT_M(theMessage != 0);

    theMessage->queue(remoteAddress_,inBox_.name());

    OTCEV_Timeout::start(id(),period_);
  }

  theEvent->destroy();
}

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -connect server [ -period milliseconds ]" << endl;
}

int main(int argc, char* argv[])
{
  OTC_Program::initialise(argc,argv);

  OTC_String theServerString;
  OTC_String thePeriodString;

  thePeriodString = "1000";

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

  OTC_String theHostname;
  int thePort = 0;

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

  int thePeriod;

  OTC_InetClient* theClient;
  theClient = new OTC_InetClient("STREAM",theHostname,thePort,5000);
  theClient->reference();

  theClient->start();

  thePeriod = atoi(thePeriodString);

  PRG_Client theTestClient(theClient->localAddress(),thePeriod);

  theClient->addObserver(theTestClient.id());

  OTCEV_Shutdown::monitor(SIGINT);

  OTC_Dispatcher::run();

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
