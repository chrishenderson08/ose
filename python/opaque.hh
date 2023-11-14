#ifndef OPY_OPAQUE_HH
#define OPY_OPAQUE_HH
/*
// ============================================================================
//
// = FILENAME
//     opaque.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2002-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/opaque.hh>

/* ------------------------------------------------------------------------- */

class OPY_Opaque : public PyObject
{
  public:

  			OPY_Opaque(OTCEV_Opaque* theMessage);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_sender(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_conversationId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_sequenceNumber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_messageId(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_description(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_content(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_contentType(PyObject*, PyObject* theArgs);

  protected:

			~OPY_Opaque();

  private:

			OPY_Opaque(OPY_Opaque const&);

    OPY_Opaque&	operator=(OPY_Opaque const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Opaque*	message_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_OPAQUE_HH */
