/*
// ============================================================================
//
// = FILENAME
//     _ValidatorService
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "ValidatorService.hh"

#include <OTC/internet/httpdaem.hh>
#include <OTC/internet/rpcgtway.hh>
#include <OTC/internet/xmlrpcgw.hh>
#include <OTC/message/exchange.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/system/sighndle.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>

#include <signal.h>

int main()
{
  // Ensure consistent log and trace output no
  // matter what build variant of the class
  // library is used or what user environment
  // variables are set. The aim is for log and
  // trace output to always go to stderr. This
  // would be captured into a log file if it
  // was necessary by piping both stdout and
  // stderr of the process into a logging
  // application.

  OTC_Exception::enableLogging(); 

  OTC_Logger::enableStderrOutput();
  OTC_Logger::enableLongFormat();

  OTC_Tracer::enableLogTrace();

  // Create a log stream for later use.

  OTC_LogStream theLogger;
  theLogger.setLevel(OTCLIB_LOG_INFO);

  // Initialise the dispatcher.

  theLogger << "Initialising dispatcher" << endl;

  OTC_Dispatcher::initialise();

  // Monitor signals for shutdown.

  OTCEV_Shutdown::monitor(SIGINT);
  OTCEV_Shutdown::monitor(SIGTERM);

  // Ensure SIGHUP doesn't kill us. This allows
  // log and trace output to be piped into the
  // logging application with any premature
  // termination of the logging application not
  // causing us to die also.

  OTC_SignalHandler::install(SIGHUP,SIG_IGN);

  // Create service agents.

  TST_ValidatorService theService("validator","*");
  theService.scheduleStartup();

  // Create a HTTP daemon and RPC over HTTP
  // gateways bound to service groups or
  // specific services which we want to make
  // accessible remotely.

  OTC_HttpDaemon theDaemon(8111);

  OTC_RpcGateway theRpcGateway1(
   theService.serviceAddress(),OTCLIB_SERVICE_ADDRESS);
  theDaemon.attach("/netrpc/validator",&theRpcGateway1);

  OTC_XmlRpcGateway theRpcGateway2(
   theService.serviceAddress(),OTCLIB_SERVICE_ADDRESS);
  theDaemon.attach("/xmlrpc/validator",&theRpcGateway2);

  theDaemon.start();

  // Create message exchange.

  OTC_Exchange theExchange(OTCLIB_EXCHANGE_SERVER);
  
  OTC_InetListener* theListener;
  theListener = new OTC_InetListener("MESSAGE",11111);
  
  theExchange.listen(theListener);

  // Run the dispatcher.

  theLogger << "Starting dispatcher" << endl;

  OTC_Dispatcher::run();

  // Exit application.

  theLogger << "Exiting application" << endl;

  return 0;
}
