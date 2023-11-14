#ifndef OPY_FAILURE_HH
#define OPY_FAILURE_HH
/*
// ============================================================================
//
// = FILENAME
//     failure.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/failure.hh>

/* ------------------------------------------------------------------------- */

class OPY_Failure : public PyObject
{
  public:

  			OPY_Failure(OTCEV_Failure* theRequest);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_conversationId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_error(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_description(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_origin(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_details(PyObject*, PyObject* theArgs);

  protected:

			~OPY_Failure();

  private:

			OPY_Failure(OPY_Failure const&);

    OPY_Failure&	operator=(OPY_Failure const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Failure*	failure_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_FAILURE_HH */
