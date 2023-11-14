#ifndef OTC_DEBUG_TOBJECT_HH
#define OTC_DEBUG_TOBJECT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/tobject.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_OTC_H
#include <OTC/OTC.h>
#endif
#ifndef OTC_THREAD_NRMUTEX_HH
#include <OTC/thread/nrmutex.hh>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/tobject.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_TObject
    // = TITLE
    //     Class to allow actions to be run on abnormal program termination.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     Any of your classes, which need to perform special actions
    //     in the event of abnormal program termination, should be derived
    //     from this class. In your derived class, you should redefine the
    //     <cleanup()> function, to initiate any actions which your class
    //     needs to perform. This function will be called when <terminate()>
    //     is invoked, provided that you have registered a terminate function
    //     which invokes the function <OTC_TObject::terminateAll()>.
    //     
    //     Note that your <cleanup()> function should do as little as
    //     possible. If your function results in another exception being
    //     thrown, then this will generally result in <abort()> immediately
    //     being called.
    //     
    //     The requirement for this class has largely vanished due to the
    //     existance of real exceptions, as most objects will be destroyed
    //     automatically as a result of the stack being unwound, when an
    //     exception occurs. This class could still be useful though to
    //     ensure that destructors of objects on the free store, referenced
    //     through pointers at global scope, are destroyed.
{
  public:

    // = DESTRUCTION

    virtual		~OTC_TObject();
				// Removes this class from the linked list of
				// all <OTC_TObject> classes.

    // = TERMINATION

    static void		terminateAll();
				// Iterates through all instances of the
				// <OTC_TObject> class and invokes
				// <cleanup()> on each one.

  protected:

    // = CONSTRUCTION

			OTC_TObject();
				// Adds this class to a linked list of all
				// <OTC_TObject> classes.

    // = CLEANUP

    virtual void	cleanup() = 0;
				// Must be redefined in your derived class to
				// initiate any actions your class needs to
				// perform, when the program is abnormally
				// terminated.

  private:

			OTC_TObject(OTC_TObject const&);
				// Do not define an implementation for this.

    OTC_TObject&	operator=(OTC_TObject const&);
				// Do not define an implementation for this.

    void		link();
				// Links this class into the chain of all
				// <OTC_TObject> classes.

    void		unlink();
				// Unlinks this class from the chain of all
				// <OTC_TObject> classes.

    OTC_TObject*	next_;
				// The next <OTC_TObject> class in the chain
				// after this one.

    OTC_TObject*	prev_;
				// The previous <OTC_TObject> class in the
				// chain to this one.


    static OTC_NRMutex	mutex_;
				// Lock for threads. Can use non recursive
				// lock as no chance of an attempt to
				// acquire lock from within scope of lock.

    static OTC_TObject*	head_;
				// The head of the chain of all <OTC_TObject>
				// classes.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_TOBJECT_HH */
