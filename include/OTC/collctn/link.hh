#ifndef OTC_COLLCTN_LINK_HH
#define OTC_COLLCTN_LINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/link.hh
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

#include <OTC/collctn/linkable.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/link.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Link : public OTC_Linkable
    // = TITLE
    //     Base class for real links in a doubly linked list.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_Link> class is a derived version of <OTC_Linkable>. If
    //     you wish to place any objects in a list of type <OTC_LinkList>,
    //     your class will need to be derived from this class.
    //     
    // = NOTES
    //     All instances of classes derived from <OTC_Link> must be
    //     created on the free store, using <new>.
    //
    // = SEE ALSO
    //     <OTC_Linkable>, <OTC_Anchor>, <OTC_LinkIterator>
{
  protected:

			OTC_Link()
			  : OTC_Linkable(true) {}
				// Creates a link.

    // = DESTRUCTION

			~OTC_Link();
				// Results in <unlink()> being called from
				// the <OTC_Linkable> destructor.

  private:

			OTC_Link(OTC_Link const&);
				// Do not define an implementation
				// for this.

    OTC_Link&		operator=(OTC_Link const&);
				// Do not define an implementation
				// for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_LINK_HH */
