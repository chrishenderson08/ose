#ifndef OTC_COLLCTN_STACK_HH
#define OTC_COLLCTN_STACK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/stack.hh
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

#include <OTC/collctn/linklist.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/stack.hh"
#endif
#pragma interface "OTC/collctn/stack.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Stack
    // = TITLE
    //     Implements a LIFO or stack.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_Stack> implements a LIFO or stack. Note, that it is the
    //     user's responsibility to deal with deletion of objects held in the
    //     stack, when it is parameterised over a pointer type; ie., this
    //     class works independently of the <OTC_BaseActions> class.
    //     
    // = NOTES
    //     To allow any number of items to be placed into the stack, it is
    //     implemented using a linked list. If efficiency is a concern, both
    //     in terms of speed and memory use, and there is a fixed upper bound
    //     on the number of items to be held, the class <OTC_BoundedStack> may
    //     be more appropriate.
    //
    // = SEE ALSO
    //     <OTC_BoundedStack>
{
  public:

			~OTC_Stack()
				{ delete list_; }

    // = INITIALISATION

			OTC_Stack();
				// Creates an empty stack.

			OTC_Stack(OTC_Stack<T> const& theStack);
				// Creates a stack which is a copy of
				// <theStack>. If the stack holds pointers,
				// only the value of the pointers is copied,
				// not what the pointer points at.

    // = ASSIGNMENT

    OTC_Stack<T>&	operator=(OTC_Stack<T> const& theStack);
				// Replaces this stack with a copy of
				// <theStack>. If the stack holds pointers,
				// only the value of the pointers is copied,
				// not what the pointer points at.

    // = QUERY

    bool		isEmpty() const
				{ return count_ == 0; }
				// Returns <true> if the stack is empty.

    u_int		count() const
				{ return count_; }
				// Returns the number of items held in the
				// stack.

    T&			top()
				{ return top_(); }
				// Returns a reference to the top item on
				// the stack. If the stack is empty, an
				// exception is raised.

    T const&		top() const
				{ return top_(); }
				// Returns a reference to the top item on
				// the stack. If the stack is empty, an
				// exception is raised.

    // = MODIFICATION

    void		push(T const& theItem);
				// Pushes <theItem> onto the top of
				// the stack.

    T			pop();
				// Removes the top item off the stack and
				// returns it. If the stack is empty, an
				// exception is raised.

    void		clear()
				{ list_->removeAll(); count_ = 0; }
				// Clears the stack of all items.

    void		discard(u_int theCount);
				// Discards the top <theCount> items on the
				// stack. If there are not that many items on
				// the stack, an exception is raised.

  private:

    T&			top_() const;
				// Returns a reference to the top item on
				// the stack. If the stack is empty, an
				// exception is raised.

    OTC_LinkList*	list_;
				// The list containing stack items.

    u_int		count_;
				// The number of items in the stack.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/stack.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_STACK_HH */
