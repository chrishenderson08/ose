#ifndef ONS_AGENT_EXECTECB_HH
#define ONS_AGENT_EXECTECB_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/exectecb.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/exectecb.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class ONS_CBObject;

class OSE_EXPORT ONS_ExecuteCallback : public OTC_Resource
{
  protected:

    // = INITIALISATION

			ONS_ExecuteCallback(
			 ONS_CBObject const* theTarget,
			 char const* theName
			);
			  	// Initialise the callback where the target
				// class of the callback is <theTarget>.
				// <theName> identifies the callback. If
				// <theName> is <0>, a unique name for the
				// callback is generated.

  public:

    // = CALLBACK TARGET

    ONS_CBObject*	target() const
    				{ return target_; }
				// Returns the target class of the callback.

    // = CALLBACK NAME

    OTC_String const&	name() const
    				{ return name_; }
				// Returns the name used to identify the
				// callback.

    // = CALLBACK DEATH

    void		kill();
    				// Marks the callback as dead. The code
				// where the callback is to be executed
				// should check whether it has been killed
				// off and not execute it if it has.

    bool		dead() const;
				// Returns true if the callback has been
				// killed off and shouldn't actually be
				// executed.

  private:

  			ONS_ExecuteCallback(ONS_ExecuteCallback const&);
				// Do not define an implementation for this.

    ONS_ExecuteCallback&	operator=(ONS_ExecuteCallback const&);
				// Do not define an implementation for this.

    static OTC_NRMutex	mutex_;
    				// Thread lock protecting member data.

    ONS_CBObject*	target_;
    				// Pointer to class containing the slot
				// which the signal is connected to.

    OTC_String		name_;
    				// The name associated with then callback.

    bool		dead_;
    				// Will be true if the callback has been
				// marked as dead and shouln't actually be
				// executed.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<ONS_ExecuteCallback*>
{
  public:

    static ONS_ExecuteCallback*	add(ONS_ExecuteCallback* theObject)
				{ theObject->reference(); return theObject; }

    static void		remove(ONS_ExecuteCallback* theObject)
				{ theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_ExecuteCallback_0 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_0(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute() = 0;
};

template<class T>
class ONS_ExecuteCallback_t0_c : public ONS_ExecuteCallback_0
{
  public:

    typedef void	(T::*M)() const;

  			ONS_ExecuteCallback_t0_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_0(theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute();

  private:

    M			method_;
};

template<class T>
inline void ONS_ExecuteCallback_t0_c<T>::execute()
{
  if (dead()) return;

  (*(T const*)target().*method_)();
}

template<class T>
class ONS_ExecuteCallback_t0 : public ONS_ExecuteCallback_0
{
  public:

    typedef void	(T::*M)();

  			ONS_ExecuteCallback_t0(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_0(theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute();

  private:

    M			method_;
};

template<class T>
inline void ONS_ExecuteCallback_t0<T>::execute()
{
  if (dead()) return;

  (*(T*)target().*method_)();
}

/* ------------------------------------------------------------------------- */

template<class A1>
class ONS_ExecuteCallback_1 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_1(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1) = 0;
};

template<class T, class A1>
class ONS_ExecuteCallback_t1_c : public ONS_ExecuteCallback_1<A1>
{
  public:

    typedef void	(T::*M)(A1) const;

  			ONS_ExecuteCallback_t1_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_1<A1>(theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1);

  private:

    M			method_;
};

template<class T, class A1>
inline void ONS_ExecuteCallback_t1_c<T,A1>::execute(A1 theArg1)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1);
}

template<class T, class A1>
class ONS_ExecuteCallback_t1 : public ONS_ExecuteCallback_1<A1>
{
  public:

    typedef void	(T::*M)(A1);

  			ONS_ExecuteCallback_t1(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_1<A1>(theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1);

  private:

    M			method_;
};

template<class T, class A1>
inline void ONS_ExecuteCallback_t1<T,A1>::execute(A1 theArg1)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1);
}

/* ------------------------------------------------------------------------- */

template<class A1, class A2>
class ONS_ExecuteCallback_2 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_2(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1,A2) = 0;
};

template<class T, class A1, class A2>
class ONS_ExecuteCallback_t2_c : public ONS_ExecuteCallback_2<A1,A2>
{
  public:

    typedef void	(T::*M)(A1,A2) const;

  			ONS_ExecuteCallback_t2_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_2<A1,A2>(theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2);

  private:

