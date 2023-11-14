#ifndef ONS_AGENT_ACTIONCB_HH
#define ONS_AGENT_ACTIONCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/actioncb.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/eventcb.hh>
#include <OTC/dispatch/action.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ActionCallback_mf1s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*) const;

			~ONS_ActionCallback_mf1s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ActionCallback_mf1s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ActionCallback_mf1s_c<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Action*)theEvent)->description());
}

template<class T1, class T2>
class ONS_ActionCallback_mf1s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*);

			~ONS_ActionCallback_mf1s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ActionCallback_mf1s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEven, void*t);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ActionCallback_mf1s<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Action*)theEvent)->description());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ActionCallback_cb1s_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback1<char const*> const	T2::*M;

			~ONS_ActionCallback_cb1s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ActionCallback_cb1s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ActionCallback_cb1s_c<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Action*)theEvent)->description());
}

template<class T1, class T2>
class ONS_ActionCallback_cb1s : public ONS_EventCallback
{
  public:

    typedef ONS_Callback1<char const*>	T2::*M;

			~ONS_ActionCallback_cb1s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ActionCallback_cb1s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEven, void*t);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ActionCallback_cb1s<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Action*)theEvent)->description());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ActionCallback_mf1S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&) const;

			~ONS_ActionCallback_mf1S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ActionCallback_mf1S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ActionCallback_mf1S_c<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Action*)theEvent)->description());
}

template<class T1, class T2>
class ONS_ActionCallback_mf1S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&);

			~ONS_ActionCallback_mf1S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ActionCallback_mf1S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEven, void*t);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ActionCallback_mf1S<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Action*)theEvent)->description());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ActionCallback_cb1S_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback1<OTC_String const&> const	T2::*M;

			~ONS_ActionCallback_cb1S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ActionCallback_cb1S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ActionCallback_cb1S_c<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Action*)theEvent)->description());
}

template<class T1, class T2>
class ONS_ActionCallback_cb1S : public ONS_EventCallback
{
  public:

    typedef ONS_Callback1<OTC_String const&>	T2::*M;

			~ONS_ActionCallback_cb1S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ActionCallback_cb1S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEven, void*t);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ActionCallback_cb1S<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Action*)theEvent)->description());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ActionCallbackFactory
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
       new ONS_EventCallback_cb0<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ActionCallback_mf1s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ActionCallback_mf1s<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback1<char const*> const T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ActionCallback_cb1s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback1<char const*> T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ActionCallback_cb1s<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ActionCallback_mf1S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ActionCallback_mf1S<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback1<OTC_String const&> const T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ActionCallback_cb1S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback1<OTC_String const&> T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ActionCallback_cb1S<T1,T2>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_ACTIONCB_HH */
