#ifndef OTC_COLLCTN_TUNIQMAP_HH
#define OTC_COLLCTN_TUNIQMAP_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/tuniqmap.hh
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
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/shlwcopy.hh>
#include <OTC/collctn/pritertr.hh>
#include <OTC/collctn/prvisitr.hh>
#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/avllnode.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/tuniqmap.hh"
#endif
#pragma interface "OTC/collctn/tuniqmap.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_TUniqMap
    // = TITLE
    //	   Class implementing a one to one mapping between two objects.
    //	   
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class implements a one to one mapping between two objects.
    //	   Since the map works both ways, both keys and items must be unique
    //	   within there respective indexes.  In addition, both key and item
    //     must be able to be ranked through the use of <OTC_RankActions>.
    //     
    // = NOTES
    //	   When a pointer type is used as either a key or item, and the hash
    //	   value generated from it, is based not on the pointer, but on some
    //	   part of the object being pointed at, it is important that the
    //	   pointer be of type pointer to const object. You should define the
    //	   map as <OTC_TUniqMap\<EX_Foo const*,EX_Bar const*\>> and not just
    //	   <OTC_TUniqMap\<EX_Foo*,EX_Bar*\>>. If this is not done, it would be
    //	   possible for a user to modify parts of the key or item from which
    //	   the hash value is generated. If the hash value for an object is
    //	   changed, the key or item will no longer be accessible.
    //	   
    //     The <OTC_PairBucket> class is used internally to hold the key and
    //     items. Thus the <OTC_BaseActions> class may be used to provide
    //     actions to be performed when key and item are inserted or
    //     removed from the map.
    //     
    //	   Since an attempt to access or remove objects which are not in the
    //	   map will raise an exception, it is important first to check that
    //	   an object is actually in the map using the appropriate
    //	   <contains()> function.
    //	   
    // = SEE ALSO
    //	   <OTC_Iterator>, <OTC_PairBucket>, <OTC_BaseActions>,
    //     <OTC_RankActions>, <OTC_PairIterator>
{
  public:

    virtual		~OTC_TUniqMap();

    // = INITIALISATION

			OTC_TUniqMap(
			 int (*theKeyRankFn)(T1 const&,T1 const&)=0,
			 int (*theItemRankFn)(T2 const&,T2 const&)=0
			);
				// Creates an empty map. <theKeyRankFn>
				// and <theItemRankFn> are rank functions,
				// which if provided will override the
				// <OTC_RankActions> class.

			OTC_TUniqMap(OTC_TUniqMap<T1,T2> const& theMap);
				// Creates a copy of <theMap>.


                        OTC_TUniqMap(
			 OTC_TUniqMap<T1,T2> const& theMap,
			 OTC_ShallowCopy
			);
                                // Creates a map which is a shallow copy
                                // of <theMap>. That is, both reference
                                // the same data with changes in one being
                                // seen in the other.

    // = INSERTION

    OTC_TUniqMap<T1,T2>& operator=(OTC_TUniqMap<T1,T2> const& theMap);
				// Replace the contents of this map with
				// a copy of the contents of <theMap>.
				// Returns a reference to this map.

    void		add(T1 const& theKey, T2 const& theItem);
				// If an item corresponding to <theKey> does
				// not exist in this map and <theItem> does
				// not already exist in the map, then
				// <theItem> will be entered into this map.
				// If an item corresponding to <theKey>
				// already exists, or <theItem> already
				// exists, an exception will be raised.

    void		add(OTC_PairIterator<T1,T2> thePairs);
				// Adds key/items accessable from the current
				// point of <thePairs> to its end, into this
				// map. The original iterator will be
				// left at its end point. If either of the
				// key or item at the time of addition to the
				// map already exists, an exception will be
				// raised.

    // = REMOVAL

    void		removeAll();
				// Removes all key/item pairs in this map.

    void		removeKey(
			 T1 const& theKey,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// If an item corresponding to <theKey>
				// exists in this map, the key/item pair will
				// be removed. If an item corresponding to
				// <theKey> does not exist, an exception will
				// be raised unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		removeItem(
			 T2 const& theItem,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// If a key corresponding to <theItem> exists
				// in this map, the key/item pair will be
				// removed. If a key corresponding to
				// <theItem> does not exist, an exception
				// will be raised unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    // = SEARCH

    bool		containsKey(T1 const& theKey) const;
				// Returns <true> if this map contains an
				// item corresponding to <theKey>.

    bool		containsItem(T2 const& theItem) const;
				// Returns <true> if this map contains a key
				// corresponding to <theItem>.

    // = RETRIEVAL

    T2 const&		item(T1 const& theKey) const;
				// If an item corresponding to <theKey>
				// exists in this map, a reference to
				// the item will be returned. If an item
				// corresponding to <theKey> does not exist,
				// an exception will be raised.

    T2 const*		pItem(T1 const& theKey) const;
				// If an item corresponding to <theKey>
				// exists in this map, a pointer to
				// the item will be returned. If an item
				// corresponding to <theKey> does not exist,
				// a null pointer is returned.

    T1 const&		key(T2 const& theItem) const;
				// If a key corresponding to <theItem>
				// exists in this map, a reference to the key
				// will be returned. If a key corresponding
				// to <theItem> does not exist, an exception
				// will be raised.

    T1 const*		pKey(T2 const& theItem) const;
				// If a key corresponding to <theItem>
				// exists in this map, a pointer to the key
				// will be returned. If a key corresponding
				// to <theItem> does not exist, a null
				// pointer will be returned.

    T1 const&		pickKey() const;
				// Returns a key from the collection. If the
				// map is empty, an exception is raised. The
				// function is not guaranteed to return a
				// different key on each call.

    T2 const&		pickItem() const;
				// Returns a item from the collection. If the
				// map is empty, an exception is raised. The
				// function is not guaranteed to return a
				// different item on each call.

    // = QUERY

    u_int		population() const
				{ return keyIndex_->population(); }
				// Returns the number of key/item mappings
				// contained in this map.

    bool		isEmpty() const
				{ return keyIndex_->population() == 0; }
				// Returns <true> if the collection is empty.

    // = ITERATION
    //     By default, iterators will perform reference counts on the
    //     buckets in the collection as the iterator moves over the items.
    //     Performing the reference counts ensures that the iterator
    //     is not corrupted by additions or removals to the collection.
    //     If an unsafe iterator is required, for example, within the bounds
    //     of a read operation against data protected by a reader/writer
    //     mutex, a second argument can be passed to the following functions.
    //     The value of this argument is either <OTCLIB_SAFE> or
    //     <OTCLIB_UNSAFE>. To get an unsafe iterator, the <OTCLIB_UNSAFE>
    //     argument should be used.
    //     
    //     The first argument to the following functions indicates the
    //     direction of traversal of the iterator. Traversal in the
    //     direction of first to last item is indicated by a value of
    //     <OTCLIB_FORWARD>. Traversal in the reverse direction is
    //     indicated by a value of <OTCLIB_BACKWARD>. The default value
    //     is <OTCLIB_FORWARD>. In the <OTC_TUniqMap> class, traversal in
    //     the forward direction will result in the first key/item being
    //     that for which the key component is least ranked. Moving the
    //     iterator will result in successively higher ranked keys.

    OTC_Iterator<T1>	keys(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over the keys.

    OTC_Iterator<T2>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over the items.

    OTC_PairIterator<T1,T2>	pairs(
				 OTC_Direction theDirection=OTCLIB_FORWARD,
				 OTC_Protection theProtection=OTCLIB_SAFE
				) const;
				// Returns an iterator over the key/item
				// pairs.

    OTC_Iterator<T1>	keys(
			 T1 const& theKey,
			 int (*theRankFn)(T1 const&, T1 const&),
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over keys in the map
				// with rank equal to <theKey> according to
				// <theRankFn>. Note that <theRankFn> must
				// define a partial ordering over the total
				// ordering of the rank function used by the
				// map when inserting keys. That is, the keys
				// which match using <theRankFn> must all be
				// adjacent to each other in the map.
				// <theKey> will always be passed as the
                                // first argument to <theRankFn>.

    OTC_Iterator<T2>	items(
			 T1 const& theKey,
			 int (*theRankFn)(T1 const&, T1 const&),
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
                                // Returns an iterator over items in
				// the map for which the corresponding key
				// ranks equal to <theKey> according to
				// <theRankFn>. Note that <theRankFn> must
				// define a partial ordering over the total
				// ordering of the rank function used by the
				// map when inserting keys. That is, the keys
				// which match using <theRankFn> must all be
				// adjacent to each other in the map.
				// <theKey> will always be passed as the
                                // first argument to <theRankFn>.

    OTC_PairIterator<T1,T2>	pairsKey(
				 T1 const& theKey,
				 int (*theRankFn)(T1 const&, T1 const&),
				 OTC_Direction theDirection=OTCLIB_FORWARD,
				 OTC_Protection theProtection=OTCLIB_SAFE
				) const;
                                // Returns an iterator over key/item pairs in
                                // the map with keys which rank equal to
                                // <theKey> according to <theRankFn>. Note
                                // that <theRankFn> must define a partial
                                // ordering over the total ordering of the
                                // rank function used by the map when
                                // inserting keys. That is, the keys which
                                // match using <theRankFn> must all be
                                // adjacent to each other in the map.
                                // <theKey> will always be passed as the
                                // first argument to <theRankFn>.

  protected:

    // = RANKING

    virtual int		rankKey(T1 const& key1, T1 const& key2) const;
				// Rank two keys.

    virtual int		rankItem(T2 const& item1, T2 const& item2) const;
				// Ranks two items.
  private:

    int                 searchKeyIndex_(
                         OTC_AVLLinkNode* theRoot,
                         T1 const& theKey,
                         OTC_AVLLinkNode*& theNode
                        ) const;
                                // Searches for the node in the tree with
                                // <theRoot> corresponding to <theKey>.
                                // <theNode> will be set the node and zero
                                // returned if the key existed. If no node
                                // corresponding to <theKey> exists, a non
                                // zero value will be returned and <theNode>
                                // will be set to the node either side of
                                // where <theKey> would have been. If the
                                // value returned if less than zero,
                                // <theNode> will be the node following where
                                // <theKey> should have been, otherwise, it
                                // will be the node which preceeded where
                                // <theKey> should have been. The return
				// value will be non zero and <theNode> also
				// zero if the index is empty.

    int                 searchItemIndex_(
                         OTC_AVLLinkNode* theRoot,
                         T2 const& theItem,
                         OTC_AVLLinkNode*& theNode
                        ) const;
                                // Searches for the node in the tree with
                                // <theRoot> corresponding to <theItem>.
                                // <theNode> will be set the node and zero
                                // returned if the key existed. If no node
                                // corresponding to <theItem> exists, a non
                                // zero value will be returned and <theNode>
                                // will be set to the node either side of
                                // where <theItem> would have been. If the
                                // value returned if less than zero,
                                // <theNode> will be the node following where
                                // <theItem> should have been, otherwise, it
                                // will be the node which preceeded where
                                // <theItem> should have been. The return
                                // value will be non zero and <theNode> also
                                // zero if the index is empty.

    OTC_LinkList*	list_;
				// List of key/item pairs.

    OTC_AVLTree*	keyIndex_;
				// Key index table.

    OTC_AVLTree*	itemIndex_;
				// Item index table.

    int			(*keyRankFn_)(T1 const&,T1 const&);
				// Comparison function to be used in
				// place of <OTC_RankActions> for keys.

    int			(*itemRankFn_)(T2 const&,T2 const&);
				// Comparison function to be used in
				// place of <OTC_RankActions> for items.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/tuniqmap.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_TUNIQMAP_HH */
