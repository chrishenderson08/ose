#ifndef OTC_DEBUG_ASRTFAIL_HH
#define OTC_DEBUG_ASRTFAIL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/asrtfail.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_DEBUG_EXCPTION_HH
#include <OTC/debug/excption.hh>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/asrtfail.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCERR_AssertionFailure : public OTC_Exception
    // = TITLE
    //     Exception class to be thrown for a failed assertion.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is to be used, when you want to throw an exception for
    //     an assertion failure. It is this exception which is thrown by
    //     <OTCLIB_ASSERT()> when the condition fails. Typically, you would
    //     not use this class directly but would use <OTCLIB_ASSERT()>.
    //     
    //     If used explicitly this class would be used in the following
    //     way:
    //
    // = BEGIN<CODE>
    //     if (!(somePointer != 0))
    //     {
    //        OTCERR_AssertionFailure exception(
    //         __FILE__,__LINE__,"somePointer != 0"
    //        );
    //        throw exception;
    //     }
    // = END<CODE>
    //
    //     The prefered method of using this class though is to write:
    //
    // = BEGIN<CODE>
    //     OTCLIB_ASSERT(somePointer != 0);
    // = END<CODE>
    //
    //     When <OTCLIB_ASSERT()> is used it will be compiled out of code, if
    //     the symbol <NDEBUG> is defined. Assertions would generally only be
    //     used during development of software. If you are using <makeit>,
    //     the symbol <NDEBUG> is automatically defined, when working in the
    //     <opt> variant.
    //     
    //     If a condition check is required to be permanently in code, ie.,
    //     it is a precondition of the code that the condition always be
    //     true; you should use the <OTCERR_PreconditionFailure> exception
    //     class, or the <OTCLIB_ENSURE()> macro.
    //     
    // = SEE ALSO
    //     <OTC_Exception>, <OTCLIB_ENSURE>, <OTCLIB_ASSERT>
{
  public:

			~OTCERR_AssertionFailure();

    // = INITIALISATION

			OTCERR_AssertionFailure(
			 char const* theFile,
			 u_int theLine,
			 char const* theCondition
			);
				// Creates an exception class with type
				// description <"Assertion Failure">, for the
				// failed condition <theCondition>.
				// <theFile>, should be the name of the file
				// in which the exception is being raised,
				// supplied as <__FILE__>, and <theLine>,
				// should be the line in that file, supplied
				// as <__LINE__>.

			OTCERR_AssertionFailure(
			 OTCERR_AssertionFailure const& theException
			);
				// Uses the description of the exception from
				// <theException>.

    // = QUERY

    char const*		condition() const
				{ return condition_; }
				// Returns the condition that failed, and
				// resulted in the exception being raised.

    // = DISPLAY

    void		display(ostream& outs) const;
				// Dumps a message which composes together
				// all the information about the error on the
				// stream <outs>.

  protected:

    // = THROW

    void		throwIt() const;
				// Throws this class as an exception.

  private:

    OTCERR_AssertionFailure&	operator=(OTCERR_AssertionFailure const&);
				// Do not define an implementation for this.

    char const*		condition_;
				// The condition which failed and which
				// resulted in the exception being raised.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_ASRTFAIL_HH */
