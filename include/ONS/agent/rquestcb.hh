#ifndef ONS_AGENT_RQUESTCB_HH
#define ONS_AGENT_RQUESTCB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/rquestcb.hh
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
#include <ONS/agent/svexcept.hh>
#include <OTC/dispatch/request.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_PayloadInvalid
    // = TITLE
    //     Exception class to be thrown request payload structure is invalid.
{
  // Nothing to see.
};

class OSE_EXPORT ONS_ArgumentMismatch
    // = TITLE
    //     Exception class to be thrown when wrong number of arguments.
{
  public:

    // = INITIALISATION

			ONS_ArgumentMismatch(u_int theCount)
			  : count_(theCount) {}
			  	// Actual number of arguments expected was
				// <theCount>.

    // = DETAILS

    u_int		count() const
				{ return count_; }
				// Returns the actual number of arguments
				// excepted.

  private:

    u_int		count_;
    				// Number of arguments expected.
};

class OSE_EXPORT ONS_ArgumentInvalid
    // = TITLE
    //     Exception class to be thrown when argument could not be decoded.
{
  public:

    // = INITIALISATION

			ONS_ArgumentInvalid(u_int theCount)
			  : count_(theCount) {}
			  	// Argument which could not be decoded was
				// as position <theCount>.

    // = DETAILS

    u_int		count() const
				{ return count_; }
				// Returns the position of the argument which
				// could not be decoded.

  private:

    u_int		count_;
				// The position of the argument which could
				// not be decoded.
};

/* ------------------------------------------------------------------------- */

template<class T>
class ONS_ArgumentWrapper
    // = TITLE
    //     Place holder variable into which argument is decoded.
    //
    // = DESCRIPTION
    //     The <ONS_ArgumentWrapper> class defines the variable into which an
    //     argument is decoded. Any type over which this template class
    //     needs to be parameterised must have a default initialiser. That
    //     is, for a C++ class it must have default constructor. Note that
    //     for a built in type, the variable will not actually be initialised
    //     to anything.
    //
    // = NOTES
    //     The template class may be specialised for specific types. For
    //     example, specialisations are already provided for "T const&"
    //     where a variable of type <T> is still created. Also, for the
    //     type "char const*" a variable of type <OTC_String> is created.
{
  public:

    T			value;
    				// Actual variable into which argument will
				// be decoded.
};

template<class T>
class ONS_ArgumentWrapper<T const&> { public: T value; };

OSE_TEMPLATE_SPECIALISATION
class ONS_ArgumentWrapper<char const*> { public: OTC_String value; };

#if 0
template<class T>
void operator>>=(OTC_PLReader theReader, ONS_ArgumentWrapper<T>& theWrapper)
{
  if (theReader.errorOccurred())
    return;

  OTC_ROPayload theObject;
  theObject = theReader.node();

  bool theStatus;
  theStatus = true;

  (theObject,theStatus) >>= theWrapper.value;

  if (theStatus == false)
    theReader.flagError();
}
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RequestCallback_mf0r_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTCEV_Request*) const;

			~ONS_RequestCallback_mf0r_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf0r_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
			    	{ ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RequestCallback_mf0r_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  (*agent_.*method_)(theRequest);

  throw ONS_DelayedResponse();
}

template<class T1, class T2>
class ONS_RequestCallback_mf0r : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(OTCEV_Request*);

			~ONS_RequestCallback_mf0r()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf0r(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RequestCallback_mf0r<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  (*agent_.*method_)(theRequest);

  throw ONS_DelayedResponse();
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RequestCallback_mf0_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)() const;

			~ONS_RequestCallback_mf0_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf0_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RequestCallback_mf0_c<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 0)
    throw ONS_ArgumentMismatch(0);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)();
}

template<class T1, class T2>
class ONS_RequestCallback_mf0 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)();

			~ONS_RequestCallback_mf0()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf0(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2>
inline void ONS_RequestCallback_mf0<T1,T2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 0)
    throw ONS_ArgumentMismatch(0);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)();
}

