#ifndef OPY_HTTPSRVR_HH
#define OPY_HTTPSRVR_HH
/*
// ============================================================================
//
// = FILENAME
//     httpsrvr.hh
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

#include <OTC/internet/httpsrvr.hh>

/* ------------------------------------------------------------------------- */

class OPY_HttpServer : public PyObject, public OTC_HttpServer
{
  public:

  			OPY_HttpServer(PyObject* theOuter);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    bool		authorise(
			 char const* theLogin,
			 char const* thePassword
			);

    OTC_HttpServlet*	servlet(OTC_HttpSession* theSession);

  protected:

			~OPY_HttpServer();

    PyObject*		outer_;

  private:

			OPY_HttpServer(OPY_HttpServer const&);

    OPY_HttpServer&	operator=(OPY_HttpServer const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_HTTPSRVR_HH */