    M			method_;
};

template<class T, class A1, class A2>
inline void ONS_ExecuteCallback_t2_c<T,A1,A2>::execute(A1 theArg1, A2 theArg2)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1,theArg2);
}

template<class T, class A1, class A2>
class ONS_ExecuteCallback_t2 : public ONS_ExecuteCallback_2<A1,A2>
{
  public:

    typedef void	(T::*M)(A1,A2);

  			ONS_ExecuteCallback_t2(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_2<A1,A2>(theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2);

  private:

    M			method_;
};

template<class T, class A1, class A2>
inline void ONS_ExecuteCallback_t2<T,A1,A2>::execute(A1 theArg1, A2 theArg2)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1,theArg2);
}

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3>
class ONS_ExecuteCallback_3 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_3(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1,A2,A3) = 0;
};

template<class T, class A1, class A2, class A3>
class ONS_ExecuteCallback_t3_c : public ONS_ExecuteCallback_3<A1,A2,A3>
{
  public:

    typedef void	(T::*M)(A1,A2,A3) const;

  			ONS_ExecuteCallback_t3_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_3<A1,A2,A3>(theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3>
inline void ONS_ExecuteCallback_t3_c<T,A1,A2,A3>::execute(A1 theArg1,
 A2 theArg2, A3 theArg3)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1,theArg2,theArg3);
}

template<class T, class A1, class A2, class A3>
class ONS_ExecuteCallback_t3 : public ONS_ExecuteCallback_3<A1,A2,A3>
{
  public:

    typedef void	(T::*M)(A1,A2,A3);

  			ONS_ExecuteCallback_t3(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_3<A1,A2,A3>(theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3>
inline void ONS_ExecuteCallback_t3<T,A1,A2,A3>::execute(A1 theArg1,
 A2 theArg2, A3 theArg3)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1,theArg2,theArg3);
}

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4>
class ONS_ExecuteCallback_4 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_4(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1,A2,A3,A4) = 0;
};

template<class T, class A1, class A2, class A3, class A4>
class ONS_ExecuteCallback_t4_c : public ONS_ExecuteCallback_4<A1,A2,A3,A4>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4) const;

  			ONS_ExecuteCallback_t4_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_4<A1,A2,A3,A4>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4>
inline void ONS_ExecuteCallback_t4_c<T,A1,A2,A3,A4>::execute(A1 theArg1,
 A2 theArg2, A3 theArg3, A4 theArg4)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1,theArg2,theArg3,theArg4);
}

template<class T, class A1, class A2, class A3, class A4>
class ONS_ExecuteCallback_t4 : public ONS_ExecuteCallback_4<A1,A2,A3,A4>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4);

  			ONS_ExecuteCallback_t4(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_4<A1,A2,A3,A4>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4>
inline void ONS_ExecuteCallback_t4<T,A1,A2,A3,A4>::execute(A1 theArg1,
 A2 theArg2, A3 theArg3, A4 theArg4)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1,theArg2,theArg3,theArg4);
}

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5>
class ONS_ExecuteCallback_5 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_5(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1,A2,A3,A4,A5) = 0;
};

template<class T, class A1, class A2, class A3, class A4, class A5>
class ONS_ExecuteCallback_t5_c : public ONS_ExecuteCallback_5<A1,A2,A3,A4,A5>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5) const;

  			ONS_ExecuteCallback_t5_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_5<A1,A2,A3,A4,A5>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5>
inline void ONS_ExecuteCallback_t5_c<T,A1,A2,A3,A4,A5>::execute(A1 theArg1,
 A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5);
}

template<class T, class A1, class A2, class A3, class A4, class A5>
class ONS_ExecuteCallback_t5 : public ONS_ExecuteCallback_5<A1,A2,A3,A4,A5>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5);

  			ONS_ExecuteCallback_t5(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_5<A1,A2,A3,A4,A5>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5>
inline void ONS_ExecuteCallback_t5<T,A1,A2,A3,A4,A5>::execute(A1 theArg1,
 A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5);
}

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5, class A6>
class ONS_ExecuteCallback_6 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_6(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1,A2,A3,A4,A5,A6) = 0;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6>
class ONS_ExecuteCallback_t6_c : public ONS_ExecuteCallback_6<A1,A2,A3,A4,A5,A6>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5,A6) const;

  			ONS_ExecuteCallback_t6_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_6<A1,A2,A3,A4,A5,A6>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5,A6);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6>
