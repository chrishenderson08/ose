#ifndef OTC_COLLCTN_IVCURSOR_HH
#define OTC_COLLCTN_IVCURSOR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ivcursor.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/prcursor.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/refcnt/count.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/ivcursor.hh"
#endif
#pragma interface "OTC/collctn/ivcursor.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_IdxVecCursor : public OTC_PairCursor<int,T>
    // = TITLE
    //     Cursor over a vector with key being the index position.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     Derived version of <OTC_Cursor>, which iterates over a vector. The
    //     index position of a value in the original vector is available
    //     as the key. In order to know when the vector should be deleted, a
    //     separate reference count object of type <OTC_Count> needs to be
    //     supplied so that it is know when nothing else is interested in the
    //     vector and that it can be deleted.
{
  public:

			~OTC_IdxVecCursor();

    // = INITIALISATION
    //     If <theProtection> is set to <OTCLIB_UNSAFE>, it will be assumed
    //     that something else always deletes the vector and it has a
    //     lifetime longer than this iterator. The reference count object
    //     can be a null pointer in unsafe mode. If it isn't null, it is
    //     again expected that something else will delete it when necessary.

			OTC_IdxVecCursor(
			  T* theVector,
			  int theLower,
			  u_int theSize,
			  OTC_Count* theCount,
			  OTC_Direction theDirection=OTCLIB_FORWARD,
			  OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Creates an iterator which will iterate
				// over values in <theVector> in
				// <theDirection>. <theVector> is taken
				// to have <theSize> values in it.
				// <theLower> is taken to be the original
				// lower bound of the vectortheCount>
				// should be the associated
				// reference count object.

			OTC_IdxVecCursor(
			  T* theVector,
			  int theLower,
			  u_int theStart,
			  u_int theLength,
			  OTC_Count* theCount,
			  OTC_Direction theDirection=OTCLIB_FORWARD,
			  OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Creates an iterator which will iterate
				// over values in <theVector> in
				// <theDirection>. The portion of the vector
				// over which iteration will occur is from
				// position <theStart> and with the range
				// having <theLength>. It is assumed that the
				// range is a valid portion of the vector.
				// <theLower> is taken to be the original
				// lower bound of the vector. <theCount>
				// should be the associated reference count
				// object.

			OTC_IdxVecCursor(OTC_IdxVecCursor<T> const& theIter);
				// Create an iterator at the same location as
				// <theIter>.

    // = MOVEMENT
    //     Following operations allow movement of the iterator over the
    //     list.

    void		reset();
				// Resets the iterator to the first item.

    void		next();
				// Moves the iterator to the next item.

    // = RETRIEVAL

    int&		key();
				// Returns the index of the value as it
				// was in the original vector.

    T&			item();
				// Returns the item under the current
				// location of the iterator. If no such item
				// exists an exception is generated.

    bool		isValid() const;
				// Returns <true> while there is a link under
				// the current location of the iterator amd
				// not an anchor point. In other words if the
				// iterator is located over a link which has
				// been killed then <true> will still be
				// returned.

    // = CLONING

    OTC_PairCursor<int,T>*	clone();
				// Create a new iterator at the same location
				// as this one but which operates
				// independently.

  private:

    T*			vector_;
				// The vector.

    int			lower_;
				// Lower bound in original vector.

    u_int		start_;
				// Offset into vector representing the
				// lower bound of iteration.

    u_int		length_;
				// Length of bound of iteration.

    OTC_Count*		refcnt_;
				// Reference count object so we know if we
				// should delete it when we are destroyed.

    OTC_Direction	direction_;
				// Direction in which to traverse.

    OTC_Protection	protection_;
				// Do we reference count the vector.

    int			offset_;
				// Current offset into vector.

    int			position_;
				// Original position in the vector of
				// the value. Separate to current offset
				// in the actual vector in case it gets
				// modified. Only update when required.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/ivcursor.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_IVCURSOR_HH */
