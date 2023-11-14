/*
// ============================================================================
//
// = FILENAME
//     opyjob.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "job.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyObject* OPY_Job::gJobCallback_ = 0;

/* ------------------------------------------------------------------------- */
OPY_Job::~OPY_Job()
{
  OPY_Dispatcher::enterPython();

  Py_XDECREF(callback_);

  OPY_Dispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Job::init_JobCallback(
 PyObject* theModule,
 PyObject* theArgs
)
{
  PyObject* theCallback = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theCallback))
    return 0;

  Py_XINCREF(theCallback);

  gJobCallback_ = theCallback;

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
OPY_Job::OPY_Job(PyObject* theCallback)
  : callback_(theCallback)
{
  Py_XINCREF(callback_);
}

/* ------------------------------------------------------------------------- */
void OPY_Job::execute()
{
  OPY_Dispatcher::enterPython();

  PyObject* theArgs;
  PyObject* theResult;

  // theArgs = Py_BuildValue("()");
  // theResult = PyEval_CallObject(callback_,theArgs);

  theArgs = Py_BuildValue("(O,())",callback_);
  theResult = PyEval_CallObject(gJobCallback_,theArgs);

  Py_XDECREF(theArgs);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    OPY_Dispatcher::leavePython();

    return;
  }

  Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
