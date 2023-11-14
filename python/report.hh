#ifndef OPY_REPORT_HH
#define OPY_REPORT_HH
/*
// ============================================================================
//
// = FILENAME
//     report.hh
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

#include <OTC/dispatch/report.hh>

/* ------------------------------------------------------------------------- */

class OPY_Report : public PyObject
{
  public:

  			OPY_Report(OTCEV_Report* theReport);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_publisher(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subjectName(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subjectPattern(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_content(PyObject*, PyObject* theArgs);

  protected:

			~OPY_Report();

  private:

			OPY_Report(OPY_Report const&);

    OPY_Report&	operator=(OPY_Report const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Report*	report_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_REPORT_HH */
