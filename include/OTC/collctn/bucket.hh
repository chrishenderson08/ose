#ifndef OTC_COLLCTN_BUCKET_HH
#define OTC_COLLCTN_BUCKET_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/bucket.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/link.hh>
#include <OTC/collctn/baseactn.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/bucket.hh"
#endif
#pragma interface "OTC/collctn/bucket.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Bucket : public OTC_Link
    // = TITLE
    //	   Bucket to hold an instance of type <T>.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class is a holder for an item of type <T> within a linked
    //	   list. In order to manage resources, when parameterised over
    //	   a pointer type, it is possible to define an explicit version
    //	   of <OTC_BaseActions>. This allows specific actions to be performed
    //	   at the point that an object is added into the bucket, and when the
    //	   bucket is deleted.
    //	   
    // = SEE ALSO
    //	   <OTC_Link>, <OTC_Linkable>, <OTC_Anchor>, <OTC_BaseActions>
{
  public:

    // = INITIALISATION

			OTC_Bucket(T const& theItem);
				// Creates a bucket containing <theItem>.
				// Invokes <OTC_BaseActions\<T\>::add()>
				// on the item, at the point of placing
				// it into the bucket.

			OTC_Bucket(OTC_Bucket<T> const& theBucket);
				// Initialise this bucket from contents of
				// <theBucket>. Invokes the function
				// <OTC_BaseActions\<T\>::add()>
				// on the item, at the point of placing
				// it into the bucket.

    // = DESTRUCTION

			~OTC_Bucket();
				// Invokes <OTC_BaseActions\<T\>::remove()>
				// on the item in the bucket.

    // = RETRIEVAL

    T&			item()
				{ return item_; }
				// Returns the item being held by this
				// bucket.

  private:

    OTC_Bucket<T>&	operator=(OTC_Bucket<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation
				// for this.

    T			item_;
				// The item being contained in this bucket.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/bucket.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_BUCKET_HH */