inline void ONS_ExecuteCallback_t6_c<T,A1,A2,A3,A4,A5,A6>::execute(A1 theArg1,
 A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5, A6 theArg6)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5,
   theArg6);
}

template<class T, class A1, class A2, class A3, class A4, class A5, class A6>
class ONS_ExecuteCallback_t6 : public ONS_ExecuteCallback_6<A1,A2,A3,A4,A5,A6>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5,A6);

  			ONS_ExecuteCallback_t6(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_6<A1,A2,A3,A4,A5,A6>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5,A6);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6>
inline void ONS_ExecuteCallback_t6<T,A1,A2,A3,A4,A5,A6>::execute(A1 theArg1,
 A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5, A6 theArg6)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5,theArg6);
}

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7>
class ONS_ExecuteCallback_7 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_7(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1,A2,A3,A4,A5,A6,A7) = 0;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7>
class ONS_ExecuteCallback_t7_c : public ONS_ExecuteCallback_7<A1,A2,A3,A4,A5,A6,
 A7>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5,A6,A7) const;

  			ONS_ExecuteCallback_t7_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_7<A1,A2,A3,A4,A5,A6,A7>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5,A6,A7);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7>
inline void ONS_ExecuteCallback_t7_c<T,A1,A2,A3,A4,A5,A6,A7>::execute(
 A1 theArg1, A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5, A6 theArg6,
 A7 theArg7)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5,
   theArg6,theArg7);
}

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7>
class ONS_ExecuteCallback_t7 : public ONS_ExecuteCallback_7<A1,A2,A3,A4,A5,A6,
 A7>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5,A6,A7);

  			ONS_ExecuteCallback_t7(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_7<A1,A2,A3,A4,A5,A6,A7>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5,A6,A7);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7>
inline void ONS_ExecuteCallback_t7<T,A1,A2,A3,A4,A5,A6,A7>::execute(
 A1 theArg1, A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5, A6 theArg6,
 A7 theArg7)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5,theArg6,
   theArg7);
}

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7,
 class A8>
class ONS_ExecuteCallback_8 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_8(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1,A2,A3,A4,A5,A6,A7,A8) = 0;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7, class A8>
class ONS_ExecuteCallback_t8_c : public ONS_ExecuteCallback_8<A1,A2,A3,A4,A5,A6,
 A7,A8>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5,A6,A7,A8) const;

  			ONS_ExecuteCallback_t8_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_8<A1,A2,A3,A4,A5,A6,A7,A8>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5,A6,A7,A8);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7, class A8>
inline void ONS_ExecuteCallback_t8_c<T,A1,A2,A3,A4,A5,A6,A7,A8>::execute(
 A1 theArg1, A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5, A6 theArg6,
 A7 theArg7, A8 theArg8)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5,
   theArg6,theArg7,theArg8);
}

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7, class A8>
class ONS_ExecuteCallback_t8 : public ONS_ExecuteCallback_8<A1,A2,A3,A4,A5,A6,
 A7,A8>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5,A6,A7,A8);

  			ONS_ExecuteCallback_t8(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_8<A1,A2,A3,A4,A5,A6,A7,A8>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5,A6,A7,A8);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7, class A8>
inline void ONS_ExecuteCallback_t8<T,A1,A2,A3,A4,A5,A6,A7,A8>::execute(
 A1 theArg1, A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5, A6 theArg6,
 A7 theArg7, A8 theArg8)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5,theArg6,
   theArg7,theArg8);
}

/* ------------------------------------------------------------------------- */

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7,
 class A8, class A9>
class ONS_ExecuteCallback_9 : public ONS_ExecuteCallback
{
  public:

  			ONS_ExecuteCallback_9(
			 ONS_CBObject const* theTarget,
			 char const* theName
			)
			  : ONS_ExecuteCallback(theTarget,theName) {}

    virtual void	execute(A1,A2,A3,A4,A5,A6,A7,A8,A9) = 0;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7, class A8, class A9>
class ONS_ExecuteCallback_t9_c : public ONS_ExecuteCallback_9<A1,A2,A3,A4,A5,A6,
 A7,A8,A9>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9) const;

  			ONS_ExecuteCallback_t9_c(
			 ONS_CBObject const* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_9<A1,A2,A3,A4,A5,A6,A7,A8,A9>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5,A6,A7,A8,A9);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7, class A8, class A9>
