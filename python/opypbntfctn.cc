/*
// ============================================================================
//
// = FILENAME
//     opypbntfctn.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "pbntfctn.hh"
#include "servbind.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_PublisherNotification::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "PublisherNotification",
  sizeof(OPY_PublisherNotification),
  0,
  OPY_PublisherNotification::sys_dealloc,
  0,
  OPY_PublisherNotification::sys_getattr,
  0,
  OPY_PublisherNotification::sys_compare,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OPY_PublisherNotification::gMethods_[] = {
  { "publisher", OPY_PublisherNotification::mfn_publisher, 1 },
  { "subscriber", OPY_PublisherNotification::mfn_subscriber, 1 },
  { "status", OPY_PublisherNotification::mfn_status, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_PublisherNotification::~OPY_PublisherNotification()
{
  notification_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_PublisherNotification::OPY_PublisherNotification(
 OTCEV_PublisherNotification* theNotification
)
  : notification_(0)
{
  OTCLIB_ENSURE_FN((theNotification != 0),
   "OPY_PublisherNotification::OPY_PublisherNotification(...)",
   "invalid publisher notification event supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  notification_ = (OTCEV_PublisherNotification*)theNotification->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_PublisherNotification::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
int OPY_PublisherNotification::obj_compare(
 OPY_PublisherNotification const* theObject
) const
{
  return notification_->publisher()->serviceAddress().rank(
   ((OPY_PublisherNotification*)theObject)->notification_->publisher()->serviceAddress());
}

/* ------------------------------------------------------------------------- */
void OPY_PublisherNotification::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_PublisherNotification*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_PublisherNotification::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_PublisherNotification*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
int OPY_PublisherNotification::sys_compare(
 PyObject* theInstance,
 PyObject* theObject
)
{
  return ((OPY_PublisherNotification*)theInstance)->obj_compare(
   (OPY_PublisherNotification*)theObject);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_PublisherNotification::mfn_publisher(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_PublisherNotification const* theNotification;
  theNotification = ((OPY_PublisherNotification*)theInstance)->notification_;

  OPY_ServiceBinding* theObject;
  theObject = new OPY_ServiceBinding(theNotification->publisher());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_PublisherNotification::mfn_subscriber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_PublisherNotification const* theNotification;
  theNotification = ((OPY_PublisherNotification*)theInstance)->notification_;

  OPY_ServiceBinding* theObject;
  theObject = new OPY_ServiceBinding(theNotification->subscriber());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_PublisherNotification::mfn_status(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_PublisherNotification const* theNotification;
  theNotification = ((OPY_PublisherNotification*)theInstance)->notification_;

  return Py_BuildValue("i",theNotification->status());
}

/* ------------------------------------------------------------------------- */
