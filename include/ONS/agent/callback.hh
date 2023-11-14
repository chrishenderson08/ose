#ifndef ONS_AGENT_CALLBACK_HH
#define ONS_AGENT_CALLBACK_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/callback.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/exectecb.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/tmanymap.hh>
#include <OTC/collctn/deque.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/callback.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_Callback
    // = TITLE
    //     Base class for signal classes in signal/slot implementation.
    //
    // = DESCRIPTION
    //     The <ONS_Callback> class is the base class of any signal classes in
    //     this signal/slot implementation.
    //     
    // = SEE ALSO
    //     <ONS_CBObject>
{
    friend class	ONS_CBObject;

  public:

    virtual		~ONS_Callback();

    // = DISCONNECTION

    void		disconnect();
				// Disconnects all connected targets of the
				// signal source.

    void		disconnect(char const* theName);
				// Disconnects the signal target callback
				// identified by <theName>.

  protected:

  			ONS_Callback();

    // = CALLBACK REGISTRATION

    OTC_String const&	connect(ONS_ExecuteCallback* theCallback);
				// Attaches <theCallback> to this signal
				// source. The name of the callback is
				// returned.

    OTC_Iterator<ONS_ExecuteCallback*>	callbacks() const;
    				// Returns list of the registered callbacks
				// for this signal source. This is a copy
				// of the stored list so it safe to use in
				// multithreaded context.

    // = CALLBACK TARGET
    //     Following method is used as a way of ensuring that the pointer
    //     to member function supplied as a callback is a member function of
    //     a class derived from <ONS_CBObject>. The main check is done at
    //     compile time so your code will not compile if you have done the
    //     wrong thing.

    template<class BASE, class DERIVED>
    DERIVED const*	derivedObject(BASE const* p)
				{
				  DERIVED const* o = 0;
				  if (0) { BASE* b = (DERIVED*)0; b++; }
				  try { o = dynamic_cast<DERIVED const*>(p); }
				  catch (...) { /* dealt with later */ }
				  return o;
				}
				// Returns argument cast to a derived class
				// type. The types of each are determined by
				// template parameterisation.

    template<class BASE, class DERIVED>
    DERIVED*		derivedObject(BASE* p)
				{
				  DERIVED* o = 0;
				  if (0) { BASE* b = (DERIVED*)0; b++; }
				  try { o = dynamic_cast<DERIVED*>(p); }
				  catch (...) { /* dealt with later */ }
				  return o;
				}
				// Returns argument cast to a derived class
				// type. The types of each are determined by
				// template parameterisation.

  private:

  			ONS_Callback(ONS_Callback const&);
				// Do not define an implementation for this.

    ONS_Callback&	operator=(ONS_Callback const&);
				// Do not define an implementation for this.

    // Note that this thread mutex is also used by <ONS_CBObject> to
    // avoid deadlock issues. Be mindful of this when making changes.

    static OTC_NRMutex	mutex_;
    				// Thread lock protecting member data.

    OTC_HMap<OTC_String,ONS_ExecuteCallback*>	callbacks_;
    				// List of registered slot callbacks for
				// this signal source indexed by their name.

    OTC_TManyMap<ONS_CBObject*,OTC_String>	targets_;
    				// Table of what targets have which callbacks
				// targetting it.
};

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_Callback0 : public ONS_Callback
{
  public:

  			ONS_Callback0() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)() const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()() const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_0*)
				     theItems.item())->execute();
				    theItems.next();
				  }
				}

    void		execute() const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_0*)
				     theItems.item())->execute();
				    theItems.next();
				  }
				}
};
/* ------------------------------------------------------------------------- */

template<class A1>
class ONS_Callback1 : public ONS_Callback
{
  public:

  			ONS_Callback1() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_1<A1>*)
				     theItems.item())->execute(theArg1);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_1<A1>*)
				     theItems.item())->execute(theArg1);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

template<class A1, class A2>
class ONS_Callback2 : public ONS_Callback
{
  public:

