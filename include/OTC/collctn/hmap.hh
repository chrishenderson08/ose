#ifndef OTC_COLLCTN_HMAP_HH
#define OTC_COLLCTN_HMAP_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/hmap.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/modifier.hh>
#include <OTC/collctn/hashactn.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/pritertr.hh>
#include <OTC/collctn/prmodifr.hh>
#include <OTC/collctn/shlwcopy.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/hashtble.hh>
#include <OTC/collctn/replcmnt.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/hmap.hh"
#endif
#pragma interface "OTC/collctn/hmap.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_HMap
    // = TITLE
    //	   Class implementing a one way map between two objects.
    //	   
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class implements a one way mapping between two objects. The
    //	   first of these objects, the key, must be unique and must be able
    //	   to have a hash value generated for it using the <OTC_HashActions>
    //	   class. It must also be possible to compare the keys. The method
    //	   of comparing keys can be determined by providing a template
    //	   override version of <OTC_RankActions> for the type of the key.
    //	   
    // = NOTES
    //	   When a pointer type is used as a key and the hash value generated
    //	   from it, is based not on the pointer, but on some part of the
    //	   object being pointed at, then it is important that the pointer be
    //	   of type pointer to const object. In other words you should define
    //	   the map as <OTC_HMap\<EX_Foo const*,EX_Bar*\>> and not just
    //	   <OTC_HMap\<EX_Foo*,EX_Bar*\>>. If this is not done, it would be
    //	   possible for a user to modify parts of the key from which the hash
    //	   value is generated. If the hash value of an object changes, it
    //	   will no longer be accessible.
    //	   
    //     The <OTC_PairBucket> class is used internally to hold both key and
    //     items. Thus the <OTC_BaseActions> class may be used to provide
    //     actions to be performed, when items are inserted or removed from
    //     the map.
    //     
    //	   Since an attempt to access or remove an item from the map, which
    //	   isn't there, will raise an exception, it is important to first
    //	   check that an item is actually in the map by calling the
    //	   <contains()> function.
    //	   
    // = SEE ALSO
    //	   <OTC_Iterator>, <OTC_Modifier>, <OTC_HashActions>,
    //     <OTC_PairBucket>, <OTC_BaseActions>, <OTC_RankActions>,
    //     <OTC_PairIterator>, <OTC_PairModifier>
{
  public:

    virtual		~OTC_HMap();

    // = INITIALISATION

			OTC_HMap(int (*theRankFn)(T1 const&,T1 const&)=0);
				// Creates an empty map. <theRankFn> is an
				// optional comparison function to be used in
				// place of <OTC_RankActions> for the key.

			OTC_HMap(OTC_HMap<T1,T2> const& theMap);
				// Creates a copy of <theMap>.

			OTC_HMap(
			 OTC_HMap<T1,T2> const& theMap,
			 OTC_ShallowCopy
			);
                                // Creates a map which is a shallow copy
                                // of <theMap>. That is, both reference
                                // the same data with changes in one being
                                // seen in the other.

    // = INSERTION

    OTC_HMap<T1,T2>&	operator=(OTC_HMap<T1,T2> const& theMap);
				// Replaces the contents of this map with
				// the contents of <theMap>.

    void		add(
			 T1 const& theKey,
			 T2 const& theItem,
			 OTC_Replacement theFlag=OTCLIB_ADDONLY
			);
				// If an item corresponding to <theKey> does
				// not exist in this map, then <theItem> will
				// be entered into this map. If an item
				// corresponding to <theKey> already exists,
				// an exception will be raised if <theFlag>
				// is <OTCLIB_ADDONLY>, but if <theFlag> is
				// <OTCLIB_REPLACE>, <theKey> and <theItem>
				// will replace those already in the map.

    void		add(
    			 OTC_PairIterator<T1,T2> thePairs,
			 OTC_Replacement theFlag=OTCLIB_ADDONLY
			);
				// Adds key/items accessable from the current
				// point of <thePairs> to its end, into this
				// map. The original iterator will be
				// left at its end point. If a key already
				// exists, an exception will be raised if
				// <theFlag> is <OTCLIB_ADDONLY>, but if
				// <theFlag> is <OTCLIB_REPLACE>, the new key
				// and item will replace those already in the
				// map.

    // = REMOVAL

    void		removeAll();
				// Removes all key/item pairs in this map.

    void		remove(
			 T1 const& theKey,
			 OTC_Protection theFlag=OTCLIB_SAFE
			);
				// If an item corresponding to <theKey>
				// exists in this map, it will be removed
				// and deleted. If an item corresponding to
				// <theKey> does not exist, an exception
				// will be raised, unless <theFlag>
				// is set to <OTCLIB_UNSAFE>.

    // = SEARCH

    bool		contains(T1 const& theKey) const;
				// Returns <true> if this map contains a item
				// corresponding to <theKey>.

    // = RETRIEVAL

    T2&			item(T1 const& theKey)
				{ return item_(theKey); }
				// If an item corresponding to <theKey>
				// exists in this map, a reference to it will
				// be returned. If an item corresponding to
				// <theKey> does not exist, an exception will
				// be raised.

    T2 const&		item(T1 const& theKey) const
				{ return item_(theKey); }
				// If an item corresponding to <theKey>
				// exists in this map, a reference to it will
				// be returned. If an item corresponding to
				// <theKey> does not exist, an exception will
				// be raised.

    T2*			pItem(T1 const& theKey)
				{ return pItem_(theKey); }
				// If an item corresponding to <theKey>
				// exists in this map, a pointer to it will
				// be returned. If an item corresponding to
				// <theKey> does not exist, a null pointer
				// will be returned.

    T2 const*		pItem(T1 const& theKey) const
				{ return pItem_(theKey); }
				// If an item corresponding to <theKey>
				// exists in this map, a pointer to it will
				// be returned. If an item corresponding to
				// <theKey> does not exist, a null pointer
				// will be returned.

    T1 const&		pickKey() const;
				// Returns a key from the collection. If
				// the map is empty, an exception is raised.
				// The function is not guaranteed to
				// return a different key on each call.

    T2 const&		pickItem() const;
				// Returns an item from the collection. If
				// the map is empty, an exception is raised.
				// The function is not guaranteed to
				// return a different item on each call.

    // = QUERY

    u_int		population() const
				{ return index_->population(); }
				// Returns the number of key/item mappings
				// contained in this map.

    bool		isEmpty() const
				{ return index_->population() == 0; }
				// Returns <true> if the collection is empty.

    // = ITERATION
    //	   By default, iterators will perform reference counts on the
    //	   buckets in the collection as the iterator moves over the items.
    //	   Performing the reference counts ensures that the iterator
    //	   is not corrupted by additions or removals to the collection.
    //     If an unsafe iterator is required, for example, within the bounds
    //     of a read operation against data protected by a reader/writer
    //     mutex, a second argument can be passed to the following functions.
    //     The value of this argument is either <OTCLIB_SAFE> or
    //     <OTCLIB_UNSAFE>. To get an unsafe iterator, the <OTCLIB_UNSAFE>
    //     argument should be used.
    //     
    //	   The first argument to the following functions indicates the
    //	   direction of traversal of the iterator. Traversal in the
    //	   direction of first to last item is indicated by a value of
    //	   <OTCLIB_FORWARD>. Traversal in the reverse direction is
    //	   indicated by a value of <OTCLIB_BACKWARD>. The default value
    //	   is <OTCLIB_FORWARD>. In the <OTC_HMap> class, traversal in
    //	   the forward direction will result in the first key/item being
    //	   accessible being the oldest key/item in the set. Moving the
    //	   iterator will result in successively newer key/items in the set
    //	   being accessible.
    //
    //	   When iterating over items, in the order in which they were
    //	   inserted, be very careful about inserting new items into the set.
    //	   This is because any new items inserted, will also be seen by the
    //	   iterator. If you were inserting a new item for every item seen,
    //	   you would end up with an infinite loop.

    OTC_Iterator<T1>	keys(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over the keys.

    OTC_Iterator<T2>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{ return items_(theDirection,theProtection); }
				// Returns an iterator over the items.

    OTC_Modifier<T2>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{ return items_(theDirection,theProtection); }
				// Returns an iterator over the items.

    OTC_PairIterator<T1,T2>	pairs(
				 OTC_Direction theDirection=OTCLIB_FORWARD,
				 OTC_Protection theProtection=OTCLIB_SAFE
				) const
				{ return pairs_(theDirection,theProtection); }
				// Returns an iterator over the key/item
				// pairs in this map.

    OTC_PairModifier<T1,T2>	pairs(
				 OTC_Direction theDirection=OTCLIB_FORWARD,
				 OTC_Protection theProtection=OTCLIB_SAFE
				)
				{ return pairs_(theDirection,theProtection); }
				// Returns an iterator over the key/item
				// pairs in this map.

  protected:

    // = RANKING

    virtual int		rank(T1 const& key1, T1 const& key2) const;
				// Ranks two items.
  private:

    T2&			item_(T1 const& theKey) const;
				// If an item corresponding to <theKey>
				// exists in this map, a reference to
				// it will be returned. If an item
				// corresponding to <theKey> does not exist,
				// an exception will be raised.

    T2*			pItem_(T1 const& theKey) const;
				// If an item corresponding to <theKey>
				// exists in this map, a pointer to
				// it will be returned. If an item
				// corresponding to <theKey> does not exist,
				// a null pointer is returned.

    OTC_Cursor<T2>*	items_(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns a cursor over the items.

    OTC_PairCursor<T1,T2>*	pairs_(
				 OTC_Direction theDirection=OTCLIB_FORWARD,
				 OTC_Protection theProtection=OTCLIB_SAFE
				) const;
				// Returns a cursor over the key/item
				// pairs in this map.

    OTC_HashTable*      index_;
                                // Hash table for index.

    OTC_LinkList*       list_;
                                // List of items.

    int			(*rankFn_)(T1 const&,T1 const&);
				// Comparison function to be used in
				// place of <OTC_RankActions> for key.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/hmap.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HMAP_HH */
