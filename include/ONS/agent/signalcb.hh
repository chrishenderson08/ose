#ifndef ONS_AGENT_SIGNALCB_HH
#define ONS_AGENT_SIGNALCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/signalcb.hh
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
#include <OTC/dispatch/signal.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_SignalCallback_mf1i_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(int) const;

			~ONS_SignalCallback_mf1i_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_SignalCallback_mf1i_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_SignalCallback_mf1i_c<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Signal*)theEvent)->signal());
}

template<class T1, class T2>
class ONS_SignalCallback_mf1i : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(int);

			~ONS_SignalCallback_mf1i()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_SignalCallback_mf1i(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_SignalCallback_mf1i<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Signal*)theEvent)->signal());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_SignalCallback_cb1i_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback1<int> const	T2::*M;

			~ONS_SignalCallback_cb1i_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_SignalCallback_cb1i_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_SignalCallback_cb1i_c<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Signal*)theEvent)->signal());
}

template<class T1, class T2>
class ONS_SignalCallback_cb1i : public ONS_EventCallback
{
  public:

    typedef ONS_Callback1<int>	T2::*M;

			~ONS_SignalCallback_cb1i()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_SignalCallback_cb1i(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_SignalCallback_cb1i<T1,T2>::execute(OTC_Event* theEvent, void*)
{
  (*agent_.*method_)(((OTCEV_Signal*)theEvent)->signal());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_SignalCallbackFactory
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
     void (T2::*theMethod)(int) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_SignalCallback_mf1i_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(int))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_SignalCallback_mf1i<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback1<int> const T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_SignalCallback_cb1i_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback1<int> T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_SignalCallback_cb1i<T1,T2>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_SIGNALCB_HH */
