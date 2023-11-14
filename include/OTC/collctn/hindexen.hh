#ifndef OTC_COLLCTN_HINDEXEN_HH
#define OTC_COLLCTN_HINDEXEN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/hindexen.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/link.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/collctn/hashtble.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/hindexen.hh"
#endif
#pragma interface "OTC/collctn/hindexen.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_HIndexEntry : public OTC_Link
    // = TITLE
    //     Entry in index for holding a key and item.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_HIndexEntry> holds the key and item for an entry
    //     in the index implemented by <OTC_HIndex>. This class is a derived
    //     version of <OTC_Link> and allows the entry to contain a key and
    //     item, where the item is stored as a void pointer. The keys are
    //     managed by <OTC_BaseActions>, the void pointers items are not.
    //     Therefore you are responsible for deleting any item held.
    //     
    // = SEE ALSO
    //     <OTC_Link>, <OTC_BaseActions>, <OTC_HIndex>
{
  public:

    // = DESTRUCTION

			~OTC_HIndexEntry();
				// This does not delete the item held. This
				// must be done explicitly by the user of the
				// class if required. Note that you shouldn't
				// call <operator delete()>, <unlink()> or
				// <kill()> explicitly on this class. If
				// you want to remove the entry from the
				// index use <removeEntry()>.

    void		removeEntry();
				// Deletes this entry from the index it is
				// in. You should not use the class after
				// you call this function.

    // = INITIALISATION

			OTC_HIndexEntry(
			 T const& theKey,
			 OTC_HashTable* theIndex,
			 int theHashValue
			);
				// Creates a entry to hold <theKey>. The
				// pointer for the item is initialised
				// to <0>.

    // = NAMING

    T const&		key() const
				{ return key_; }
				// Returns the key.

    // = LINK

    void*		item()
				{ return item_; }
				// Returns the item.

    void const*		item() const
				{ return item_; }
				// Returns the item.

    void		setItem(void* theItem)
				{ item_ = theItem; }
				// Sets the item to <theItem>.

    // = CHAIN
 
    OTC_HIndexEntry<T>* prevEntry()
				{ return prevEntry_(); }
                                // Returns the entry preceeding this entry.
 
    OTC_HIndexEntry<T> const*   prevEntry() const
				{ return prevEntry_(); }
                                // Returns the entry preceeding this entry.
 
    OTC_HIndexEntry<T>* nextEntry()
				{ return nextEntry_(); }
                                // Returns the entry following this entry.
 
    OTC_HIndexEntry<T> const*   nextEntry() const
				{ return nextEntry_(); }
                                // Returns the entry following this entry.

  private:

			OTC_HIndexEntry(OTC_HIndexEntry<T> const&)
				{ terminate(); }
				// Do not define an implementation for this.

    OTC_HIndexEntry<T>&	operator=(OTC_HIndexEntry<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.
 
    OTC_HIndexEntry<T>*	prevEntry_() const;
                                // Returns the entry preceeding this entry.

    OTC_HIndexEntry<T>*	nextEntry_() const;
                                // Returns the entry following this entry.

    T			key_;
				// The key.

    OTC_HashTable*	index_;
				// The index.

    int			hashValue_;
				// The hash value.

    void*		item_;
				// The item.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/hindexen.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HINDEXEN_HH */
