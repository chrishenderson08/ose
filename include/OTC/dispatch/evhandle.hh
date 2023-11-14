#ifndef OTC_DISPATCH_EVHANDLE_HH
#define OTC_DISPATCH_EVHANDLE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/evhandle.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/evthread.hh>
#include <OTC/dispatch/evcllbck.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/evhandle.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_EVAgent;
class OTC_EVHandler;

class OSE_EXPORT OTC_EVHandlerCallback : public OTC_EVCallback
    // = TITLE
    //    Callback object for <OTC_EVHandler> from <OTC_EVThreadPool>.
    //
    // = DESCRIPTION
    //    The <OTC_EVHandlerCallback> class is a special callback object
    //    for having the <filter()> member function of an <OTC_EVHandler>
    //    object called from <OTC_EVThreadPool>.
{
  public:

			~OTC_EVHandlerCallback();

    // = INITIALISATION

			OTC_EVHandlerCallback(OTC_EVHandler* theHandler)
			  : handler_(theHandler) {}
				// Sets <theHandler> to be the object to
				// callback to.

    // = EXECUTION

    void		execute(OTC_Event* theEvent);
				// Calls the <filter()> member function of
				// the referenced handler with <theEvent> as
				// argument.

    // = CLONING

    OTC_EVCallback*	clone();
				// Returns pointer to the same object and
				// ignores the reference count.

    void		destroy();
				// Does nothing. Assumes that the callback
				// object is member variable of the handler
				// class.

  private:

			OTC_EVHandlerCallback(OTC_EVHandlerCallback const&);
				// Do not define an implementation for this.

    OTC_EVHandlerCallback&	operator=(OTC_EVHandlerCallback const&);
				// Do not define an implementation for this.

    OTC_EVHandler*	handler_;
				// The handler.
};

