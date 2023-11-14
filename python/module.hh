#ifndef OPY_MODULE_HH
#define OPY_MODULE_HH
/*
// ============================================================================
//
// = FILENAME
//     module.hh
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

#include <OTC/OTC.h>

/* ------------------------------------------------------------------------- */

extern "C" void otclib_init_python();

extern PyObject* OTCLIB_PYTHON_ERROR_OBJECT;

/* ------------------------------------------------------------------------- */

#endif /* OPY_MODULE_HH */
