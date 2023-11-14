#ifndef ONS_AGENT_RGSTRYCB_HH
#define ONS_AGENT_RGSTRYCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/rgstrycb.hh
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
#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/grpannc.hh>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_mf2oe_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,OTC_ServiceStatus) const;

			~ONS_RegistryCallback_mf2oe_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf2oe_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf2oe_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->status());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_mf2oe : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,OTC_ServiceStatus);

			~ONS_RegistryCallback_mf2oe()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf2oe(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf2oe<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->status());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_cb2oe_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback2<OTC_ServiceBinding*,OTC_ServiceStatus> const	T2::*M;

			~ONS_RegistryCallback_cb2oe_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb2oe_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb2oe_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->status());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_cb2oe : public ONS_EventCallback
{
  public:

    typedef ONS_Callback2<OTC_ServiceBinding*,OTC_ServiceStatus>	T2::*M;

			~ONS_RegistryCallback_cb2oe()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb2oe(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb2oe<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->status());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_mf3ose_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,char const*,
			 OTC_ServiceStatus) const;

			~ONS_RegistryCallback_mf3ose_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf3ose_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf3ose_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_mf3ose : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,char const*,
			 OTC_ServiceStatus);

			~ONS_RegistryCallback_mf3ose()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf3ose(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf3ose<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_cb3ose_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback3<OTC_ServiceBinding*,char const*,
    			 OTC_ServiceStatus> const	T2::*M;

			~ONS_RegistryCallback_cb3ose_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb3ose_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb3ose_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_cb3ose : public ONS_EventCallback
{
  public:

    typedef ONS_Callback3<OTC_ServiceBinding*,char const*,
    			 OTC_ServiceStatus>	T2::*M;

			~ONS_RegistryCallback_cb3ose()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb3ose(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb3ose<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_mf3oSe_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,OTC_String const&,
			 OTC_ServiceStatus) const;

			~ONS_RegistryCallback_mf3oSe_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf3oSe_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf3oSe_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_mf3oSe : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,OTC_String const&,
			 OTC_ServiceStatus);

			~ONS_RegistryCallback_mf3oSe()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf3oSe(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf3oSe<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_cb3oSe_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback3<OTC_ServiceBinding*,OTC_String const&,
    			 OTC_ServiceStatus> const	T2::*M;

			~ONS_RegistryCallback_cb3oSe_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb3oSe_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb3oSe_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_cb3oSe : public ONS_EventCallback
{
  public:

    typedef ONS_Callback3<OTC_ServiceBinding*,OTC_String const&,
    			 OTC_ServiceStatus>	T2::*M;

			~ONS_RegistryCallback_cb3oSe()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb3oSe(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb3oSe<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_mf4oses_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,char const*,
			 OTC_ServiceStatus,char const*) const;

			~ONS_RegistryCallback_mf4oses_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf4oses_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf4oses_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status(),
     tmpEvent->subscriptionName());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status(),tmpEvent->subscriptionName());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_mf4oses : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,char const*,
			 OTC_ServiceStatus,char const*);

			~ONS_RegistryCallback_mf4oses()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf4oses(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf4oses<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status(),
     tmpEvent->subscriptionName());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status(),tmpEvent->subscriptionName());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_cb4oses_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback4<OTC_ServiceBinding*,char const*,
    			 OTC_ServiceStatus,char const*> const	T2::*M;

			~ONS_RegistryCallback_cb4oses_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb4oses_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb4oses_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status(),
     tmpEvent->subscriptionName());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status(),tmpEvent->subscriptionName());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_cb4oses : public ONS_EventCallback
{
  public:

    typedef ONS_Callback4<OTC_ServiceBinding*,char const*,
    			 OTC_ServiceStatus,char const*>	T2::*M;

			~ONS_RegistryCallback_cb4oses()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb4oses(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb4oses<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status(),
     tmpEvent->subscriptionName());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status(),tmpEvent->subscriptionName());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_mf4oSeS_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,OTC_String const&,
			 OTC_ServiceStatus,OTC_String const&) const;

			~ONS_RegistryCallback_mf4oSeS_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf4oSeS_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf4oSeS_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status(),
     tmpEvent->subscriptionName());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status(),tmpEvent->subscriptionName());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_mf4oSeS : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTC_ServiceBinding*,OTC_String const&,
			 OTC_ServiceStatus,OTC_String const&);

			~ONS_RegistryCallback_mf4oSeS()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_mf4oSeS(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_mf4oSeS<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status(),
     tmpEvent->subscriptionName());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status(),tmpEvent->subscriptionName());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallback_cb4oSeS_c : public ONS_EventCallback
{
  public:

    typedef ONS_Callback4<OTC_ServiceBinding*,OTC_String const&,
    			 OTC_ServiceStatus,OTC_String const&> const T2::*M;

			~ONS_RegistryCallback_cb4oSeS_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb4oSeS_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb4oSeS_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status(),
     tmpEvent->subscriptionName());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status(),tmpEvent->subscriptionName());
  }
}

