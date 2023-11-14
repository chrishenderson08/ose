#ifndef OTC_DISPATCH_CONDTION_HH
#define OTC_DISPATCH_CONDTION_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/condtion.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/condtion.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Condition
    // = TITLE
    //     A boolean condition value.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_Condition> class contains a single boolean value to be
    //     used as a condition value. Users of the class should call
    //     <evaluate()> before calling the <test()> method to determine if
    //     the condition has been statisfied. A derived class may override
    //     the <evaluate()> method to call <set()> to enable the condition
    //     based on some external criteria.
    //     
    //     The class is used as a way of conditionally causing the dispatcher
    //     to exit based on some external stimulus. For example, the
    //     condition may be set through a timer expiring, thus giving the
    //     ability to run the dispatcher for a set time.
    //     
    // = SEE ALSO
    //     <OTC_Dispatcher>, <OTC_JobQueue>
{
  public:

    virtual		~OTC_Condition();

    // = INITIALISATION

			OTC_Condition()
			 : flag_(false) {}
				// The condition flag is set to be off.

    // = EVAULATION

    virtual void	evaluate();
				// By default does nothing. Can be overridden
				// in a derived class. This method should
				// always be called before testing the state
				// of the condition flag.

    void		set()
				{ flag_ = true; }
				// Sets the condition flag to be on.

    void		reset()
				{ flag_ = false; }
				// Sets the condition flag to be off.

    // = CONDITION

    bool		test() const
				{ return flag_; }
				// Returns the state of the condition flag.

  private:

			OTC_Condition(OTC_Condition const&);
				// Do not define an implementation for this.

    OTC_Condition&	operator=(OTC_Condition const&);
				// Do not define an implementation for this.

    bool		flag_;
				// Boolean flag recording if the condition
				// has been satisfied.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_CONDTION_HH */
