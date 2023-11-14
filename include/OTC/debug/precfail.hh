#ifndef OTC_DEBUG_PRECFAIL_HH
#define OTC_DEBUG_PRECFAIL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/precfail.hh
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
#pragma interface "OTC/debug/precfail.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCERR_PreconditionFailure : public OTC_Exception
    // = TITLE
    //     Exception class to be thrown for a failed precondition.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is to be used when wishing to throw an exception for a
    //     precondition failure. It is this exception which is thrown by
    //     <OTCLIB_ENSURE()> when the condition fails. Typically, you would
    //     not use this class directly, but would use <OTCLIB_ENSURE()>.
    //     
    //     If used explicitly this class would be used in the following
    //     way:
    //
    // = BEGIN<CODE>
    //     if (!(somePointer != 0))
    //     {
    //        OTCERR_PreconditionFailure exception(
    //         __FILE__,__LINE__,"somePointer != 0","some description"
    //        );
    //        throw exception;
    //     }
    // = END<CODE>
    //
    //     The preferred method of using this class, is to write:
    //
    // = BEGIN<CODE>
    //     OTCLIB_ENSURE((somePointer != 0),"some description");
    // = END<CODE>
    //
    //     <OTCLIB_ENSURE()> cannot be compiled out of code. If a condition
    //     check is only required during development the <OTCLIB_ASSERT()>
    //     macro, which can be compiled out of code by defining <NDEBUG>
    //     should be used.
    //     
    // = SEE ALSO
    //     <OTC_Exception>, <OTCLIB_ASSERT>, <OTCLIB_ENSURE>
{
  public:

			~OTCERR_PreconditionFailure();

    // = INITIALISATION

			OTCERR_PreconditionFailure(
			 char const* theFile,
			 u_int theLine,
			 char const* theCondition,
			 char const* theDescription,
			 char const* theFunction=0
			);
				// Creates an exception class with type
				// description <"Precondition Failure">, for
				// the failed condition <theCondition>.
				// <theFile> should be the name of the file;
				// supplied as <__FILE__>, and <theLine>
				// should be the line in the file; supplied
				// as <__LINE__>. <theDescription> should be
				// an English description of the failure
				// which has occurred. <theFunction> should
				// be the name of the function in which
				// the error occurred. If <theFunction>
				// is <0>, not function will be dumped out
				// in the description.

			OTCERR_PreconditionFailure(
			 OTCERR_PreconditionFailure const& theException
			);
				// Uses the description of the exception from
				// <theException>.

    // = QUERY

    char const*		condition() const
				{ return condition_; }
				// Returns the condition that failed and
				// which resulted in the exception being
				// raised.

    char const*		description() const
				{ return description_; }
				// Returns the English description of the
				// failure.

    char const*		function() const
				{ return function_; }
				// Returns the function in which the
				// failure occurred.

    // = DISPLAY

    void		display(ostream& outs) const;
				// Dumps a message which composes together
				// all the information about the error on the
				// stream <outs>.

  protected:

    // = THROW

    void                throwIt() const;
                                // Throws this class as an exception.

  private:

    OTCERR_PreconditionFailure&	operator=(OTCERR_PreconditionFailure const&);
				// Do not define an implementation for this.

    char const*		condition_;
				// The condition which failed and which
				// resulted in the exception being raised.

    char const*		description_;
				// The English description of the failure.

    char const*		function_;
				// The function in which the failure occurred.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_PRECFAIL_HH */
