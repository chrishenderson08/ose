#ifndef OTC_COLLCTN_SINGLE_HH
#define OTC_COLLCTN_SINGLE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/single.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/mpobject.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/single.hh"
#pragma interface "OTC/collctn/single.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Single : public OTC_MPObject
    // = TITLE
    //     Template class for encapsulating a single value.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_Single> class can be used to encapsulate a single item.
    //     This class is generally only useful where you want to ensure
    //     that an object is allocated from the common memory pool rather
    //     than the normal free store. Multi valued variants of this class
    //     are available in the form of the <OTC_Pair> and <OTC_Triple>
    //     classes.
    //
    // = NOTES
    //     Naming of member functions is to be consistant with multi
    //     valued variants of this class.
    //
    // = SEE ALSO
    //     <OTC_Pair>, <OTC_Triple>
{
  public:

    // = INITIALISATION

			OTC_Single(T const& theFirst)
			  : first_(theFirst)
				{}
				// Sets the values of this class to <theFirst>.

			OTC_Single(OTC_Single<T> const& theSingle)
			  : first_(theSingle.first_)
				{}
				// Sets the value of this class to that
				// of <theSingle>.

    // = ASSIGNMENT

    OTC_Single<T>&	operator=(OTC_Single<T> const& theSingle)
				{
				  first_ = theSingle.first_;
				  return *this;
				}
				// Sets the value of this class to that
				// of <theSingle>.

    // = ACCESS

    T const&		first() const
				{ return first_; }
				// Returns the value of this class.

    T&			first()
				{ return first_; }
				// Returns the value of this class.

  private:

    T			first_;
				// Value held by this class.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_SINGLE_HH */
