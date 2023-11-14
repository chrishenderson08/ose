/*
// ============================================================================
//
// = FILENAME
//     opyxmlrpcgw.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef ANY
#undef ANY
#endif

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>

#include "xmlrpcgw.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_XmlRpcGateway::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "RpcGateway",
  sizeof(OPY_XmlRpcGateway),
  0,
  OPY_XmlRpcGateway::sys_dealloc,
  0,
  OPY_XmlRpcGateway::sys_getattr,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OPY_XmlRpcGateway::gMethods_[] = {
  { "instance", OPY_XmlRpcGateway::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_XmlRpcGateway::~OPY_XmlRpcGateway()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_XmlRpcGateway::OPY_XmlRpcGateway(
 PyObject* theOuter,
 char const* theSelection,
 OTC_ServiceSubscription theType
)
  : OTC_XmlRpcGateway(theSelection,theType), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_XmlRpcGateway::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  char const* theSelection;
  int theType;

  if (!PyArg_ParseTuple(theArgs,"Osi",&theOuter,&theSelection,&theType))
    return 0;

  OPY_XmlRpcGateway* theServer;
  theServer = new OPY_XmlRpcGateway(theOuter,
   theSelection,OTC_ServiceSubscription(theType));
  OTCLIB_ASSERT_M(theServer != 0);

  return (PyObject*)theServer;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_XmlRpcGateway::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_XmlRpcGateway::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_XmlRpcGateway*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_XmlRpcGateway::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_XmlRpcGateway*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_XmlRpcGateway::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServer* theSelf;
  theSelf = (OPY_XmlRpcGateway*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
bool OPY_XmlRpcGateway::authorise(
 char const* theLogin,
 char const* thePassword
)
{
  OPY_Dispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_authorise","ss",theLogin,thePassword);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    return false;
  }

  bool theAnswer;
  theAnswer = true;

  if (theResult == Py_None)
    theAnswer = false;

  Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython();

  return theAnswer;
}

/* ------------------------------------------------------------------------- */
