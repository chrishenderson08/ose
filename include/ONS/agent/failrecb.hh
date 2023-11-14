#ifndef ONS_AGENT_FAILRECB_HH
#define ONS_AGENT_FAILRECB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/failrecb.hh
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
#include <OTC/dispatch/failure.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallback_mf0f_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTCEV_Failure*) const;

			~ONS_FailureCallback_mf0f_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf0f_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf0f_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure);
}

template<class T1, class T2>
class ONS_FailureCallback_mf0f : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTCEV_Failure*);

			~ONS_FailureCallback_mf0f()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf0f(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf0f<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallback_mf0_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)() const;

			~ONS_FailureCallback_mf0_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf0_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf0_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  (*agent_.*method_)();
}

template<class T1, class T2>
class ONS_FailureCallback_mf0 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)();

			~ONS_FailureCallback_mf0()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf0(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf0<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  (*agent_.*method_)();
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallback_mf1s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*) const;

			~ONS_FailureCallback_mf1s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf1s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf1s_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->conversationId());
}

template<class T1, class T2>
class ONS_FailureCallback_mf1s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*);

			~ONS_FailureCallback_mf1s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf1s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf1s<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->conversationId());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallback_mf1S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&) const;

			~ONS_FailureCallback_mf1S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf1S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf1S_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->conversationId());
}

template<class T1, class T2>
class ONS_FailureCallback_mf1S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&);

			~ONS_FailureCallback_mf1S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf1S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf1S<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->conversationId());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallback_mf4s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(int,char const*,char const*,
     char const*) const;

			~ONS_FailureCallback_mf4s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf4s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf4s_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->error(),theFailure->description(),
   theFailure->origin(),theFailure->details());
}

template<class T1, class T2>
class ONS_FailureCallback_mf4s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(int,char const*,char const*,char const*);

			~ONS_FailureCallback_mf4s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf4s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf4s<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->error(),theFailure->description(),
   theFailure->origin(),theFailure->details());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallback_mf4S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(int,OTC_String const&,
     OTC_String const&,OTC_String const&) const;

			~ONS_FailureCallback_mf4S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf4S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf4S_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->error(),theFailure->description(),
   theFailure->origin(),theFailure->details());
}

template<class T1, class T2>
class ONS_FailureCallback_mf4S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(int,OTC_String const&,
     OTC_String const&,OTC_String const&);

			~ONS_FailureCallback_mf4S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf4S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf4S<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->error(),theFailure->description(),
   theFailure->origin(),theFailure->details());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallback_mf5s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,int,char const*,
     char const*,char const*) const;

			~ONS_FailureCallback_mf5s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf5s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf5s_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->conversationId(),theFailure->error(),
   theFailure->description(),theFailure->origin(),theFailure->details());
}

template<class T1, class T2>
class ONS_FailureCallback_mf5s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,int,char const*,
     char const*,char const*);

			~ONS_FailureCallback_mf5s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf5s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf5s<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->conversationId(),theFailure->error(),
   theFailure->description(),theFailure->origin(),theFailure->details());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallback_mf5S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,int,
     OTC_String const&,OTC_String const&,OTC_String const&) const;

			~ONS_FailureCallback_mf5S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf5S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf5S_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->conversationId(),theFailure->error(),
   theFailure->description(),theFailure->origin(),theFailure->details());
}

template<class T1, class T2>
class ONS_FailureCallback_mf5S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,int,
     OTC_String const&,OTC_String const&,OTC_String const&);

			~ONS_FailureCallback_mf5S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_FailureCallback_mf5S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_FailureCallback_mf5S<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Failure* theFailure;
  theFailure = (OTCEV_Failure*)theEvent;

  (*agent_.*method_)(theFailure->conversationId(),theFailure->error(),
   theFailure->description(),theFailure->origin(),theFailure->details());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_FailureCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTCEV_Failure*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf0f_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTCEV_Failure*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf0f<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)() const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf0_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)())
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf0<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf1s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf1s<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf1S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf1S<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(int,char const*,
      char const*,char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf4s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(int,char const*,
      char const*,char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf4s<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(int,OTC_String const&,
      OTC_String const&,OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf4S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(int,OTC_String const&,
      OTC_String const&,OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf4S<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*,int,char const*,
      char const*,char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf5s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*,int,char const*,
      char const*,char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf5s<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&,int,OTC_String const&,
      OTC_String const&,OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf5S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&,int,OTC_String const&,
      OTC_String const&,OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_FailureCallback_mf5S<T1,T2>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_AGENT_FAILRECB_HH */
