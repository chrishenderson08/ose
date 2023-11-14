#ifndef OPY_REQUEST_HH
#define OPY_REQUEST_HH
/*
// ============================================================================
//
// = FILENAME
//     request.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/request.hh>

/* ------------------------------------------------------------------------- */

class OPY_Request : public PyObject
{
  public:

  			OPY_Request(OTCEV_Request* theRequest);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_sender(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_conversationId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sequenceNumber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_methodName(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_content(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_bounceRequest(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sendResponse(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sendFailure(PyObject*, PyObject* theArgs);

  protected:

			~OPY_Request();

  private:

			OPY_Request(OPY_Request const&);

    OPY_Request&	operator=(OPY_Request const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Request*	request_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_REQUEST_HH */
