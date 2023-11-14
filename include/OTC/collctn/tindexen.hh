#ifndef OTC_COLLCTN_TINDEXEN_HH
#define OTC_COLLCTN_TINDEXEN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/tindexen.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/avlnode.hh>
#include <OTC/collctn/baseactn.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/tindexen.hh"
#endif
#pragma interface "OTC/collctn/tindexen.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_TIndexEntry : public OTC_AVLNode
    // = TITLE
    //     AVL tree node for holding a key and item.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is a derived version of <OTC_AVLNode> and allows
    //     the node to contain a key and item, where the item is stored
    //     as a void pointer. The keys are managed by <OTC_BaseActions>,
    //     the void pointers items are not. Therefore you are responsible
    //     for deleting any item held.
    //
    // = SEE ALSO
    //     <OTC_AVLNode>, <OTC_AVLTree>, <OTC_BaseActions>
{
  public:

    // = DESTRUCTION
 
                        ~OTC_TIndexEntry();
                                // This does not delete the item held. This
                                // must be done explicitly by the user of the
                                // class if required. Note that you shouldn't
                                // call <operator delete()> or <unlink()>
				// explicitly on this class. If you want to
				// remove the entry from the index use
				// <removeEntry()>.

    void                removeEntry();
                                // Deletes this entry from the index it is
                                // in. You should not use the class after
                                // you call this function.

    // = INITIALISATION

			OTC_TIndexEntry(T const& theKey);
				// Creates a node to hold <theKey>. The
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

    OTC_TIndexEntry<T>*	prevEntry()
				{ return (OTC_TIndexEntry<T>*)prev(); }
				// Returns the entry preceeding this entry.

    OTC_TIndexEntry<T> const*	prevEntry() const
				{ return (OTC_TIndexEntry<T> const*)prev(); }
				// Returns the entry preceeding this entry.

    OTC_TIndexEntry<T>*	nextEntry()
				{ return (OTC_TIndexEntry<T>*)next(); }
				// Returns the entry following this entry.

    OTC_TIndexEntry<T> const*	nextEntry() const
				{ return (OTC_TIndexEntry<T> const*)next(); }
				// Returns the entry following this entry.

  private:

			OTC_TIndexEntry(OTC_TIndexEntry<T> const&)
				{ terminate(); }
				// Do not define an implementation for this.

    OTC_TIndexEntry<T>&	operator=(OTC_TIndexEntry<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    T			key_;
				// The key.

    void*		item_;
				// The item.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/tindexen.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_TINDEXEN_HH */
