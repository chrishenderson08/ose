#ifndef OTC_COLLCTN_PRITERTR_HH
#define OTC_COLLCTN_PRITERTR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/pritertr.hh
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

#include <OTC/collctn/prcursor.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/pritertr.hh"
#pragma interface "OTC/collctn/pritertr.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_PairIterator
    // = TITLE
    //     Interface to a cursor of a key/item pair.
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
    //     should never be created using <new>.
{
  protected:

    T2&			item_() const
				{
				  OTCLIB_ENSURE_FN((iter_ != 0),
				   "OTC_PairIterator<T1,T2>::item_()",
				   "null iterator");
				  return iter_->item();
				}
				// Returns the item under the current
				// location of the iterator. Raises an
				// exception if there is no pair under the
				// iterator, or if this is a null iterator.

    OTC_PairCursor<T1,T2>*	clone_() const
				{ return iter_ ? iter_->clone() : 0; }
				// Returns an iterator which is a clone of
				// this iterator. The new iterator will
				// initially be located over the same item as
				// this iterator, however subsequent
				// operations will be independent of this
				// iterator.

    void		apply_(OTC_PairVisitor<T1,T2>& theApplicator)
				{
				  OTC_PairCursor<T1,T2>::apply(iter_,
				   theApplicator);
				}
                                // Applies <theApplicator> to each key/item
                                // pair accessible by this iterator.

    void		apply_(OTC_PairWorker<T1,T2>& theApplicator)
				{
				  OTC_PairCursor<T1,T2>::apply(iter_,
				   theApplicator);
				}
                                // Applies <theApplicator> to each key/item
                                // pair accessible by this iterator.

  public:

			~OTC_PairIterator()
				{ OTC_PairCursor<T1,T2>::replace(iter_,0); }

    // = INITIALISATION

			OTC_PairIterator()
			  : iter_(0) {}
				// Creates a null iterator.

			OTC_PairIterator(OTC_PairCursor<T1,T2>* theIter)
				{
				  OTC_PairCursor<T1,T2>::assign(
				   iter_,theIter);
				}
				// Constructor used when an iterator
				// for a particular type of collection is
				// first created. Passing in <0> will
				// result in a nil iterator being
				// constructed.

			OTC_PairIterator(
			 OTC_PairIterator<T1,T2> const& theIter
			)
				{
				  OTC_PairCursor<T1,T2>::assign(
				   iter_,theIter.iter_);
				}
				// Copy constructor for when an iterator is
				// used as return type.

    OTC_PairIterator<T1,T2>&    operator=(OTC_PairCursor<T1,T2>* theIter)
				{
				  OTC_PairCursor<T1,T2>::replace(
				   iter_,theIter);
				  return *this;
				}
                                // Sets this iterator to <theIter>.
                                // If <theIter> is <0>, the iterator
                                // is turned into a null iterator and
                                // all associations with the previous
                                // collection are lost.

    OTC_PairIterator<T1,T2>&	operator=(
				 OTC_PairIterator<T1,T2> const& theIter
				)
				{
				  OTC_PairCursor<T1,T2>::replace(
				   iter_,theIter.iter_);
				  return *this;
				}
				// Sets this iterator to <theIter>.

    // = MOVEMENT

    void		next()
				{ if (iter_ != 0) iter_->next(); }
				// Moves the iterator to the next item.

    void		reset()
				{ if (iter_ != 0) iter_->reset(); }
				// Resets the iterator to the start.

    // = RETRIEVAL

    T1 const&		key() const
				{
				  OTCLIB_ENSURE_FN((iter_ != 0),
				   "OTC_PairIterator<T1,T2>::key()",
				   "null iterator");
				  return iter_->key();
				}
				// Returns the key under the current
				// location of the iterator. Raises an
				// exception if there is no pair under the
				// iterator, or if this is a null iterator.

    T2 const&		item() const
				{ return item_(); }
				// Returns the item under the current
				// location of the iterator. Raises an
				// exception if there is no pair under the
				// iterator, or if this is a null iterator.

    bool		isValid() const
				{
				  return (iter_ != 0) ?
				   iter_->isValid() : false;
				}
				// Returns <true> while there is a valid data
				// item under the current location of the
				// iterator.

    // = CLONING

    OTC_PairIterator<T1,T2>	clone() const
				{ return clone_(); }
				// Returns an iterator which is a clone of
				// this iterator. The new iterator will
				// initially be located over the same item as
				// this iterator, however subsequent
				// operations will be independent of this
				// iterator.

    // = APPLICATOR

    void		apply(OTC_PairVisitor<T1,T2>& theApplicator)
				{ apply_(theApplicator); }
                                // Applies <theApplicator> to each key/item
                                // pair accessible by this iterator.

  private:

    OTC_PairCursor<T1,T2>*	iter_;
				// The real iterator.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRITERTR_HH */
