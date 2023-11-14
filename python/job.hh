#ifndef OPY_JOB_HH
#define OPY_JOB_HH
/*
// ============================================================================
//
// = FILENAME
//     job.hh
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

#include <OTC/dispatch/job.hh>

/* ------------------------------------------------------------------------- */

class OPY_Job : public OTC_Job
{
  public:

			~OPY_Job();

			OPY_Job(PyObject* theCallback);

    void		execute();

    static PyObject*	init_JobCallback(PyObject*, PyObject*);

  private:

			OPY_Job(OPY_Job const&);

    OPY_Job&		operator=(OPY_Job const&);

    PyObject*		callback_;

    static PyObject*	gJobCallback_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_JOB_HH */
