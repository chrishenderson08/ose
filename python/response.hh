#ifndef OPY_RESPONSE_HH
#define OPY_RESPONSE_HH
/*
// ============================================================================
//
// = FILENAME
//     response.hh
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

#include <OTC/dispatch/response.hh>

/* ------------------------------------------------------------------------- */

class OPY_Response : public PyObject
{
  public:

  			OPY_Response(OTCEV_Response* theRequest);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_sender(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_conversationId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sequenceNumber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_content(PyObject*, PyObject* theArgs);

  protected:

			~OPY_Response();

  private:

			OPY_Response(OPY_Response const&);

    OPY_Response&	operator=(OPY_Response const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Response*	response_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_RESPONSE_HH */
