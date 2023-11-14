#ifndef OTC_DEBUG_OUTOFMEM_HH
#define OTC_DEBUG_OUTOFMEM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/outofmem.hh
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
#pragma interface "OTC/debug/outofmem.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCERR_OutOfMemory : public OTC_Exception
    // = TITLE
    //     Exception class to be thrown when out of memory.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is to be used, when you want to throw an exception,
    //     to indicate that there is no more memory. It is this exception
    //     which is thrown by <otclib_new_handler()>.
    //
    // = SEE ALSO
    //     <OTC_Exception>
{
  public:

			~OTCERR_OutOfMemory();

    // = INITIALISATION

			OTCERR_OutOfMemory();
				// Creates an exception with type
				// description <"Out of Memory">.

			OTCERR_OutOfMemory(char const* theFile, u_int theLine);
				// Creates an exception with type
				// description <"Out of Memory">.
				// <theFile> is recorded as the file
				// in which exception was raised and
				// <theLine> the line number in that
				// file.

			OTCERR_OutOfMemory(
			 OTCERR_OutOfMemory const& theException
			);
				// Uses the description of the exception from
				// <theException>.

  protected:

    // = THROW

    void                throwIt() const;
                                // Throws this class as an exception.

  private:

    OTCERR_OutOfMemory&	operator=(OTCERR_OutOfMemory const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_OUTOFMEM_HH */
