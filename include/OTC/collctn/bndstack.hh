#ifndef OTC_COLLCTN_BNDSTACK_HH
#define OTC_COLLCTN_BNDSTACK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/bndstack.hh
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

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/bndstack.hh"
#endif
#pragma interface "OTC/collctn/bndstack.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_BoundedStack
    // = TITLE
    //     Implements a LIFO or stack with fixed capacity.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_BoundedStack> implements a LIFO or stack with fixed capacity.
    //     Note, that it is the user's responsibility to deal with deletion
    //     of objects held in the stack when it is parameterised over a
    //     pointer type; ie., this class works independently of the
    //     <OTC_BaseActions> class.
    //     
    // = NOTES
    //     Being a bounded stack, for efficiency, an array is used in the
    //     internal implementation instead of a linked list. The consequences
    //     are, that when parameterised over a class type, the class must have
    //     a constructor which accepts no arguments and also be capable of
    //     having assignment performed upon it.
{
  public:

			~OTC_BoundedStack();

    // = INITIALISATION

			OTC_BoundedStack(u_int theCapacity, T* theMem=0);
				// Creates an empty stack which has
				// capacity to hold <theCapacity> items.
				// If a section of memory is provided
				// through <theMem> it will be used,
				// else memory from the free store will
				// be allocated. If the memory is provided
				// by the user, it is the user's responsibility
				// to delete it.

			OTC_BoundedStack(OTC_BoundedStack<T> const& theStack);
				// Creates a copy of <theStack>. Space for
				// this stack will always be allocated from
				// the free store. If holding pointers, only
				// the pointers are copied, not what the
				// pointers point at.

    // = ASSIGNMENT

    OTC_BoundedStack<T>&	operator=(OTC_BoundedStack<T> const& theStack);
				// Replaces this stack with a copy of
				// <theStack>. The capacity of this stack
				// will be changed to that of <theStack>.
				// Space for this stack will always be
				// allocated from the free store. If holding
				// pointers, only the pointers are
				// copied, not what the pointers point at.

    // = QUERY

    bool		isEmpty() const
				{ return put_ == 0; }
				// Returns <true> if the stack is empty.

    bool		isFull() const
				{ return put_ >= capacity_; }
				// Returns <true> if the stack is full.

    u_int		count() const
				{ return put_; }
				// Returns the number of items in the stack.

    u_int		capacity() const
				{ return capacity_; }
				// Returns the maximum number of items
				// which can be placed in the stack.

    T&			top()
				{
				  OTCLIB_ENSURE_FN((put_ != 0),
				   "OTC_Stack<T>::top()","stack empty");
				  return items_[put_-1];
				}
				// Returns a reference to the top item on
				// the stack. If the stack is empty, an
				// exception is raised.

    T const&		top() const
				{
				  OTCLIB_ENSURE_FN((put_ != 0),
				   "OTC_Stack<T>::top()","stack empty");
				  return items_[put_-1];
				}
				// Returns a reference to the top item on
				// the stack. If the stack is empty, an
				// exception is raised.

    T&			peek(u_int theIndex)
				{
				  OTCLIB_ENSURE_FN((theIndex < put_),
				   "OTC_Stack<T>::peek()","index out of range");
				  return items_[put_-theIndex-1];
				}
				// Returns a reference to the item
				// in the stack given by <theIndex>.
				// An index of <0> is the top of the
				// stack. Items below the top of the
				// stack are numbered from <1> onwards.
				// If <theIndex> is outside the bounds
				// of the stack, an exception is
				// raised.

    T const&		peek(u_int theIndex) const
				{
				  OTCLIB_ENSURE_FN((theIndex < put_),
				   "OTC_Stack<T>::peek(u_int)",
				   "index out of range");
				  return items_[put_-theIndex-1];
				}
				// Returns a reference to the item
				// in the stack given by <theIndex>.
				// An index of <0> is the top of the
				// stack. Items below the top of the
				// stack are numbered from <1> onwards.
				// If <theIndex> is outside the bounds
				// of the stack, an exception is
				// raised.

    // = MODIFICATION

    void		push(T const& theItem)
				{
				  OTCLIB_ENSURE_FN((put_ < capacity_),
				   "OTC_Stack<T>::push(T const&)",
				   "stack full");
				  items_[put_++] = theItem;
				}
				// Pushes <theItem> onto the top of
				// the stack. If the capacity of the
				// stack would be exceeded, an exception
				// is raised.

    T			pop()
				{
				  OTCLIB_ENSURE_FN((put_ != 0),
				   "OTC_Stack<T>::pop()","stack empty");
				  return items_[--put_];
				}
				// Removes the top item off the stack and
				// returns it. If the stack is empty, an
				// exception is raised.

    void		clear()
				{ put_ = 0; }
				// Clears the stack.

    void		discard(u_int theCount)
				{
				  OTCLIB_ENSURE_FN((theCount <= put_),
				   "OTC_Stack<T>::discard(u_int)",
				   "insufficient items");
				  put_ = put_ - theCount;
				}
				// Discard the top <theCount> items in
				// the stack. If there are not that many
				// items in the stack, an exception
				// is raised.

  private:

    u_int		capacity_;
				// The capacity of the stack.

    u_int		put_;
				// The index of the next available
				// location to place an item.

    T*			items_;
				// Array holding items.

    bool		destroy_;
				// Set to <true> if <items_> needs to be
				// destroyed.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/bndstack.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_BNDSTACK_HH */
