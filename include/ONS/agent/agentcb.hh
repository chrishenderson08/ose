#ifndef ONS_AGENT_AGENTCB_HH
#define ONS_AGENT_AGENTCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/agentcb.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/refcnt/resource.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_AgentCallback : public OTC_Resource
    // = TITLE
    //     Base class for callbacks to member functions of agent class.
    //
    // = DESCRIPTION
    //     The <ONS_AgentCallback> class is a special base class which
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

			ONS_AgentCallback() {}

  private:

			ONS_AgentCallback(ONS_AgentCallback const&);
				// Do not define an implementation for this.

    ONS_AgentCallback&	operator=(ONS_AgentCallback const&);
				// Do not define an implementation for this.
};

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<ONS_AgentCallback*>
{
  public:

    static ONS_AgentCallback*	add(ONS_AgentCallback* theObject)
				{ theObject->reference(); return theObject; }

    static void		remove(ONS_AgentCallback* theObject)
				{ theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

template<class T>
class ONS_AgentCallback_v0_c : public ONS_AgentCallback
{
  public:

    typedef void	(T::*M)() const;

			ONS_AgentCallback_v0_c(
			 T const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    void		execute(OTC_Event* theEvent, void*)
				{ (*agent_.*method_)(); }

  private:

    T const*		agent_;

    M			method_;
};

template<class T>
class ONS_AgentCallback_v0 : public ONS_AgentCallback
{
  public:

    typedef void	(T::*M)();

			ONS_AgentCallback_v0(
			 T* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    void		execute(OTC_Event* theEvent, void*)
				{ (*agent_.*method_)(); }

  private:

    T*			agent_;

    M			method_;
};

/* ------------------------------------------------------------------------- */

template<class T>
class ONS_AgentCallbackFactory
{
  public:

    static ONS_AgentCallback*	create(T const* theAgent,
     void (T::*theMethod)() const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_AgentCallback_v0_c<T>(theAgent,theMethod) : 0; }

    static ONS_AgentCallback*	create(T* theAgent,
     void (T::*theMethod)())
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_AgentCallback_v0<T>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_AGENTCB_HH */