template<class T1, class T2, class RT>
class ONS_RequestCallback_o0_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)() const;

			~ONS_RequestCallback_o0_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o0_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_RequestCallback_o0_c<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 0)
    throw ONS_ArgumentMismatch(0);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= (*agent_.*method_)();
}

template<class T1, class T2, class RT>
class ONS_RequestCallback_o0 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)();

			~ONS_RequestCallback_o0()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o0(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT>
inline void ONS_RequestCallback_o0<T1,T2,RT>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 0)
    throw ONS_ArgumentMismatch(0);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  (*thePayload) <<= (*agent_.*method_)();
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1>
class ONS_RequestCallback_mf1_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1) const;

			~ONS_RequestCallback_mf1_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf1_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1>
inline void ONS_RequestCallback_mf1_c<T1,T2,A1>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 1)
    throw ONS_ArgumentMismatch(1);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value);
}

template<class T1, class T2, class A1>
class ONS_RequestCallback_mf1 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1);

			~ONS_RequestCallback_mf1()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf1(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1>
inline void ONS_RequestCallback_mf1<T1,T2,A1>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 1)
    throw ONS_ArgumentMismatch(1);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value);
}

template<class T1, class T2, class RT, class A1>
class ONS_RequestCallback_o1_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1) const;

			~ONS_RequestCallback_o1_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o1_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1>
inline void ONS_RequestCallback_o1_c<T1,T2,RT,A1>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 1)
    throw ONS_ArgumentMismatch(1);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value);
}

template<class T1, class T2, class RT, class A1>
class ONS_RequestCallback_o1 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1);

			~ONS_RequestCallback_o1()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o1(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1>
inline void ONS_RequestCallback_o1<T1,T2,RT,A1>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 1)
    throw ONS_ArgumentMismatch(1);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1, class A2>
class ONS_RequestCallback_mf2_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2) const;

			~ONS_RequestCallback_mf2_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf2_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2>
inline void ONS_RequestCallback_mf2_c<T1,T2,A1,A2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 2)
    throw ONS_ArgumentMismatch(2);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value);
}

template<class T1, class T2, class A1, class A2>
class ONS_RequestCallback_mf2 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2);

			~ONS_RequestCallback_mf2()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf2(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2>
inline void ONS_RequestCallback_mf2<T1,T2,A1,A2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 2)
    throw ONS_ArgumentMismatch(2);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value);
}

template<class T1, class T2, class RT, class A1, class A2>
class ONS_RequestCallback_o2_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2) const;

			~ONS_RequestCallback_o2_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o2_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2>
inline void ONS_RequestCallback_o2_c<T1,T2,RT,A1,A2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 2)
    throw ONS_ArgumentMismatch(2);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value);
}

template<class T1, class T2, class RT, class A1, class A2>
class ONS_RequestCallback_o2 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2);

			~ONS_RequestCallback_o2()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o2(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2>
inline void ONS_RequestCallback_o2<T1,T2,RT,A1,A2>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 2)
    throw ONS_ArgumentMismatch(2);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1, class A2, class A3>
class ONS_RequestCallback_mf3_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3) const;

			~ONS_RequestCallback_mf3_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf3_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3>
inline void ONS_RequestCallback_mf3_c<T1,T2,A1,A2,A3>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 3)
    throw ONS_ArgumentMismatch(3);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value);
}

template<class T1, class T2, class A1, class A2, class A3>
class ONS_RequestCallback_mf3 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3);

			~ONS_RequestCallback_mf3()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf3(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3>
inline void ONS_RequestCallback_mf3<T1,T2,A1,A2,A3>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 3)
    throw ONS_ArgumentMismatch(3);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3>
class ONS_RequestCallback_o3_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3) const;

			~ONS_RequestCallback_o3_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o3_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3>
inline void ONS_RequestCallback_o3_c<T1,T2,RT,A1,A2,A3>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 3)
    throw ONS_ArgumentMismatch(3);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3>
