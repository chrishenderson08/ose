#ifndef OTC_COLLCTN_SVLINK_HH
#define OTC_COLLCTN_SVLINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/svlink.hh
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
#pragma interface "OTC/text/svlink.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_SVLink : public OTC_Link
    // = TITLE
    //     Derived version of <OTC_Link> for holding <OTC_String> and <void*>.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_SVLink> class is a derived version of the <OTC_Link>
    //     class which supports the holding of a key of type <OTC_String> and
    //     and item of type <void*>.
    //
    // = SEE ALSO
    //     <OTC_Link>, <OTC_LinkList>
{
  public:

			~OTC_SVLink();

    // = CONSTRUCTION

			OTC_SVLink(OTC_String const& theKey, void* theItem);
				// Saves <theKey> as the key and <theItem>
				// as the item to be held by this link.

			OTC_SVLink(OTC_SVLink const& theLink);
				// Saves key/item being held by <theLink> as
				// the key/item to be held by this link.

    // = RETRIEVAL

    OTC_String&		key()
				{ return key_; }
				// Returns the key which is being held
				// by this link.

    void*&		item()
				{ return item_; }
				// Returns the item which is being held
				// by this link.

  private:

    OTC_SVLink&		operator=(OTC_SVLink const&);
				// Do not provide an implementation for this.

    OTC_String		key_;
				// The key.

    void*		item_;
				// The item.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_SVLINK_HH */
