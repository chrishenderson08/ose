#ifndef OTC_COLLCTN_PRMODIFR_HH
#define OTC_COLLCTN_PRMODIFR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/prmodifr.hh
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

#include <OTC/collctn/pritertr.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/prmodifr.hh"
#pragma interface "OTC/collctn/prmodifr.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_PairModifier : public OTC_PairIterator<T1,T2>
    // = TITLE
    //     Interface to a cursor over a key/item pair.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class exists so that the user of an iterator for collections
    //     holding key/item pairs, does not have to worry about deleting the
    //     iterator when finished with it, nor need to know how to create an
    //     iterator for a specific type of collection. This class will ensure
    //     that the iterator is destroyed, when this class goes out of scope.
    //     In order for this to work correctly, an instance of this class
    //     should never be created using <new>. The difference between
    //     this class and <OTC_PairModifier> is that it is possible to
    //     modify the items through this class.
{
  public:

			~OTC_PairModifier() {}

    // = INITIALISATION

			OTC_PairModifier() {}
				// Creates a null iterator.

			OTC_PairModifier(OTC_PairCursor<T1,T2>* theIter)
			  : OTC_PairIterator<T1,T2>(theIter) {}
				// Constructor used when an iterator
				// for a particular type of collection is
				// first created. If <theIter> is <0> a
				// null iterator is constructed.

			OTC_PairModifier(
			 OTC_PairModifier<T1,T2> const& theIter
			)
			  : OTC_PairIterator<T1,T2>(theIter) {}
				// Copy constructor for when an iterator is
				// used as return type.

    OTC_PairModifier<T1,T2>&	operator=(OTC_PairCursor<T1,T2>* theIter)
				{
				  OTC_PairIterator<T1,T2>::operator=(theIter);
				  return *this;
				}
				// Sets this iterator to <theIter>.
				// If <theIter> is <0>, the iterator
				// is turned into a null iterator and
				// all associations with the previous
				// collection are lost.

    OTC_PairModifier<T1,T2>&	operator=(
				 OTC_PairModifier<T1,T2> const& theIter
				)
				{
				  OTC_PairIterator<T1,T2>::operator=(theIter);
				  return *this;
				}
				// Sets this iterator to <theIter>.

    // = RETRIEVAL

    T2&			item() const
				{ return OTC_PairIterator<T1,T2>::item_(); }
				// Returns the item under the current
				// location of the iterator. If no such item
				// exists, or this is a null iterator, an
				// exception is raised.

    // = CLONING

    OTC_PairModifier<T1,T2>	clone() const
				{ return OTC_PairIterator<T1,T2>::clone_(); }
				// Returns an iterator which is
                                // a clone of this iterator. The new
                                // iterator will initially be located
                                // over the same item as this iterator,
                                // however, subsequent operations will be
                                // independent of this iterator.

    // = APPLICATOR

    void		apply(OTC_PairVisitor<T1,T2>& theApplicator)
				{ apply_(theApplicator); }
				// Applies <theApplicator> to each key/item
				// pair accessible by this iterator.

    void		apply(OTC_PairWorker<T1,T2>& theApplicator)
				{ apply_(theApplicator); }
				// Applies <theApplicator> to each key/item
				// pair accessible by this iterator.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRMODIFR_HH */
