#ifndef OTC_COLLCTN_ARRAY_HH
#define OTC_COLLCTN_ARRAY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/array.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/range.hh>
#include <OTC/collctn/copyactn.hh>
#include <OTC/collctn/worker.hh>
#include <OTC/collctn/visitor.hh>
#include <OTC/collctn/prworker.hh>
#include <OTC/collctn/prvisitr.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/shlwcopy.hh>
#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/modifier.hh>
#include <OTC/collctn/pritertr.hh>
#include <OTC/collctn/prmodifr.hh>
#include <OTC/refcnt/count.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/array.hh"
#endif
#pragma interface "OTC/collctn/array.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Array
    // = TITLE
    //     Wrapper around a conventional C vector.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_Array> is a wrapper around a conventional C style
    //     vector. A major difference to a traditional C style vector is
    //     that you define the range of indicies for the vector, meaning
    //     that you aren't limited to the least index always been <0>. You
    //     could for example, create a vector which has indicies ranging
    //     from <4> to <10>. Ie., a vector with indicies starting at <4>
    //     and having length <7>.
    //     
    //     If the vector is to hold a data type possessing a constructor, the
    //     data type must have a constructor which accepts no arguments. If
    //     the data type being held, doesn't automatically initialise itself
    //     through a constructor, it is the user's responsibility to
    //     initialise the vector or provide a value to which those in the
    //     vector can be initialised. For example, if the vector is to hold
    //     pointers, the user will have to initialise each location in the
    //     vector to <0> before use. In addition to having to initialise the
    //     vector when holding pointers, it is also the user's responsibility
    //     to delete each object pointed at, when the vector is destroyed, if
    //     those objects would no longer be referenced.
    //     
    // = NOTES
    //     This class replaces <OTC_Vector>. The retained functions should
    //     be compatable, although, it mean't that the action of some
    //     functions may not be consistant with use of newer functions.
    //
    // = SEE ALSO
    //     <OTC_Range>, <OTC_Worker>, <OTC_Visitor>, <OTC_PairWorker>,
    //     <OTC_PairVisitor>
{
  public:

			~OTC_Array();

    // = CLASS INITIALISATION

			OTC_Array(u_int theLength);
				// Creates a vector of size <theLength>,
				// where the starting index will be <0>.
				// There is no way to provide a value to
				// initialise elements in the vector when not
				// explicitly suppying a starting index.

			OTC_Array(T const* theVector, u_int theLength);
				// Creates a vector of size <theLength> which
				// contains a copy of the first <theLength>
				// values in <theVector>. The starting index
				// in the created vector will be <0>.

			OTC_Array(
			 OTC_Array<T> const& theVector,
			 u_int theLength
			);
				// Creates a vector of size <theLength> which
				// contains a copy of the first <theLength>
				// values in <theVector>. The starting index
				// in the created vector will be <0>.

			OTC_Array(
			 OTC_Array<T> const& theVector,
			 int theStart,
			 u_int theLength
			);
				// Creates a vector which is a copy of the
				// region of <theVector> described by
				// <theStart> and <theLength>. Note that
				// <theStart> is the starting index in
				// <theVector> and not an offset from the
				// start of <theVector>. The starting
				// index of the created vector will be <0>.

			OTC_Array(
			 OTC_Array<T> const& theVector,
			 OTC_Range const& theRange
			);
				// Creates a vector which is a copy of the
				// region of <theVector> described by
				// <theRange>. The starting index of the
				// created vector will be <0>.

			OTC_Array(int theStart, u_int theLength);
				// Creates a vector of size <theLength>,
				// where indicies start at <theStart>.

			OTC_Array(
			 int theStart,
			 u_int theLength,
			 T const& theValue
			);
				// Creates a vector of size <theLength>,
				// where indicies start at <theStart>.
				// Elements in the vector will be initialised
				// to <theValue>.

			OTC_Array(OTC_Range const& theRange);
				// Creates a vector where indicies cover
				// <theRange>.

			OTC_Array(OTC_Range const& theRange, T const& theValue);
				// Creates a vector where indicies cover
				// <theRange>. Elements in the vector will be
				// initialised to <theValue>.

			OTC_Array(OTC_Array<T> const& theVector);
				// Creates a vector which is a copy of
				// <theVector>. The same range of indicies
				// as in <theVector> will be used.

			OTC_Array(OTC_Array<T>& theVector, OTC_ShallowCopy);
				// Creates a vector which is a shallow copy
				// of <theVector>. That is, both reference
				// the same data with changes in one being
				// seen in the other. The same range of
				// indicies as in <theVector> will be used.
				// The sharing will be broken if the size
				// of either array is changed, one is
				// assigned to, or the bounds of the array
				// shifts with the length staying the same.

    // = QUERY

    u_int		size() const
				{ return range_.length(); }
				// Returns the size of the vector. That is,
				// how many elements the vector contains.
				// Note that this is not necessarily the
				// upper bound of the vector. This will only
				// be the case if the lower bound is <0>.
				// Use the <range()> method to query the
				// true bounds of the vector.

    OTC_Range const&	range() const
				{ return range_; }
				// Returns the range of indicies the vector
				// is covered by.

    // = ASSIGNMENT

    void		assign(OTC_Array<T> const& theVector);
				// Replaces the contents of this vector with
				// a copy of contents of <theVector>. Except
				// in the case of self assignment, new memory
				// will always be allocated to hold the
				// new data. The old memory held by this
				// vector will be deleted. If the vector holds
				// pointers to objects, the objects being
				// pointed at are not deleted.

    OTC_Array<T>&	operator=(OTC_Array<T> const& theVector)
				{ assign(theVector); return *this; }
				// Replaces the contents of this vector with
				// a copy of contents of <theVector>. Except
				// in the case of self assignment, new memory
				// will always be allocated to hold the
				// new data. The old memory held by this
				// vector will be deleted. If the vector holds
				// pointers to objects, the objects being
				// pointed at are not deleted.

    // = ACCESSING

			operator T*()
				{ return vector_; }
				// Conversion operator. Returns a pointer to
				// the underlying buffer. This particular
				// conversion operator will only be used on a
				// non-const instance of this class and will
				// allow modification of the underlying
				// buffer using the pointer returned.

			operator T const*() const
				{ return vector_; }
				// Conversion operator. Returns a pointer
				// to the underlying buffer. This particular
				// conversion operator will always be used
				// for a const instance of this class, or a
				// non-const instance of this class, when the
				// conversion is to a pointer to a vector of
				// const objects. It is not possible to
				// modify the underlying vector using the
				// pointer returned.

    T*			buffer()
				{ return vector_; }
				// Returns a pointer to the underlying
				// buffer. This particular version of
				// <buffer()> will only be used on a
				// non-const instance of this class, and will
				// allow modification of the underlying
				// buffer using the pointer returned.

    T const*		buffer() const
				{ return vector_; }
				// Returns a pointer to the underlying
				// buffer. This particular version of
				// <buffer()> will always be used for a const
				// object, or a non-const instance of this
				// class, when the conversion is to a pointer
				// to a vector of const objects. It is not
				// possible to modify the underlying vector
				// using the pointer returned.

    T&			item(int theIndex)
				{
				  OTCLIB_ENSURE_FN((range_.contains(theIndex)),
				   "OTC_Array<T>::operator[]","invalid index");
				  return vector_[theIndex-range_.lower()];
				}
				// Returns a reference to the object held
				// in the vector at index <theIndex>.
				// <theIndex> must be a valid position
				// within the vector.

    T const&		item(int theIndex) const
				{
				  OTCLIB_ENSURE_FN((range_.contains(theIndex)),
				   "OTC_Array::operator[]","invalid index");
				  return vector_[theIndex-range_.lower()];
				}
				// Returns a reference to the object held
				// in the vector at index <theIndex>.
				// <theIndex> must be a valid position
				// within the vector.

    T&			operator[](int theIndex)
				{
				  OTCLIB_ENSURE_FN((range_.contains(theIndex)),
				   "OTC_Array::operator[]","invalid index");
				  return vector_[theIndex-range_.lower()];
				}
				// Returns a reference to the object held
				// in the vector at index <theIndex>. This
				// version of <operator[]()> will only be
				// used on a non-const instance of this
				// class. The reference to the object held,
				// will allow modification of the actual
				// object. <theIndex> must be less than the
				// size of the vector.

    T const&		operator[](int theIndex) const
				{
				  OTCLIB_ENSURE_FN((range_.contains(theIndex)),
				   "OTC_Array::operator[]","invalid index");
				  return vector_[theIndex-range_.lower()];
				}
				// Returns a reference to the object held
				// in the vector at index <theIndex>. This
				// version of <operator[]()> will only be
				// used on a const instance of this
				// class, or a non-const instance, when
				// a reference to a const object is required.
				// The reference to the object held will not
				// allow modification of the actual object.
				// <theIndex> must be less than the size of
				// the vector.

    // = REPLACEMENT

    void		replaceItem(
			 T const& theValue,
			 int theIndex
			)
				{
				  replaceRange(OTC_Range(theIndex,1),
				   theValue);
				}
				// Replace the element at position
				// <theIndex> with <theValue>. Raises
				// an exception if <theIndex> is not
				// a valid position.

    void		replaceRange(
			 int theStart,
			 u_int theLength,
			 T const& theValue
			)
				{
				  replaceRange(OTC_Range(theStart,theLength),
				   theValue);
				}
				// Replace the elements starting at position
				// <theStart> and continuing for <theLength>
				// elements with <theValue>. Raises an
				// exception if <theStart> isn't a valid
				// position, or <theLength> would reach
				// beyond the last position in the vector.

    void		replaceRange(
			 OTC_Range const& theRange,
			 T const& theValue
			);
				// Replace the elements in <theRange>
				// with <theValue>. Raises an exception if
				// <theRange> doesn't lie within the vector.

    // = ITERATION
    //     By default, iterators will express an interest in the vector
    //     by manipulating a reference count. Performing the reference counts
    //     is to ensure that the vector still exists for the lifetime of
    //     the iterator, but also to enable the vector to be correctly
    //     deleted when nothing else requires it. If an unsafe iterator is
    //     required, for example, within the bounds of a read operation
    //     against data protected by a reader/writer mutex, a second argument
    //     can be passed to the following functions. The value of this
    //     argument is either <OTCLIB_SAFE> or <OTCLIB_UNSAFE>. To get an
    //     unsafe iterator, the <OTCLIB_UNSAFE> argument should be used.
    //     
    //     The second to last argument to the following functions indicates
    //     the direction of traversal of the iterator. Traversal in the
    //     direction of first to last item is indicated by a value of
    //     <OTCLIB_FORWARD>. Traversal in the reverse direction is indicated
    //     by a value of <OTCLIB_BACKWARD>. The default value is
    //     <OTCLIB_FORWARD>.

    OTC_Iterator<T>     items(
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const
                                {
				  return items_(range_.lower(),
				   range_.length(),theDirection,
				   theProtection);
				}
                                // Returns an iterator over all elements
				// in the vector.

    OTC_Modifier<T>     items(
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        )
                                {
				  return items_(range_.lower(),
				   range_.length(),theDirection,
				   theProtection);
				}
                                // Returns an iterator over all elements
				// in the vector.

    OTC_Iterator<T>     items(
                         OTC_Range const& theRange,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const
                                {
				  return items_(theRange.lower(),
				   theRange.length(),theDirection,
				   theProtection);
                                }
                                // Returns an iterator over a range of
                                // elements in the vector.

    OTC_Modifier<T>     items(
                         OTC_Range const& theRange,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        )
                                {
				  return items_(theRange.lower(),
				   theRange.length(),theDirection,
				   theProtection);
                                }
                                // Returns an iterator over a range of
                                // elements in the vector.

    OTC_Iterator<T>     items(
                         int theStart,
                         u_int theLength,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const
                                {
                                  return items_(theStart,theLength,
                                   theDirection,theProtection);
                                }
                                // Returns an iterator over a range of
                                // elements in the vector.

    OTC_Modifier<T>     items(
                         int theStart,
                         u_int theLength,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        )
                                {
                                  return items_(theStart,theLength,
                                   theDirection,theProtection);
                                }
                                // Returns an iterator over a range of
                                // elements in the vector.

    // In the following, the index position into the vector is available
    // as the key from the iterator.

    OTC_PairIterator<int,T>     pairs(
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const
                                {
				  return pairs_(range_.lower(),
				   range_.length(),theDirection,
				   theProtection);
				}
                                // Returns an iterator over all elements
				// in the vector.

    OTC_PairModifier<int,T>     pairs(
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        )
                                {
				  return pairs_(range_.lower(),
				   range_.length(),theDirection,
				   theProtection);
				}
                                // Returns an iterator over all elements
				// in the vector.

    OTC_PairIterator<int,T>     pairs(
                         OTC_Range const& theRange,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const
                                {
				  return pairs_(theRange.lower(),
				   theRange.length(),theDirection,
				   theProtection);
                                }
                                // Returns an iterator over a range of
                                // elements in the vector.

    OTC_PairModifier<int,T>     pairs(
                         OTC_Range const& theRange,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        )
                                {
				  return pairs_(theRange.lower(),
				   theRange.length(),theDirection,
				   theProtection);
                                }
                                // Returns an iterator over a range of
                                // elements in the vector.

    OTC_PairIterator<int,T>     pairs(
                         int theStart,
                         u_int theLength,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const
                                {
                                  return pairs_(theStart,theLength,
                                   theDirection,theProtection);
                                }
                                // Returns an iterator over a range of
                                // elements in the vector.

    OTC_PairModifier<int,T>     pairs(
                         int theStart,
                         u_int theLength,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        )
                                {
                                  return pairs_(theStart,theLength,
                                   theDirection,theProtection);
                                }
                                // Returns an iterator over a range of
                                // elements in the vector.

    // = APPLICATORS

    void		apply(
			 OTC_Worker<T>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD
			);
                                // Applies <theApplicator> to each of the
                                // elements in the vector. The direction
                                // being determined by <theDirection>. Valid
                                // values are <OTCLIB_FORWARD> and
                                // <OTCLIB_BACKWARD>.

    void		apply(
			 OTC_Visitor<T>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD
			);
                                // Applies <theApplicator> to each of the
                                // elements in the vector. The direction
                                // being determined by <theDirection>. Valid
                                // values are <OTCLIB_FORWARD> and
                                // <OTCLIB_BACKWARD>.

    // For the following, the index position of the element in the
    // vector is passed in as the key.

    void		apply(
			 OTC_PairWorker<int,T>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD
			);
                                // Applies <theApplicator> to each of the
                                // elements in the vector. The direction
                                // being determined by <theDirection>. Valid
                                // values are <OTCLIB_FORWARD> and
                                // <OTCLIB_BACKWARD>.

    void		apply(
			 OTC_PairVisitor<int,T>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD
			);
                                // Applies <theApplicator> to each of the
                                // elements in the vector. The direction
                                // being determined by <theDirection>. Valid
                                // values are <OTCLIB_FORWARD> and
                                // <OTCLIB_BACKWARD>.

    // = RESIZING

    void		resize(u_int theLength)
				{ resize(OTC_Range(0,theLength)); }
				// Resizes the vector to one with starting
				// index of <0> and with length <theLength>.
				// Unless the range of the new vector is
				// the same current vector, a new block of
				// memory is always allocated, even when the
				// size of the vector is being decreased. If
				// the size of the vector is increased, the
				// new section is not explicitly initialised,
				// thus unless the vector holds class objects
				// which perform their own initialisation you
				// will need to initialise it yourself.

    void		resize(int theStart, u_int theLength)
				{ resize(OTC_Range(theStart,theLength)); }
				// Resizes the vector to one with starting
				// index of <theStart> and with length
				// <theLength>. Unless the range of the new
				// vector is the same current vector, a new
				// block of memory is always allocated, even
				// when the size of the vector is being
				// decreased. If the size of the vector is
				// increased, the new section is not
				// explicitly initialised, thus unless the
				// vector holds class objects which perform
				// their own initialisation you will need to
				// initialise it yourself.

    void		resize(
			 int theStart,
			 u_int theLength,
			 T const& theValue
			)
				{
				  resize(OTC_Range(theStart,theLength),
				   theValue);
				}
				// Resizes the vector to one with starting
				// index of <theStart> and with length
				// <theLength>. Unless the range of the new
				// vector is the same current vector, a new
				// block of memory is always allocated, even
				// when the size of the vector is being
				// decreased. If the size of the vector is
				// increased, the new section is initialised
				// with <theValue>.

    void		resize(OTC_Range const& theRange);
				// Resizes the vector to one with range
				// <theRange>. Unless the range of the new
				// vector is the same current vector, a new
				// block of memory is always allocated, even
				// when the size of the vector is being
				// decreased. If the size of the vector is
				// increased, the new section is not
				// explicitly initialised, thus unless the
				// vector holds class objects which perform
				// their own initialisation you will need to
				// initialise it yourself.

    void		resize(OTC_Range const& theRange, T const& theValue);
				// Resizes the vector to one with range
				// <theRange>. Unless the range of the new
				// vector is the same current vector, a new
				// block of memory is always allocated, even
				// when the size of the vector is being
				// decreased. If the size of the vector is
				// increased, the new section is initialised
				// with <theValue>.

  private:

    OTC_Cursor<T>*	items_(
                         int theStart,
                         u_int theLength,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const;
                                // Returns an cursor over a range of
                                // elements in the vector.

    OTC_PairCursor<int,T>*	pairs_(
                         int theStart,
                         u_int theLength,
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const;
                                // Returns an cursor over a range of
                                // elements in the vector.

    OTC_Count*		refcnt_;
				// Count of how many share the actual buffer.

    OTC_Range		range_;
				// The range of indicies for the vector.

    T*			vector_;
				// The actual buffer space used by the vector.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/array.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ARRAY_HH */
