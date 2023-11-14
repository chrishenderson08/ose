/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/_consumer.cc
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
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/system/program.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class PRG_Consumer : public OTC_EVAgent
{
  public:

			PRG_Consumer();
    void                handle(OTC_Event* theEvent);

  private:

    OTC_Receiver	inBox_;
};

PRG_Consumer::PRG_Consumer()
{
  inBox_.bind(this,"HEARTBEAT");
}

void PRG_Consumer::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->subject() == "BLIP")
    {
      OTCEV_Message* theMessage;
      theMessage = theEnvelope->message();

      cout << theEnvelope->conversationId();
      cout << " ";
      cout << theMessage->content();
      cout << endl;

      OTCEV_Action::schedule(id(),OTCLIB_IDLE_JOB);
    }
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction;
    theAction = (OTCEV_Action*)theEvent;

    cout << "idle " << theAction->action() << endl;
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

  int theValidArgs = 0;
  OTC_String theGroupName;

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

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_CLIENT,theGroupName);

  OTC_EPClient* theClient = 0;

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

    OTC_InetClient* tmpClient;
    tmpClient = new OTC_InetClient("MESSAGE",theHostname,thePort,1000);
    tmpClient->reference();

    theClient = tmpClient;
  }

  theExchange.connect(theClient);

  PRG_Consumer theConsumer;

  OTC_Dispatcher::run();

  theClient->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
