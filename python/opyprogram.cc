/*
// ============================================================================
//
// = FILENAME
//     opyprogram.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "program.hh"

#include <OTC/text/slogger.hh>

#include <OTC/stdlib/fstream.h>

/* ------------------------------------------------------------------------- */
OPY_Program* OPY_Program::gInstance_ = 0;

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Program::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Program",
  sizeof(OPY_Program),
  0,
  OPY_Program::sys_dealloc,
  0,
  OPY_Program::sys_getattr,
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
PyMethodDef OPY_Program::gMethods_[] = {
  { "removeAllConfig", OPY_Program::mfn_removeAllConfig, 1 },
  { "removeConfig", OPY_Program::mfn_removeConfig, 1 },
  { "loadConfig", OPY_Program::mfn_loadConfig, 1 },
  { "mergeConfig", OPY_Program::mfn_mergeConfig, 1 },
  { "lookupConfig", OPY_Program::mfn_lookupConfig, 1 },
  { "saveConfig", OPY_Program::mfn_saveConfig, 1 },
  { "lookupEnviron", OPY_Program::mfn_lookupEnviron, 1 },
  { "mergeEnviron", OPY_Program::mfn_mergeEnviron, 1 },
  { "expandEnviron", OPY_Program::mfn_expandEnviron, 1 },
  { "processIdentity", OPY_Program::mfn_processIdentity, 1 },
  { "uniqueId", OPY_Program::mfn_uniqueId, 1 },
  { "threadingEnabled", OPY_Program::mfn_threadingEnabled, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Program::~OPY_Program()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OPY_Program::OPY_Program()
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::instance(PyObject* theModule, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  if (gInstance_ == 0)
  {
    gInstance_ = new OPY_Program;
    OTCLIB_ASSERT_M(gInstance_ != 0);
  }

  Py_XINCREF(gInstance_);

  return (PyObject*)gInstance_;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Program::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Program*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Program*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_removeAllConfig(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Program::removeAllConfig();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_removeConfig(PyObject*, PyObject* theArgs)
{
  char const* theKey;

  if (!PyArg_ParseTuple(theArgs,"s",&theKey))
    return 0;

  OTC_Program::removeConfig(theKey);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_loadConfig(PyObject*, PyObject* theArgs)
{
  char const* theFileName;
  char const* theChannel = "";

  if (!PyArg_ParseTuple(theArgs,"s|z",&theFileName,&theChannel))
    return 0;

  ifstream theStream(theFileName);

  if (theStream.fail())
  {
    PyErr_SetString(PyExc_IOError,
     "invalid configuration file");

    return 0;
  }

  OTC_String tmpChannel;
  tmpChannel = OTC_Program::uniqueId("$CID?",OTCLIB_ID_SHORT_FORMAT);

  OTC_SLogger tmpLogger(tmpChannel);

  OTC_Program::loadConfig(theStream,tmpChannel);

  OTC_String tmpMessages(tmpLogger.messages());

  if (theChannel != 0 && !tmpMessages.isEmpty())
  {
    OTC_Logger::notify(theChannel,OTCLIB_LOG_DEBUG,
     tmpMessages,tmpMessages.length());
  }

  if (!tmpMessages.isEmpty())
    return Py_BuildValue("s#",tmpMessages.string(),tmpMessages.length());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_mergeConfig(PyObject*, PyObject* theArgs)
{
  char const* theKey;
  char const* theValue;

  if (!PyArg_ParseTuple(theArgs,"ss",&theKey,&theValue))
    return 0;

  OTC_Program::mergeConfig(theKey,theValue);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_lookupConfig(PyObject*, PyObject* theArgs)
{
  char const* theKey;

  if (!PyArg_ParseTuple(theArgs,"s",&theKey))
    return 0;

  OTC_String theValue;
  theValue = OTC_Program::lookupConfig(theKey);

  if (theValue.isUndefined())
  {
    Py_XINCREF(Py_None);

    return Py_None;
  }

  return Py_BuildValue("s",theValue.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_saveConfig(PyObject*, PyObject* theArgs)
{
  char const* theFileName;

  if (!PyArg_ParseTuple(theArgs,"s",&theFileName))
    return 0;

  ofstream theStream(theFileName);

  OTC_Program::saveConfig(theStream);

  if (theStream.fail())
  {
    PyErr_SetString(PyExc_IOError,
     "error writing configuration file");

    return 0;
  }

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_lookupEnviron(PyObject*, PyObject* theArgs)
{
  char const* theKey;

  if (!PyArg_ParseTuple(theArgs,"s",&theKey))
    return 0;

  OTC_String theValue;
  theValue = OTC_Program::lookupEnviron(theKey);

  if (theValue.isUndefined())
  {
    Py_XINCREF(Py_None);

    return Py_None;
  }

  return Py_BuildValue("s",theValue.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_mergeEnviron(PyObject*, PyObject* theArgs)
{
  char const* theKey;
  char const* theValue;

  if (!PyArg_ParseTuple(theArgs,"ss",&theKey,&theValue))
    return 0;

  OTC_Program::mergeEnviron(theKey,theValue);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_expandEnviron(PyObject*, PyObject* theArgs)
{
  char const* theString;

  if (!PyArg_ParseTuple(theArgs,"s",&theString))
    return 0;

  OTC_String theValue;
  theValue = OTC_Program::expandEnviron(theString);

  return Py_BuildValue("s",theValue.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_processIdentity(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_String theResult;
  theResult = OTC_Program::processIdentity();

  return Py_BuildValue("s",theResult.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_uniqueId(PyObject*, PyObject* theArgs)
{
  int theType = 0;
  char const* thePrefix = 0;

  if (!PyArg_ParseTuple(theArgs,"i|z",&theType,&thePrefix))
    return 0;

  OTC_String theResult;
  theResult = OTC_Program::uniqueId(thePrefix,OTC_UniqueIdType(theType));

  return Py_BuildValue("s",theResult.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Program::mfn_threadingEnabled(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  bool theResult;
  theResult = OTC_Program::threadingEnabled();

  return Py_BuildValue("i",theResult);
}

/* ------------------------------------------------------------------------- */
