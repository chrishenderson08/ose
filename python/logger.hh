#ifndef OPY_LOGGER_HH
#define OPY_LOGGER_HH
/*
// ============================================================================
//
// = FILENAME
//     logger.hh
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

#include <OTC/debug/logger.hh>

/* ------------------------------------------------------------------------- */

class OPY_Logger : public PyObject, public OTC_Logger
{
  public:

			~OPY_Logger();

  			OPY_Logger(PyObject* theCallback);

    static PyObject*	create(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_notify(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_notifyChannel(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_logFile(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_setLogFile(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_stderrOutputEnabled(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_enableStderrOutput(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_disableStderrOutput(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_longFormatEnabled(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_enableLongFormat(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_disableLongFormat(PyObject*, PyObject* theArgs);

  protected:

    void		log(
			 char const* theChannel,
			 OTC_LogLevel theLevel,
			 char const* theMessage,
			 u_int theLength
			);

  private:

			OPY_Logger(OPY_Logger const&);

    OPY_Logger&	operator=(OPY_Logger const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    PyObject*		callback_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_LOGGER_HH */