inline void ONS_ExecuteCallback_t9_c<T,A1,A2,A3,A4,A5,A6,A7,A8,A9>::execute(
 A1 theArg1, A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5, A6 theArg6,
 A7 theArg7, A8 theArg8, A9 theArg9)
{
  if (dead()) return;

  (*(T const*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5,
   theArg6,theArg7,theArg8,theArg9);
}

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7, class A8, class A9>
class ONS_ExecuteCallback_t9 : public ONS_ExecuteCallback_9<A1,A2,A3,A4,A5,A6,
 A7,A8,A9>
{
  public:

    typedef void	(T::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9);

  			ONS_ExecuteCallback_t9(
			 ONS_CBObject* theTarget,
			 M theMethod,
			 char const* theName
			)
			  : ONS_ExecuteCallback_9<A1,A2,A3,A4,A5,A6,A7,A8,A9>(
			     theTarget,theName),
			    method_(theMethod)
			    	{}

    inline void		execute(A1,A2,A3,A4,A5,A6,A7,A8,A9);

  private:

    M			method_;
};

template<class T, class A1, class A2, class A3, class A4, class A5, class A6,
 class A7, class A8, class A9>
inline void ONS_ExecuteCallback_t9<T,A1,A2,A3,A4,A5,A6,A7,A8,A9>::execute(
 A1 theArg1, A2 theArg2, A3 theArg3, A4 theArg4, A5 theArg5, A6 theArg6,
 A7 theArg7, A8 theArg8, A9 theArg9)
{
  if (dead()) return;

  (*(T*)target().*method_)(theArg1,theArg2,theArg3,theArg4,theArg5,theArg6,
   theArg7,theArg8,theArg9);
}

/* ------------------------------------------------------------------------- */

template<class T>
class ONS_ExecuteCallbackFactory
{
  public:

    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)() const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t0_c<T>(theTarget,theMethod,theName) : 0; }

    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t0<T>(theTarget,theMethod,theName) : 0; }

    template<class A1>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1) const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t1_c<T,A1>(theTarget,theMethod,theName) : 0; }

    template<class A1>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t1<T,A1>(theTarget,theMethod,theName) : 0; }

    template<class A1, class A2>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1,A2) const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t2_c<T,A1,A2>(theTarget,theMethod,theName) : 0; }

    template<class A1, class A2>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1,A2), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t2<T,A1,A2>(theTarget,theMethod,theName) : 0; }

    template<class A1, class A2, class A3>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1,A2,A3) const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t3_c<T,A1,A2,A3>(theTarget,theMethod,
       theName) : 0; }

    template<class A1, class A2, class A3>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1,A2,A3), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t3<T,A1,A2,A3>(theTarget,theMethod,
       theName) : 0; }

    template<class A1, class A2, class A3, class A4>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4) const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t4_c<T,A1,A2,A3,A4>(theTarget,theMethod,
       theName) : 0; }

    template<class A1, class A2, class A3, class A4>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t4<T,A1,A2,A3,A4>(theTarget,theMethod,
       theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5) const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t5_c<T,A1,A2,A3,A4,A5>(theTarget,theMethod,
       theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t5<T,A1,A2,A3,A4,A5>(theTarget,theMethod,
       theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5,A6) const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t6_c<T,A1,A2,A3,A4,A5,A6>(theTarget,theMethod,
       theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5,A6), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t6<T,A1,A2,A3,A4,A5,A6>(theTarget,theMethod,
       theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7) const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t7_c<T,A1,A2,A3,A4,A5,A6,A7>(theTarget,
       theMethod, theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t7<T,A1,A2,A3,A4,A5,A6,A7>(theTarget,
       theMethod,theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7, class A8>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8) const, char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t8_c<T,A1,A2,A3,A4,A5,A6,A7,A8>(theTarget,
       theMethod, theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7, class A8>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t8<T,A1,A2,A3,A4,A5,A6,A7,A8>(theTarget,
       theMethod,theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7, class A8, class A9>
    static ONS_ExecuteCallback*	create(T const* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9) const,
     char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t9_c<T,A1,A2,A3,A4,A5,A6,A7,A8,A9>(theTarget,
       theMethod, theName) : 0; }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
     class A7, class A8, class A9>
    static ONS_ExecuteCallback*	create(T* theTarget,
     void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9), char const* theName)
      { return (theTarget != 0 && theMethod != 0) ?
       new ONS_ExecuteCallback_t9<T,A1,A2,A3,A4,A5,A6,A7,A8,A9>(theTarget,
       theMethod,theName) : 0; }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_EXECTECB_HH */