class ONS_RequestCallback_o3 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3);

			~ONS_RequestCallback_o3()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o3(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3>
inline void ONS_RequestCallback_o3<T1,T2,RT,A1,A2,A3>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 3)
    throw ONS_ArgumentMismatch(3);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1, class A2, class A3, class A4>
class ONS_RequestCallback_mf4_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4) const;

			~ONS_RequestCallback_mf4_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf4_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4>
inline void ONS_RequestCallback_mf4_c<T1,T2,A1,A2,A3,A4>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 4)
    throw ONS_ArgumentMismatch(4);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value);
}

template<class T1, class T2, class A1, class A2, class A3, class A4>
class ONS_RequestCallback_mf4 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4);

			~ONS_RequestCallback_mf4()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf4(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4>
inline void ONS_RequestCallback_mf4<T1,T2,A1,A2,A3,A4>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 4)
    throw ONS_ArgumentMismatch(4);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4>
class ONS_RequestCallback_o4_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4) const;

			~ONS_RequestCallback_o4_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o4_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4>
inline void ONS_RequestCallback_o4_c<T1,T2,RT,A1,A2,A3,A4>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 4)
    throw ONS_ArgumentMismatch(4);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4>
class ONS_RequestCallback_o4 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4);

			~ONS_RequestCallback_o4()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o4(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4>
inline void ONS_RequestCallback_o4<T1,T2,RT,A1,A2,A3,A4>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 4)
    throw ONS_ArgumentMismatch(4);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5>
class ONS_RequestCallback_mf5_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5) const;

			~ONS_RequestCallback_mf5_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf5_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5>
inline void ONS_RequestCallback_mf5_c<T1,T2,A1,A2,A3,A4,A5>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 5)
    throw ONS_ArgumentMismatch(5);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value);
}

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5>
class ONS_RequestCallback_mf5 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5);

			~ONS_RequestCallback_mf5()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf5(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5>
inline void ONS_RequestCallback_mf5<T1,T2,A1,A2,A3,A4,A5>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 5)
    throw ONS_ArgumentMismatch(5);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5>
class ONS_RequestCallback_o5_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5) const;

			~ONS_RequestCallback_o5_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o5_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5>
inline void ONS_RequestCallback_o5_c<T1,T2,RT,A1,A2,A3,A4,A5>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 5)
    throw ONS_ArgumentMismatch(5);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5>
class ONS_RequestCallback_o5 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5);

			~ONS_RequestCallback_o5()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o5(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5>
inline void ONS_RequestCallback_o5<T1,T2,RT,A1,A2,A3,A4,A5>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 5)
    throw ONS_ArgumentMismatch(5);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6>
class ONS_RequestCallback_mf6_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5,A6) const;

			~ONS_RequestCallback_mf6_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf6_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6>
inline void ONS_RequestCallback_mf6_c<T1,T2,A1,A2,A3,A4,A5,A6>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 6)
    throw ONS_ArgumentMismatch(6);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value,theArg6.value);
}

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6>
class ONS_RequestCallback_mf6 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5,A6);

			~ONS_RequestCallback_mf6()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf6(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6>
inline void ONS_RequestCallback_mf6<T1,T2,A1,A2,A3,A4,A5,A6>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 6)
    throw ONS_ArgumentMismatch(6);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value,theArg6.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6>
class ONS_RequestCallback_o6_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5,A6) const;

			~ONS_RequestCallback_o6_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o6_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6>
inline void ONS_RequestCallback_o6_c<T1,T2,RT,A1,A2,A3,A4,A5,A6>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 6)
    throw ONS_ArgumentMismatch(6);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value,theArg6.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6>
class ONS_RequestCallback_o6 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5,A6);

			~ONS_RequestCallback_o6()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o6(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6>
inline void ONS_RequestCallback_o6<T1,T2,RT,A1,A2,A3,A4,A5,A6>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 6)
    throw ONS_ArgumentMismatch(6);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value,theArg6.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7>
