#ifndef	OTC_COLLCTN_TINDEX_HH
#define	OTC_COLLCTN_TINDEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/tindex.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/tindexen.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/tindex.hh"
#endif
#pragma interface "OTC/collctn/tindex.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_TIndex
    // = TITLE
    //	   Index mapping objects to void pointers.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This	class is provided to allow the construction of maps between a
    //	   key and an item stored as a void pointer. The index uses a height
    //	   balance AVL tree, maintaining in order the lookup keys to
    //	   facilitate searching. The index does	not in any way manage the
    //	   items referenced as void pointers from the index. This means	that
    //	   when	the index is destroyed,	it is your responsibility to ensure
    //	   the referenced items	are destroyed. Normally	the items would
    //	   reside in a separate	list and thus this can be achieved by
    //	   deleting the	list. The keys are managed through the use of the
    //	   <OTC_BaseActions> class. Thus, if the keys are pointers, you	can
    //	   define actions to be	performed when keys are	added or removed
    //	   from	the index. Searching with keys is carried out using the
    //	   <OTC_RankActions> class.
    //	   
    // = SEE ALSO
    //	   <OTC_TIndexEntry>, <OTC_AVLTree>, <OTC_BaseActions>,
    //	   <OTC_RankActions>
{
  public:

    // = DESTRUCTION

    virtual		~OTC_TIndex();
				// Index is destroyed.

    // = INITIALISATION

			OTC_TIndex(int (*theRankFn)(T const&,T const&)=0)
			 : rankFn_(theRankFn) {}
				// Creates an empty index. <theRankFn>
				// is an alternate rank	function to use
				// in place of <OTC_RankActions>.

    // = QUERY

    u_int		population() const
				{ return index_.population(); }
				// Returns the number of keys in the
				// index.

    bool		isEmpty() const
				{ return index_.isEmpty(); }
				// Returns true	of the index is	empty.

    bool		contains(T const& theKey) const;
				// Returns <true> if <theKey> is in the
				// index.

    OTC_TIndexEntry<T>*	entry(T	const& theKey)
				{ return entry_(theKey); }
				// Returns a pointer to	the node in
				// the tree corresponding to <theKey>.
				// Returns <0> if <theKey> doesn't
				// exist.

    OTC_TIndexEntry<T> const*	entry(T	const& theKey) const
				{ return entry_(theKey); }
				// Returns a pointer to	the node in
				// the tree corresponding to <theKey>.
				// Returns <0> if <theKey> doesn't
				// exist.

    // For the following operations, if	retrieving the first entry and
    // then iterating through to the last, the entries will be visited
    // in sorted order.

    OTC_TIndexEntry<T>*	firstEntry()
				{
				  return isEmpty() ? 0 :
				   (OTC_TIndexEntry<T>*)
				   index_.root()->first();
				}
				// Returns the first entry in the set of
				// entries.

    OTC_TIndexEntry<T> const*	firstEntry() const
				{
				  return isEmpty() ? 0 :
				   (OTC_TIndexEntry<T> const*)
				   index_.root()->first();
				}
				// Returns the first entry in the set of
				// entries.

    OTC_TIndexEntry<T>*	lastEntry()
				{
				  return isEmpty() ? 0 :
				   (OTC_TIndexEntry<T>*)
				   index_.root()->last();
				}
				// Returns the last entry in the set of
				// entries.

    OTC_TIndexEntry<T> const*	lastEntry() const
				{
				  return isEmpty() ? 0 :
				   (OTC_TIndexEntry<T> const*)
				   index_.root()->last();
				}
				// Returns the last entry in the set of
				// entries.

    // = INSERTION

    OTC_TIndexEntry<T>*	insert(T const&	theKey);
				// Inserts into	the index a node
				// corresponding to <theKey> and
				// returns it. If a node corresponding
				// to <theKey> already exists, it
				// will	be returned. If	the node didn't
				// already exist, the item field will
				// be <0>, presuming it	was set	to a
				// non zero value when the key was first
				// added to the	index.

    // = REMOVAL
    //	   In addition to the following, a entry can be	removed	by
    //	   calling <removeEntry()> on it. You should never explicitly
    //	   delete an entry by calling <operator	delete()> or unlink
    //	   it from whatever data structure it is held in as doing so
    //	   will	corrupt	the index.

    void		removeAll()
				{ index_.removeAll(); }
				// Index is destroyed.

    void		remove(T const&	theKey);
				// Removes from	the index the node
				// corresponding to <theKey>. If there exists
				// no node corresponding to <theKey>, no
				// action is taken.

    // = TREE
    //	   Following operations	are valid for <OTC_TIndex> but not for
    //	   <OTC_HIndex>. If you	want to	be able	to switch between either
    //	   of these classes, you should	not use	the following operations.

    OTC_TIndexEntry<T>*	entryAtIndex(u_int theIndex)
				{ return entryAtIndex_(theIndex); }
				// Returns a pointer to	the node in
				// the tree corresponding to <theIndex>.
				// Returns <0> if <theIndex> doesn't
				// exist.

    OTC_TIndexEntry<T> const*	entryAtIndex(u_int theIndex) const
				{ return entryAtIndex_(theIndex); }
				// Returns a pointer to	the node in
				// the tree corresponding to <theIndex>.
				// Returns <0> if <theIndex> doesn't
				// exist.

    int			index(T	const& theKey) const;
				// Returns the position	of <theKey> within
				// the tree. Keys are maintained in an order
				// as defined by <OTC_RankActions>. If
				// <theKey> doesn't exist, a value of <-1> is
				// returned.

    void		removeAtIndex(u_int theIndex);
				// Removes from	the index the node in
				// position <theIndex>.	If there exists	no
				// node	corresponding to the index
				// <theIndex>, no action is taken.

  public:

    // Following functions are provided	for backward compatability
    // with old	<OTC_Index> class.

    OTC_TIndexEntry<T>*	node(T const& theKey)
				{ return entry_(theKey); }

    OTC_TIndexEntry<T> const*	node(T const& theKey) const
				{ return entry_(theKey); }

    OTC_TIndexEntry<T>*	nodeAtIndex(u_int theIndex)
				{ return entryAtIndex_(theIndex); }

    OTC_TIndexEntry<T> const*	nodeAtIndex(u_int theIndex) const
				{ return entryAtIndex_(theIndex); }

    OTC_TIndexEntry<T>*	root()
				{ return (OTC_TIndexEntry<T>*)index_.root(); }

    OTC_TIndexEntry<T> const*	root() const
				{
				  return (OTC_TIndexEntry<T> const*)
				   index_.root();
				}

  protected:

    // = RANKING

    virtual int		rank(T const& item1, T const& item2) const;
				// Rank	two items.

  private:

			OTC_TIndex(OTC_TIndex<T> const&)
				{ terminate(); }
				// Do not provide an implementation for	this.

    OTC_TIndex<T>&	operator=(OTC_TIndex<T>	const&)
				{ terminate(); return *this; }
				// Do not provide an implementation for	this.

    OTC_TIndexEntry<T>*	entry_(T const&	theKey)	const;
				// Returns a pointer to	the node in
				// the tree corresponding to <theKey>.
				// Returns <0> if <theKey> doesn't exist.

    OTC_TIndexEntry<T>*	entryAtIndex_(u_int theIndex) const;
				// Returns a pointer to	the node in
				// the tree corresponding to <theIndex>.
				// Returns <0> if <theIndex> doesn't exist.

    OTC_AVLTree		index_;
				// The AVL tree	forming	the index.

    int			(*rankFn_)(T const&,T const&);
				// Comparison function to be used in
				// place of <OTC_RankActions>.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/tindex.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_TINDEX_HH	*/