  			ONS_Callback2() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1,A2) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1,A2),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1, A2 theArg2) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_2<A1,A2>*)
				     theItems.item())->execute(theArg1,
				     theArg2);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1, A2 theArg2) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_2<A1,A2>*)
				     theItems.item())->execute(theArg1,
				     theArg2);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3>
class ONS_Callback3 : public ONS_Callback
{
  public:

  			ONS_Callback3() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1,A2,A3) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1,A2,A3),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1, A2 theArg2, A3 theArg3) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_3<A1,A2,A3>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1, A2 theArg2, A3 theArg3) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_3<A1,A2,A3>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4>
class ONS_Callback4 : public ONS_Callback
{
  public:

  			ONS_Callback4() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_4<A1,A2,A3,A4>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_4<A1,A2,A3,A4>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5>
class ONS_Callback5 : public ONS_Callback
{
  public:

  			ONS_Callback5() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_5<A1,A2,A3,A4,A5>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_5<A1,A2,A3,A4,A5>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5, class A6>
class ONS_Callback6 : public ONS_Callback
{
  public:

  			ONS_Callback6() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5, A6 theArg6) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_6<A1,A2,A3,A4,A5,A6>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5,theArg6);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5, A6 theArg6) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_6<A1,A2,A3,A4,A5,A6>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5,theArg6);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7>
class ONS_Callback7 : public ONS_Callback
{
  public:

  			ONS_Callback7() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5, A6 theArg6, A7 theArg7) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_7<A1,A2,A3,A4,A5,
				     A6,A7>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5,theArg6,
				     theArg7);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5, A6 theArg6, A7 theArg7) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_7<A1,A2,A3,A4,A5,
				     A6,A7>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5,theArg6,
				     theArg7);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7,
 class A8>
class ONS_Callback8 : public ONS_Callback
{
  public:

  			ONS_Callback8() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5, A6 theArg6, A7 theArg7,
			 A8 theArg8) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_8<A1,A2,A3,A4,A5,
				     A6,A7,A8>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5,theArg6,
				     theArg7,theArg8);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5, A6 theArg6, A7 theArg7,
			 A8 theArg8) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_8<A1,A2,A3,A4,A5,
				     A6,A7,A8>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5,theArg6,
				     theArg7,theArg8);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7,
 class A8, class A9>
class ONS_Callback9 : public ONS_Callback
{
  public:

  			ONS_Callback9() {}

    template<class T>
    OTC_String const&	connect(
			 T const* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,
			  A9) const,
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    template<class T>
    OTC_String const&	connect(
			 T* theTarget,
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9),
			 char const* theName=0
			)
				{
				  ONS_ExecuteCallback* theCallback =
				   ONS_ExecuteCallbackFactory<T>::create(
				   derivedObject<ONS_CBObject,T>(theTarget),
				   theMethod,theName);
				  return ONS_Callback::connect(theCallback);
				}

    void		operator()(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5, A6 theArg6, A7 theArg7,
			 A8 theArg8, A9 theArg9) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_9<A1,A2,A3,A4,A5,
				     A6,A7,A8,A9>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5,theArg6,
				     theArg7,theArg8,theArg9);
				    theItems.next();
				  }
				}

    void		execute(A1 theArg1, A2 theArg2, A3 theArg3,
    			 A4 theArg4, A5 theArg5, A6 theArg6, A7 theArg7,
			 A8 theArg8, A9 theArg9) const
    				{
				  OTC_Iterator<ONS_ExecuteCallback*> theItems;
				  theItems = callbacks();
				  while (theItems.isValid())
				  {
				    ((ONS_ExecuteCallback_9<A1,A2,A3,A4,A5,
				     A6,A7,A8,A9>*)
				     theItems.item())->execute(theArg1,
				     theArg2,theArg3,theArg4,theArg5,theArg6,
				     theArg7,theArg8,theArg9);
				    theItems.next();
				  }
				}
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_CALLBACK_HH */