class ONS_RequestCallback_mf7_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5,A6,A7) const;

			~ONS_RequestCallback_mf7_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf7_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7>
inline void ONS_RequestCallback_mf7_c<T1,T2,A1,A2,A3,A4,A5,A6,A7>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 7)
    throw ONS_ArgumentMismatch(7);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value,theArg6.value,theArg7.value);
}

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7>
class ONS_RequestCallback_mf7 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5,A6,A7);

			~ONS_RequestCallback_mf7()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf7(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7>
inline void ONS_RequestCallback_mf7<T1,T2,A1,A2,A3,A4,A5,A6,A7>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 7)
    throw ONS_ArgumentMismatch(7);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value,theArg6.value,theArg7.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7>
class ONS_RequestCallback_o7_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5,A6,A7) const;

			~ONS_RequestCallback_o7_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o7_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7>
inline void ONS_RequestCallback_o7_c<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 7)
    throw ONS_ArgumentMismatch(7);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value,theArg6.value,theArg7.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7>
class ONS_RequestCallback_o7 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5,A6,A7);

			~ONS_RequestCallback_o7()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o7(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7>
inline void ONS_RequestCallback_o7<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 7)
    throw ONS_ArgumentMismatch(7);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value,theArg6.value,theArg7.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7, class A8>
class ONS_RequestCallback_mf8_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5,A6,A7,A8) const;

			~ONS_RequestCallback_mf8_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf8_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7, class A8>
inline void ONS_RequestCallback_mf8_c<T1,T2,A1,A2,A3,A4,A5,A6,A7,A8>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 8)
    throw ONS_ArgumentMismatch(8);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A8> theArg8;
  (theArgument,theStatus) >>= theArg8.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(8);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value,theArg6.value,theArg7.value,theArg8.value);
}

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7, class A8>
class ONS_RequestCallback_mf8 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5,A6,A7,A8);

			~ONS_RequestCallback_mf8()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf8(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7, class A8>
inline void ONS_RequestCallback_mf8<T1,T2,A1,A2,A3,A4,A5,A6,A7,A8>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 8)
    throw ONS_ArgumentMismatch(8);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A8> theArg8;
  (theArgument,theStatus) >>= theArg8.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(8);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value,theArg6.value,theArg7.value,theArg8.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7, class A8>
class ONS_RequestCallback_o8_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5,A6,A7,A8) const;

			~ONS_RequestCallback_o8_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o8_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7, class A8>
inline void ONS_RequestCallback_o8_c<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7,A8>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 8)
    throw ONS_ArgumentMismatch(8);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A8> theArg8;
  (theArgument,theStatus) >>= theArg8.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(8);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value,theArg6.value,theArg7.value,
   theArg8.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7, class A8>
class ONS_RequestCallback_o8 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5,A6,A7,A8);

			~ONS_RequestCallback_o8()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o8(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7, class A8>
inline void ONS_RequestCallback_o8<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7,A8>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 8)
    throw ONS_ArgumentMismatch(8);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A8> theArg8;
  (theArgument,theStatus) >>= theArg8.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(8);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value,theArg6.value,theArg7.value,
   theArg8.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7, class A8, class A9>
class ONS_RequestCallback_mf9_c : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9) const;

			~ONS_RequestCallback_mf9_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf9_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7, class A8, class A9>
inline void ONS_RequestCallback_mf9_c<T1,T2,A1,A2,A3,A4,A5,A6,A7,A8,A9>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 9)
    throw ONS_ArgumentMismatch(9);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A8> theArg8;
  (theArgument,theStatus) >>= theArg8.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(8);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A9> theArg9;
  (theArgument,theStatus) >>= theArg9.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(9);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value,theArg6.value,theArg7.value,theArg8.value,
   theArg9.value);
}

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7, class A8, class A9>
class ONS_RequestCallback_mf9 : public ONS_EventCallback
{
  public:

    typedef void	(T2::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9);

