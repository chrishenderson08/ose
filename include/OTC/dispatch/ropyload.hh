#ifndef OTC_DISPATCH_ROPYLOAD_HH
#define OTC_DISPATCH_ROPYLOAD_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/ropyload.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/plreader.hh>
#include <OTC/text/domnode.hh>
#include <OTC/memory/cluster.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/ropyload.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_DOMNode;

class OSE_EXPORT OTC_ROPayload
    // = TITLE
    //     Reader for data model implemented as a document object model.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_ROPayload> class implements a specialised view onto a
    //     document object model for represention of a program data model.
    //     In conjunction with the derived class called <OTC_RWPayload>, this
    //     class provides an easy way of creating a DOM for representing
    //     arbitrary data structures such that they can be serialised for
    //     the purpose of interprocess communications. The opposite of
    //     building a DOM from its serialised form is also possible. XML is
    //     used for the serialised format.
    //     
    //     A specific instance of this class is only ever used as a transient
    //     reference to some node within the DOM. In normal use an instance
    //     of the <OTC_SVPayload> class, which ultimately derives from this
    //     class, would be the real owner of the complete structure being
    //     referenced.
    //     
    //     This particular class permits read only operations on the target
    //     node. If you want to modify the target node, use the derived
    //     version of this class called <OTC_RWPayload>.
    //
    // = NOTES
    //     If any operation can't be satisfied, the result will generally
    //     be that an invalid node reference is returned. Because of this
    //     your code may not actually fail but may appear to work. Thus it
    //     is important where necessary to check that the node reference
    //     which is returned is valid before using it.
    //
    // = SEE ALSO
    //     <OTC_RWPayload>, <OTC_SVPayload>
{
    friend class	OTC_SVPayload;
    friend class	OTC_RWPayload;

  public:

    			~OTC_ROPayload() {}

    // = CREATION

			OTC_ROPayload()
			  : root_(0), path_(0), length_(0), index_(-2) {}
				// Creates an unbound reference to a node.

    			OTC_ROPayload(OTC_ROPayload const& theReader)
			  : root_(theReader.root_),
			    path_(theReader.path_),
			    length_(theReader.length_),
			    index_(theReader.index_)
			    	{}
				// Creates a reference to same node as
				// refered to by <theReader>.

    			OTC_ROPayload(OTC_DOMNode* theRoot)
			  : root_(theRoot), path_(0), length_(0), index_(-2)
			  	{}
				// <theRoot> will be used as the target node
				// for this reference. <theRoot> doesn't
				// necessarily have to be the root of the DOM
				// it is contained in, but it will become the
				// root in respect of operations performed
				// through this instance. Ie., operations
				// involving access to members of a structure
				// or array will be relative to <theRoot>.

    // = ASSIGNMENT

    OTC_ROPayload&	operator=(OTC_ROPayload const& theRoot)
      				{
				  root_ = theRoot.root_;
				  path_ = theRoot.path_;
				  length_ = theRoot.length_;
				  index_ = theRoot.index_;
				  return *this;
				}
				// Initialises this node reference to the
				// same as <theRoot>.

    // = REFERENCE

    bool		isValid() const
      				{ return nodeType() != "invalid"; }
				// Returns the value <true> if this reference
				// actually refers to an existing node.

    // = NODE

    OTC_DOMString	nodeType() const;
				// Returns the type of node this reference
				// refers to. This will be either "scalar",
				// "array", "index" or "invalid". If
				// the node reference isn't valid, "invalid"
				// is also returned.

    OTC_DOMString	nodeName() const
      				{ return property("name"); }
				// Returns the name associated with the
				// node this reference refers to.

    // = VALUE

    OTC_DOMString	value() const;
				// Returns the value associated with node
				// this reference refers to. The returned
				// value only really has meaning if the node
				// has type "scalar". For all other data
				// structure element types an empty string is
				// returned. If a type for the value has been
				// defined, you can obtain it by querying the
				// "type" property.

    OTC_DOMString	valueType() const
      				{ return property("type"); }
				// Returns any type which may have been
				// associated with the value. This is the
				// same as querying the "type" property.

    OTC_PLReader	operator,(bool& theStatus) const
      				{ return OTC_PLReader(*this,theStatus); }

    // = PROPERTIES

    bool		hasProperty(char const* theName) const;
				// Returns <true> if the node has a property
				// with <theName>. Will return <false> if the
				// node reference is invalid. Raises an
				// exception if <theName> is <0>.

    OTC_DOMString	property(char const* theName) const;
				// Returns the property with <theName> which
				// is associated with the node. If no such
				// property exists, or the node reference is
				// invalid an empty string is returned.
				// Raises an exception if <theName> is <0>.
				// Calling this function with <theName> set
				// to "name", is the same as calling the
				// <name()> member function.

    OTC_ROPayload	findProperty(char const* theName) const;
				// Does a preorder traversal of the subtree
				// rooted at the reference node, and returns
				// a reference to the first node which
				// contains a property with <theName>. If
				// no such property is found, an invalid
				// reference is returned. Raises an exception
				// if <theName> is <0>.

    OTC_ROPayload	findProperty(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			) const;
				// Does a preorder traversal of the subtree
				// rooted at the reference node, and returns
				// a reference to the first node which
				// contains a property with <theName> having
				// value matching the first <theLength>
				// characters of <theValue>. If no such
				// property is found, an invalid reference is
				// returned. Raises an exception if <theName>
				// is <0> or <theValue> is <0>.

    OTC_ROPayload	findProperty(
			 char const* theName,
			 char const* theValue
			) const
      				{
				  return findProperty(theName,theValue,
				   (theValue!=0)?strlen(theValue):0);
				}
				// Does a preorder traversal of the subtree
				// rooted at the reference node, and returns
				// a reference to the first node which
				// contains a property with <theName> having
				// <theValue>. If no such property is found,
				// an invalid reference is returned. Raises
				// an exception if <theName> is <0>.

    OTC_ROPayload	findProperty(
			 char const* theName,
			 OTC_DOMString const& theValue
			) const
      				{
				  return findProperty(theName,
				   theValue.string(),theValue.length());
				}
				// Does a preorder traversal of the subtree
				// rooted at the reference node, and returns
				// a reference to the first node which
				// contains a property with <theName> having
				// <theValue>. If no such property is found,
				// an invalid reference is returned. Raises
				// an exception if <theName> is <0>.

    // = OBJECT

    OTC_ROPayload	sibling(char const* thePath, size_t theLength) const;
				// Returns a new reference to the node below
				// this node as described by <thePath> where
				// <thePath> has <theLength>. <thePath> is
				// taken as is, as the node name. This node
				// must have type "index". If <thePath>
				// doesn't refer to a valid member of an
				// index, an invalid node is returned. Raises
				// an exception if <thePath> is <0>.

    OTC_ROPayload	sibling(char const* thePath) const
      				{
				  return sibling(thePath,
				   thePath?strlen(thePath):0);
				}
				// Returns a new reference to the node below
				// this node as described by <thePath>.
				// <thePath> is taken as is, as the node
				// name. This node must have type "index".
				// If <thePath> doesn't refer to a valid
				// member of an index, an invalid node is
				// returned. Raises an exception if <thePath>
				// is <0>.

    OTC_ROPayload	sibling(OTC_String const& thePath) const
      				{
				  return sibling(thePath.string(),
				   thePath.length());
				}
				// Returns a new reference to the node below
				// this node as described by <thePath>.
				// <thePath> is taken as is, as the node
				// name. This node must have type "index".
				// If <thePath> doesn't refer to a valid
				// member of an index, an invalid node is
				// returned. Raises an exception if <thePath>
				// is <0>.

    OTC_ROPayload	sibling(OTC_DOMString const& thePath) const
      				{
				  return sibling(thePath.string(),
				   thePath.length());
				}
				// Returns a new reference to the node below
				// this node as described by <thePath>.
				// <thePath> is taken as is, as the node
				// name. This node must have type "index".
				// If <thePath> doesn't refer to a valid
				// member of an index, an invalid node is
				// returned. Raises an exception if <thePath>
				// is <0>.

    // Note that the path in the following is interpreted differently to
    // the <sibling()> function. In the following function, the path is
    // interpreted as a hierarchy where the separator is ".".

    OTC_ROPayload	operator[](char const* thePath) const;
				// Returns a new reference to the node below
				// this node as described by <thePath>.
				// <thePath> can be a list of one or more
				// node names separated by ".". For this to
				// work, the root node referred to and any
				// subsequent nodes, with the exception of
				// the final node, as referred to by
				// <thePath> must have node type of "index".
				// If <thePath> doesn't refer to a valid
				// member of an index, an invalid node
				// reference is returned. Raises an exception
				// if <thePath> is <0>.

    // = ARRAY
    //     The most efficient way of traversing over an array is to use the
    //     method <firstChild()> to access the first item, and then use
    //     <nextSibling()> to successively get the next item until there are
    //     no more. When an integer index is used, a linear traversal of
    //     the list of child nodes must be made for each access.

    OTC_ROPayload	operator[](int theIndex) const;
				// Returns a new reference to the node below
				// this node as described by <theIndex>. For
				// this to work, the root node referred to
				// must have node type of "array". <theIndex>
				// then refers to the item at that position
				// in the represented array, where the first
				// item has index of <0>. If <theIndex> is
				// <-1> a reference to the last item in the
				// array is returned. If <theIndex> is
				// greater than or equal to the number of
				// items in the array an invalid node is
				// returned. If <theIndex> is less than <-1>
				// an exception is raised.

    // = HIERARCHY

    OTC_ROPayload	parent() const;
				// Returns a reference to the parent of the
				// node referred to by this node reference.
				// If the node refered to is the root node,
				// an invalid node reference is returned.

    u_int		numChildren() const;
				// If the node referred to by this node
				// reference has type "index" or "array",
				// returns a count of the number of child
				// nodes of that node. Returns <0> where
				// the node is of type "scalar".

    OTC_ROPayload	firstChild() const;
				// If the node referred to by this node
				// reference has type "index" or "array",
				// returns a node reference to the first
				// element in that object or array.

    OTC_ROPayload	lastChild() const;
				// If the node referred to by this node
				// reference has type "index" or "array",
				// returns a node reference to the last
				// element in that object or array.

    OTC_ROPayload	prevSibling() const;
				// If the node referred to by this node
				// reference is an element within a "index"
				// or an "array", a reference to the previous
				// element is returned. That is, a reference
				// to the previous sibling node of the node
				// referred to by this node reference.

    OTC_ROPayload	nextSibling() const;
				// If the node referred to by this node
				// reference is an element within a "index"
				// or an "array", a reference to the next
				// element is returned. That is, a reference
				// to the next sibling node of the node
				// referred to by this node reference.

    // = ENCODING
    //     Note that although the serialised form is XML, an XML prologue is
    //     not output so that it can be used as a fragment of a larger XML
    //     document. Also be aware that XML is quite verbose. If you need
    //     a system which can handle very high throughput, you might want
    //     to use something with a more condensed wire format. XML is only
    //     really used as it is where everyone is going, although for this
    //     same reason, it will possibly make it easier to interwork with
    //     other systems or to create debugging and analysis tools.
    //
    //     Where appropriate, property values and content is escaped such
    //     that only valid XML characters are used. Note that for the special
    //     case where content has had the type "base64Binary" attributed to
    //     it, "base64" encoding will be used instead.

    OTC_String		encode() const;
				// Encodes the data model, returning it as a
				// string in an XML format.

    void		encode(ostream& theStream) const
      				{ encodeNode_(theStream,resolve()); }
				// Encodes the data model onto <theStream> in
				// an XML format.

    friend OSE_EXPORT ostream&	operator<<(
			 ostream& theStream,
			 OTC_ROPayload const& theObject
			)
      				{
				  theObject.encode(theStream);
				  return theStream;
				}
				// Encodes the data model in <theObject> onto
				// <theStream>.

    // = NULL ELEMENTS

    static OTC_ROPayload	nullValue();
    				// Returns a reference to a null value.

    static OTC_ROPayload 	nullArray();
    				// Returns a reference to an empty array.

    static OTC_ROPayload	nullIndex();
    				// Returns a reference to a index with
				// no members.

  protected:

    // = RESOLVE

    OTC_DOMNode*	peek() const;
				// Resolves any path associated with the
				// reference to determine the target node and
				// applies any index if applicable. The
				// derived target node is then returned.
				// When resolving the path, if a node doesn't
				// exist or a node is of the wrong type, a
				// null pointer is returned. That is, no new
				// nodes are added to the data structure.

    // Note that following actually update the root reference to be the
    // actual target node and the path and index are reset.

    OTC_DOMNode*	resolve() const;
				// Resolves any path associated with the
				// reference to determine the target node and
				// applies any index if applicable. The
				// derived target node is then returned.
				// When resolving the path, if a node doesn't
				// exist or a node is of the wrong type, a
				// null pointer is returned. That is, no new
				// nodes are added to the data structure.
				// This is the same as <peek()> except that
				// it updates the root node to be the target
				// node and the path and index are reset.

    OTC_DOMNode*	install() const;
				// Resolves any path associated with the
				// reference to determine the target node and
				// applies and index if applicable. When
				// resolving the path, if a node doesn't
				// exist or a node is of the wrong type, the
				// tree is amended in order to satisfy the
				// request. This can resulting in existing
				// nodes being replaced with nodes of a
				// different node type and with existing
				// values being destroyed.

    // = CLUSTER

    OTC_Cluster*	cluster() const
      				{ return OTC_Cluster::of(root_); }
    				// Returns the cluster against which the
				// root node.

    // = REFERENCE

    OTC_DOMNode*	root() const
      				{ return root_; }
    				// Returns the root of the subtree being
				// referenced.

    OTC_DOMString	path() const
      				{ return path_; }
    				// Returns the path to the target node with
				// respect to the root of the subtree.

    int			index() const
      				{ return index_; }
    				// Returns the index if any into an array
				// when the target node is an array.

    // = NAMING

    bool		validProperty(char const* theName) const;
				// Returns <true> if <theName> is a valid
				// property name according to the XML rules.

    // = RESET

    void		resetProperties(OTC_DOMNode* theNode) const;
    				// Removals all properties from <theNode>
				// except the "name" property.

  protected:

    // = DERIVED INITIALISATION

    			OTC_ROPayload(
			 OTC_DOMNode* theRoot,
			 char const* thePath,
			 size_t theLength,
			 int theIndex
			)
			  : root_(theRoot),
			    path_(thePath),
			    length_(theLength),
			    index_(theIndex) {}
				// Sets up a reference to a node where
				// <theRoot> is the top node of some subtree
				// of the associated DOM with <thePath> being
				// the path below <theRoot> giving the actual
				// target data structure element. <theIndex>
				// describes a specific array item where the
				// node described by <thePath> is an array.
				// If <theIndex> is greater than or equal to
				// <0> it identifies a specific item within
				// that array. If <theIndex> is <-1>, it is
				// the last item in the array. If value less
				// than <-1> is used for <theIndex>, it
				// indicates we aren't talking about an item
				// of an array but the actual data structure
				// element described by <thePath>. <theLength>
				// should be the actual length of <thePath>
				// or <0> if none provided.

  private:

    OTC_DOMNode*	resolve_(
			 OTC_DOMNode* theNode,
			 char const* thePath,
			 size_t theLength,
			 int theIndex
			) const;
				// Resolves <thePath> and <theIndex> against
				// <theNode> to determine the target node.
				// <theLength> is the length of the path.
				// When resolving the path, if a node doesn't
				// exist or a node is of the wrong type, a
				// null pointer is returned. That is, no new
				// nodes are added to the data structure.

    OTC_DOMNode*	install_(
			 OTC_DOMNode* theNode,
			 char const* thePath,
			 size_t theLength,
			 int theIndex
			) const;
				// Resolves <thePath> and <theIndex> against
				// <theNode> to determine the target node.
				// <theLength> is the length of the path.
				// When resolving the path, if a node doesn't
				// exist or a node is of the wrong type, the
				// tree is amended in order to satisfy the
				// request. This can resulting in existing
				// nodes being replaced with nodes of a
				// different node type and with existing
				// values being destroyed.

    void		encodeNode_(
			 ostream& theStream,
			 OTC_DOMNode const* theNode
			) const;
				// Encodes the data model in <theNode> onto
				// <theStream>.

    void		encodeString_(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength
			) const;
				// Outputs the first <theLength> characters
				// of <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data.

    void		encodeString_(
			 ostream& theStream,
			 char const* theString
			) const
      				{
				  encodeString_(theStream,theString,
				   theString?strlen(theString):0);
				}
				// Outputs <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data.

    void		encodeString_(
			 ostream& theStream,
			 OTC_DOMString const& theString
			) const
      				{
				  encodeString_(theStream,theString.string(),
				   theString.length());
				}
				// Outputs <theString> onto <theStream>. The
				// characters are escaped as per the XML
				// requirements for character data.

    void		encodeBinary_(
			 ostream& theStream,
			 char const* theString,
			 size_t theLength
			) const;
				// Outputs the first <theLength> characters
				// of <theString> onto <theStream>. The
				// characters are encoded in base64 format.

    void		encodeBinary_(
			 ostream& theStream,
			 char const* theString
			) const
      				{
				  encodeBinary_(theStream,theString,
				   theString?strlen(theString):0);
				}
				// Outputs <theString> onto <theStream>. The
				// characters are encoded in base64 format.

    void		encodeBinary_(
			 ostream& theStream,
			 OTC_DOMString const& theString
			) const
      				{
				  encodeBinary_(theStream,theString.string(),
				   theString.length());
				}
				// Outputs <theString> onto <theStream>. The
				// characters are encoded in base64 format.

  private:

    static OTC_NRMutex	mutex_;
    				// Shared mutex for null elements.

    static OTC_Cluster*	gCluster_;
    				// Cluster for null elements.

    static OTC_DOMNode*	gNullValue_;
    				// DOM for null value.

    static OTC_DOMNode*	gNullArray_;
    				// DOM for empty array.

    static OTC_DOMNode*	gNullStruct_;
    				// DOM for index with no members.

    OTC_DOMNode*	root_;
				// The root of the subtree being referenced.
				// If this is <0>, the object is an unbound
				// reference. Note that this isn't the target
				// node. The target node is defined with
				// respect to this root node by the path and
				// the index.

    // The following only have meaning prior to the reference being resolved.
    // That is, resolving the target node is actually deferred until the
    // context in which it is being used is known. This means we will only
    // create nodes as necessary if the context is such that a write
    // operation ie being performed as opposed to a read operation.

    char const*		path_;
				// The path of the target element with
				// respect to the root of the subtree.
				// If this is <0>, it indicates the target
				// element is the same node as the root node.
				// Note that the target element isn't
				// necessarily the target node. When an
				// index is defined appropriately and the
				// target element is an array, the target
				// node is the element within the array
				// identified by the index.

    size_t		length_;
    				// The length of the path. Will be zero
				// if path not being used.

    int			index_;
    				// An identifier for a particular element
				// within an array. If this is less than
				// <-1>, it indicates we aren't interested in
				// an element of the array after all, but
				// that the target node is the target element
				// identified by the path. Note that if <-2>
				// it indicates path should be interpreted
				// as a hierarchy, if less than <-2> it
				// should be taken as is. If this is <-1>,
				// it indicates we are interested in the
				// last element of the array. The context
				// in which this is used though is important.
				// If the context is a write operation, a
				// new last element is actually created
				// and that becomes the target node.
};

/* ------------------------------------------------------------------------- */

#include <OTC/dispatch/plreader.i>

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_ROPYLOAD_HH */
