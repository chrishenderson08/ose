#ifndef OTC_COLLCTN_HOLDER_HH
#define OTC_COLLCTN_HOLDER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/holder.hh
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

#include <OTC/collctn/link.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/holder.hh"
#endif
#pragma interface "OTC/collctn/holder.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Holder : public OTC_Link
    // = TITLE
    //	   Holder to hold an instance of type <T>.
    //	   
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class is a holder for an item of type <T> within a linked
    //	   list. Note that when parameterised over a pointer type, the object
    //	   held is not deleted when the holder is destroyed.
    //	   
    // = SEE ALSO
    //	   <OTC_Link>, <OTC_Bucket>
{
  public:

    // = INITIALISATION

			OTC_Holder(T const& theItem);
				// Creates a bucket containing <theItem>.

			OTC_Holder(OTC_Holder<T> const& theHolder);
				// Creates a bucket containing the item
				// held by <theHolder>.

    // = DESTRUCTION

			~OTC_Holder();

    // = RETRIEVAL

    T&			item()
				{ return item_; }
				// Returns the item being held by this
				// bucket.

  private:

    OTC_Holder<T>&	operator=(OTC_Holder<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    T			item_;
				// The item being contained in this bucket.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/holder.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HOLDER_HH */
