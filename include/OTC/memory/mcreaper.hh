#ifndef OTC_MEMORY_MCREAPER_HH
#define OTC_MEMORY_MCREAPER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/mcreaper.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <malloc.h>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_MallocReaper
    // = TITLE
    //     Destroys data allocated using malloc.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_MallocReaper> assists in ensuring that data
    //     allocated from the free store using <malloc()> is deleted when the
    //     stack is unwound as a result of an exception. This is achieved by
    //     using an instance of this class as a handle to the data allocated
    //     using <malloc()>. Once the data is grabbed using an instance of
    //     this class, the section of code in which an exception could be
    //     raised is executed. If an exception does occur then the instance
    //     of this class will be destroyed and the data freed. If no
    //     exception occurs a call can be made to release the data, when the
    //     destructor is finally called the data would not be freed.
    //     
    // = EXAMPLE
    //
    // = BEGIN<CODE>
    //     void function()
    //     {
    //       OTC_MallocReaper xxxData;
    //       char* theData = malloc(2);
    //       OTCLIB_ASSERT_M(theData != 0);
    //       xxxData.grab(theData);
    //
    //       ... code which could throw an exception
    //
    //       xxxData.release();
    //     }
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_Reaper>, <OTC_VecReaper>
{
  public:

    // = INITIALISATION

			OTC_MallocReaper()
			 : data_(0) {}
				// Initialises the handle to <0>.

    // = DESTRUCTION

			~OTC_MallocReaper()
				{ if (data_) free((char*)data_); }
				// If the handle references any data, ie.,
				// it is not <0>, the data will be freed.

    // = GRAB/RELEASE

    void		grab(void* theData)
				{ data_ = theData; }
				// Sets the handle to <theData>. If the
				// handle was set to point at another block
				// of data, it will now point to the new data
				// and the first block of data will not be
				// changed.

    void		release()
				{ data_ = 0; }
                                // Sets the handle to <0>. This is equivalent
                                // to calling <grab(0)>.

  private:

			OTC_MallocReaper(OTC_MallocReaper const&);
				// Do not define an implementation for this.

    OTC_MallocReaper&	operator=(OTC_MallocReaper const&);
				// Do not define an implementation for this.

    void*		data_;
				// Pointer to the data.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_MCREAPER_HH */
