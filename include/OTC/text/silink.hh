#ifndef OTC_COLLCTN_SILINK_HH
#define OTC_COLLCTN_SILINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/silink.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/link.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/silink.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_SILink : public OTC_Link
    // = TITLE
    //     Derived version of <OTC_Link> for holding <OTC_String> and <int>.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_SILink> class is a derived version of the <OTC_Link>
    //     class which supports the holding of a key of type <OTC_String> and
    //     and item of type <int>.
    //
    // = SEE ALSO
    //     <OTC_Link>, <OTC_LinkList>
{
  public:

			~OTC_SILink();

    // = CONSTRUCTION

			OTC_SILink(OTC_String const& theKey, int theItem);
				// Saves <theKey> as the key and <theItem>
				// as the item to be held by this link.

			OTC_SILink(OTC_SILink const& theLink);
				// Saves key/item being held by <theLink> as
				// the key/item to be held by this link.

    // = RETRIEVAL

    OTC_String&		key()
				{ return key_; }
				// Returns the key which is being held
				// by this link.

    int&		item()
				{ return item_; }
				// Returns the item which is being held
				// by this link.

  private:

    OTC_SILink&		operator=(OTC_SILink const&);
				// Do not provide an implementation for this.

    OTC_String		key_;
				// The key.

    int			item_;
				// The item.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_SILINK_HH */
