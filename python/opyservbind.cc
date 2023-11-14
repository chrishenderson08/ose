/*
// ============================================================================
//
// = FILENAME
//     opyservbind.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "servbind.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_ServiceBinding::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "ServiceBinding",
  sizeof(OPY_ServiceBinding),
  0,
  OPY_ServiceBinding::sys_dealloc,
  0,
  OPY_ServiceBinding::sys_getattr,
  0,
  OPY_ServiceBinding::sys_compare,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OPY_ServiceBinding::gMethods_[] = {
  { "serviceName", OPY_ServiceBinding::mfn_serviceName, 1 },
  { "serviceAudience", OPY_ServiceBinding::mfn_serviceAudience, 1 },
  { "receiverName", OPY_ServiceBinding::mfn_receiverName, 1 },
  { "agentIdentity", OPY_ServiceBinding::mfn_agentIdentity, 1 },
  { "processAddress", OPY_ServiceBinding::mfn_processAddress, 1 },
  { "processIdentity", OPY_ServiceBinding::mfn_processIdentity, 1 },
  { "networkGroup", OPY_ServiceBinding::mfn_networkGroup, 1 },
  { "agentAddress", OPY_ServiceBinding::mfn_agentAddress, 1 },
  { "serviceAddress", OPY_ServiceBinding::mfn_serviceAddress, 1 },
  { "serviceLocation", OPY_ServiceBinding::mfn_serviceLocation, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_ServiceBinding::~OPY_ServiceBinding()
{
  serviceBinding_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_ServiceBinding::OPY_ServiceBinding(
 OTC_ServiceBinding* theServiceBinding
)
  : serviceBinding_(0)
{
  OTCLIB_ENSURE_FN((theServiceBinding != 0),
   "OPY_ServiceBinding::OPY_ServiceBinding(OTC_ServiceBinding*)",
   "invalid service binding supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  serviceBinding_ = theServiceBinding->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
int OPY_ServiceBinding::obj_compare(
 OPY_ServiceBinding const* theObject
) const
{
  return serviceBinding_->serviceAddress().rank(
   ((OPY_ServiceBinding*)theObject)->serviceBinding_->serviceAddress());
}

/* ------------------------------------------------------------------------- */
void OPY_ServiceBinding::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_ServiceBinding*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_ServiceBinding*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
int OPY_ServiceBinding::sys_compare(
 PyObject* theInstance,
 PyObject* theObject
)
{
  return ((OPY_ServiceBinding*)theInstance)->obj_compare(
   (OPY_ServiceBinding*)theObject);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_serviceName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->serviceName().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_serviceAudience(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->serviceAudience().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_receiverName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->receiverName().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_agentIdentity(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->agentIdentity().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_processAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->processAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_processIdentity(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->processIdentity().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_networkGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->networkGroup().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_agentAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->agentAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_serviceAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("s",theBinding->serviceAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ServiceBinding::mfn_serviceLocation(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding const* theBinding;
  theBinding = ((OPY_ServiceBinding*)theInstance)->serviceBinding_;

  return Py_BuildValue("i",theBinding->serviceLocation());
}

/* ------------------------------------------------------------------------- */
