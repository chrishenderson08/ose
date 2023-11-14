/*
// ============================================================================
//
// = FILENAME
//     opyexchannc.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "exchannc.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_ExchangeAnnouncement::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "ExchangeAnnouncement",
  sizeof(OPY_ExchangeAnnouncement),
  0,
  OPY_ExchangeAnnouncement::sys_dealloc,
  0,
  OPY_ExchangeAnnouncement::sys_getattr,
  0,
  OPY_ExchangeAnnouncement::sys_compare,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OPY_ExchangeAnnouncement::gMethods_[] = {
  { "group", OPY_ExchangeAnnouncement::mfn_group, 1 },
  { "localAddress", OPY_ExchangeAnnouncement::mfn_localAddress, 1 },
  { "remoteAddress", OPY_ExchangeAnnouncement::mfn_remoteAddress, 1 },
  { "remoteProcess", OPY_ExchangeAnnouncement::mfn_remoteProcess, 1 },
  { "state", OPY_ExchangeAnnouncement::mfn_state, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_ExchangeAnnouncement::~OPY_ExchangeAnnouncement()
{
  announcement_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_ExchangeAnnouncement::OPY_ExchangeAnnouncement(
 OTCEV_ExchangeAnnouncement* theAnnouncement
)
  : announcement_(0)
{
  OTCLIB_ENSURE_FN((theAnnouncement != 0),
   "OPY_ExchangeAnnouncement::OPY_ExchangeAnnouncement(...)",
   "invalid exchange announcement supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  announcement_ = (OTCEV_ExchangeAnnouncement*)theAnnouncement->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ExchangeAnnouncement::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
int OPY_ExchangeAnnouncement::obj_compare(
 OPY_ExchangeAnnouncement const* theObject
) const
{
  return announcement_->remoteProcess().rank(
   ((OPY_ExchangeAnnouncement*)theObject)->announcement_->remoteProcess());
}

/* ------------------------------------------------------------------------- */
void OPY_ExchangeAnnouncement::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_ExchangeAnnouncement*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ExchangeAnnouncement::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_ExchangeAnnouncement*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
int OPY_ExchangeAnnouncement::sys_compare(
 PyObject* theInstance,
 PyObject* theObject
)
{
  return ((OPY_ExchangeAnnouncement*)theInstance)->obj_compare(
   (OPY_ExchangeAnnouncement*)theObject);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ExchangeAnnouncement::mfn_group(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OPY_ExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("s",theAnnouncement->group().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ExchangeAnnouncement::mfn_localAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OPY_ExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("s",theAnnouncement->localAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ExchangeAnnouncement::mfn_remoteAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OPY_ExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("s",theAnnouncement->remoteAddress().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ExchangeAnnouncement::mfn_remoteProcess(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OPY_ExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("s",theAnnouncement->remoteProcess().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_ExchangeAnnouncement::mfn_state(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_ExchangeAnnouncement const* theAnnouncement;
  theAnnouncement = ((OPY_ExchangeAnnouncement*)theInstance)->announcement_;

  return Py_BuildValue("i",theAnnouncement->state());
}

/* ------------------------------------------------------------------------- */
