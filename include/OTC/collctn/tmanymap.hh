#ifndef OTC_COLLCTN_TMANYMAP_HH
#define OTC_COLLCTN_TMANYMAP_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/tmanymap.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/shlwcopy.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/pritertr.hh>
#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/avltnode.hh>
#include <OTC/collctn/avllnode.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/refcnt/rcount.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/tmanymap.hh"
#endif
#pragma interface "OTC/collctn/tmanymap.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_TManyMap
    // = TITLE
    //     Class to maintain a one to many relationship.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_TManyMap> is used for holding keys which may reference
    //     one or more items. It must be possible to compare both keys
    //     and items using <OTC_RankActions>. This is irrespective of
    //     whether a rank function is provided in the constructor.
    //     
    // = NOTES
    //     The map is not bidirectional. You can only get all items
    //     coresponding to a key, you cannot do the reverse and get all keys
    //     which may reference a particular item. When iterating over the
    //     key/item pairs in the forward direction, they will be visiting
    //     in ascending order based on supplied rank function.
    //
    // = SEE ALSO
    //     <OTC_Iterator>, <OTC_PairBucket>, <OTC_BaseActions>,
    //     <OTC_RankActions>, <OTC_PairIterator>
{
  public:

    virtual		~OTC_TManyMap();

    // = INITIALISATION

			OTC_TManyMap(
                         int (*theKeyRankFn)(T1 const&,T1 const&)=0,
                         int (*theItemRankFn)(T2 const&,T2 const&)=0
			);
				// Creates an empty map. <theKeyRankFn>
                                // and <theItemRankFn> are rank functions,
                                // which if provided will override the
                                // <OTC_RankActions> class.

			OTC_TManyMap(OTC_TManyMap<T1,T2> const& theMap);
				// Creates a map which is a copy
				// of <theMap>.

			OTC_TManyMap(
			 OTC_TManyMap<T1,T2> const& theMap,
			 OTC_ShallowCopy
			);
				// Creates a map which is a shallow copy
				// of <theMap>. That is, both reference
				// the same data with changes in one being
				// seen in the other.

    // = QUERY

    u_int		population() const
				{ return count_->count(); }
				// Returns the number of key/item pairs in
				// the map. Note that this is not simply the
				// number of unique keys.

    bool		isEmpty() const
				{ return count_->count() == 0; }
				// Returns <true> if the map is empty.

    u_int		populationKey() const
				{ return index_->population(); }
				// Returns the number of unique keys in the
				// map.

    u_int		frequencyKey(T1 const& theKey) const;
				// Returns the number of items in the map for
				// <theKey>.

    // = SEARCH

    bool		containsKey(T1 const& theKey) const;
				// Returns <TRUE> if the map contains
				// items for <theKey>.

    bool		contains(T1 const& theKey, T2 const& theItem) const;
				// Returns <TRUE> if the map contains
				// the key/item pair described by <theKey>
				// and <theItem>.

    // = ASSIGNMENT

    OTC_TManyMap<T1,T2>&	operator=(OTC_TManyMap<T1,T2> const& theMap);
				// Replaces the contents of this
				// map with the contents of <theMap>.

    // = INSERTION

    void		add(T1 const& theKey, T2 const& theItem);
				// Adds the key/item pair into
				// the map. Note that duplicates of the
				// key are allowed, however if there
				// already exists an item for a particular
				// key then an exception is generated.
				// Use <contains()> for occurence of
				// a pair before insertion.

    void		add(OTC_PairIterator<T1,T2> thePairs);
				// Adds key/items accessable from the current
				// point of <thePairs> to its end, into this
				// map. The original iterator will be
				// left at its end point. Note that
				// duplicates of a key are allowed, however
				// if there already exists an item for a
				// particular key then an exception is
				// generated.

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
    //     The first optional argument to the following functions indicates
    //     the direction of traversal of the iterator. Traversal in the
    //     direction of first to last item is indicated by a value of
    //     <OTCLIB_FORWARD>. Traversal in the reverse direction is
    //     indicated by a value of <OTCLIB_BACKWARD>. The default value
    //     is <OTCLIB_FORWARD>. In the <OTC_TManyMap> class, traversal in
    //     the forward direction will result in the first key/item being
    //     that for which the key component is least ranked. Moving the
    //     iterator will result in successively higher ranked keys.

    OTC_Iterator<T2>	items(
			 T1 const& theKey,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{
				  return items_(theKey,0,theDirection,
				   theProtection);
				}
				// Returns an iterator over items
				// corresponding to <theKey>. If <theKey> is
				// not in the map, a null iterator is
				// returned. <theForward> specifies the
				// direction of the iterator. The default
				// direction is forward, that is
				// least ranked item first.

    OTC_PairIterator<T1,T2>	pairsKey(
			 T1 const& theKey,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{
				  return pairsKey_(theKey,0,theDirection,
				   theProtection);
				}
				// Returns an iterator over key/item pairs
				// corresponding to <theKey>. If <theKey> is
				// not in the map, a null iterator is
				// returned. <theForward> specifies the
				// direction of the iterator. The default
				// direction is forward, that is
				// least ranked item first.

    OTC_PairIterator<T1,T2>	pairs(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over all key/item
				// pairs. <theForward> specifies the
				// direction of the iterator. The default
				// direction is forward, that is least ranked
				// key first.

    OTC_Iterator<T2>	items(
			 T1 const& theKey,
			 int (*theRankFn)(T1 const&, T1 const&),
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{
				  return items_(theKey,theRankFn,
				   theDirection,theProtection);
				}
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

    OTC_PairIterator<T1,T2>	pairsKey(
			 T1 const& theKey,
			 int (*theRankFn)(T1 const&, T1 const&),
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{
				  return pairsKey_(theKey,theRankFn,
				   theDirection,theProtection);
				}
				// Returns an iterator over key/item pairs in
				// the map where the key has rank equal to
				// <theKey> according to <theRankFn>. Note
				// that <theRankFn> must define a partial
				// ordering over the total ordering of the
				// rank function used by the map when
				// inserting keys. That is, the keys which
				// match using <theRankFn> must all be
				// adjacent to each other in the map.
				// <theKey> will always be passed as the
                                // first argument to <theRankFn>.

    // = PICK

    T1 const&		pickKey() const;
				// Returns a key from the collection. If the
				// map is empty, an exception is raised. The
				// function is not guaranteed to return a
				// different key on each call.

    // = DESTRUCTION

    void		removeAll();
				// Remove all key/item pairs in the map.

    void		removeKey(
			 T1 const& theKey,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Removes all items corresponding to
				// <theKey>. If <theKey> is not in the
				// map then an exception is generated,
				// unless <theProtection> has been set
				// to <OTCLIB_UNSAFE>.

    void		remove(
			 T1 const& theKey,
			 T2 const& theItem,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Removes the key/item pair described
				// by <theKey> and <theItem>. If
				// no such key/item pair exists then an
				// exception is generated, unless
				// <theProtection> has been set to
				// <OTCLIB_UNSAFE>.

  protected:

    // = RANKING
 
    virtual int         rankKey(T1 const& key1, T1 const& key2) const;
                                // Rank two keys.
 
    virtual int         rankItem(T2 const& item1, T2 const& item2) const;
                                // Ranks two items.

  private:

    OTC_Cursor<T2>*	items_(
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
                                // first argument to <theRankFn>. If
                                // <theRankFn> is <0>, the internal rank
                                // function is used.

    OTC_PairCursor<T1,T2>*	pairsKey_(
			 T1 const& theKey,
			 int (*theRankFn)(T1 const&, T1 const&),
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over key/item pairs in
				// the map where the key has rank equal to
				// <theKey> according to <theRankFn>. Note
				// that <theRankFn> must define a partial
				// ordering over the total ordering of the
				// rank function used by the map when
				// inserting keys. That is, the keys which
				// match using <theRankFn> must all be
				// adjacent to each other in the map.
				// <theKey> will always be passed as the
                                // first argument to <theRankFn>. If
                                // <theRankFn> is <0>, the internal rank
                                // function is used.

    OTC_AVLTreeNode*    rangeIndexKey_(
                         OTC_AVLTreeNode* theRoot,
                         T1 const& theKey,
                         int (*theRankFn)(T1 const&, T1 const&),
                         OTC_AVLTreeNode*& theStartNode,
                         OTC_AVLTreeNode*& theEndNode
                        ) const;
                                // Searches for the node in the tree with
                                // <theRoot> corresponding to <theKey>
                                // according to <theRankFn>. If no such
                                // key is found, a null value is returned.
                                // If such a key is found, a pointer to
                                // that node is returned. Also, when the
                                // key is found, the range of nodes matching
                                // <theKey> is calculated, with the start
                                // and end of the range returned through
                                // <theStartNode> and <theEndNode>.

    int			searchIndexKey_(
			 OTC_AVLTreeNode* theRoot,
			 T1 const& theKey,
			 OTC_AVLTreeNode*& theNode
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
				// value will be non zero and
				// <theNode> also zero if the map is empty.

    int			searchGroupItem_(
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
				// value will be non zero and
				// <theNode> also zero if the map is empty.

    OTC_Link*		link_(T1 const& theKey, T2 const& theItem);
				// Creates an instance of <OTC_PairBucket>
				// to hold <theKey> and <theItem>.

    OTC_AVLTree*        index_;
                                // Index tree for keys.

    OTC_LinkList*       list_;
                                // List of key/item pairs.

    OTC_RCount*         count_;
                                // The population.


    int                 (*keyRankFn_)(T1 const&,T1 const&);
                                // Comparison function to be used in
                                // place of <OTC_RankActions> for keys.
 
    int                 (*itemRankFn_)(T2 const&,T2 const&);
                                // Comparison function to be used in
                                // place of <OTC_RankActions> for items.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/tmanymap.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_TMANYMAP_HH */
