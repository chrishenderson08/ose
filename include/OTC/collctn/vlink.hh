#ifndef OTC_COLLCTN_VLINK_HH
#define OTC_COLLCTN_VLINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/vlink.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/link.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/vlink.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_VLink : public OTC_Link
    // = TITLE
    //     Derived version of <OTC_Link> for holding <void*>.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_VLink> class is a derived version of the <OTC_Link>
    //     class which supports the holding of pointers of type <void*>.
    //     No resource management is performed on any objects being
    //     pointed at. If objects need to be deleted, you will need to
    //     cast the <void*> to the correct type and delete it yourself.
    //
    // = SEE ALSO
    //     <OTC_Link>, <OTC_LinkList>
{
  public:

			~OTC_VLink();

    // = CONSTRUCTION

			OTC_VLink(void* theItem);
				// Saves <theItem> as the item to be held by
				// this link.

			OTC_VLink(OTC_VLink const& theLink);
				// Saves item being held by <theLink> as
				// the item to be held by this link.

    // = RETRIEVAL

    void*&		item()
				{ return item_; }
				// Returns the item which is being held
				// by this link.

  private:

    OTC_VLink&		operator=(OTC_VLink const&);
				// Do not provide an implementation for this.

    void*		item_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_VLINK_HH */
