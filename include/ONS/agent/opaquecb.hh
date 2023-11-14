#ifndef ONS_AGENT_OPAQUECB_HH
#define ONS_AGENT_OPAQUECB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/opaquecb.hh
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
#include <OTC/dispatch/opaque.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_OpaqueCallback_mf3s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,OTC_String const&,
     char const*) const;

			~ONS_OpaqueCallback_mf3s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf3s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf3s_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->description(),
   theOpaque->content(),theOpaque->contentType());
}

template<class T1, class T2>
class ONS_OpaqueCallback_mf3s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,OTC_String const&,
     char const*);

			~ONS_OpaqueCallback_mf3s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf3s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf3s<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->description(),
   theOpaque->content(),theOpaque->contentType());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_OpaqueCallback_mf3S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,
     OTC_String const&,OTC_String const&) const;

			~ONS_OpaqueCallback_mf3S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf3S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf3S_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->description(),
   theOpaque->content(),theOpaque->contentType());
}

template<class T1, class T2>
class ONS_OpaqueCallback_mf3S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,
     OTC_String const&,OTC_String const&);

			~ONS_OpaqueCallback_mf3S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf3S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf3S<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->description(),
   theOpaque->content(),theOpaque->contentType());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_OpaqueCallback_mf4s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,OTC_String const&,
     char const*,int) const;

			~ONS_OpaqueCallback_mf4s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf4s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf4s_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->description(),
   theOpaque->content(),theOpaque->contentType(),
   theOpaque->sequenceNumber());
}

template<class T1, class T2>
class ONS_OpaqueCallback_mf4s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,OTC_String const&,
     char const*,int);

			~ONS_OpaqueCallback_mf4s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf4s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf4s<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->description(),
   theOpaque->content(),theOpaque->contentType(),
   theOpaque->sequenceNumber());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_OpaqueCallback_mf4S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,
     OTC_String const&,OTC_String const&,int) const;

			~ONS_OpaqueCallback_mf4S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf4S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf4S_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->description(),
   theOpaque->content(),theOpaque->contentType(),
   theOpaque->sequenceNumber());
}

template<class T1, class T2>
class ONS_OpaqueCallback_mf4S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,
     OTC_String const&,OTC_String const&,int);

			~ONS_OpaqueCallback_mf4S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf4S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf4S<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->description(),
   theOpaque->content(),theOpaque->contentType(),
   theOpaque->sequenceNumber());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_OpaqueCallback_mf6s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,char const*,
     char const*,OTC_String const&,char const*,int) const;

			~ONS_OpaqueCallback_mf6s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf6s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf6s_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->sender(),theOpaque->messageId(),
   theOpaque->description(),theOpaque->content(),
   theOpaque->contentType(),theOpaque->sequenceNumber());
}

template<class T1, class T2>
class ONS_OpaqueCallback_mf6s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,char const*,
     char const*,OTC_String const&,char const*,int);

			~ONS_OpaqueCallback_mf6s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf6s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf6s<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->sender(),theOpaque->messageId(),
   theOpaque->description(),theOpaque->content(),
   theOpaque->contentType(),theOpaque->sequenceNumber());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_OpaqueCallback_mf6S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,OTC_String const&,
     OTC_String const&,OTC_String const&,OTC_String const&,int) const;

			~ONS_OpaqueCallback_mf6S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf6S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf6S_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->sender(),theOpaque->messageId(),
   theOpaque->description(),theOpaque->content(),
   theOpaque->contentType(),theOpaque->sequenceNumber());
}

template<class T1, class T2>
class ONS_OpaqueCallback_mf6S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,OTC_String const&,
     OTC_String const&,OTC_String const&,OTC_String const&,int);

			~ONS_OpaqueCallback_mf6S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_OpaqueCallback_mf6S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_OpaqueCallback_mf6S<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Opaque* theOpaque;
  theOpaque = (OTCEV_Opaque*)theEvent;

  (*agent_.*method_)(theOpaque->sender(),theOpaque->messageId(),
   theOpaque->description(),theOpaque->content(),
   theOpaque->contentType(),theOpaque->sequenceNumber());
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_OpaqueCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*,OTC_String const&,
     char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf3s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*,OTC_String const&,
     char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf3s<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&,OTC_String const&,
     OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf3S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&,OTC_String const&,
     OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf3S<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*,OTC_String const&,
     char const*,int) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf4s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*,OTC_String const&,
     char const*,int))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf4s<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&,OTC_String const&,
     OTC_String const&,int) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf4S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&,OTC_String const&,
     OTC_String const&,int))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf4S<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*,char const*,
     char const*,OTC_String const&,char const*,int) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf6s_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*,char const*,
     char const*,OTC_String const&,char const*,int))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf6s<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&,OTC_String const&,
     OTC_String const&,OTC_String const&,OTC_String const&,int) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf6S_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&,OTC_String const&,
     OTC_String const&,OTC_String const&,OTC_String const&,int))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_OpaqueCallback_mf6S<T1,T2>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_AGENT_OPAQUECB_HH */
