#ifndef OTC_COLLCTN_HINDEX_HH
#define OTC_COLLCTN_HINDEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/hindex.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/hashactn.hh>
#include <OTC/collctn/hindexen.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/hashtble.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/hindex.hh"
#endif
#pragma interface "OTC/collctn/hindex.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_HIndex
    // = TITLE
    //     Index mapping objects to void pointers.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is provided to allow the construction of maps between a
    //     key and an item stored as a void pointer. The index uses a hash
    //     table. Keys are managed in an arbitrary order and thus are not
    //     kept in any order relating to their ranking. The index does not in
    //     any way manage the items referenced as void pointers from the
    //     index. This means that when the index is destroyed, it is your
    //     responsibility to ensure the referenced items are destroyed.
    //     Normally the items would reside in a separate list and thus this
    //     can be achieved by deleting the list. The keys are managed through
    //     the use of the <OTC_BaseActions> class. Thus, if the keys are
    //     pointers, you can define actions to be performed when keys are
    //     added or removed from the index. Hashing with keys is carried out
    //     using the <OTC_HashActions> class. Searching with keys is carried
    //     out using the <OTC_RankActions> class.
    //     
    // = SEE ALSO
    //     <OTC_HIndexEntry>, <OTC_BaseActions>, <OTC_HashActions>,
    //     <OTC_RankActions>
{
  public:

    // = DESTRUCTION

    virtual		~OTC_HIndex();
				// Index is destroyed.

    // = INITIALISATION
    //     In the following constructors, the initial size of the hash table
    //     is determined by <theStartSize>. The floor size of the hash table,
    //     ie., that size which once exceeded the size of the hash table will
    //     not be reduced below, is determined by <theFloorSize>. The low
    //     threshold of the hash table, that is the number of items in the
    //     hash table, expressed as a percentage, which the population of the
    //     hash table must drop below before the size of the hash table is
    //     reduced is determined by <theLowThreshold>. The high threshold of
    //     the hash table, that is the number of items in the hash table,
    //     expressed as a percentage, which the population of the hash table
    //     must rise above before the size of the hash table is increased is
    //     determined by <theHighThreshold>. Note that <theStartSize> may be
    //     increased internally in size due to the manner in which the hash
    //     table is implemented. Note also, that if setting <theStartSize> to
    //     ensure some initial capacity in order to avoid the hash table
    //     needing to grow in size while being populated, you will need to
    //     consider that the table will be forced to grow when
    //     <theHighThreshold> is reached. Thus, <theStartSize> will actually
    //     need to be set to be higher than the capacity you want to ensure
    //     is available. Note that <theLowThreshold> must be less than <50>
    //     and <theHighThreshold> must greater than <50>.

			OTC_HIndex(
                         u_int theStartSize=16,
                         u_int theFloorSize=256,
                         u_int theLowThreshold=25,
                         u_int theHighThreshold=75
			);
				// Creates an empty index.

			OTC_HIndex(
			 int (*theRankFn)(T const&,T const&),
                         u_int theStartSize=16,
                         u_int theFloorSize=256,
                         u_int theLowThreshold=25,
                         u_int theHighThreshold=75
			);
				// Creates an empty index. <theRankFn>
				// is an alternate rank function to use
				// in place of <OTC_RankActions>.

    // = QUERY

    u_int		population() const
				{ return index_->population(); }
				// Returns the number of keys in the
				// index.

    bool		isEmpty() const
				{ return index_->population() == 0; }
				// Returns <true> if the index is empty.

    bool		contains(T const& theKey) const;
				// Returns <true> if <theKey> is in the
				// index.

    OTC_HIndexEntry<T>*	entry(T const& theKey)
				{ return entry_(theKey); }
				// Returns a pointer to the entry
				// corresponding to <theKey>. Returns <0> if
				// <theKey> doesn't exist.

    OTC_HIndexEntry<T> const*	entry(T const& theKey) const
				{ return entry_(theKey); }
				// Returns a pointer to the entry
				// corresponding to <theKey>. Returns <0> if
				// <theKey> doesn't exist.

    // For the following operations, if retrieving the first entry and
    // then iterating through to the last, the entries will be visited
    // in sorted order.

    OTC_HIndexEntry<T>* firstEntry()
                                { return (OTC_HIndexEntry<T>*)list_->first(); }
                                // Returns the first entry in the set of
                                // entries.
 
    OTC_HIndexEntry<T> const*   firstEntry() const
                                {
                                  return (OTC_HIndexEntry<T> const*)
                                   list_->first();
                                }
                                // Returns the first entry in the set of
                                // entries.
 
    OTC_HIndexEntry<T>* lastEntry()
                                { return (OTC_HIndexEntry<T>*)list_->last(); }
                                // Returns the last entry in the set of
                                // entries.
 
    OTC_HIndexEntry<T> const*   lastEntry() const
                                {
                                  return (OTC_HIndexEntry<T> const*)
                                   list_->last();
                                }
                                // Returns the last entry in the set of
                                // entries.

    // = INSERTION

    OTC_HIndexEntry<T>*	insert(T const& theKey);
				// Inserts into the index a entry
				// corresponding to <theKey> and
				// returns it. If an entry corresponding
				// to <theKey> already exists, it
				// will be returned. If the entry didn't
				// already exist, the item field will
				// be <0>, presuming it was set to a
				// non zero value when the key was first
				// added to the index.

    // = REMOVAL
    //     In addition to the following, a entry can be removed by
    //     calling <removeEntry()> on it. You should never explicitly
    //     delete an entry by calling <operator delete()> or unlink
    //     it from whatever data structure it is held in as doing so
    //     will corrupt the index.

    void		removeAll()
				{ index_->removeAll(); list_->removeAll(); }
				// Index is destroyed.

    void		remove(T const& theKey);
				// Removes from the index the entry
				// corresponding to <theKey>. If there exists
				// no entry corresponding to <theKey>, no
				// action is taken.

  protected:

    // = RANKING

    virtual int		rank(T const& key1, T const& key2) const;
				// Rank two keys.

  private:

			OTC_HIndex(OTC_HIndex<T> const&)
				{ terminate(); }
				// Do not provide an implementation for this.

    OTC_HIndex<T>&	operator=(OTC_HIndex<T> const&)
				{ terminate(); return *this; }
				// Do not provide an implementation for this.

    OTC_HIndexEntry<T>*	entry_(T const& theKey) const;
				// Returns a pointer to the entry in
				// the tree corresponding to <theKey>.
				// Returns <0> if <theKey> doesn't exist.

    int			(*rankFn_)(T const&,T const&);
                                // Comparison function to be used in
                                // place of <OTC_RankActions>.

    OTC_HashTable*      index_;
                                // Hash table for index.

    OTC_LinkList*       list_;
                                // List of items.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/hindex.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HINDEX_HH */
