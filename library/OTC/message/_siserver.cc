/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/_iserver.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetlstn.hh>
#include <OTC/message/connect.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/system/program.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/logger.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

OTC_InetListener* theListener;

class PRG_Server : public OTC_EVAgent
{
  public:

                        ~PRG_Server() {}

                        PRG_Server(char const* theReceiver)
				{ inBox_.bind(this,theReceiver); }

    void                handle(OTC_Event* theEvent);

  private:

    OTC_Receiver        inBox_;
};

void PRG_Server::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  cout << *theEvent << endl;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    if (theConnection->state() == OTCLIB_CONNECTION_INACTIVE)
      delete this;
  }
  else if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->message() != 0 &&
     theEnvelope->message()->content() == "HELLO")
    {
      OTCEV_Message* theMessage;
      theMessage = new OTCEV_Message("HELLO IN REPLY");
      OTCLIB_ASSERT(theMessage != 0);

      theEnvelope->reply(theMessage);
    }
  }

  theEvent->destroy();
}

void service_callback(OTC_Event* theEvent, int)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    if (theConnection->state() == OTCLIB_CONNECTION_ACTIVE)
    {
      cout << *theEvent << endl;

      OTC_EndPoint* theEndPoint;
      theEndPoint = OTC_EndPoint::local(theConnection->localAddress());

      if (theEndPoint != 0)
      {
	PRG_Server* theServer;
	theServer = new PRG_Server(theConnection->localAddress());

	theEndPoint->addObserver(theServer->id());
      }
    }
  }

  theEvent->destroy();
}

void usage()
{
  cerr << "Usage: " << OTC_Program::basename();
  cerr << " -listen port" << endl;
}

int main(int argc, char* argv[])
{
  OTC_Program::initialise(argc,argv);

  OTC_String thePortString;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::option(1) == "-listen")
    {
      OTC_Program::shift(1);
      if (OTC_Program::numOptions() == 0)
        break;

      thePortString = OTC_Program::option(1);
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

  int thePort;

  if (sscanf(thePortString,"%d",&thePort) != 1)
  {
    cerr << OTC_Program::basename() << ": invalid port number" << endl;
    return 1;
  }

  theListener = new OTC_InetListener("STREAM",thePort);
  theListener->reference();

  theListener->start();

  if (theListener->fd() == -1)
  {
    cerr << OTC_Program::basename() << ": listener failed" << endl;
    return 1;
  }

  OTC_FNAgent theService(service_callback);

  theListener->addObserver(theService.id());

  OTC_Dispatcher::run();

  theListener->unReference();

  return 0;
}

/* ------------------------------------------------------------------------- */
