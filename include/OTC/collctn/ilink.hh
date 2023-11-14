#ifndef OTC_COLLCTN_ILINK_HH
#define OTC_COLLCTN_ILINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ilink.hh
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
#pragma interface "OTC/collctn/ilink.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_ILink : public OTC_Link
    // = TITLE
    //     Derived version of <OTC_Link> for holding <int>.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_ILink> class is a derived version of the <OTC_Link>
    //     class which supports the holding of a value of type <int>.
    //
    // = SEE ALSO
    //     <OTC_Link>, <OTC_LinkList>
{
  public:

			~OTC_ILink();

    // = CONSTRUCTION

			OTC_ILink(int theItem);
				// Saves <theItem> as the item to be held by
				// this link.

			OTC_ILink(OTC_ILink const& theLink);
				// Saves item being held by <theLink> as
				// the item to be held by this link.

    // = RETRIEVAL

    int&		item()
				{ return item_; }
				// Returns the item which is being held
				// by this link.

  private:

    OTC_ILink&		operator=(OTC_ILink const&);
				// Do not provide an implementation for this.

    int			item_;
				//  The item.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ILINK_HH */
