#ifndef OTC_TEXT_SOBJECT_HH
#define OTC_TEXT_SOBJECT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/sobject.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/rstring.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/sobject.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_SObject
    // = TITLE
    //     Abstract base class for string type objects.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_SObject> class serves as a base class for string type
    //     objects which should not have the full <OTC_String> class
    //     interface visible to users. This class makes the conversion
    //     to an instance of <OTC_String> automatic without the derived
    //     class having to provide a conversion operator to an instance
    //     of <OTC_String>.
    //
    // = SEE ALSO
    //     <OTC_RString>, <OTC_String>
{
    friend class	OTC_String;

  protected:

			OTC_SObject();

    virtual		~OTC_SObject();

    // = QUERY

    virtual OTC_RString	rawString() const = 0;
				// Must be redefined in a derived class
				// to return a raw string representation
				// of the derived class.

  private:

			OTC_SObject(OTC_SObject const&);
				// Do not define an implementation for this.

    OTC_SObject&	operator=(OTC_SObject const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_SOBJECT_HH */
