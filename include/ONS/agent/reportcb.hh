#ifndef ONS_AGENT_REPORTCB_HH
#define ONS_AGENT_REPORTCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/reportcb.hh
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
#include <OTC/dispatch/report.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ReportCallback_mf0r_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTCEV_Report*) const;

			~ONS_ReportCallback_mf0r_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf0r_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ReportCallback_mf0r_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  (*agent_.*method_)((OTCEV_Report*)theEvent);
}

template<class T1, class T2>
class ONS_ReportCallback_mf0r : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTCEV_Report*);

			~ONS_ReportCallback_mf0r()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf0r(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_ReportCallback_mf0r<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  (*agent_.*method_)((OTCEV_Report*)theEvent);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf1_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(RT) const;

			~ONS_ReportCallback_mf1_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf1_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf1_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theObject.value);
}

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf1 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(RT);

			~ONS_ReportCallback_mf1()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf1(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf1<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theObject.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf2s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,RT) const;

			~ONS_ReportCallback_mf2s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf2s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf2s_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theReport->subjectName(),theObject.value);
}

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf2s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,RT);

			~ONS_ReportCallback_mf2s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf2s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf2s<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theReport->subjectName(),theObject.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf2S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,RT) const;

			~ONS_ReportCallback_mf2S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf2S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf2S_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theReport->subjectName(),theObject.value);
}

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf2S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,RT);

			~ONS_ReportCallback_mf2S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf2S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf2S<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theReport->subjectName(),theObject.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf3s_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,char const*,RT) const;

			~ONS_ReportCallback_mf3s_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf3s_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf3s_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theReport->publisher()->serviceName(),
   theReport->subjectName(),theObject.value);
}

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf3s : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(char const*,char const*,RT);

			~ONS_ReportCallback_mf3s()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf3s(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf3s<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theReport->publisher()->serviceName(),
   theReport->subjectName(),theObject.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf3S_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,OTC_String const&,RT) const;

			~ONS_ReportCallback_mf3S_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf3S_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf3S_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theReport->publisher()->serviceName(),
   theReport->subjectName(),theObject.value);
}

template<class T1, class T2, class RT>
class ONS_ReportCallback_mf3S : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_String const&,OTC_String const&,RT);

			~ONS_ReportCallback_mf3S()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_ReportCallback_mf3S(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_ReportCallback_mf3S<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void*
)
{
  OTCEV_Report* theReport;
  theReport = (OTCEV_Report*)theEvent;

  OTC_ROPayload thePayload;
  thePayload = theReport->content();

  bool theStatus = true;

  ONS_ArgumentWrapper<RT> theObject;
  (thePayload,theStatus) >>= theObject.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(0);

  (*agent_.*method_)(theReport->publisher()->serviceName(),
   theReport->subjectName(),theObject.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_ReportCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTCEV_Report*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf0r_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTCEV_Report*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf0r<T1,T2>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(RT) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf1_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(RT))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf1<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*,RT) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf2s_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*,RT))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf2s<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&,RT) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf2S_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&,RT))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf2S<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(char const*,char const*,RT) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf3s_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(char const*,char const*,RT))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf3s<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_String const&,
     OTC_String const&,RT) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf3S_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_String const&,
     OTC_String const&,RT))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_ReportCallback_mf3S<T1,T2,RT>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_REPORTCB_HH */
