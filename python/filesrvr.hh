#ifndef OPY_FILESRVR_HH
#define OPY_FILESRVR_HH
/*
// ============================================================================
//
// = FILENAME
//     filesrvr.hh
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

#include <OTC/internet/filesrvr.hh>

/* ------------------------------------------------------------------------- */

class OPY_FileServer : public PyObject, public OTC_FileServer
{
  public:

  			OPY_FileServer(
			 PyObject* theOuter,
			 char const* theDirectory
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    static PyObject*	mfn_directory(PyObject*, PyObject*);

    static PyObject*	mfn_map(PyObject*, PyObject*);

    bool		authorise(
			 char const* theLogin,
			 char const* thePassword
			);

  protected:

			~OPY_FileServer();

    PyObject*		outer_;

  private:

			OPY_FileServer(OPY_FileServer const&);

    OPY_FileServer&	operator=(OPY_FileServer const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_FILESRVR_HH */
