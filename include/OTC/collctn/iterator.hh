#ifndef OTC_COLLCTN_ITERATOR_HH
#define OTC_COLLCTN_ITERATOR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/iterator.hh
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

#include <OTC/collctn/cursor.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/iterator.hh"
#pragma interface "OTC/collctn/iterator.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Iterator
    // = TITLE
    //     A smart pointer for iterators.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class exists so that the user of an iterator for a particular
    //     type of collection, does not have to worry about deleting the
    //     iterator when finished with it, nor need to know how to create an
    //     iterator for a specific type of collection. This class will ensure
    //     that the iterator is destroyed, when this class goes out of scope.
    //     In order for this to work correctly, an instance of this class
    //     should never be created using <new>.
    //     
    // = EXAMPLE
    //
    // = BEGIN<CODE>
    //     void sum(OTC_Collection<int> const& aCollection)
    //     {
    //       OTC_Iterator<int> anIter = 0;
    //       anIter = aCollection.items();
    //       int i=0;
    //       for (anIter.reset(); anIter.isValid(); anIter.next())
    //         i += anIter.item();
    //       cout << i << endl;
    //     }
    // = END<CODE>
{
  protected:

    T&			item_() const
				{
				  OTCLIB_ENSURE_FN((iter_ != 0),
				   "OTC_Iterator<T>::item()",
				   "null iterator");
				  return iter_->item();
				}
				// Returns the item under the current
				// location of the iterator. Raises an
				// exception if there is no item under the
				// iterator, or if this is a null iterator.

    OTC_Cursor<T>*	clone_() const
				{ return iter_ ? iter_->clone() : 0; }
				// Returns a clone of this iterator.

    void		apply_(OTC_Visitor<T>& theApplicator)
				{ OTC_Cursor<T>::apply(iter_,theApplicator); }
				// Applies <theApplicator> to each item
				// accessible by this iterator.

    void		apply_(OTC_Worker<T>& theApplicator)
				{ OTC_Cursor<T>::apply(iter_,theApplicator); }
				// Applies <theApplicator> to each item
				// accessible by this iterator.

  public:

			~OTC_Iterator()
				{ OTC_Cursor<T>::replace(iter_,0); }

    // = INITIALISATION

			OTC_Iterator()
			  : iter_(0) {}
				// Creates a null iterator.

			OTC_Iterator(OTC_Cursor<T>* anIter)
				{ OTC_Cursor<T>::assign(iter_,anIter); }
				// Constructor used when an iterator
				// for a particular type of collection is
				// first created. Passing in <0> will
				// result in a null iterator being
				// constructed.

			OTC_Iterator(OTC_Iterator<T> const& anIter)
				{ OTC_Cursor<T>::assign(iter_,anIter.iter_); }
				// Copy constructor for when an iterator is
				// used as return type.

    OTC_Iterator<T>&    operator=(OTC_Cursor<T>* anIter)
				{
				  OTC_Cursor<T>::replace(iter_,anIter);
				  return *this;
				}
                                // Sets this iterator to <anIter>.
                                // If <anIter> is <0>, the iterator
                                // is turned into a null iterator and
                                // all associations with the previous
                                // collection are lost.

    OTC_Iterator<T>&	operator=(OTC_Iterator<T> const& anIter)
				{
				  OTC_Cursor<T>::replace(iter_,anIter.iter_);
				  return *this;
				}
				// Sets this iterator to <anIter>.

    // = MOVEMENT

    void		next()
				{ if (iter_ != 0) iter_->next(); }
				// Moves the iterator to the next item.

    void		reset()
				{ if (iter_ != 0) iter_->reset(); }
				// Resets the iterator to the start.

    // = RETRIEVAL

    T const&		item() const
				{ return item_(); }
				// Returns the item under the current
				// location of the iterator. Raises an
				// exception if there is no item under the
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

    OTC_Iterator<T>	clone() const
				{ return clone_(); }
				// Returns an iterator which is a clone of
				// this iterator. The new iterator will
				// initially be located over the same item as
				// this iterator, however subsequent
				// operations will be independent of this
				// iterator.

    // = APPLICATOR

    void		apply(OTC_Visitor<T>& theApplicator)
				{ apply_(theApplicator); }
				// Applies <theApplicator> to each item
				// accessible by this iterator.

  private:

    OTC_Cursor<T>*	iter_;
				// The real iterator.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ITERATOR_HH */
