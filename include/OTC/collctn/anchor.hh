#ifndef OTC_COLLCTN_ANCHOR_HH
#define OTC_COLLCTN_ANCHOR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/anchor.hh
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

#include <OTC/collctn/linkable.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/anchor.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Anchor : public OTC_Linkable
    // = TITLE
    //     Anchor point for objects in a linked list.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_Anchor> is special derived version of the class
    //     <OTC_Linkable>. The class is used as the anchor point on which to
    //     hang linked lists of objects. The class, when used in pairs, also
    //     serves the purpose of marking the boundaries of the range of an
    //     iterator in a linked list.
    //     
    // = NOTES
    //     Instances of <OTC_Anchor> must always be created on the
    //     free store using <new>.
    //     
    // = SEE ALSO
    //     <OTC_Linkable>, <OTC_Link>, <OTC_LinkIterator>
{
  public:

    // = CONSTRUCTION

			OTC_Anchor()
			 : OTC_Linkable(false) {}
				// Creates an anchor point.

    // = DESTRUCTION

			~OTC_Anchor();
				// Results in the invokation of <unlink()>
				// from the <OTC_Linkable> destructor.

  private:

			OTC_Anchor(OTC_Anchor const&);
				// Do not define an implementation for this.

    OTC_Anchor&		operator=(OTC_Anchor const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ANCHOR_HH */