			~ONS_RequestCallback_mf9()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_mf9(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class A1, class A2, class A3, class A4, class A5,
 class A6, class A7, class A8, class A9>
inline void ONS_RequestCallback_mf9<T1,T2,A1,A2,A3,A4,A5,A6,A7,A8,A9>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 9)
    throw ONS_ArgumentMismatch(9);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A8> theArg8;
  (theArgument,theStatus) >>= theArg8.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(8);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A9> theArg9;
  (theArgument,theStatus) >>= theArg9.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(9);

  (*thePayload) <<= OTC_ROPayload::nullValue();

  (*agent_.*method_)(theArg1.value,theArg2.value,theArg3.value,
   theArg4.value,theArg5.value,theArg6.value,theArg7.value,theArg8.value,
   theArg9.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7, class A8, class A9>
class ONS_RequestCallback_o9_c : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9) const;

			~ONS_RequestCallback_o9_c()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o9_c(
			 T2 const* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2 const*		agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7, class A8, class A9>
inline void ONS_RequestCallback_o9_c<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7,A8,
 A9>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 9)
    throw ONS_ArgumentMismatch(9);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A8> theArg8;
  (theArgument,theStatus) >>= theArg8.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(8);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A9> theArg9;
  (theArgument,theStatus) >>= theArg9.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(9);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value,theArg6.value,theArg7.value,
   theArg8.value,theArg9.value);
}

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7, class A8, class A9>
class ONS_RequestCallback_o9 : public ONS_EventCallback
{
  public:

    typedef RT		(T2::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9);

			~ONS_RequestCallback_o9()
				{ ONS_CallbackObject<T1>::release(agent_); }

			ONS_RequestCallback_o9(
			 T2* theAgent, M theMethod)
			  : agent_(theAgent), method_(theMethod)
                                { ONS_CallbackObject<T1>::acquire(theAgent); }

    inline void		execute(OTC_Event* theEvent, void* theResult);

  private:

    T2*			agent_;

    M			method_;
};

template<class T1, class T2, class RT, class A1, class A2, class A3, class A4,
 class A5, class A6, class A7, class A8, class A9>
