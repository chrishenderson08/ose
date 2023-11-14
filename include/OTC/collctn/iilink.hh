#ifndef OTC_COLLCTN_IILINK_HH
#define OTC_COLLCTN_IILINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/iilink.hh
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
#pragma interface "OTC/collctn/iilink.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_IILink : public OTC_Link
    // = TITLE
    //     Derived version of <OTC_Link> for holding two <int> values.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_IILink> class is a derived version of the <OTC_Link>
    //     class which supports the holding two values of type <int>.
    //     They two values are referred to as the key and item.
    //
    // = SEE ALSO
    //     <OTC_Link>, <OTC_LinkList>
{
  public:

			~OTC_IILink();

    // = CONSTRUCTION

			OTC_IILink(int theKey, int theItem);
				// Saves <theKey> as the key and <theItem>
				// as the item to be held by this link.

			OTC_IILink(OTC_IILink const& theLink);
				// Saves key/item being held by <theLink> as
				// the key/item to be held by this link.

    // = RETRIEVAL

    int&		key()
				{ return key_; }
				// Returns the key which is being held
				// by this link.

    int&		item()
				{ return item_; }
				// Returns the item which is being held
				// by this link.

  private:

    OTC_IILink&		operator=(OTC_IILink const&);
				// Do not provide an implementation for this.

    int			key_;
				// The key.

    int			item_;
				// The item.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_IILINK_HH */