class OSE_EXPORT OTC_EVHandler
    // = TITLE
    //	   Class to assist in special handling of events by an agent.
    //
    // = DESCRIPTION
    //     The <OTC_EVHandler> class can be used by itself to cause the
    //     handling of events received by an agent to be dealt with by
    //     separate threads. Rather than a thread being created for each
    //     event, a pool of threads is created and each event is in
    //     turn handed off to an available thread. A queueing mechanism
    //     for events is implemented to handle the case where no threads
    //     are currently available.
    //     
    //     By default, each thread when it receives the event will call
    //     the <filter()> member function of the instance of <OTC_EVAgent>
    //     to which it is attached. You can instead derive from this
    //     class and redefine the <filter()> member function of this class
    //     to implement some other behaviour, either handling the event
    //     within this class or calling some other member function of
    //     the associated agent.
    //
    //     When an event is passed to this class by calling the <process()>
    //     member function, by deriving from this class and redefining
    //     <process()> to check the type of the event, you have the option of
    //     rejecting the event. This supports having multiple instances of
    //     derived versions of this class being attached to an agent.
    //     Specific instances can handle only certain events. Each of the
    //     handlers can be checked and if none of them accepts the event,
    //     the event can finally be passed off to the <filter()> member
    //     function of <OTC_EVAgent> instance to which the event was
    //     originally delivered.
    //
    //     The ability to reject events and/or change where they are sent,
    //     can be used independently of having any threads. That is, you
    //     can indicate you want no threads to be created. This means that
    //     the thread which delivered the event in the first place will do
    //     all the work. When multithreading isn't available, even if you
    //     specify that threads should be created, this will be how events
    //     will always be handled, ie., in the main thread.
    //     
    // = NOTES
    //     Note that if an external thread pool was provided, the
    //     <shutdown()> method of this class will not shutdown the thread
    //     pool, you will need to do that yourself in some way.
    //     
    // = SEE ALSO
    //	   <OTC_Event>, <OTC_EVAgent>, <OTC_EVThreadPool>
{
    friend		class OTC_EVAgent;
    friend		class OTC_EVHandlerCallback;

  public:

    // = DESTRUCTION

    virtual		~OTC_EVHandler();
				// Destroys the thread pool if it was created
				// internally and not supplied from an
				// external source. If the thread pool was
				// created internally an attempt will be
				// made to shut it down before destroying
				// it. Note that it really should have been
				// shutdown by this point, if it hasn't
				// there is the danger that threads may
				// access data that no longer exists.

    // = INITIALISATION

			OTC_EVHandler(
			 OTC_EVAgent* theAgent,
			 u_int theNumThreads=0,
			 u_int theQueueSize=0,
			 u_int theThreshold=0
			);
				// In a threaded environment where threads
				// within the agent are supported,
				// <theNumThreads> is the number of threads
				// to create to handle incoming events. Where
				// the threads aren't supported,
				// <theNumThreads> is ignored. Where threads
				// are created, if <theQueueSize> is <0>, all
				// events will be queued and the event queue
				// will be allowed to grow to any size. If
				// <theQueueSize> is non zero and
				// <theThreshold> is also non zero, when the
				// number of events in the exceeds the value
				// in <theThreshold>, the <throttle()>
				// function of the thread pool is called.
				// <theAgent> is the agent to which this
				// handler will be attached and to which
				// events will normally be passed to by
				// threads.

			OTC_EVHandler(
			 OTC_EVAgent* theAgent,
			 OTC_EVThreadPool* theThreadPool
			);
				// <theThreadPool> is a pool of threads used
				// to service events. <theAgent> is the agent
				// to which this handler will be attached.
				// <theThreadPool> can be shared between
				// multiple handlers. Note however that this
				// handler will not be responsible for
				// starting it, shutting it down or deleting
				// it.

    // = THREADS

    void                suspend();
                                // Suspends processing of any available      
                                // events by threads. This doesn't stop      
                                // any threads which may already be
                                // handling an event. This function has no   
                                // affect when there are no active threads,
                                // that is, events will still be handled.  

    void                resume();
                                // Resumes the processing of events by    
                                // threads. This function has no affect when
                                // there are no active threads. That is,     
                                // events will still be handled.

    void		limit(u_int theNumThreads);
				// Temporarily limits the number of threads
				// which are allowed to handle incoming
				// events to <theNumThreads>. This might be
				// used during an initialisation phase to set
				// the number of threads to <1> so that
				// events are handled in the order received
				// until a state is reached where having
				// multiple threads running and where the
				// events may appear to be processed out of
				// order is okay. To resume all available
				// threads, provide the argument of <0>. If
				// you want to stop all threads for a while,
				// use <suspend()> instead. Note that if
				// there are threads currently handling
				// events such that more threads are
				// active than <theNumThreads>, currently
				// running threads are not stopped. It just
				// means that the excess threads when they
				// finally finish with the event they are
				// handling will not necessarily grab the
				// next event if there are some available.
				// Instead, the excess threads will wait
				// until the number of active threads is
				// less than <theNumThreads>. This function
				// has no affect when there are no active
				// threads. That is, events will still be
				// handled.

    // = CONTEXT

    u_int		threadId() const;
				// Returns an ID identifying the calling
				// thread within the context of the
				// associated thread pool. If thread support
				// is not compiled in to the library or there
				// is no associated thread pool, <0> will be
				// returned. If the calling thread is not
				// from the associated thread pool, <0> will
				// be returned. Otherwise, the thread IDs
				// will range form <1> up to the number of
				// threads.

    // = SHUTDOWN

    virtual void	shutdown();
				// When threads are active and the thread
				// pool created internally, it will suspend
				// further processing of events and wait for
				// currently active threads to finish
				// handling the events they responded to.
				// When the threads are no longer doing
				// anything they are made to kill themselves.
				// If threads are being used, must be called
				// before the agent is deleted to ensure that
				// threads are not still trying to access any
				// data when it is being deleted. If desired,
				// a derived class can override this member
				// function to do its own shutdown
				// operations. The derived class member
				// function would normally call this function
				// as the first thing it does to ensure that
				// any threads have been stopped. The only
				// exception to this might be to first set
				// some flag which is checked by long running
				// operations to see if they should abort
				// before finishing. This function needs to
				// be called in some way before this class is
				// to be deleted. This function should not be
				// called by a thread bound to this handler
				// as it will hang waiting for itself to
				// finish. Even if no threads are being used,
				// a derived class might override this to do
				// its own shutdown operations. If this is
				// done, it should assume that threads are
				// being used and still call the base class
				// function. This will ensure that the code
				// will function if it is decied to start
				// using threads at a later time.

    // = PROCESSING

    virtual OTC_Event*	process(OTC_Event* theEvent);
				// When an agent to which this handler is
				// attached receives an event, your code
				// would probably call this member function
				// for each of the handlers associated with
				// the agent. The purpose of this function is
				// to determine if this handler will accept
				// the event. The default implementation will
				// call <accept()> and return <0> indicating
				// that this handler has accepted the event
				// and that the agent should stop looking for
				// another handler to deal with the event.
				// You can override this member function in a
				// derived class to check the type of the
				// event and details of the event to
				// determine if the handler can deal with the
				// event. If it can't handle the event, you
				// can return the event to indicate that the
				// agent should keep look for a handler to
				// deal with the event. If the agent can't
				// find a handler for the event, it will call
				// <filter()> on the agent which by default
				// will then call <handle()> on the agent.
				// Note that if in a derived class you
				// override this member function and you
				// decide to accept the event, you should
				// call this base class version of the
				// function, returning the result it returns
				// if you want to accept the event. Note that
				// it is feasible that a different event
				// might be returned. If this is the case,
				// your code should check for the new event
				// and do something with it as appropriate.
				// This might be used to generate some
				// other type of event which should make its
				// way to the agent via the <filter()>
				// function without rescheduling a completely
				// new event.

  protected:

    // = EVENT CALLBACKS

    virtual void	accept(OTC_Event* theEvent);
				// Events are passed to this function by
				// <process()>. If no threads are being used
				// in this agent, <theEvent> is handed off to
				// <filter()> otherwise the event is put on
				// the internal event queue. Would only be
				// redefined if it was necessary to detect
				// events which should jump any event queue
				// when threads are used internal to the
				// agent. If the derived class didn't do
				// anything with the event, it must call this
				// function. If the derived class did do
				// something with the event, it should
				// destroy the event.

    virtual void	filter(OTC_Event* theEvent);
				// By default calls <filter()> on the agent
				// to which this handler is attached, passing
				// <theEvent> as argument. Can be redefined
				// in a derived class, if you want to filter
				// out certain events and convert them to
				// specific function callbacks or to handle
				// them within the derived instance rather
				// than having them passed back to the agent.
				// If overridden in a derived class, but
				// the event isn't filtered out for special
				// processing, the derived member function
				// should call this base class version of
				// the function to deliver the event to the
				// agent. If this base class version of the
				// function gets called and the handler is
				// not attached to an agent, an exception
				// will be raised.

  private:

			OTC_EVHandler(OTC_EVHandler const&);
				// Do not define an implementation for this.

    OTC_EVHandler&	operator=(OTC_EVHandler const&);
				// Do not define an implementation for this.

    OTC_EVAgent*	agent_;
				// Agent to which events will be given
				// back to.

    OTC_EVThreadPool*	threadPool_;
				// Thread pool if it exists.

    bool		poolInternal_;
				// If there is a thread pool, indicates
				// that if it was created internally.

    OTC_EVHandlerCallback	callback_;
				// Callback object for events coming back
				// from the thread pool.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_EVHANDLE_HH */
