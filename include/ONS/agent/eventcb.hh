#ifndef ONS_AGENT_EVENTCB_HH
#define ONS_AGENT_EVENTCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/eventcb.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/callback.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/refcnt/resource.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_EventCallback : public OTC_Resource
    // = TITLE
    //     Base class for callbacks to member functions of agent class.
    //
    // = DESCRIPTION
    //     The <ONS_EventCallback> class is a special base class which
    //     is used for any callback objects encapsulating a pointer to
    //     a member function of an agent class.
    //
    // = NOTES
    //     None of the derived versions of this class are documented within
    //     the actual class because it is a waste of time. Derived classes
    //     also tend to have cryptic names and should not really be used
    //     directly as they are specific to the implementation of the classes
    //     which use them and could change. Normal rigourous rules with
    //     respect to always having destructors or copy constructors defined
    //     and always ensuring virtual functions are not defined inline are
    //     ignored as compilers will more often than not trip up on expanding
    //     the derived templates properly if such things were done.
    //
    // = SEE ALSO
    //     <ONS_Agent>, <ONS_Monitor>, <ONS_Service>
{
  public:

    // = EXECUTION

    virtual void	execute(OTC_Event* theEvent, void* theResult) = 0;
				// Must be overridden in a derived class to
				// use some attribute of <theEvent> as
				// parameters to the member function being
				// called. How exactly it is used is
				// dependent on the derived class as is
				// how <theResult> is used to pass back
				// any results. For correct operation any
				// implementation making use of the derived
				// classes has to be confident it is passing
				// correct values for these arguments as no
				// strict type checking is done.

  protected:

			ONS_EventCallback() {}

  private:

			ONS_EventCallback(ONS_EventCallback const&);
				// Do not define an implementation for this.

    ONS_EventCallback&	operator=(ONS_EventCallback const&);
				// Do not define an implementation for this.
};

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<ONS_EventCallback*>
{
  public:

    static ONS_EventCallback*	add(ONS_EventCallback* theObject)
				{ theObject->reference(); return theObject; }

    static void		remove(ONS_EventCallback* theObject)
				{ theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

template<class T>
class ONS_CallbackObject
    // = TITLE
    //     Template helper class for managing target object of a callback.
    //
    // = DESCRIPTION
    //     The <ONS_CallbackObject> is a template helper class for managing a
    //     target object as a resource so that it stays valid while ever a
    //     callback exists which targets it. The template would be
    //     specialised as appropriate for a particular type, normally to
    //     manipulate some form of reference counting mechanism or otherwise
    //     embodied within the class which controls its existance.
{
  public:

    static void		acquire(T const* theObject) {}
    				// Called on object supplied to a derived
				// agent callback, when the callback object
				// is being created.

    static void		release(T const* theObject) {}
    				// Called on object supplied to a derived
				// agent callback, when the callback object
				// is being destroyed.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class ONS_CallbackObject<OTC_Resource>
{
  public:

    static void		acquire(OTC_Resource const* theObject)
    				{ theObject->reference(); }

    static void		release(OTC_Resource const* theObject)
    				{ theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_EventCallback_mf0_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)() const;

			~ONS_EventCallback_mf0_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_EventCallback_mf0_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    void		execute(OTC_Event* theEvent, void*)
				{ (*agent_.*method_)(); }

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
class ONS_EventCallback_mf0 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)();

			~ONS_EventCallback_mf0()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_EventCallback_mf0(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    void		execute(OTC_Event* theEvent, void*)
				{ (*agent_.*method_)(); }

  private:

    T2*			agent_;

    M			method_;
};

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_EventCallback_cb0_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback0 const	T2::*M;

			~ONS_EventCallback_cb0_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_EventCallback_cb0_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    void		execute(OTC_Event* theEvent, void*)
				{ (*agent_.*method_)(); }

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
class ONS_EventCallback_cb0 : public ONS_EventCallback
{
  public:

    typedef ONS_Callback0 const	T2::*M;

			~ONS_EventCallback_cb0()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_EventCallback_cb0(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    void		execute(OTC_Event* theEvent, void*)
				{ (*agent_.*method_)(); }

  private:

    T2*			agent_;

    M			method_;
};

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_EventCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)() const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_EventCallback_mf0_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)())
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_EventCallback_mf0<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback0 const T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_EventCallback_cb0_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback0 T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_EventCallback_cb0_c<T1,T2>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_EVENTCB_HH */
