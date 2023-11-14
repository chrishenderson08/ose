/*
// ============================================================================
//
// = FILENAME
//     echo-server.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetlstn.hh>
#include <OTC/message/connect.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/shutdown.hh>

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

    OTC_Receiver        inBox_;
};

Server::~Server()
{
  // Nothing to do.
}

Server::Server(char const* theLocalAddress)
{
  OTC_EndPoint* theEndPoint;
  theEndPoint = OTC_EndPoint::local(theLocalAddress);

  // Add server as observer so we know
  // when connection is closed.

  theEndPoint->addObserver(id());

  // Bind local address for connection
  // so we can receive data read from
  // socket.

  inBox_.bind(this,theLocalAddress);
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
      // Socket has closed so delete this
      // server.

      delete this;
    }
  }
  else if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    // Received data read from the socket. Actual
    // data is the content of the contained
    // message. Echo back what was sent to us.

    if (theEnvelope->message() != 0)
      theEnvelope->reply(theEnvelope->message()->clone());
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
    cerr << "Usage: echo-server port" << endl;
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
