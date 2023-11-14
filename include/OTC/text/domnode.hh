#ifndef OTC_TEXT_DOMNODE_HH
#define OTC_TEXT_DOMNODE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/domnode.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/domprpty.hh>
#include <OTC/memory/mcobject.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/domnode.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_DOMNode : public OTC_MCObject
    // = TITLE
    //     Node in a document object model.
    //     
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_DOMNode> class implements a node in a document object
    //     model. Each node has an optional name and content associated with
    //     it. Each node may also have associated with it a list of
    //     properties. Nodes may be added as children of another node in
    //     order to form a hierarchy. The functionality of the class is
    //     loosely mirrored after the requirements of XML although naming
    //     doesn't agree with that in the DOM specification. As a result, a
    //     conversion to XML (with a few caveats), should be possible,
    //     although such a conversion routine is not actually provided in the
    //     class. It should also be possible to use these classes to hold
    //     an arbitrary XML document but again a routine to read XML is
    //     not provided.
    //     
    //     One of the main purposes for the class is to build up an object
    //     model for some data structure which can then be encoded into a
    //     string for saving into a file or writing over a socket. In doing
    //     this, large numbers of small objects can potentially be created
    //     only to subsequently be destroyed once the encoded form of the
    //     structure was produced. Allocation of many such small objects can
    //     potentially be quite inefficient due to the overhead of having to
    //     call into the system malloc libraries.
    //     
    //     As a consequence, in order to improve efficiency the class doesn't
    //     use global <operator new()> and <operator delete()> as one would
    //     usually. Instead, allocations are done against a memory cluster as
    //     implemented by the <OTC_Cluster> class.  The memory cluster gets
    //     larger blocks of memory from the system malloc libraries in one
    //     go and then subsequently splits this up as required. What this
    //     does mean though, is that all memory is released when the memory
    //     cluster is destroyed. Overall, for this specific purpose, this
    //     method has shown to produce code which is significantly quicker
    //     than when standard <operator new()> and <operator delete()> was
    //     used.
    //     
    //     Because the class has been optimised for quick creation and
    //     deletion, it may not be suitable if you want an object model
    //     that is kept in memory for long periods and which is heavily
    //     edited. This is because each time you make a change, that
    //     memory which is no longer used as a result of that editing,
    //     doesn't get reused and will not get reclaimed until the associated
    //     memory cluster is destroyed. Thus heavy editing can cause an
    //     increase in memory use over time with no way to get it back
    //     besides cloning the structure against a new cluster and deleting
    //     the old.
    //
    //     If using the class to hold the document object model of a HTML
    //     or XML document which you repetitively serve up, but in each
    //     case only change a few elements, use one instance to hold a
    //     template. When you need to serve up the document, make a clone
    //     of the template against a new cluster and then edit the elements
    //     you want to change.
    //     
    // = EXAMPLES
    //     Instances of <OTC_DOMNode> cannot be created on the stack, as
    //     member variables, in arrays etc. You must use the placement
    //     version of <operator new()> against an instance of <OTC_Cluster>.
    //     You should not delete individual instances of <OTC_DOMNode>,
    //     instead memory will be reclaimed in one go when the memory cluster
    //     is destroyed.
    //
    // = BEGIN<CODE>
    //     OTC_Cluster* theCluster;
    //     theCluster = new OTC_Cluster;
    //
    //     OTC_DOMNode* theNode1;
    //     theNode1 = new (*theCluster) OTC_DOMNode;
    //     theNode1->setName("root");
    //     theNode1->setProperty("id","1");
    //
    //     OTC_DOMNode* theNode2;
    //     theNode2 = new (*theCluster) OTC_DOMNode("child","text");
    //
    //     theNode1->insertChild(theNode2);
    //
    //     theNode1->createChild("child","text");
    //
    //     // To delete everything, delete the cluster.
    //
    //     delete theCluster;
    // = END<CODE>
    //     
    // = SEE ALSO
    //     <OTC_DOMProperty>
{
    friend class	OTC_DOMProperty;

  private:

    // = DESTRUCTION

			~OTC_DOMNode();
				// Unlinks this node from its parent.

  public:

    // = INITIALISATION

			OTC_DOMNode(
			 char const* theName=0,
			 char const* theContent=0
			);
				// Creates a node with <theName> and holding
				// <theContent>.

			OTC_DOMNode(
			 int theType,
			 char const* theName=0,
			 char const* theContent=0
			);
				// Creates a node of <theType> with <theName>
				// and holding <theContent>.

    // = TYPING
    //     The class doesn't impose specific values for identifying the
    //     type of a node. This is left up to the user. You need not even
    //     make use of the type attribute if you don't need to.

    int			type() const
      				{ return type_; }
    				// Returns the node type.

    void		setType(int theType)
      				{ type_ = theType; }
    				// Sets the node type to <theType>.

    // = NAMING

    OTC_DOMString	name() const
				{ return name_; }
				// Returns the name of this node. If no name
				// has yet been associated with the node, an
				// empty string will be returned.

    void		setName(char const* theName);
				// Sets the name of this node to <theName>.

    void		setName(char const* theName, size_t theLength);
				// Sets the name of this node to the first
				// <theLength> characters of <theName>. If
				// <theName> contains a null character
				// within that length, only the part of
				// <theName> before the null character will
				// actually be used as the name. The name
				// as stored will be null terminated. Raises
				// an exception if <theName> is <0> and
				// <theLength> is not <0>.

    // = CONTENT

    OTC_DOMString	content() const
				{
				  return OTC_DOMString(
				   content_,contentLength_);
				}
				// Returns the content associated with this
				// node. If no content has yet been
				// associated with the node, an empty string
				// will be returned.

    void		setContent(char const* theContent);
				// Sets the content of this node to a copy of
				// <theContent>.

    void		setContent(char const* theContent, size_t theLength);
				// Sets the content of this node to the first
				// <theLength> characters of <theContent>.
				// Embedded null characters are permitted
				// within the length specified. The content
				// as stored will be null terminated. Raises
				// an exception if <theContent> is <0> and
				// <theLength> is not <0>.

    void		setContent(OTC_DOMString const& theContent)
      				{
				  setContent(theContent.string(),
				   theContent.length());
				}
				// Sets the content of this node to a copy of
				// <theContent>.

    // = PROPERTIES

    bool		hasProperties() const
				{ return firstProperty_ != 0; }
				// Returns <true> if this node has properties
				// associated with it.

    OTC_DOMProperty*	property(char const* theName)
      				{ return property_(theName); }
				// Returns the property of this node with
				// <theName> or <0> if no such property
				// exists. Raises an exception if <theName>
				// is <0>.

    OTC_DOMProperty const*	property(char const* theName) const
      				{ return property_(theName); }
				// Returns the property of this node with
				// <theName> or <0> if no such property
				// exists. Raises an exception if <theName>
				// is <0>.

    OTC_DOMProperty*	property(char const* theName, char const* theValue)
      				{ return property_(theName,theValue); }
				// Returns the property of this node with
				// <theName> if it has <theValue> or <0> if
				// no such property exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMProperty const*	property(
			 char const* theName,
			 char const* theValue
			) const
      				{ return property_(theName,theValue); }
				// Returns the property of this node with
				// <theName> if it has <theValue> or <0> if
				// no such property exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMProperty*	property(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			)
      				{
				  return property_(theName,
				   theValue,theLength);
				}
				// Returns the property of this node with
				// <theName> if it has a value equivalent to
				// the first <theLength> characters of
				// <theValue> or <0> if no such property
				// exists. Raises an exception if <theName>
				// or <theValue> is <0>.

    OTC_DOMProperty const*	property(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			) const
      				{
				  return property_(theName,
				   theValue,theLength);
				}
				// Returns the property of this node with
				// <theName> if it has a value equivalent to
				// the first <theLength> characters of
				// <theValue> or <0> if no such property
				// exists. Raises an exception if <theName>
				// or <theValue> is <0>.

    OTC_DOMProperty*	property(
			 char const* theName,
			 OTC_DOMString const& theValue
			)
      				{
				  return property_(theName,
				   theValue.string(),theValue.length());
				}
				// Returns the property of this node with
				// <theName> if it has <theValue> or <0> if
				// no such property exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMProperty const*	property(
			 char const* theName,
			 OTC_DOMString const& theValue
			) const
      				{
				  return property_(theName,
				   theValue.string(),theValue.length());
			        }
				// Returns the property of this node with
				// <theName> if it has <theValue> or <0> if
				// no such property exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMProperty*	properties()
				{ return firstProperty_; }
				// Returns the first property in the list of
				// properties associated with this node.

    OTC_DOMProperty const*	properties() const
				{ return firstProperty_; }
				// Returns the first property in the list of
				// properties associated with this node.

    OTC_DOMProperty*	setProperty(
			 char const* theName,
			 char const* theValue
			);
				// Sets the property associated with this
				// node with <theName> to <theValue>. If the
				// property already existed, its value is
				// replaced. Raises an exception if <theName>
				// is <0> or an empty string.

    OTC_DOMProperty*	setProperty(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			);
				// Sets the property associated with this
				// node with <theName> to the first
				// <theLength> characters of <theValue>. If
				// the property already existed, its value is
				// replaced. Raises an exception if <theName>
				// is <0> or any empty string or if
				// <theValue> is <0> and <theLength> is not
				// <0>. If there are embedded null
				// characters within the first <theLength>
				// characters of <theValue>, only the part of
				// the string before that point is used.
				// The resultant string is always null
				// terminated.

    OTC_DOMProperty*	setProperty(
			 char const* theName,
			 OTC_DOMString const& theValue
			)
      				{
				  return setProperty(theName,
				   theValue.string(),theValue.length());
				}
				// Sets the property associated with this
				// node with <theName> to <theValue>. If the
				// property already existed, its value is
				// replaced. Raises an exception if <theName>
				// is <0> or an empty string.

    void		removeProperty(char const* theName);
    				// If a property with <theName> exists,
				// it is removed. Raises an exception if
				// <theName> is <0>.

    void		removeAllProperties();
				// Deletes all properties associated with
				// this node.

    // = HIERARCHY

    OTC_DOMNode*	parent()
				{ return parent_; }
				// Returns the parent of this node or <0> if
				// this element is the root of the hierarchy.

    OTC_DOMNode const*	parent() const
				{ return parent_; }
				// Returns the parent of this node or <0> if
				// this element is the root of the hierarchy.

    bool		hasChildren() const
      				{ return firstChild_ != 0; }
				// Returns <true> if this node has child
				// nodes.

    u_int		numChildren() const
      				{ return numChildren_; }
    				// Returns the number of child nodes this
				// node has.

    OTC_DOMNode*	child(u_int theIndex)
				{ return child_(theIndex); }
				// Returns the child of this node at position
				// <theIndex>. The first child has index of
				// <0>.

    OTC_DOMNode const*	child(u_int theIndex) const
				{ return child_(theIndex); }
				// Returns the child of this node at position
				// <theIndex>. The first child has index of
				// <0>.

    OTC_DOMNode*	firstChild()
				{ return firstChild_; }
				// Returns the first child of this node.

    OTC_DOMNode const*	firstChild() const
				{ return firstChild_; }
				// Returns the first child of this node.

    OTC_DOMNode*	lastChild()
				{ return lastChild_; }
				// Returns the last child of this node.

    OTC_DOMNode const*	lastChild() const
				{ return lastChild_; }
				// Returns the last child of this node.

    OTC_DOMNode*	createChild(
			 char const* theName=0,
			 char const* theContent=0
			);
				// Creates and appends a new child with
				// <theName> and with <theContent> to the
				// list of children for this node. The new
				// child will be returned.

    OTC_DOMNode*	createChild(
			 int theType,
			 char const* theName=0,
			 char const* theContent=0
			);
				// Creates and appends a new child of
				// <theType> with <theName> and with
				// <theContent> to the list of children for
				// this node. The new child will be returned.

    OTC_DOMNode*	insertChild(
			 OTC_DOMNode* theNewChild,
			 OTC_DOMNode* theOldChild=0
			);
				// Unlinks <theNewChild> from its parent
				// if it has one, and inserts it before
				// <theOldChild>. Raises an exception if
				// <theNewChild is <0>, if <theOldChild> is
				// not a child of this node, or if
				// <theNewChild> is an ancestor of this
				// node or is the same as this element. If
				// <theOldChild> is <0>, <theNewChild>
				// will be appended to the list of children
				// associated with this node. Returns
				// <theNewChild>.

    void		removeAllChildren();
				// Unlinks all the children of this node. If
				// you have references to any of the nodes,
				// they are still useable up to the point the
				// cluster they were allocated against is
				// destroyed.

    OTC_DOMNode*	nextSibling()
				{ return nextSibling_; }
				// Where this node is a child of some parent,
				// returns the next child in the list of
				// children of the parent or <0> if there are
				// no more siblings.

    OTC_DOMNode const*	nextSibling() const
				{ return nextSibling_; }
				// Where this node is a child of some parent,
				// returns the next child in the list of
				// children of the parent or <0> if there are
				// no more siblings.

    OTC_DOMNode*	prevSibling()
				{ return prevSibling_; }
				// Where this node is a child of some parent,
				// returns the previous child in the list of
				// children of the parent or <0> if there are
				// no more siblings.

    OTC_DOMNode const*	prevSibling() const
				{ return prevSibling_; }
				// Where this node is a child of some parent,
				// returns the previous child in the list of
				// children of the parent or <0> if there are
				// no more siblings.

    OTC_DOMNode*	clone(OTC_Cluster* theCluster=0) const;
				// Returns a copy of the subtree rooted at
				// this node. If <theCluster> is <0> the
				// memory will come from the same cluster
				// as is used by this node, else it will
				// be allocated from <theCluster>.

    OTC_DOMNode*	cloneChildren(OTC_Cluster* theCluster=0) const;
				// Returns a copy of the subtree rooted at
				// this node except that the name and properies
				// of the root node are not copied. In essence
				// the children are cloned into an empty root
				// node. If <theCluster> is <0> the memory
				// will come from the same cluster as is used
				// by this node, else it will be allocated
				// from <theCluster>.

    // In the current implementation, <unlink()> and <destroy()> do
    // the same thing as memory is reclaimed when the cluster is reused.
    // Adhere to the difference in the notional meaning of each in case
    // the implementation changes in some way such that <destroy()>
    // does actually destroy objects in some way.

    void		unlink();
				// Unlinks the subtree rooted at this node
				// from its parent. You can still make use
				// of node which is the root of the subtree,
				// including adding it as a child node of
				// some other node. If you no longer want
				// any part of the subtree use <destroy()>
				// instead.

    void		destroy()
      				{ unlink(); }
				// Unlinks the subtree rooted at this node
				// from its parent. You should attempt to
				// use any part of the subtree after using
				// this function.

    // = SEARCHING

    OTC_DOMNode*	findNode(char const* theName)
      				{ return findNode_(theName); }
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has <theName>. Raises
				// an exception if <theName> is <0>.

    OTC_DOMNode const*	findNode(char const* theName) const
      				{ return findNode_(theName); }
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has <theName>. Raises
				// an exception if <theName> is <0>.

    OTC_DOMNode*	findNode(char const* theName, size_t theLength)
      				{ return findNode_(theName,theLength); }
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a name matching the
				// first <theLength> characters of <theName>.
				// Raises an exception if <theName> is <0>.

    OTC_DOMNode const*	findNode(char const* theName, size_t theLength) const
      				{ return findNode_(theName,theLength); }
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a name matching the
				// first <theLength> characters of <theName>.
				// Raises an exception if <theName> is <0>.

    OTC_DOMNode*	findProperty(char const* theName)
      				{ return findProperty_(theName); }
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> or <0> if no such node exists.
				// Raises an exception if <theName> is <0>.

    OTC_DOMNode const*	findProperty(char const* theName) const
      				{ return findProperty_(theName); }
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> or <0> if no such node exists.
				// Raises an exception if <theName> is <0>.

    OTC_DOMNode*	findProperty(
			 char const* theName,
			 char const* theValue
			)
      				{ return findProperty_(theName,theValue); } 
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> and with <theValue> or <0>
				// if no such node exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMNode const*	findProperty(
			 char const* theName,
			 char const* theValue
			) const
      				{ return findProperty_(theName,theValue); } 
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> and with <theValue> or <0>
				// if no such node exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMNode*	findProperty(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			)
      				{
				  return findProperty_(theName,
				   theValue,theLength);
			        } 
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> and with value matching the first
				// <theLength> characters of <theValue> or
				// <0> if no such node exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMNode const*	findProperty(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			) const
      				{
				  return findProperty_(theName,
				   theValue,theLength);
				} 
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> and with value matching the first
				// <theLength> characters of <theValue> or
				// <0> if no such node exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMNode*	findProperty(
			 char const* theName,
			 OTC_DOMString const& theValue
			)
      				{
				  return findProperty_(theName,
				   theValue.string(),theValue.length());
				} 
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> and with <theValue> or <0>
				// if no such node exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMNode const*	findProperty(
			 char const* theName,
			 OTC_DOMString const& theValue
			) const
      				{
				  return findProperty_(theName,
				   theValue.string(),theValue.length());
				} 
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> and with <theValue> or <0>
				// if no such node exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMNode*	findChild(char const* theName)
      				{ return findChild_(theName); }
    				// Returns the first child node of this
				// node which has <theName> or <0> if no
				// such child node exists. Raises an
				// exception if <theName> is <0>.

    OTC_DOMNode const*	findChild(char const* theName) const
      				{ return findChild_(theName); }
    				// Returns the first child node of this
				// node which has <theName> or <0> if no
				// such child node exists. Raises an
				// exception if <theName> is <0>.

    OTC_DOMNode*	findChild(char const* theName, size_t theLength)
      				{ return findChild_(theName,theLength); }
    				// Returns the first child node of this
				// node which has a name matching the first
				// <theLength> characters of <theName> or <0>
				// if no such child node exists. Raises an
				// exception if <theName> is <0>.

    OTC_DOMNode const*	findChild(char const* theName, size_t theLength) const
      				{ return findChild_(theName,theLength); }
    				// Returns the first child node of this
				// node which has a name matching the first
				// <theLength> characters of <theName> or <0>
				// if no such child node exists. Raises an
				// exception if <theName> is <0>.

    OTC_DOMNode*	findSibling(char const* theName)
      				{ return findSibling_(theName); }
    				// Returns the first sibling node following
				// this node which has <theName> or <0>
				// if no such sibling node exists. Raises
				// an exception if <theName> is <0>.

    OTC_DOMNode const*	findSibling(char const* theName) const
      				{ return findSibling_(theName); }
    				// Returns the first sibling node following
				// this node which has <theName> or <0>
				// if no such sibling node exists. Raises
				// an exception if <theName> is <0>.

    OTC_DOMNode*	findSibling(char const* theName, size_t theLength)
      				{ return findSibling_(theName,theLength); }
    				// Returns the first sibling node following
				// this node which has a name matching the
				// first <theLength> characters of <theName>
				// or <0> if no such sibling node exists.
				// Raises an exception if <theName> is <0>.

    OTC_DOMNode const*	findSibling(
			 char const* theName,
			 size_t theLength
			) const
      				{ return findSibling_(theName,theLength); }
    				// Returns the first sibling node following
				// this node which has a name matching the
				// first <theLength> characters of <theName>
				// or <0> if no such sibling node exists.
				// Raises an exception if <theName> is <0>.

  private:

    OTC_DOMNode*	child_(u_int theIndex) const;
				// Returns the child of this node at position
				// <theIndex>. The first child has index of
				// <0>.

    OTC_DOMNode*	findNode_(char const* theName) const;
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has <theName>. Raises
				// an exception if <theName> is <0>.

    OTC_DOMNode*	findNode_(char const* theName, size_t theLength) const;
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a name matching the
				// first <theLength> characters of <theName>.
				// Raises an exception if <theName> is <0>.

    OTC_DOMProperty*	property_(char const* theName) const;
				// Returns the property of this node with
				// <theName> or <0> if no such property
				// exists. Raises an exception if <theName>
				// is <0>.

    OTC_DOMProperty*	property_(
			 char const* theName,
			 char const* theValue
			) const;
				// Returns the property of this node with
				// <theName> if it has <theValue> or <0> if
				// no such property exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMProperty*	property_(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			) const;
				// Returns the property of this node with
				// <theName> if it has a value equivalent to
				// the first <theLength> characters of
				// <theValue> or <0> if no such property
				// exists. Raises an exception if <theName>
				// or <theValue> is <0>.

    OTC_DOMNode*	findProperty_(char const* theName) const;
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> or <0> if no such node exists.
				// Raises an exception if <theName> is <0>.

    OTC_DOMNode*	findProperty_(
			 char const* theName,
			 char const* theValue
			) const;
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> and with <theValue> or <0>
				// if no such node exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMNode*	findProperty_(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			) const;
    				// Does a preorder traversal of the subtree
				// rooted at this node and returns the first
				// node found which has a property with
				// <theName> and with a value matching the
				// first <theLength> characters of <theValue>
				// or <0> if no such node exists. Raises an
				// exception if <theName> or <theValue> is
				// <0>.

    OTC_DOMNode*	findChild_(char const* theName) const;
    				// Returns the first child node of this
				// node which has <theName> or <0> if no
				// such child node exists. Raises an
				// exception if <theName> is <0>.

    OTC_DOMNode*	findChild_(char const* theName, size_t theLength) const;
    				// Returns the first child node of this
				// node which has a name matching the first
				// <theLength> characters of <theName> or <0>
				// if no such child node exists. Raises an
				// exception if <theName> is <0>.

    OTC_DOMNode*	findSibling_(char const* theName) const;
    				// Returns the first sibling node following
				// this node which has <theName> or <0>
				// if no such sibling node exists. Raises
				// an exception if <theName> is <0>.

    OTC_DOMNode*	findSibling_(
			 char const* theName,
			 size_t theLength
			) const;
    				// Returns the first sibling node following
				// this node which has a name matching the
				// first <theLength> characters of <theName>
				// or <0> if no such sibling node exists.
				// Raises an exception if <theName> is <0>.

  private:

			OTC_DOMNode(OTC_DOMNode const& theNode);
				// Do not provide an implementation for this.

    OTC_DOMNode&	operator=(OTC_DOMNode const&);
				// Do not provide an implementation for this.

  private:

    int			type_;
    				// Type associated with this node.

    char*		name_;
				// The name of this node.

    char*		content_;
    				// The content associated with this node.

    size_t		contentLength_;
    				// The length of the content.

    OTC_DOMNode*	parent_;
				// The parent of this node.

    OTC_DOMNode*	nextSibling_;
				// When this node is a child node, the
				// next node in the list of child nodes
				// of the parent.

    OTC_DOMNode*	prevSibling_;
				// When this node is a child node, the
				// previous node in the list of child nodes
				// of the parent.

    OTC_DOMProperty*	firstProperty_;
				// Pointer to first property associated with
				// this node.

    OTC_DOMProperty*	lastProperty_;
				// Pointer to first property associated with
				// this node.

    OTC_DOMNode*	firstChild_;
				// Pointer to first child node of this node.

    OTC_DOMNode*	lastChild_;
				// Pointer to last child node of this node.

    u_int		numChildren_;
    				// Number of children.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_DOMNODE_HH */
