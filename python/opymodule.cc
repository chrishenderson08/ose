/*
// ============================================================================
//
// = FILENAME
//     opymodule.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef ANY
#undef ANY
#endif

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>

#include "module.hh"
#include "dispatch.hh"
#include "program.hh"
#include "logger.hh"
#include "agent.hh"
#include "service.hh"
#include "exchange.hh"
#include "rpcgtway.hh"
#include "filesrvr.hh"
#include "httpdaem.hh"
#include "httpsrvr.hh"
#include "httpsrvl.hh"
#include "monitor.hh"
#include "echosrvl.hh"
#include "filesrvl.hh"
#include "rpcsrvlt.hh"
#include "xmlrpcgw.hh"
#include "xmlrpcsl.hh"
#include "job.hh"

/* ------------------------------------------------------------------------- */
PyObject* OTCLIB_PYTHON_ERROR_OBJECT;

/* ------------------------------------------------------------------------- */
static struct PyMethodDef otclib_module_methods[] = {
  { "init_JobCallback", OPY_Job::init_JobCallback, 1 },
  { "new_Logger", OPY_Logger::create, 1 },
  { "new_Agent", OPY_Agent::create, 1 },
  { "new_Service", OPY_Service::create, 1 },
  { "new_Monitor", OPY_Monitor::create, 1 },
  { "new_Exchange", OPY_Exchange::create, 1 },
  { "new_HttpDaemon", OPY_HttpDaemon::create, 1 },
  { "new_HttpServer", OPY_HttpServer::create, 1 },
  { "new_HttpServlet", OPY_HttpServlet::create, 1 },
  { "new_EchoServlet", OPY_EchoServlet::create, 1 },
  { "new_FileServlet", OPY_FileServlet::create, 1 },
  { "new_RpcServlet", OPY_RpcServlet::create, 1 },
  { "new_XmlRpcServlet", OPY_XmlRpcServlet::create, 1 },
  { "new_FileServer", OPY_FileServer::create, 1 },
  { "new_RpcGateway", OPY_RpcGateway::create, 1 },
  { "new_XmlRpcGateway", OPY_XmlRpcGateway::create, 1 },
  { "Dispatcher", OPY_Dispatcher::instance, 1 },
  { "Program", OPY_Program::instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
void otclib_init_python()
{
  PyObject* theModule;

  // Enable display of exceptions.

  OTC_Exception::enableLogging();


  // Create the module and add the functions.

  theModule = Py_InitModule("_ose",otclib_module_methods);

  // Check for errors.

  if (PyErr_Occurred())
    Py_FatalError("can't initialise module _ose");
}

/* ------------------------------------------------------------------------- */
