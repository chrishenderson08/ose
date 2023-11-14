#ifndef OTC_TEXT_SLINK_HH
#define OTC_TEXT_SLINK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/slink.hh
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
#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/slink.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_SLink : public OTC_Link
    // = TITLE
    //     Derived version of <OTC_Link> for holding <OTC_String>.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_SLink> class is a derived version of the <OTC_Link> class
    //     which supports the holding of a value of type <OTC_String>.
    //     
    // = SEE ALSO
    //     <OTC_Link>, <OTC_LinkList>
{
  public:

			~OTC_SLink();

    // = CONSTRUCTION

			OTC_SLink(OTC_String const& theItem);
				// Saves <theItem> as the item to be held by
				// this link.

			OTC_SLink(OTC_SLink const& theLink);
				// Saves item being held by <theLink> as
				// the item to be held by this link.

    // = RETRIEVAL

    OTC_String const&	item()
				{ return item_; }
				// Returns the item which is being held
				// by this link.

  private:

    OTC_SLink&		operator=(OTC_SLink const&);
				// Do not provide an implementation for this.

    OTC_String		item_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_SLINK_HH */
