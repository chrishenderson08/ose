#ifndef OPY_PROGRAM_HH
#define OPY_PROGRAM_HH
/*
// ============================================================================
//
// = FILENAME
//     program.hh
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

#include <OTC/system/program.hh>

/* ------------------------------------------------------------------------- */

class OPY_Program : public PyObject
{
  public:

			~OPY_Program();

  			OPY_Program();

    static PyObject*	instance(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_removeAllConfig(PyObject*, PyObject*);

    static PyObject*	mfn_removeConfig(PyObject*, PyObject*);

    static PyObject*	mfn_loadConfig(PyObject*, PyObject*);

    static PyObject*	mfn_mergeConfig(PyObject*, PyObject*);

    static PyObject*	mfn_lookupConfig(PyObject*, PyObject*);

    static PyObject*	mfn_saveConfig(PyObject*, PyObject*);

    static PyObject*	mfn_lookupEnviron(PyObject*, PyObject*);

    static PyObject*	mfn_mergeEnviron(PyObject*, PyObject*);

    static PyObject*	mfn_expandEnviron(PyObject*, PyObject*);

    static PyObject*	mfn_processIdentity(PyObject*, PyObject*);

    static PyObject*	mfn_uniqueId(PyObject*, PyObject*);

    static PyObject*	mfn_threadingEnabled(PyObject*, PyObject*);

  private:

			OPY_Program(OPY_Program const&);

    OPY_Program&	operator=(OPY_Program const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    static OPY_Program*	gInstance_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_PROGRAM_HH */
