#ifndef OPY_HTTPDAEM_HH
#define OPY_HTTPDAEM_HH
/*
// ============================================================================
//
// = FILENAME
//     httpdaem.hh
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

#include <OTC/internet/httpdaem.hh>

/* ------------------------------------------------------------------------- */

class OPY_HttpDaemon : public PyObject, public OTC_HttpDaemon
{
  public:

  			OPY_HttpDaemon(PyObject* theOuter, int thePort);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_port(PyObject*, PyObject*);

    static PyObject*	mfn_start(PyObject*, PyObject*);

    static PyObject*	mfn_stop(PyObject*, PyObject*);

    static PyObject*	mfn_attach(PyObject*, PyObject*);

    bool		authorise(OTC_String const& theClientHost);

  protected:

			~OPY_HttpDaemon();

    PyObject*		outer_;

  private:

			OPY_HttpDaemon(OPY_HttpDaemon const&);

    OPY_HttpDaemon&	operator=(OPY_HttpDaemon const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_HTTPDAEM_HH */
