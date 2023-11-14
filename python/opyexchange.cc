/*
// ============================================================================
//
// = FILENAME
//     opyexchange.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "exchange.hh"
#include "exchannc.hh"

#ifdef ANY
#undef ANY
#endif

#include "dispatch.hh"

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>
#include <OTC/message/exchannc.hh>

/* ------------------------------------------------------------------------- */
class OPY_ExchangeListener : public OTC_InetListener
{
  public:
    			~OPY_ExchangeListener();
    			OPY_ExchangeListener(
			 char const* theInterface,
			 int thePort,
			 OPY_Exchange* theExchange
			)
			  : OTC_InetListener(theInterface,thePort),
			    exchange_(theExchange) {}
  protected:
    bool		authorise(OTC_String const& theClientHost);
  private:
    OPY_Exchange*	exchange_;
};

/* ------------------------------------------------------------------------- */
OPY_ExchangeListener::~OPY_ExchangeListener() { }

/* ------------------------------------------------------------------------- */
bool OPY_ExchangeListener::authorise(OTC_String const& theClientHost)
{
  return exchange_->authorise(theClientHost);
}

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Exchange::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Exchange",
  sizeof(OPY_Exchange),
  0,
  OPY_Exchange::sys_dealloc,
  0,
  OPY_Exchange::sys_getattr,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OPY_Exchange::gMethods_[] = {
  { "connect", OPY_Exchange::mfn_connect, 1 },
  { "listen", OPY_Exchange::mfn_listen, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Exchange::~OPY_Exchange()
{
  exchange_.registry()->unsubscribe(id());

  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_Exchange::OPY_Exchange(
 PyObject* theOuter,
 OTC_ExchangeType theType,
 char const* theGroup
)
  : outer_(theOuter), exchange_(theType,theGroup)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  exchange_.registry()->subscribe(id());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Exchange::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  OTC_ExchangeType theType = OTCLIB_EXCHANGE_CLIENT;
  char* theGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"Ois",
   &theOuter,&theType,&theGroup))
  {
    return 0;
  }

  OPY_Exchange* theExchange;
  theExchange = new OPY_Exchange(theOuter,theType,theGroup);
  OTCLIB_ASSERT_M(theExchange != 0);

  return (PyObject*)theExchange;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Exchange::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Exchange::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Exchange*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Exchange::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Exchange*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Exchange::mfn_connect(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theHostName = 0;
  int thePort = 0;
  double theRetryDelay = -1;

  if (!PyArg_ParseTuple(theArgs,"sid",
   &theHostName,&thePort,&theRetryDelay))
  {
    return 0;
  }

  if (theRetryDelay > 0)
    theRetryDelay = 1000*theRetryDelay;

  OPY_Exchange* theSelf;
  theSelf = (OPY_Exchange*)theInstance;

  OTC_InetClient* theClient;
  theClient = new OTC_InetClient("MESSAGE",
   theHostName,thePort,int(theRetryDelay));
  OTCLIB_ASSERT_M(theClient != 0);

  OTC_String theLocalAddress;
  theLocalAddress = theClient->localAddress();

  theSelf->exchange_.connect(theClient);

  return Py_BuildValue("s",theLocalAddress.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Exchange::mfn_listen(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int thePort = 0;

  if (!PyArg_ParseTuple(theArgs,"i",&thePort))
    return 0;

  OPY_Exchange* theSelf;
  theSelf = (OPY_Exchange*)theInstance;

  OTC_InetListener* theListener;
  theListener = new OPY_ExchangeListener("MESSAGE",thePort,theSelf);
  OTCLIB_ASSERT_M(theListener != 0);

  theSelf->exchange_.listen(theListener);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OPY_Exchange::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_ExchangeAnnouncement::typeId())
  {
    OTCEV_ExchangeAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ExchangeAnnouncement*)theEvent;

    OPY_Dispatcher::enterPython();

    OPY_ExchangeAnnouncement* theObject;
    theObject = new OPY_ExchangeAnnouncement(theAnnouncement);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleConnection","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    OPY_Dispatcher::leavePython();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
bool OPY_Exchange::authorise(OTC_String const& theClientHost)
{
  bool theBoolean;
  theBoolean = false;

  OPY_Dispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_authorise","s",theClientHost.string());

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
  {
    if (theResult != Py_None)
      theBoolean = true;

    Py_XDECREF(theResult);
  }

  OPY_Dispatcher::leavePython();

  return theBoolean;
}

/* ------------------------------------------------------------------------- */
