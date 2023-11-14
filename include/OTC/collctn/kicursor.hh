#ifndef OTC_COLLCTN_KICURSOR_HH
#define OTC_COLLCTN_KICURSOR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/kicursor.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/linkcurs.hh>
#include <OTC/collctn/prcursor.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/kicursor.hh"
#endif
#pragma interface "OTC/collctn/kicursor.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class L>
class OTC_KeyItemCursor : public OTC_LinkCursor, public OTC_PairCursor<T1,T2>
    // = TITLE
    //     Cursor over buckets containing both a key and an item.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     Derived version of <OTC_Cursor>, which iterates over the key/item
    //     pairs in an instance of <OTC_LinkList>. The type of the class
    //     derived from <OTC_Link> and which are held by the link list must
    //     be supplied as the second template argument. The class derived
    //     from <OTC_Link> must provide functions called <key()> and
    //     <item()>. The <key()> function must return a reference to a
    //     modifiable object of type <T1>. The <item()> function must return
    //     a reference to a modifiable object of type <T2>.
{
  public:

			~OTC_KeyItemCursor();

    // = INITIALISATION

			OTC_KeyItemCursor(
			  OTC_LinkList* aList,
			  OTC_Direction theDirection=OTCLIB_FORWARD,
			  OTC_Protection theProtection=OTCLIB_SAFE
			);
				// <aList> is the list being iterated over.
				// <theDirection> indicates which way
				// traversal is to occur. If <theDirection>
				// is <OTCLIB_FORWARD> then traversal will be
				// from the start <aList> to the end. If it
				// is <OTCLIB_BACKWARD> then it will be in
                                // the opposite direction. <theProtection>
                                // determines whether reference counting is
                                // performed on the links in the list.
                                // Reference counting ensures the integrity of
                                // the iterator when removals and additions
                                // are perfomed. The values for
                                // <theProtection> are <OTCLIB_SAFE> and
                                // <OTCLIB_UNSAFE>. The default argument is
                                // <OTCLIB_UNSAFE>.

			OTC_KeyItemCursor(
			  OTC_LinkList* aList,
			  OTC_Linkable* theStart,
			  OTC_Linkable* theEnd,
			  OTC_Direction theDirection=OTCLIB_FORWARD,
			  OTC_Protection theProtection=OTCLIB_SAFE
			);
				// <aList> is the list being iterated over.
				// <theStart> and <theEnd> are the anchor
				// points within that list which define the
				// bounds of the iterator. If <theDirection>
				// is <OTCLIB_FORWARD> then traversal will be
				// from the <theStart> to <theEnd>. If it
				// is <OTCLIB_BACKWARD> then it will be in
				// the opposite direction. Note that this
				// class relies on <theStart> preceeding
				// <theEnd> in the list. <theProtection>
                                // determines whether reference counting is
                                // performed on the links in the list.
                                // Reference counting ensures the integrity of
                                // the iterator when removals and additions
                                // are perfomed. The values for
                                // <theProtection> are <OTCLIB_SAFE> and
                                // <OTCLIB_UNSAFE>. The default argument is
                                // <OTCLIB_UNSAFE>.

			OTC_KeyItemCursor(
			 OTC_KeyItemCursor<T1,T2,L> const& theIter
			);
				// Create an iterator at the same location as
				// <theIter>.

    // = MOVEMENT
    //     Following operations allow movement of the iterator over the
    //     list.

    void		reset();
				// Resets the iterator to the first item.

    void		next();
				// Moves the iterator to the next item.

    // = RETRIEVAL

    T1&			key();
				// Returns the key under the current
				// location of the iterator. If no such key
				// exists then an exception is generated.
				// Note that this returns the key even
				// if the bucket has been marked as dead.

    T2&			item();
				// Returns the item under the current
				// location of the iterator. If no such item
				// exists then an exception is generated.
				// Note that this returns the item even
				// if the bucket has been marked as dead.

    bool		isValid() const;
				// Returns <true> while there is a link under
				// the current location of the iterator amd
				// not an anchor point. In other words if the
				// iterator is located over a link which has
				// been killed then <true> will still be
				// returned.

    // = CLONING

    OTC_PairCursor<T1,T2>*	clone();
				// Create a new iterator at the same location
				// as this one but which operates
				// independently.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/kicursor.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_KICURSOR_HH */
