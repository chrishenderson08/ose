#ifndef ONS_AGENT_RSPNSECB_HH
#define ONS_AGENT_RSPNSECB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/rspnsecb.hh
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
#include <ONS/agent/failrecb.hh>
#include <OTC/dispatch/response.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ResponseCallback_mf0r_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTCEV_Response*) const;

			~ONS_ResponseCallback_mf0r_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf0r_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ResponseCallback_mf0r_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  (*agent_.*method_)(theResponse);
}

template<class T1, class T2>
class ONS_ResponseCallback_mf0r : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTCEV_Response*);

			~ONS_ResponseCallback_mf0r()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf0r(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ResponseCallback_mf0r<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  (*agent_.*method_)(theResponse);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ResponseCallback_mf0_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)() const;

			~ONS_ResponseCallback_mf0_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf0_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ResponseCallback_mf0_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theResponse->content();

  if (thePayload.nodeType() != "scalar")
    throw ONS_ArgumentInvalid(0);
  if (thePayload.valueType() != "")
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)();
}

template<class T1, class T2>
class ONS_ResponseCallback_mf0 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)();

			~ONS_ResponseCallback_mf0()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf0(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ResponseCallback_mf0<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theResponse->content();

  if (thePayload.nodeType() != "scalar")
    throw ONS_ArgumentInvalid(0);
  if (thePayload.valueType() != "")
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)();
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_ResponseCallback_mf1_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(RT) const;

			~ONS_ResponseCallback_mf1_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf1_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ResponseCallback_mf1_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theResponse->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theObject.value);
}

template<class T1, class T2, class RT>
class ONS_ResponseCallback_mf1 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(RT);

			~ONS_ResponseCallback_mf1()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf1(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ResponseCallback_mf1<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theResponse->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theObject.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_ResponseCallback_mf2s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,RT) const;

			~ONS_ResponseCallback_mf2s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf2s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ResponseCallback_mf2s_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theResponse->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theResponse->conversationId(),theObject.value);
}

template<class T1, class T2, class RT>
class ONS_ResponseCallback_mf2s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,RT);

			~ONS_ResponseCallback_mf2s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf2s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ResponseCallback_mf2s<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theResponse->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theResponse->conversationId(),theObject.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_ResponseCallback_mf2S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,RT) const;

			~ONS_ResponseCallback_mf2S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf2S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ResponseCallback_mf2S_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theResponse->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theResponse->conversationId(),theObject.value);
}

template<class T1, class T2, class RT>
class ONS_ResponseCallback_mf2S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,RT);

			~ONS_ResponseCallback_mf2S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ResponseCallback_mf2S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ResponseCallback_mf2S<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Response* theResponse;
  theResponse = (OTCEV_Response*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theResponse->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theResponse->conversationId(),theObject.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ResponseCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTCEV_Response*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf0r_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTCEV_Response*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf0r<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)() const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf0_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)())
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf0<T1,T2>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(RT) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf1_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(RT))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf1<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*,RT) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf2s_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*,RT))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf2s<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&,RT) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf2S_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&,RT))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ResponseCallback_mf2S<T1,T2,RT>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_AGENT_RSPNSECB_HH */
