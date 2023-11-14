#ifndef OTC_COLLCTN_IVLINK_HH
#define OTC_COLLCTN_IVLINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ivlink.hh
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
#pragma interface "OTC/collctn/ivlink.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_IVLink : public OTC_Link
    // = TITLE
    //     Derived version of <OTC_Link> for holding <int> and <void*>.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_IVLink> class is a derived version of the <OTC_Link>
    //     class which supports the holding of a key of type <int> and
    //     and item of type <void*>.
    //
    // = SEE ALSO
    //     <OTC_Link>, <OTC_LinkList>
{
  public:

			~OTC_IVLink();

    // = CONSTRUCTION

			OTC_IVLink(int theKey, void* theItem);
				// Saves <theKey> as the key and <theItem>
				// as the item to be held by this link.

			OTC_IVLink(OTC_IVLink const& theLink);
				// Saves key/item being held by <theLink> as
				// the key/item to be held by this link.

    // = RETRIEVAL

    int&		key()
				{ return key_; }
				// Returns the key which is being held
				// by this link.

    void*&		item()
				{ return item_; }
				// Returns the item which is being held
				// by this link.

  private:

    OTC_IVLink&		operator=(OTC_IVLink const&);
				// Do not provide an implementation for this.

    int			key_;
				// The key.

    void*		item_;
				// The item.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_IVLINK_HH */
