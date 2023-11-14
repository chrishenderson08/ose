#ifndef OTC_COLLCTN_AVLHNODE_HH
#define OTC_COLLCTN_AVLHNODE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/avlhnode.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/avlnode.hh>
#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/hashtble.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/avlhnode.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_AVLHashTableNode : public OTC_AVLNode
    // = TITLE
    //     AVL tree node for holding a hash table index.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is a derived version of <OTC_AVLNode> and contains a
    //     hash table.
    //     
    // = SEE ALSO
    //     <OTC_AVLNode>, <OTC_AVLTree>, <OTC_HashTable>
{
  public:

    // = DESTRUCTION

			~OTC_AVLHashTableNode();
				// Deletes the hash table. Anything
				// referenced by the hash table is not
				// deleted.

    // = INITIALISATION

                        OTC_AVLHashTableNode(
                         u_int theStartSize=16,
                         u_int theFloorSize=256,
                         u_int theLowThreshold=25,
                         u_int theHighThreshold=75
                        );
                                // Initialises the hash table. The initial
                                // size of the hash table is determined by
                                // <theStartSize>. The floor size of the hash
                                // table, ie., that size which once exceeded
                                // the size of the hash table will not be
                                // reduced below, is determined by
                                // <theFloorSize>. The low threshold of the
                                // hash table, that is the number of items in
                                // the hash table, expressed as a percentage,
                                // which the population of the hash table
                                // must drop below before the size of the
                                // hash table is reduced is determined by
                                // <theLowThreshold>. The high threshold of
                                // the hash table, that is the number of
                                // items in the hash table, expressed as a
                                // percentage, which the population of the
                                // hash table must rise above before the size
                                // of the hash table is increased is
                                // determined by <theHighThreshold>. Note
                                // that <theStartSize> may be increased
                                // internally in size due to the manner in
                                // which the hash table is implemented. Note
                                // also, that if setting <theStartSize> to
                                // ensure some initial capacity in order to
                                // avoid the hash table needing to grow in
                                // size while being populated, you will need
                                // to consider that the table will be forced
                                // to grow when <theHighThreshold> is reached.
                                // Thus, <theStartSize> will actually need
                                // to be set to be higher than the capacity
                                // you want to ensure is available. Note that
                                // <theLowThreshold> must be less than <50>
                                // and <theHighThreshold> must greater than
                                // <50>.

    // = QUERY

    OTC_HashTable*	index()
				{ return &index_; }
				// Returns a pointer to the hash table.

    OTC_HashTable const*	index() const
				{ return &index_; }
				// Returns a pointer to the hash table.

  private:

			OTC_AVLHashTableNode(OTC_AVLHashTableNode const&);
				// Do not define an implementation for this.

    OTC_AVLHashTableNode&	operator=(OTC_AVLHashTableNode const&);
				// Do not define an implementation for this.

    OTC_HashTable	index_;
				// The hash table.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_AVLHNODE_HH */
