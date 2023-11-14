#ifndef OTC_COLLCTN_RLINK_HH
#define OTC_COLLCTN_RLINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/rlink.hh
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
#include <OTC/refcnt/resource.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/rlink.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_RLink : public OTC_Link
    // = TITLE
    //     Derived version of <OTC_Link> for holding <OTC_Resource*>.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_RLink> class is a derived version of the <OTC_Link>
    //     class which supports the holding of pointers to types derived
    //     from the <OTC_Resource> class. Virtual inheritance should not
    //     be used when deriving your class from <OTC_Resource> as it will
    //     prevent down casting to the correct type when retrieving the
    //     item. When this class is created it will invoke <reference()>
    //     for the item. When this class is destroyed it will invoke
    //     <unReference()> for the item. Ie., the reference counting
    //     mechanism is applied.
    //
    // = SEE ALSO
    //     <OTC_Link>, <OTC_LinkList>, <OTC_Resource>
{
  public:

			~OTC_RLink();

    // = CONSTRUCTION

			OTC_RLink(OTC_Resource* theItem);
				// Saves <theItem> as the item to be held by
				// this link.

			OTC_RLink(OTC_RLink const& theLink);
				// Saves item being held by <theLink> as
				// the item to be held by this link.

    // = RETRIEVAL

    OTC_Resource*&	item()
				{ return item_; }
				// Returns the item which is being held
				// by this link.

  private:

    OTC_RLink&		operator=(OTC_RLink const&);
				// Do not provide an implementation for this.

    OTC_Resource*	item_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_RLINK_HH */
