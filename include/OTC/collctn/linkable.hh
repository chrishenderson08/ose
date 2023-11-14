#ifndef OTC_COLLCTN_LINKABLE_HH
#define OTC_COLLCTN_LINKABLE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/linkable.hh
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

#include <OTC/memory/mpobject.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/linkable.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Linkable : public OTC_MPObject
    // = TITLE
    //     Base class for objects which can be linked together.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_Linkable> class, is the basic building block for creating
    //     a doubly linked list of objects. Derived from <OTC_Linkable> are
    //     two classes which can be used when constructing a linked list.
    //     These two classes are <OTC_Link> and <OTC_Anchor>. The first
    //     of these is the base class for any user objects being placed
    //     into a linked list. The second class is used as an anchor point,
    //     from which to hang a linked list of objects. The <OTC_Anchor>
    //     class is also used in pairs to mark the boundaries for the
    //     traversal of an iterator.
    //     
    // = NOTES
    //     All instances of classes derived from <OTC_Linkable> must
    //     be created on the free store, using <new>.
    //
    // = SEE ALSO
    //     <OTC_Link>, <OTC_Anchor>, <OTC_LinkIterator>
{
  public:

    // = UNLINKING

    void		unlink()
      				{
				  if (prev_ != 0)
				    prev_->next_ = next_;
				  if (next_ != 0)
				    next_->prev_ = prev_;
				  prev_ = 0;
				  next_ = 0;
				}
				// Detaches this object from the list it is
				// contained in.

    // = REFERENCES

    void		reference()
      				{ count_++; }
				// Increments the reference count, ie., the
				// number of references to this object.

    void		unReference()
      				{
				  OTCLIB_ENSURE_FN((count_ > 0),
				   "OTC_Linkable::unReference()",
				   "link is not referenced");
				  count_--;
				  if (count_ == 0)
				    delete this;
				}
				// Decrements the reference count. If the
				// reference count reaches <0>, the object
				// deletes itself.

    // = KILLING

    void		kill()
      				{
				  if (!isDead_)
				  {
				    isDead_ = true;
				    unReference();
				  }
				}
				// If this object is unreferenced, the object
				// will be deleted, otherwise it will be
				// marked as dead.

    // = QUERY

    bool		isLink() const
				{ return isLink_ != 0; }
				// Returns <true> if this object is not an
				// anchor point but a link.

    bool		isDead() const
				{ return isDead_ != 0; }
				// Returns <true> if the object has been
				// killed but references still exist. If
				// there are no references, you shouldn't
				// be calling this function as no references
				// implies this objects has been deleted.

    OTC_Linkable*	prev()
				{ return prev_; }
				// Returns a pointer to the object before
				// this object.

    OTC_Linkable const*	prev() const
				{ return prev_; }
				// Returns a pointer to the object before
				// this object.

    OTC_Linkable*	next()
				{ return next_; }
				// Returns a pointer to the object after
				// this object.

    OTC_Linkable const*	next() const
				{ return next_; }
				// Returns a pointer to the object after
				// this object.

    // = ADDITIONS

    void		addBefore(OTC_Linkable* theObject);
				// Inserts <theObject> before this object in
				// the list. If <theObject> is already
				// contained within a list, it will first be
				// unlinked from that list.

    void		addAfter(OTC_Linkable* theObject);
				// Inserts <theObject> after this object in
				// the list. If <theObject> is already
				// contained within a list, it will first be
				// unlinked from that list.

  protected:

    // = INITIALISATION

			OTC_Linkable(bool theIsLink=false)
			  : prev_(0), next_(0), count_(0),
			    isLink_(theIsLink), isDead_(false)
			      	{ reference(); }
				// By default, initialises the class to be an
				// anchor. <theIsLink> should be set to
				// <true> if the derived class is not an
				// anchor point, but a link.

    // = DESTRUCTION

    virtual		~OTC_Linkable();
				// Invokes <unlink()> to detach this object
				// from the list in which it is contained.

  private:

			OTC_Linkable(OTC_Linkable const& theLink);
				// Do not define an implementation for this.

    OTC_Linkable const&	operator=(OTC_Linkable const& theLink);
				// Do not define an implementation for this.

    void		setPrev(OTC_Linkable* thePrev)
				{ prev_ = thePrev; }
				// Sets the pointer to the previous object
				// to be <thePrev>.

    void		setNext(OTC_Linkable* theNext)
				{ next_ = theNext; }
				// Sets the pointer to the next object
				// to be <theNext>.

    OTC_Linkable*	prev_;
				// Pointer to object before to this object.

    OTC_Linkable*	next_;
				// Pointer to object after this object.

    short		count_;
				// Absolute value is the count of the number
				// of references to this object.

    char		isLink_;
				// Set to <true> if derived class is not an
				// anchor but a link.

    char		isDead_;
				// Set to <true> if link is dead.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_LINKABLE_HH */
