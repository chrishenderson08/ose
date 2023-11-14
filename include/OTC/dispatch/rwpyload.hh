#ifndef OTC_DISPATCH_RWPYLOAD_HH
#define OTC_DISPATCH_RWPYLOAD_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/rwpyload.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/ropyload.hh>
#include <OTC/types/duration.hh>
#include <OTC/types/time.hh>
#include <OTC/text/symbol.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/rwpyload.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_DOMNode;

class OSE_EXPORT OTC_RWPayload : public OTC_ROPayload
    // = TITLE
    //     Writer for data model implemented as a document object model.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_ROPayload> class implements a specialised view onto a
    //     document object model for represention of a program data model.
    //     In conjunction with the base class called <OTC_ROPayload>, this
    //     class provides an easy way of creating a DOM for representing
    //     arbitrary data structures such that they can be serialised for
    //     storage or for the purpose of interprocess communications. The
    //     opposite of building a DOM from its serialised form is also
    //     possible. XML is used for the serialised format.
    //
    //     A specific instance of this class is only ever used as a transient
    //     reference to some node within the DOM. In normal use an instance
    //     of the <OTC_SVPayload> class, which ultimately derives from this
    //     class, would be the real owner of the complete structure being
    //     referenced.
    //     
    //     The class extends the base class <OTC_ROPayload> to permit write
    //     operations on a target node. When being used to modify a node,
    //     what type the node will take on will depend on the type of
    //     operation performed. Also, as required a hierarchy of nodes will
    //     be automatically created dependent on the path and index defined
    //     in a node reference.
    //
    // = NOTES
    //     If any operation can't be satisfied, the result will generally
    //     be that an invalid node reference is returned. Because of this
    //     your code may not actually fail but may appear to work. Thus it
    //     is important where necessary to check that the node reference
    //     which is returned is valid before using it.
    //
    //     In decoding the XML, it can't be known how much memory may need to
    //     be preallocated in a buffer to hold the content of an XML element.
    //     As a consequence, one has to guess a bit. In the case of decoding
    //     XML which is stored in a string, a workarea buffer with capacity
    //     the same size as the input string is used. This will always be
    //     more than required, but is the simplest thing to do to avoid the
    //     need to reallocate the buffer and recopy memory. If the XML is
    //     decoded direct from a stream object, a workarea buffer with a
    //     default capacity of about 8K is used. If the content of any XML
    //     element exceeds this value the buffer will be resized to hold it.
    //     The same buffer is used throughout the XML decoding for that
    //     message so once it has been increased in size to the maximum
    //     required, it will not need to be resized again. If an application
    //     consistently handles requests where XML element content can be
    //     larger than this, the performance of the program will be
    //     suboptimal as it will be necessary to resize the buffer in stages
    //     until all data is read. If this is occuring you can set the
    //     environment variable <OTCLIB_PAYLOADINPUT> to a larger value to
    //     encompass the largest possible size of content in any one XML
    //     element. This will ensure the buffer is adequate from the outset.
    //     Note though that when used with the messaging system, XML is
    //     always decoded from a string object and thus the use of the
    //     environment variable is not required.
    //     
    // = SEE ALSO
    //     <OTC_ROPayload>, <OTC_SVPayload>
{
  public:

    			~OTC_RWPayload() {}

    // = CREATION

			OTC_RWPayload() {}
				// Creates an unbound reference to a node.

    			OTC_RWPayload(OTC_RWPayload const& theWriter)
			  : OTC_ROPayload(theWriter) {}
				// Creates a reference to same node as
				// refered to by <theWriter>.

    			OTC_RWPayload(OTC_DOMNode* theRoot)
			  : OTC_ROPayload(theRoot) {}
				// <theRoot> will be used as the target node
				// for this reference. <theRoot> doesn't
				// necessarily have to be the root of the DOM
				// it is contained in, but it will become the
				// root in respect of operations performed
				// through this instance. Ie., operations
				// involving access to members of a structure
				// or array will be relative to <theRoot>.

    // = ASSIGNMENT

    OTC_RWPayload&	operator=(OTC_RWPayload const& theWriter)
      				{
				    OTC_ROPayload::operator=(theWriter);
				    return *this;
				}
				// Initialises this node reference to the
				// same as <theWriter>.

    // = PROPERTIES
    //     Note that a property name of "name" is reserved for internal use
    //     and an attempt to set or remove this property will be ignored.
    //     The implementation also makes use of the property name "type",
    //     using this to indicate the type of a value when a node is
    //     assigned using a value. There is however no limitation on you
    //     changing the "type" property associated with a value.
    //     
    //     You should avoid using a property name which is invalid according
    //     to the XML rules for attribute names. That is, the first character
    //     must be a letter, underscore or colon. Any subsequent letters must
    //     be one of these or a digit, a hypen or a period. These constraints
    //     are specified such that XML can be used as an encoding format. If
    //     a property name doesn't adhere to the XML naming rules, that
    //     property will not be output when the element is output to XML. You
    //     should also avoid using any name which starts with "xml" in upper
    //     or lower case or any mix as these are reserved for use in XML.
    //     
    //     When setting properties, the data structure element must already
    //     exist. If the element doesn't exist, no new nodes are created
    //     in order to permit the operation to succeed.

    void		setProperty(
			 char const* theName,
			 char const* theValue
			);
				// If the node is valid, associates a
				// property with this node having <theName>
				// and <theValue>. Raises an exception if
				// <theName> is <0>.

    void		setProperty(
			 char const* theName,
			 char const* theValue,
			 size_t theLength
			);
				// If the node is valid, associates a
				// property with this node having <theName>
				// and having a value consisting of the first
				// <theLength> characters of <theValue>.
				// Raises an exception if <theName> is <0> or
				// <theValue> is <0> and <theLength> is not
				// <0>.

    void		setProperty(
			 char const* theName,
			 OTC_DOMString const& theValue
			)
      				{
				  setProperty(theName,
				   theValue.string(),theValue.length());
				}
				// If the node is valid, associates a
				// property with this node having <theName>
				// and <theValue>. Raises an exception if
				// <theName> is <0>.

    void		removeProperty(char const* theName);
    				// If the node is valid and has associated
				// with it a property with <theName>, that
				// property is removed. Raises an exception
				// if <theName> is <0>.

    void		removeAllProperties();
				// Removes all properties which are
				// associated with the node.

    OTC_ROPayload	findProperty(char const* theName) const;
				// Does a preorder traversal of the subtree
				// rooted at the reference node, and returns
				// a reference to the first node which
				// contains a property with <theName>. If
				// no such property is found, an invalid
				// reference is returned. Raises an exception
				// if <theName> is <0>.

    OTC_RWPayload	findProperty(char const* theName);
				// Does a preorder traversal of the subtree
				// rooted at the reference node, and returns
				// a reference to the first node which
				// contains a property with <theName>. If
				// no such property is found, an invalid
				// reference is returned. Raises an exception
				// if <theName> is <0>.

    OTC_ROPayload	findProperty(
			 char const* theName,
			 char const* theValue
			) const;
				// Does a preorder traversal of the subtree
				// rooted at the reference node, and returns
				// a reference to the first node which
				// contains a property with <theName> having
				// <theValue>. If no such property is found,
				// an invalid reference is returned. Raises
				// an exception if <theName> is <0>.

    OTC_RWPayload	findProperty(
			 char const* theName,
			 char const* theValue
			);
				// Does a preorder traversal of the subtree
				// rooted at the reference node, and returns
				// a reference to the first node which
				// contains a property with <theName> having
				// <theValue>. If no such property is found,
				// an invalid reference is returned. Raises
				// an exception if <theName> is <0>.

    // = VALUE

    OTC_RWPayload	assign(
			 char const* theString,
			 char const* theType="xsd:string"
			);
				// Turns this node into a value and sets it
				// to <theString>. If <theType> is not <0> and
				// not empty, the "type" property for the
				// value will be set to <theType>.

    OTC_RWPayload	assign(
			 char const* theString,
			 size_t theLength,
			 char const* theType="xsd:string"
			);
				// Turns this node into a value and sets it
				// to the first <theLength> characters of
				// <theString>. If <theType> is not <0> and not
				// empty, the "type" property for the value
				// will be set to <theType>. Raises an
				// exception if <theString> is <0> and
				// <theLength> is not <0>.

    OTC_RWPayload	assign(
			 OTC_String const& theString,
			 char const* theType="xsd:string"
			)
      				{
				  return assign(theString.string(),
				   theString.length(),theType);
				}
				// Turns this node into a value and sets it
				// to <theString>. If <theType> is not <0>
				// and not empty, the "type" property for the
				// value will be set to <theType>.

    OTC_RWPayload	assign(
			 OTC_DOMString const& theString,
			 char const* theType="xsd:string"
			)
      				{
				  return assign(theString.string(),
				   theString.length(),theType);
				}
				// Turns this node into a value and sets it
				// to <theString>. If <theType> is not <0>
				// and not empty, the "type" property for the
				// value will be set to <theType>.

    void		operator<<=(char const* theString)
      				{ assign(theString); }
				// Turns this node into a value and sets it
				// to <theString>.

    void		operator<<=(OTC_String const& theString)
      				{ assign(theString,theString.length()); }
				// Turns this node into a value and sets it
				// to <theString>.

    void		operator<<=(OTC_TString const& theString)
      				{
				  assign(theString.rawString().string(),
				   theString.rawString().length());
				}
				// Turns this node into a value and sets it
				// to <theString>.

    void		operator<<=(OTC_Symbol const& theSymbol)
      				{
				  assign(theSymbol.string(),
				   theSymbol.length());
				}
				// Turns this node into a value and sets it
				// to <theSymbol>.

    void		operator<<=(OTC_DOMString const& theString)
      				{
				  assign(theString.string(),
				   theString.length());
				}
				// Turns this node into a value and sets it
				// to <theString>.

    OTC_RWPayload	assign(OTC_ROPayload const& theObject);
				// Makes this node a copy of <theObject>.
				// Note that any name associated with
				// <theObject> is ignored and the name
				// associated with the node in this tree to
				// which the assignment is being made is
				// used instead. Any properties associated
				// with <theObject> will however be
				// transfered across. If <theObject> is
				// an invalid node reference, the node
				// being assigned to will be set to have
				// "invalid" node type.

    void		operator<<=(OTC_ROPayload const& theObject)
      				{ assign(theObject); }
				// Makes this node a copy of <theObject>.
				// Note that any name associated with
				// <theObject> is ignored and the name
				// associated with the node in this tree to
				// which the assignment is being made is
				// used instead. Any properties associated
				// with <theObject> will however be
				// transfered across. If <theObject> is
				// an invalid node reference, the node
				// being assigned to will be set to have
				// "invalid" node type.

    // Following will cause a string representation of the specific type
    // to be assigned to the node as per <assign()> member function. In
    // doing this, these functions will set the property "type" as appropriate
    // for the value.

    void		operator<<=(bool theValue);
				// Turns this node into a value and sets it
				// to "false" if <theValue> is <false> and to
				// "true", otherwise. The "type" property
				// will be set to "xsd:boolean" for that
				// value.

    void		operator<<=(long theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:long" if <theValue> is an 8 byte
				// integer on this platform or "xsd:int"
				// otherwise.

    void		operator<<=(int theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:int" for that value.

    void		operator<<=(short theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:short" for that value.

    void		operator<<=(char theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:byte" for that value.

    void		operator<<=(u_long theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:unsignedLong" if <theValue> is an 8
				// byte integer on this platform or
				// "xsd:unsignedInt" otherwise.

    void		operator<<=(u_int theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:unsignedInt" for that value.

    void		operator<<=(u_short theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:unsignedShort" for that value.

    void		operator<<=(u_char theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:unsignedByte" for that value.

#if SIZEOF_LONG_LONG == 8
//#if defined(HAVE_STRTOLL)

    void		operator<<=(long long theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:long" for that value.

    void		operator<<=(unsigned long long theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:unsignedLong" for that value.

//#endif
#endif

    void		operator<<=(double theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:double" for that value.

#if SIZEOF_LONG_DOUBLE != 0
#if SIZEOF_LONG_DOUBLE != SIZEOF_DOUBLE
//#if defined(HAVE_STRTOLD)

    void		operator<<=(long double theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:double" for that value.

//#endif
#endif
#endif

    void		operator<<=(float theValue);
				// Turns this node into a value and sets it
				// the string representation of <theValue>.
				// The "type" property will be set to
				// "xsd:real" for that value.

    void		operator<<=(OTC_Date const& theDate)
      				{ assign(theDate.asString(),"xsd:date"); }
				// Turns this node into a value and sets it
				// the string representation of <theDate>.
				// The "type" property will be set to
				// "xsd:date" for that value.

    void		operator<<=(OTC_Time const& theTime)
      				{ assign(theTime.asString(),"xsd:dateTime"); }
				// Turns this node into a value and sets it
				// the string representation of <theTime>.
				// The "type" property will be set to
				// "xsd:dateTime" for that value.

    void		operator<<=(OTC_Duration const& theDuration)
      				{
				  assign(theDuration.asString(),
				   "xsd:duration");
			        }
				// Turns this node into a value and sets it
				// the string representation of <theDuration>.
				// The "type" property will be set to
				// "xsd:duration" for that value.

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

    OTC_RWPayload	sibling(char const* thePath, size_t theLength);
				// Returns a new reference to the node below
				// this node as described by <thePath> where
				// <thePath> has <theLength>. <thePath> is
				// taken as is, as the node name. Using this
				// function will force the current node
				// reference to be resolved, meaning that the
				// tree structure may be modified in order to
				// make the request valid. If you are wanting
				// to do read only queries, this may have the
				// unwanted side effect of modifying your
				// structure and giving the appearance that
				// nodes exist which didn't. If you are
				// wanting to do read only queries, you
				// should assign the node to an instance of
				// the <OTC_ROPayload> class. Raises an
				// exception if <thePath> is <0>.

    OTC_RWPayload	sibling(char const* thePath)
      				{
				  return sibling(thePath,
				   thePath?strlen(thePath):0);
				}
				// Returns a new reference to the node below
				// this node as described by <thePath>.
				// <thePath> is taken as is, as the node
				// name. Using this function will force the
				// current node reference to be resolved,
				// meaning that the tree structure may be
				// modified in order to make the request
				// valid. If you are wanting to do read only
				// queries, this may have the unwanted side
				// effect of modifying your structure and
				// giving the appearance that nodes exist
				// which didn't. If you are wanting to do
				// read only queries, you should assign the
				// node to an instance of the <OTC_ROPayload>
				// class. Raises an exception if <thePath> is
				// <0>.

    OTC_RWPayload	sibling(OTC_String const& thePath)
      				{
				  return sibling(thePath.string(),
				   thePath.length());
				}
				// Returns a new reference to the node below
				// this node as described by <thePath>.
				// <thePath> is taken as is, as the node
				// name. Using this function will force the
				// current node reference to be resolved,
				// meaning that the tree structure may be
				// modified in order to make the request
				// valid. If you are wanting to do read only
				// queries, this may have the unwanted side
				// effect of modifying your structure and
				// giving the appearance that nodes exist
				// which didn't. If you are wanting to do
				// read only queries, you should assign the
				// node to an instance of the <OTC_ROPayload>
				// class. Raises an exception if <thePath> is
				// <0>.

    OTC_RWPayload	sibling(OTC_DOMString const& thePath)
      				{
				  return sibling(thePath.string(),
				   thePath.length());
				}
				// Returns a new reference to the node below
				// this node as described by <thePath>.
				// <thePath> is taken as is, as the node
				// name. Using this function will force the
				// current node reference to be resolved,
				// meaning that the tree structure may be
				// modified in order to make the request
				// valid. If you are wanting to do read only
				// queries, this may have the unwanted side
				// effect of modifying your structure and
				// giving the appearance that nodes exist
				// which didn't. If you are wanting to do
				// read only queries, you should assign the
				// node to an instance of the <OTC_ROPayload>
				// class. Raises an exception if <thePath> is
				// <0>.

    // Note that the path in the following is interpreted differently to
    // the <sibling()> function. In the following function, the path is
    // interpreted as a hierarchy where the separator is ".".

    OTC_ROPayload	operator[](char const* thePath) const;
				// Returns a new reference to the node below
				// this node as described by <thePath>.
				// <thePath> should be a list of one or more
				// node names separated by ".". For this to
				// work, the root node referred to and any
				// subsequent nodes, with the exception of
				// the final node, as referred to by
				// <thePath> must have node type of "index".
				// If <thePath> doesn't refer to a valid
				// member of an index, an invalid node
				// reference is returned. Raises an exception
				// if <thePath> is <0>.

    OTC_RWPayload	operator[](char const* thePath);
				// Returns a new reference to the node below
				// this node as described by <thePath>. Using
				// this function will force the current node
				// reference to be resolved, meaning that the
				// tree structure may be modified in order to
				// make the request valid. If you are wanting
				// to do read only queries, this may have the
				// unwanted side effect of modifying your
				// structure and giving the appearance that
				// nodes exist which didn't. If you are
				// wanting to do read only queries, you
				// should assign the node to an instance of
				// the <OTC_ROPayload> class. <thePath> should
				// be a list of one or more node names
				// separated by ".". Raises an exception if
				// <thePath> is <0>.

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
				// items in the array or less than <-1> an
				// exception is raised.

    OTC_RWPayload	operator[](int theIndex);
				// Returns a new reference to the node below
				// this node as described by <theIndex>.
				// When applied to a non const object if the
				// node isn't an array it will be made into
				// one. A value for <theIndex> equal to the
				// number of existing items in the array, or
				// a value of <-1> will cause a new item to
				// be appended to the array and that will be
				// the target node. If you are wanting to do
				// read only queries, this may have the
				// unwanted side effect of modifying your
				// structure and giving the appearance that
				// nodes exist which didn't. If you are
				// wanting to do read only queries, you
				// should assign the node to an instance of
				// the <OTC_ROPayload> class. If <theIndex> is
				// greater than the number of items in the
				// array or less than <-1> an exception is
				// raised.

    // = HIERARCHY

    OTC_RWPayload	parent();
				// Returns a reference to the parent of the
				// node referred to by this node reference.
				// If the node refered to is the root node,
				// an invalid node reference is returned.

    OTC_ROPayload	parent() const;
				// Returns a reference to the parent of the
				// node referred to by this node reference.
				// If the node refered to is the root node,
				// an invalid node reference is returned.

    OTC_RWPayload	firstChild();
				// If the node referred to by this node
				// reference has type "index" or "array",
				// returns a node reference to the first
				// element in that object or array.

    OTC_ROPayload	firstChild() const;
				// If the node referred to by this node
				// reference has type "index" or "array",
				// returns a node reference to the first
				// element in that object or array.

    OTC_RWPayload	lastChild();
				// If the node referred to by this node
				// reference has type "index" or "array",
				// returns a node reference to the last
				// element in that object or array.

    OTC_ROPayload	lastChild() const;
				// If the node referred to by this node
				// reference has type "index" or "array",
				// returns a node reference to the last
				// element in that object or array.

    OTC_RWPayload	prevSibling();
				// If the node referred to by this node
				// reference is an element within a "index"
				// or "array", a reference to the previous
				// element is returned. That is, a reference
				// to the previous sibling node of the node
				// referred to by this node reference.

    OTC_ROPayload	prevSibling() const;
				// If the node referred to by this node
				// reference is an element within a "index"
				// or "array", a reference to the previous
				// element is returned. That is, a reference
				// to the previous sibling node of the node
				// referred to by this node reference.

    OTC_RWPayload	nextSibling();
				// If the node referred to by this node
				// reference is an element within a "index"
				// or "array", a reference to the next element
				// is returned. That is, a reference to the
				// next sibling node of the node referred to
				// by this node reference.

    OTC_ROPayload	nextSibling() const;
				// If the node referred to by this node
				// reference is an element within a "index"
				// or "array", a reference to the next element
				// is returned. That is, a reference to the
				// next sibling node of the node referred to
				// by this node reference.

    // = DECODING

    bool		decode(istream& theStream, size_t theCapacity=0);
				// Decodes the data model from its serialised
				// XML form as present on <theStream>. If the
				// decoding fails, <theStream> will be left
				// with the fail bit set and the node will be
				// left with node type "invalid". If the
				// decoding is successful <true> will also be
				// returned and <false> otherwise. To improve
				// decoding performance, <theCapacity> should
				// be a guess at the maximum amount of memory
				// which would need to be used to decode any
				// one element in the XML. By providing this
				// you avoid unecessary memory reallocations
				// and memory copying. If <theCapacity> is
				// <0>, a default value of reasonable size
				// will be used.

    bool		decode(
			 OTC_String const& theString,
			 size_t theCapacity=0
			);
				// Decodes the data model from its serialised
				// XML form as present in <theString>. Note
				// that if there is anything but white space
				// following the first complete encoded
				// data model in <theString>, the decoding
				// will be deemed as have failed. If the
				// decoding is successful <true> will also be
				// returned and <false> otherwise with the
				// node type set to "invalid". To improve
				// decoding performance, <theCapacity> should
				// be a guess at the maximum amount of memory
				// which would need to be used to decode any
				// one element in the XML. By providing this
				// you avoid unecessary memory reallocations
				// and memory copying. If <theCapacity> is
				// <0>, a default value corresponding to the
				// size of <theString> is used. In general
				// with this method there should be no reason
				// to override the value the default for
				// <theCapacity> unless the input is very big
				// but the largest elements are quite small.

  private:

    // All the following return <true> if successful or <false> if a problem
    // was encountered.

    bool		decode_(istream& theStream, OTC_String& theWorkarea);
				// Decodes the data model from its serialised
				// XML form as present on <theStream>. If the
				// decoding fails, <theStream> will be left
				// with the fail bit set and the node will be
				// left with node type "invalid". If the
				// decoding is successful <true> will also be
				// returned and <false> otherwise.
				// <theWorkarea> should be a preallocated
				// working buffer estimated to be large
				// enough to hold the content of any element.
				// If it isn't large enough it will be
				// increased in size as necessary but by
				// having it large enough to start with can
				// avoid reallocations and memory copying.

    bool		decodeElement_(
			 istream& theStream,
			 OTC_String const& theTag,
			 OTC_DOMNode* theNode,
			 OTC_String& theWorkarea
			);
				// Decodes single element with <theTag> from
				// <theStream>, populating <theNode> as
				// necessary. <theWorkarea> should be a
				// preallocated working buffer estimated to
				// be large enough to hold the content of any
				// element. If it isn't large enough it will
				// be increased in size as necessary but by
				// having it large enough to start with can
				// avoid reallocations and memory copying.

    bool		decodeProperty_(
			 istream& theStream,
			 OTC_DOMNode* theNode
			);
				// Decodes a single property associated with
				// an element from <theStream> and associates
				// it with <theNode>.

    bool		decodeChildren_(
			 istream& theStream,
			 OTC_String const& theTag,
			 OTC_DOMNode* theNode,
			 OTC_String& theWorkarea
			);
				// Decodes a sequence of child elements with
				// parent having element with <theTag> from
				// <theStream>, populating <theNode> as
				// necessary. <theWorkarea> should be a
				// preallocated working buffer estimated to
				// be large enough to hold the content of any
				// element. If it isn't large enough it will
				// be increased in size as necessary but by
				// having it large enough to start with can
				// avoid reallocations and memory copying.

    bool		decodeContent_(
			 istream& theStream,
			 OTC_String const& theTag,
			 OTC_DOMNode* theNode,
			 OTC_String& theWorkarea
			);
    				// Decodes the content associated with an
				// element with <theTag> form <theStream>,
				// setting <theNode> as necessary.
				// <theWorkarea> should be a preallocated
				// working buffer estimated to be large
				// enough to hold the content of any element.
				// If it isn't large enough it will be
				// increased in size as necessary but by
				// having it large enough to start with can
				// avoid reallocations and memory copying.

    bool		decodeString_(
			 istream& theStream,
			 char theDelim,
			 OTC_String& theString
			);
				// Reads characters from <theStream> undoing
				// any XML escapes until <theDelim> is
				// encountered. <theDelim> is removed from
				// the stream. The result is appended to
				// <theString>.

    bool		decodeBinary_(
			 istream& theStream,
			 char theDelim,
			 OTC_String& theString
			);
				// Reads characters from <theStream> undoing
				// any base64 encoding until <theDelim> is
				// encountered. <theDelim> is removed from
				// the stream. The result is appended to
				// <theString>.

  private:

    // Don't know if it is a good idea to provied this yet. If structure
    // is used for building up stuff, why would one want to prune away
    // stuff. Don't forget to reenable tests if this is added back.

    // = PRUNING

    void		pruneChild(char const* thePath);
				// Removes the subtree rooted at the node
				// described by <thePath>. Raises an
				// exception if <thePath> is <0>. Doesn't do
				// anything if the node reference is invalid
				// or the path and index don't describe a
				// valid node. Note that if removal of the
				// subtree means the parent of that subtree
				// is now an empty object, that object will
				// also be pruned. This process will continue
				// upwards as necessary. If the root itself
				// now no longer has any elements in it, it
				// will be converted back into an empty
				// value. This process may mean that node
				// references you have may effectively be
				// invalidated. It is recommened that pruning
				// only been done via the top level node.
				// Note that it isn't possible to remove
				// individual elements from an array.

  protected:

    			OTC_RWPayload(
			 OTC_DOMNode* theNode,
			 char const* thePath,
			 size_t theLength,
			 int theIndex
			)
			  : OTC_ROPayload(theNode,thePath,theLength,theIndex)
			  	{}
				// Sets up a reference to a node. <theNode>
				// is the top node of some subtree of the
				// associated hierarchical data structure,
				// with <thePath> being the path below
				// <theNode> giving the actual target data
				// structure element. <theIndex> describes a
				// specific array item where the element
				// described by <thePath> is an array. If
				// <theIndex> is greater than or equal to <0>
				// it identifies a specific item within that
				// array. If <theIndex> is <-1>, it is the
				// last element in the array. If a value
				// less than <-1> is used for <theIndex>, it
				// indicates we aren't talking about an item
				// of an array but the actual data structure
				// element described by <thePath>. <theLength>
				// should be the length of the path or <0> if
				// none is provided.

  private:

    static OTC_NRMutex	mutex_;
				// Mutex to protect access by threads.

    static bool		initialised_;
				// Flag to indicate input buffer size
				// has been initialised.

    static size_t	capacity_;
				// Initial working buffer size into which
				// content of values in XML are read.
};

/* ------------------------------------------------------------------------- */

#include <OTC/dispatch/rwpyload.i>

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_RWPYLOAD_HH */
