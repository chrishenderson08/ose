#ifndef ONS_AGENT_SHTDWNCB_HH
#define ONS_AGENT_SHTDWNCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/shtdwncb.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Mopyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/evcllbck.hh>
#include <OTC/dispatch/shutdown.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ShutdownCallback_mf1e_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ShutdownNotification) const;

			~ONS_ShutdownCallback_mf1e_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_mf1e_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_mf1e_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  (*agent_.*method_)(((OTCEV_Shutdown*)theEvent)->category());
}

template<class T1, class T2>
class ONS_ShutdownCallback_mf1e : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ShutdownNotification);

			~ONS_ShutdownCallback_mf1e()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_mf1e(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_mf1e<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  (*agent_.*method_)(((OTCEV_Shutdown*)theEvent)->category());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ShutdownCallback_cb1e_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback1<OTC_ShutdownNotification> const	T2::*M;

			~ONS_ShutdownCallback_cb1e_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_cb1e_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_cb1e_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  (*agent_.*method_)(((OTCEV_Shutdown*)theEvent)->category());
}

template<class T1, class T2>
class ONS_ShutdownCallback_cb1e : public ONS_EventCallback
{
  public:

    typedef ONS_Callback1<OTC_ShutdownNotification>	T2::*M;

			~ONS_ShutdownCallback_cb1e()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_cb1e(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_cb1e<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  (*agent_.*method_)(((OTCEV_Shutdown*)theEvent)->category());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ShutdownCallback_mf2es_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ShutdownNotification,
    			 char const*) const;

			~ONS_ShutdownCallback_mf2es_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_mf2es_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_mf2es_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Shutdown* theShutdown;
  theShutdown = (OTCEV_Shutdown*)theEvent;

  (*agent_.*method_)(theShutdown->category(),theShutdown->description());
}

template<class T1, class T2>
class ONS_ShutdownCallback_mf2es : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ShutdownNotification,char const*);

			~ONS_ShutdownCallback_mf2es()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_mf2es(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_mf2es<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Shutdown* theShutdown;
  theShutdown = (OTCEV_Shutdown*)theEvent;

  (*agent_.*method_)(theShutdown->category(),theShutdown->description());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ShutdownCallback_cb2es_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback2<OTC_ShutdownNotification,
     char const*> const	T2::*M;

			~ONS_ShutdownCallback_cb2es_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_cb2es_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_cb2es_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Shutdown* theShutdown;
  theShutdown = (OTCEV_Shutdown*)theEvent;

  (*agent_.*method_)(theShutdown->category(),theShutdown->description());
}

template<class T1, class T2>
class ONS_ShutdownCallback_cb2es : public ONS_EventCallback
{
  public:

    typedef ONS_Callback2<OTC_ShutdownNotification,
     char const*>	T2::*M;

			~ONS_ShutdownCallback_cb2es()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_cb2es(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_cb2es<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Shutdown* theShutdown;
  theShutdown = (OTCEV_Shutdown*)theEvent;

  (*agent_.*method_)(theShutdown->category(),theShutdown->description());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ShutdownCallback_mf2eS_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ShutdownNotification,
    			 OTC_String const&) const;

			~ONS_ShutdownCallback_mf2eS_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_mf2eS_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_mf2eS_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Shutdown* theShutdown;
  theShutdown = (OTCEV_Shutdown*)theEvent;

  (*agent_.*method_)(theShutdown->category(),theShutdown->description());
}

template<class T1, class T2>
class ONS_ShutdownCallback_mf2eS : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ShutdownNotification,
    			 OTC_String const&);

			~ONS_ShutdownCallback_mf2eS()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_mf2eS(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_mf2eS<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Shutdown* theShutdown;
  theShutdown = (OTCEV_Shutdown*)theEvent;

  (*agent_.*method_)(theShutdown->category(),theShutdown->description());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ShutdownCallback_cb2eS_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback2<OTC_ShutdownNotification,
     OTC_String const&> const	T2::*M;

			~ONS_ShutdownCallback_cb2eS_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_cb2eS_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_cb2eS_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Shutdown* theShutdown;
  theShutdown = (OTCEV_Shutdown*)theEvent;

  (*agent_.*method_)(theShutdown->category(),theShutdown->description());
}

template<class T1, class T2>
class ONS_ShutdownCallback_cb2eS : public ONS_EventCallback
{
  public:

    typedef ONS_Callback2<OTC_ShutdownNotification,
     OTC_String const&>	T2::*M;

			~ONS_ShutdownCallback_cb2eS()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ShutdownCallback_cb2eS(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ShutdownCallback_cb2eS<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Shutdown* theShutdown;
  theShutdown = (OTCEV_Shutdown*)theEvent;

  (*agent_.*method_)(theShutdown->category(),theShutdown->description());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ShutdownCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_ShutdownNotification) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_mf1e_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_ShutdownNotification))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_mf1e<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback1<OTC_ShutdownNotification> const T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_cb1e_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback1<OTC_ShutdownNotification> T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_cb1e<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_ShutdownNotification,char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_mf2es_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_ShutdownNotification,char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_mf2es<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback2<OTC_ShutdownNotification,
     char const*> const T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_cb2es_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback2<OTC_ShutdownNotification,
     char const*> T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_cb2es<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_ShutdownNotification,OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_mf2eS_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_ShutdownNotification,OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_mf2eS<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback2<OTC_ShutdownNotification,
     OTC_String const&> const T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_cb2eS_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback2<OTC_ShutdownNotification,
     OTC_String const&> T2::*theMethod)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ShutdownCallback_cb2eS<T1,T2>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_SHTDWNCB_HH */
