#ifndef ONS_AGENT_CBOBJECT_HH
#define ONS_AGENT_CBOBJECT_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/cbobject.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/hset.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/cbobject.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class ONS_Callback;

class OSE_EXPORT ONS_CBObject
    // = TITLE
    //     Base class for target classes in signal/slot implementation.
    //
    // = DESCRIPTION
    //     The <ONS_CBObject> class should be used as the base class of any
    //     class which notionally contains slots, or callback member
    //     functions, which are the target of a signal in this signal/slot
    //     implementation.
    //     
    // = SEE ALSO
    //     <ONS_Callback>
{
    friend class	ONS_Callback;

  public:

    // = DESTRUCTION

    virtual		~ONS_CBObject();
				// Detaches all remaining signal sources from
				// this target.

    // = SIGNAL SOURCES

    void		detachAllCallbacks();
				// Detaches all signal sources from this
				// target.

  protected:

    // = INITIALISATION

  			ONS_CBObject();
				// Target is not initially attached to any
				// signal sources.

  private:

  			ONS_CBObject(ONS_CBObject const&);
				// Do not define an implementation for this.

    ONS_CBObject&	operator=(ONS_CBObject const&);
				// Do not define an implementation for this.

    // Note that the thread mutex in <ONS_Callback> is used when locking
    // data in this class so as to avoid deadlock problems.

    OTC_HSet<ONS_Callback*>*	sources_;
    				// Set of signal sources which target slots
				// in a derived instance of this class.
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_CBOBJECT_HH */
