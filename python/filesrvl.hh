#ifndef OPY_FILESRVL_HH
#define OPY_FILESRVL_HH
/*
// ============================================================================
//
// = FILENAME
//     filesrvl.hh
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

#include <OTC/internet/filesrvl.hh>

/* ------------------------------------------------------------------------- */

class OPY_FileServlet : public PyObject, public OTC_FileServlet
{
  public:

  			OPY_FileServlet(
			 PyObject* theOuter,
			 OTC_HttpSession* theSession,
			 char const* theFileName,
			 char const* theFileType
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    void		destroy();

  protected:

			~OPY_FileServlet();

    PyObject*		outer_;

  private:

			OPY_FileServlet(OPY_FileServlet const&);

    OPY_FileServlet&	operator=(OPY_FileServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_FILESRVL_HH */