inline void ONS_RequestCallback_o9<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7,A8,
 A9>::execute(
 OTC_Event* theEvent,
 void* theResult
)
{
  OTCEV_Request* theRequest;
  theRequest = (OTCEV_Request*)theEvent;

  OTC_ROPayload theArguments;
  theArguments = theRequest->content();

  if (theArguments.nodeType() != "array")
    throw ONS_PayloadInvalid();
  if (theArguments.numChildren() != 9)
    throw ONS_ArgumentMismatch(9);

  OTC_SVPayload* thePayload;
  thePayload = (OTC_SVPayload*)theResult;

  bool theStatus = true;

  OTC_ROPayload theArgument;
  theArgument = theArguments[0];

  ONS_ArgumentWrapper<A1> theArg1;
  (theArgument,theStatus) >>= theArg1.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(1);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A2> theArg2;
  (theArgument,theStatus) >>= theArg2.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(2);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A3> theArg3;
  (theArgument,theStatus) >>= theArg3.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(3);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A4> theArg4;
  (theArgument,theStatus) >>= theArg4.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(4);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A5> theArg5;
  (theArgument,theStatus) >>= theArg5.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(5);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A6> theArg6;
  (theArgument,theStatus) >>= theArg6.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(6);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A7> theArg7;
  (theArgument,theStatus) >>= theArg7.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(7);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A8> theArg8;
  (theArgument,theStatus) >>= theArg8.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(8);
  theArgument = theArgument.nextSibling();
  ONS_ArgumentWrapper<A9> theArg9;
  (theArgument,theStatus) >>= theArg9.value;
  if (theStatus == false)
    throw ONS_ArgumentInvalid(9);

  (*thePayload) <<= (*agent_.*method_)(theArg1.value,theArg2.value,
   theArg3.value,theArg4.value,theArg5.value,theArg6.value,theArg7.value,
   theArg8.value,theArg9.value);
}

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class ONS_RequestCallbackFactory
{
  public:

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(OTCEV_Request*) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf0r_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(OTCEV_Request*))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf0r<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)() const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf0_c<T1,T2>(theAgent,theMethod) : 0; }

    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)())
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf0<T1,T2>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)() const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o0_c<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class RT>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)())
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o0<T1,T2,RT>(theAgent,theMethod) : 0; }

    template<class A1>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf1_c<T1,T2,A1>(theAgent,theMethod) : 0; }

    template<class A1>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf1<T1,T2,A1>(theAgent,theMethod) : 0; }

    template<class RT, class A1>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o1_c<T1,T2,RT,A1>(theAgent,theMethod) : 0; }

    template<class RT, class A1>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o1<T1,T2,RT,A1>(theAgent,theMethod) : 0; }

    template<class A1, class A2>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1,A2) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf2_c<T1,T2,A1,A2>(theAgent,theMethod) : 0; }

    template<class A1, class A2>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1,A2))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf2<T1,T2,A1,A2>(theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1,A2) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o2_c<T1,T2,RT,A1,A2>(theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1,A2))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o2<T1,T2,RT,A1,A2>(theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1,A2,A3) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf3_c<T1,T2,A1,A2,A3>(theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1,A2,A3))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf3<T1,T2,A1,A2,A3>(theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1,A2,A3) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o3_c<T1,T2,RT,A1,A2,A3>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1,A2,A3))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o3<T1,T2,RT,A1,A2,A3>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf4_c<T1,T2,A1,A2,A3,A4>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf4<T1,T2,A1,A2,A3,A4>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o4_c<T1,T2,RT,A1,A2,A3,A4>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o4<T1,T2,RT,A1,A2,A3,A4>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf5_c<T1,T2,A1,A2,A3,A4,A5>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf5<T1,T2,A1,A2,A3,A4,A5>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o5_c<T1,T2,RT,A1,A2,A3,A4,A5>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o5<T1,T2,RT,A1,A2,A3,A4,A5>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5,A6) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf6_c<T1,T2,A1,A2,A3,A4,A5,A6>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5,A6))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf6<T1,T2,A1,A2,A3,A4,A5,A6>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5,
     class A6>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5,A6) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o6_c<T1,T2,RT,A1,A2,A3,A4,A5,A6>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5,
     class A6>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5,A6))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o6<T1,T2,RT,A1,A2,A3,A4,A5,A6>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf7_c<T1,T2,A1,A2,A3,A4,A5,A6,A7>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf7<T1,T2,A1,A2,A3,A4,A5,A6,A7>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o7_c<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o7<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7, class A8>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf8_c<T1,T2,A1,A2,A3,A4,A5,A6,A7,A8>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7, class A8>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf8<T1,T2,A1,A2,A3,A4,A5,A6,A7,A8>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o8_c<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7,A8>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o8<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7,A8>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7, class A8, class A9>
    static ONS_EventCallback*	create(T2 const* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf9_c<T1,T2,A1,A2,A3,A4,A5,A6,A7,A8,A9>(
       theAgent,theMethod) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7, class A8, class A9>
    static ONS_EventCallback*	create(T2* theAgent,
     void (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_mf9<T1,T2,A1,A2,A3,A4,A5,A6,A7,A8,A9>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8, class A9>
    static ONS_EventCallback*	create(T2 const* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9) const)
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o9_c<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7,A8,A9>(
       theAgent,theMethod) : 0; }

    template<class RT, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8, class A9>
    static ONS_EventCallback*	create(T2* theAgent,
     RT (T2::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9))
      { return (theAgent != 0 && theMethod != 0) ?
       new ONS_RequestCallback_o9<T1,T2,RT,A1,A2,A3,A4,A5,A6,A7,A8,A9>(
       theAgent,theMethod) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_AGENT_RQUESTCB_HH */
