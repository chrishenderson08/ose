#ifndef OTC_TEXT_DOMPRPTY_HH
#define OTC_TEXT_DOMPRPTY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/domprpty.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/mcobject.hh>
#include <OTC/text/domstrng.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma	interface "OTC/text/domprpty.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_DOMNode;

class OSE_EXPORT OTC_DOMProperty : public OTC_MCObject
    // = TITLE
    //     Holds a property related to a node of a document object model.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_DOMProperty> class holds a single property value which is
    //     associated with a node in a document object model. An instance
    //     of this class would be created by the <OTC_DOMNode> class. When an
    //     instance of this class is created, it must be created against an
    //     instance of <OTC_Cluster> using the placement version of <operator
    //     new()>. An instance of this class is only reclaimed when the memory
    //     cluster it was allocated against is deleted.
    //     
    // = SEE ALSO
    //     <OTC_DOMNode>, <OTC_Cluster>, <OTC_MCObject>
{
    friend		class OTC_Dummy;

  private:

    			~OTC_DOMProperty();

  public:

    // = INITIALISATION

			OTC_DOMProperty(
			 OTC_DOMNode* theNode,
			 char const* theName,
			 char const* theValue=0
			);
				// Appends a property with <theName> and
				// with <theValue> to <theNode>.

    // = PROPERTY

    OTC_DOMString	name() const
				{ return name_; }
				// Returns the name of the property.

    OTC_DOMString	value() const
      				{ return OTC_DOMString(value_,valueLength_); }
    				// Returns the value of the property.

    void		setValue(char const* theValue);
    				// Sets the value of this property to
				// <theValue>.

    void		setValue(char const* theValue, size_t theLength);
				// Sets the value of this property to the
				// first <theLength> characters of
				// <theValue>. Raises an exception if
				// <theValue> is <0> and <theLength> is
				// not <0>. The resultant string is
				// always null terminated.

    void		setValue(OTC_DOMString const& theValue)
      				{
				  setValue(theValue.string(),
				   theValue.length());
				}
    				// Sets the value of this property to
				// <theValue>.

    // = ELEMENT

    OTC_DOMNode*	node()
				{ return node_; }
				// Returns the node this property is
				// associated with.

    OTC_DOMNode const*	node() const
				{ return node_; }
				// Returns the node this property is
				// associated with.

    OTC_DOMProperty*	prev()
				{ return prev_; }
				// Returns the previous property in the list
				// of properties associated with the same
				// node.

    OTC_DOMProperty const*	prev() const
				{ return prev_; }
				// Returns the previous property in the list
				// of properties associated with the same
				// node.

    OTC_DOMProperty*	next()
				{ return next_; }
				// Returns the next property in the list
				// of properties associated with the same
				// node.

    OTC_DOMProperty const*	next() const
				{ return next_; }
				// Returns the next property in the list
				// of properties associated with the same
				// node.

    void		destroy()
      				{ unlink_(); }
    				// Unlinks this property from the element
				// it is associated with. After calling
				// this function you shouldn't try to use
				// the object.

  private:

    void		unlink_();
    				// Unlinks this property from the element
				// it is associated with.

  private:

			OTC_DOMProperty(OTC_DOMProperty const&);
				// Do not define an implementation for this.

    OTC_DOMProperty&	operator=(OTC_DOMProperty const&);
				// Do not define an implementation for this.

    OTC_DOMNode*	node_;
				// The node this property is associated
				// with.

    OTC_DOMProperty*	prev_;
				// The previous property in the list of
				// properties associated with the same
				// node.

    OTC_DOMProperty*	next_;
				// The next property in the list of
				// properties associated with the same
				// node.

    char*		name_;
				// The name of the property.

    char*		value_;
				// The value of the property.

    size_t		valueLength_;
    				// The length of the property value.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_DOMPRPTY_HH */
