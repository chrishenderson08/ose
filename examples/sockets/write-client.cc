/*
// ============================================================================
// 
// = FILENAME
//     write-client.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetclnt.hh>
#include <OTC/message/connect.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/types/time.hh>

#include <signal.h>

/* ------------------------------------------------------------------------- */

void event_callback(OTC_Event* theEvent, int theAgentId)
{
  if (theEvent == 0)
    return;

  static OTC_String theRemoteAddress;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    // Received notification about state of
    // connection.

    if (theConnection->state() == OTCLIB_CONNECTION_ACTIVE)
    {
      // New connection established. Start a timer
      // to trigger sending of data over the
      // socket. Need to remember what remote
      // address is, as that is what we are going
      // to send to.

      theRemoteAddress = theConnection->remoteAddress();

      OTCEV_Timeout::start(theAgentId,1000);
    }
    else
    {
      // Connection has been closed or could
      // not be made. Stop the dispatcher so
      // program exits.

      OTCEV_Timeout::cancelAgent(theAgentId);

      OTC_Dispatcher::stop();
    }
  }
  else if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    // Received data read from the socket.
    // Write it out so we can see it.

    if (theEnvelope->message() != 0)
      cout << theEnvelope->message()->content() << flush;
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    // Write date/time out onto socket. The
    // destination address to achieve this
    // is the remote address we saved earlier.

    OTC_String theContent;
    theContent += OTC_Time::currentTime().asString();
    theContent += EOL;

    OTCEV_Message* theMessage;
    theMessage = new OTCEV_Message(theContent);

    theMessage->queue(theRemoteAddress);

    // Start timer again.

    OTCEV_Timeout::start(theAgentId,1000);
  }

  theEvent->destroy();
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    cerr << "Usage: " << "write-client hostname port" << endl;
    return 1;
  }

  // Initialise the dispatcher.

  OTC_Dispatcher::initialise();

  // Shutdown gracefully if we get CTRL-C sent
  // to the process.

  OTCEV_Shutdown::monitor(SIGINT);

  // Create callback agent.

  OTC_FNAgent theAgent(event_callback);

  // Create client.

  OTC_InetClient theClient("STREAM",argv[1],atoi(argv[2]));

  // Add callback as observer of the listener
  // so we know about change in state of the
  // connection.

  theClient.addObserver(theAgent.id());

  // Bind local address of client to callback
  // agent so we receive any data read from
  // the socket.

  OTC_Receiver theReceiver(&theAgent,theClient.localAddress());

  // Start the client.

  theClient.start();

  // Run the dispatcher.

  OTC_Dispatcher::run();

  return 0;
}

/* ------------------------------------------------------------------------- */
