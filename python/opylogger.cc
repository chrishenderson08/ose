/*
// ============================================================================
//
// = FILENAME
//     opylogger.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "logger.hh"
#include "dispatch.hh"

#include <OTC/debug/logger.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Logger::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Logger",
  sizeof(OPY_Logger),
  0,
  OPY_Logger::sys_dealloc,
  0,
  OPY_Logger::sys_getattr,
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
PyMethodDef OPY_Logger::gMethods_[] = {
  { "notify", OPY_Logger::mfn_notify, 1 },
  { "notifyChannel", OPY_Logger::mfn_notifyChannel, 1 },
  { "logFile", OPY_Logger::mfn_logFile, 1 },
  { "setLogFile", OPY_Logger::mfn_setLogFile, 1 },
  { "stderrOutputEnabled", OPY_Logger::mfn_stderrOutputEnabled, 1 },
  { "enableStderrOutput", OPY_Logger::mfn_enableStderrOutput, 1 },
  { "disableStderrOutput", OPY_Logger::mfn_disableStderrOutput, 1 },
  { "longFormatEnabled", OPY_Logger::mfn_longFormatEnabled, 1 },
  { "enableLongFormat", OPY_Logger::mfn_enableLongFormat, 1 },
  { "disableLongFormat", OPY_Logger::mfn_disableLongFormat, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Logger::~OPY_Logger()
{
  Py_XDECREF(callback_);
}

/* ------------------------------------------------------------------------- */
OPY_Logger::OPY_Logger(PyObject* theCallback)
  : callback_(theCallback)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(callback_);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theCallback;

  if (!PyArg_ParseTuple(theArgs,"O",&theCallback))
    return 0;

  OPY_Logger* theObject;
  theObject = new OPY_Logger(theCallback);
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Logger::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Logger*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Logger*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_notify(PyObject*, PyObject* theArgs)
{
  OTC_LogLevel theLevel;
  char* theMessage = 0;

  theLevel = OTCLIB_LOG_WARNING;

  if (!PyArg_ParseTuple(theArgs,"is",&theLevel,&theMessage))
    return 0;

  Py_BEGIN_ALLOW_THREADS

  OTC_Logger::notify(theLevel,theMessage);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_notifyChannel(PyObject*, PyObject* theArgs)
{
  OTC_LogLevel theLevel;
  char* theChannel = 0;
  char* theMessage = 0;

  theLevel = OTCLIB_LOG_WARNING;

  if (!PyArg_ParseTuple(theArgs,"sis",&theChannel,&theLevel,&theMessage))
    return 0;

  Py_BEGIN_ALLOW_THREADS

  OTC_Logger::notify(theChannel,theLevel,theMessage);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_logFile(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  return Py_BuildValue("s",OTC_Logger::logFile());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_setLogFile(PyObject*, PyObject* theArgs)
{
  char* theLogFile = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theLogFile))
    return 0;

  OTC_Logger::setLogFile(theLogFile);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_stderrOutputEnabled(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  return Py_BuildValue("i",OTC_Logger::stderrOutputEnabled());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_enableStderrOutput(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Logger::enableStderrOutput();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_disableStderrOutput(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Logger::disableStderrOutput();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_longFormatEnabled(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  return Py_BuildValue("i",OTC_Logger::longFormatEnabled());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_enableLongFormat(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Logger::enableLongFormat();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Logger::mfn_disableLongFormat(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Logger::disableLongFormat();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OPY_Logger::log(
 char const* theChannel,
 OTC_LogLevel theLevel,
 char const* theMessage,
 u_int theLength
)
{
  if (theChannel == 0 || *theChannel == EOS)
    return;

  OPY_Dispatcher::enterPython();

  PyObject* theArgs;
  PyObject* theResult;

  theArgs = Py_BuildValue("(sis#)",
   theChannel,int(theLevel),theMessage,theLength);

  if (theArgs == 0)
  {
    PyErr_Clear();

    OPY_Dispatcher::leavePython();

    return;
  }

  theResult = PyEval_CallObject(callback_,theArgs);

  Py_XDECREF(theArgs);

  if (theResult == 0)
  {
    PyErr_Clear();

    OPY_Dispatcher::leavePython();

    return;
  }

  Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
