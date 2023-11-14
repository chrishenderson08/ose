/*
// ============================================================================
//
// = FILENAME
//     chargen-server.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetlstn.hh>
#include <OTC/message/connect.hh>
#include <OTC/message/congest.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/action.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

class Server : public OTC_EVAgent
{
  public:

                        ~Server();

                        Server(char const* theLocalAddress);

  protected:

    void                handle(OTC_Event* theEvent);

  private:

    OTC_EndPoint*	endPoint_;

    char		start_;
};

Server::~Server()
{
  endPoint_->unReference();
}

Server::Server(char const* theLocalAddress)
  : start_(0)
{
  endPoint_ = OTC_EndPoint::local(theLocalAddress);
  endPoint_->reference();

  // Add server as observer so we know
  // when connection is closed and to
  // received congestion events.

  endPoint_->addObserver(id());

  // Queue action to trigger server to
  // send data.

  OTCEV_Action::schedule(id(),OTCLIB_IDLE_JOB);
}

void Server::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    if (theConnection->state() != OTCLIB_CONNECTION_ACTIVE)
    {
      // Socket has closed so stop any outstanding
      // action and delete this server.

      OTCEV_Action::cancelAgent(id());

      delete this;
    }
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    // Construct message content and send it.

    OTC_String theContent;

    for (int i=start_; i<start_+72; i++)
      theContent += char(' '+(i%95));

    theContent += EOL;

    OTCEV_Message* theMessage;
    theMessage = new OTCEV_Message(theContent);
    theMessage->queue(endPoint_->remoteAddress());

    start_ = (start_+1) % 95;

    // Queue action to trigger server to
    // send more data.

    OTCEV_Action::schedule(id(),OTCLIB_IDLE_JOB);
  }
  else if (theEvent->type() == OTCEV_Congestion::typeId())
  {
    OTCEV_Congestion* theCongestion;
    theCongestion = (OTCEV_Congestion*)theEvent;

    if (theCongestion->status() == OTCLIB_CONNECTION_BLOCKED)
    {
      // Connection has blocked. Cancel any
      // outstanding action.

      OTCEV_Action::cancelAgent(id());
    }
    else if (theCongestion->status() == OTCLIB_CONNECTION_CLEARED)
    {
      // Connection has been cleared. Queue
      // action to trigger server to send
      // more data.

      OTCEV_Action::schedule(id(),OTCLIB_IDLE_JOB);
    }
  }

  theEvent->destroy();
}

void event_callback(OTC_Event* theEvent, int)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    // Received notification about state of
    // connection.

    if (theConnection->state() == OTCLIB_CONNECTION_ACTIVE)
    {
      // Create a server object to deal with
      // the connection.

      Server* theServer;
      theServer = new Server(theConnection->localAddress());
    }
  }

  theEvent->destroy();
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cerr << "Usage: chargen-server port" << endl;
    return 1;
  }

  // Initialise dispatcher.

  OTC_Dispatcher::initialise();

  // Shutdown gracefully if we get CTRL-C sent
  // to the process.

  OTCEV_Shutdown::monitor(SIGINT);

  // Create callback agent.

  OTC_FNAgent theAgent(event_callback);

  // Create listener.

  OTC_InetListener theListener("STREAM",atoi(argv[1]));

  // Add callback as observer of the listener
  // so we know about new connections.

  theListener.addObserver(theAgent.id());

  // Start the listener.

  theListener.start();

  // Start the dispatcher.

  OTC_Dispatcher::run();

  return 0;
}

/* ------------------------------------------------------------------------- */
