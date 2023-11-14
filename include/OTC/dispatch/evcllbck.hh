#ifndef OTC_DISPATCH_EVCLLBCK_HH
#define OTC_DISPATCH_EVCLLBCK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/evcllbck.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/cmmnpool.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/atomic.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/evcllbck.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Event;

class OSE_EXPORT OTC_EVCallback
    // = TITLE
    //     Base class for implementing callback objects for events.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_EVCallback> class is the base class for all callback
    //     objects to be used in conjunction with <OTC_EVThreadPool>. The
    //     derived class might hold a pointer to a function to execute,
    //     passing the event as argument, or it may actually contain an
    //     implementation which directly deals with the event in some way.
    //     The class may be used for other purposes as well where a generic
    //     base class for executing an action based on the arrival of an
    //     event is required.
    //
    // = NOTES
    //     Because a callback might be a permanent registration, there might
    //     be a chance that it could be called more than once at the same
    //     time. If a derived callback object maintains state which gets
    //     changed during the process of execution of the callback, the
    //     derived class should override <clone()> to return a copy of the
    //     object which can then be used in isolation. The base class
    //     <clone()> and <destroy()> methods implement a reference count,
    //     whereby <clone()> increments the reference count and simply returns
    //     a pointer to the same object. The <destroy()> method decrements
    //     the reference count and if it reaches <0> the callback deletes
    //     itself. If the callback is purposely created as a static object,
    //     the derived class would need to override <destroy()> not to do
    //     anything, with <clone()> return a pointer to the same object but
    //     not manipulating the reference count.
    //     
    // = SEE ALSO
    //     <OTC_EVThreadPool>, <OTC_EVHandler>
{
  public:

    virtual             ~OTC_EVCallback();

    void*               operator new(size_t theSize)
                                { return OTC_CommonPool::allocate(theSize); }

    void                operator delete(void* theMemory, size_t theSize)
                                { OTC_CommonPool::release(theMemory,theSize); }

    // = EXECUTION

    virtual void	execute(OTC_Event* theEvent) = 0;
				// Must be redefined in a derived class to
				// perform the actual delivery of <theEvent>
				// to a callback function.

    // = CLONING

    virtual OTC_EVCallback*	clone();
				// By default increments a reference count
				// and returns a pointer to the same object.

    virtual void	destroy();
				// Generally called once the callback has
				// been executed. The default action is to
				// decrement the reference count and if it
				// reaches <0> delete the object.

  protected:

			OTC_EVCallback() : refCount_(1) {}

  private:

			OTC_EVCallback(OTC_EVCallback const&);
				// Do not define an implementation for this.

    OTC_EVCallback&	operator=(OTC_EVCallback const&);
				// Do not define an implementation for this.

    static OTC_NRMutex	mutex_;
				// Mutex to protect thread access to count.

    OTC_Atomic		refCount_;
				// Reference count.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_EVCLLBCK_HH */