template<class T1, class T2>
class ONS_RegistryCallback_cb4oSeS : public ONS_EventCallback
{
  public:

    typedef ONS_Callback4<OTC_ServiceBinding*,OTC_String const&,
    			 OTC_ServiceStatus,OTC_String const&> T2::*M;

			~ONS_RegistryCallback_cb4oSeS()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RegistryCallback_cb4oSeS(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void*);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RegistryCallback_cb4oSeS<T1,T2>::execute(
 OTC_Event* theEvent,
 void*
)
{
  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_ServiceAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),"",tmpEvent->status(),
     tmpEvent->subscriptionName());
  }
  else
  {
    OTCEV_GroupAnnouncement* tmpEvent;
    tmpEvent = (OTCEV_GroupAnnouncement*)theEvent;
    (*agent_.*method_)(tmpEvent->serviceBinding(),tmpEvent->serviceGroup(),
     tmpEvent->status(),tmpEvent->subscriptionName());
  }
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RegistryCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,OTC_ServiceStatus) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf2oe_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,OTC_ServiceStatus))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf2oe<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback2<OTC_ServiceBinding*,OTC_ServiceStatus> const T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb2oe_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback2<OTC_ServiceBinding*,OTC_ServiceStatus> T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb2oe<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,char const*,
     OTC_ServiceStatus) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf3ose_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,char const*,
     OTC_ServiceStatus))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf3ose<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback3<OTC_ServiceBinding*,char const*,
     OTC_ServiceStatus> const T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb3ose_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback3<OTC_ServiceBinding*,char const*,
     OTC_ServiceStatus> T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb3ose<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,OTC_String const&,
     OTC_ServiceStatus) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf3oSe_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,OTC_String const&,
     OTC_ServiceStatus))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf3oSe<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback3<OTC_ServiceBinding*,OTC_String const&,
     OTC_ServiceStatus> const T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb3oSe_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback3<OTC_ServiceBinding*,OTC_String const&,
     OTC_ServiceStatus> T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb3oSe<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,char const*,
     OTC_ServiceStatus,char const*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf4oses_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,char const*,
     OTC_ServiceStatus,char const*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf4oses<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback4<OTC_ServiceBinding*,char const*,
     OTC_ServiceStatus,char const*> const T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb4oses_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback4<OTC_ServiceBinding*,char const*,
     OTC_ServiceStatus,char const*> T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb4oses<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,OTC_String const&,
     OTC_ServiceStatus,OTC_String const&) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf4oSeS_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTC_ServiceBinding*,OTC_String const&,
     OTC_ServiceStatus,OTC_String const&))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_mf4oSeS<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     ONS_Callback4<OTC_ServiceBinding*,OTC_String const&,
     OTC_ServiceStatus,OTC_String const&> const T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb4oSeS_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     ONS_Callback4<OTC_ServiceBinding*,OTC_String const&,
     OTC_ServiceStatus,OTC_String const&> T2::*theMethod) 
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RegistryCallback_cb4oSeS<T1,T2>(theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_RGSTRYCB_HH */
