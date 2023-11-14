#ifndef OPY_EXCHANGE_HH
#define OPY_EXCHANGE_HH
/*
// ============================================================================
//
// = FILENAME
//     exchange.hh
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

#include <OTC/message/exchange.hh>

/* ------------------------------------------------------------------------- */

class OPY_Exchange : public PyObject, public OTC_EVAgent
{
    friend class	OPY_ExchangeListener;

  public:

  			OPY_Exchange(
			 PyObject* theOuter,
			 OTC_ExchangeType theType,
			 char const* theGroup=0
			);

    static PyObject*	create(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_connect(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_listen(PyObject*, PyObject* theArgs);

  protected:

    void		handle(OTC_Event* theEvent);

    bool		authorise(OTC_String const& theClientHost);

  protected:

			~OPY_Exchange();

    PyObject*		outer_;

  private:

			OPY_Exchange(OPY_Exchange const&);

    OPY_Exchange&	operator=(OPY_Exchange const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTC_Exchange	exchange_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_EXCHANGE_HH */
