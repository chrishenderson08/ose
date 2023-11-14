/*
// ============================================================================
//
// = FILENAME
//     opyfilesrvr.cc
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

#include "filesrvr.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_FileServer::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "FileServer",
  sizeof(OPY_FileServer),
  0,
  OPY_FileServer::sys_dealloc,
  0,
  OPY_FileServer::sys_getattr,
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
PyMethodDef OPY_FileServer::gMethods_[] = {
  { "instance", OPY_FileServer::mfn_instance, 1 },
  { "directory", OPY_FileServer::mfn_directory, 1 },
  { "map", OPY_FileServer::mfn_map, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_FileServer::~OPY_FileServer()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_FileServer::OPY_FileServer(
 PyObject* theOuter,
 char const* theDirectory
)
  : OTC_FileServer(theDirectory), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServer::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  char const* theDirectory;

  if (!PyArg_ParseTuple(theArgs,"Os",&theOuter,&theDirectory))
    return 0;

  OPY_FileServer* theServer;
  theServer = new OPY_FileServer(theOuter,theDirectory);
  OTCLIB_ASSERT_M(theServer != 0);

  return (PyObject*)theServer;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServer::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_FileServer::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_FileServer*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServer::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_FileServer*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServer::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServer* theSelf;
  theSelf = (OPY_FileServer*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServer::mfn_directory(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_FileServer* theSelf;
  theSelf = (OPY_FileServer*)theInstance;

  return Py_BuildValue("s",theSelf->directory().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServer::mfn_map(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theExtension;
  char const* theFileType;

  if (!PyArg_ParseTuple(theArgs,"ss",&theExtension,&theFileType))
    return 0;

  OPY_FileServer* theSelf;
  theSelf = (OPY_FileServer*)theInstance;

  theSelf->map(theExtension,theFileType);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
bool OPY_FileServer::authorise(
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

    OPY_Dispatcher::leavePython();

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
