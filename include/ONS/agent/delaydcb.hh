#ifndef ONS_AGENT_DELAYDCB_HH
#define ONS_AGENT_DELAYDCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/delaydcb.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/rquestcb.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_DelayedCallback_mf0_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)() const;

			ONS_DelayedCallback_mf0_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_DelayedCallback_mf0_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)();
}

template<class T1, class T2>
class ONS_DelayedCallback_mf0 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)();

			ONS_DelayedCallback_mf0(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_DelayedCallback_mf0<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)();
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_DelayedCallback_o0_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)() const;

			ONS_DelayedCallback_o0_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_DelayedCallback_o0_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= (*agent_.*method_)();
}

template<class T1, class T2, class RT>
class ONS_DelayedCallback_o0 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)();

			ONS_DelayedCallback_o0(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_DelayedCallback_o0<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= (*agent_.*method_)();
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_DelayedCallback_mf1s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*) const;

			ONS_DelayedCallback_mf1s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_DelayedCallback_mf1s_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theRequest->conversationId());
}

template<class T1, class T2>
class ONS_DelayedCallback_mf1s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*);

			ONS_DelayedCallback_mf1s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_DelayedCallback_mf1s<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theRequest->conversationId());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_DelayedCallback_mf1S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&) const;

			ONS_DelayedCallback_mf1S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_DelayedCallback_mf1S_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theRequest->conversationId());
}

template<class T1, class T2>
class ONS_DelayedCallback_mf1S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&);

			ONS_DelayedCallback_mf1S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_DelayedCallback_mf1S<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theRequest->conversationId());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_DelayedCallback_o1s_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(char const*) const;

			ONS_DelayedCallback_o1s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_DelayedCallback_o1s_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= (*agent_.*method_)(theRequest->conversationId());
}

template<class T1, class T2, class RT>
class ONS_DelayedCallback_o1s : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(char const*);

			ONS_DelayedCallback_o1s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_DelayedCallback_o1s<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= (*agent_.*method_)(theRequest->conversationId());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_DelayedCallback_o1S_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(OTC_String const&) const;

			ONS_DelayedCallback_o1S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_DelayedCallback_o1S_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= (*agent_.*method_)(theRequest->conversationId());
}

template<class T1, class T2, class RT>
class ONS_DelayedCallback_o1S : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(OTC_String const&);

			ONS_DelayedCallback_o1S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod) {}

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_DelayedCallback_o1S<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= (*agent_.*method_)(theRequest->conversationId());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_DelayedCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)() const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_mf0_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)())
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_mf0<T1,T2>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)() const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_o0_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)())
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_o0<T1,T2,RT>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_mf1s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_mf1s<T1,T2>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_o1s_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_o1s<T1,T2,RT>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_mf1S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_mf1S<T1,T2>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_o1S_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_DelayedCallback_o1S<T1,T2,RT>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_DELAYDCB_HH */
