#ifndef OTC_DISPATCH_SVPYLOAD_HH
#define OTC_DISPATCH_SVPYLOAD_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/svpyload.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/rwpyload.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/svpyload.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_DOMNode;

class OSE_EXPORT OTC_SVPayload : public OTC_RWPayload
    // = TITLE
    //     Root of data model implemented as a document object model.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_SVPayload> class is the root of a data model implemented
    //     as a document object model. In essence what the class is doing is
    //     enforcing a particular view on top of a document object model such
    //     that arbitrary programming data structures including typed values
    //     can be represented. This functionality is used specifically for
    //     building up a message payload for use in conjunction with the
    //     request/reply and publish/subscribe features of the service agent
    //     framework implemented by <OTC_SVBroker>.
    //     
    //     All functionality of this class is actually inherited from the
    //     classes <OTC_RWPayload> and <OTC_ROPayload>. The only purpose of
    //     this class is to hold the root node and associated memory cluster
    //     which is used for memory allocations by the <OTC_DOMNode> class
    //     used as part of the implementation.
    //
    //     You should take note of warnings related to <OTC_DOMNode> class
    //     with respect to performing modifications on this structure. The
    //     class is mainly intended for application in situations where a
    //     data model is built, serialised and then deleted. To this end,
    //     memory use has been optimised to avoid a large overhead in calling
    //     into system malloc and free routines for lots of small objects,
    //     something which on some systems can be quite bad. What this means
    //     is that when modifications are made to the structure which result
    //     in existing parts of it being deleted, the memory associated with
    //     those parts is only reclaimed when the whole structure is deleted.
    //     
    //     Note that once this class is destroyed, you should not use any
    //     of the references to internal nodes as the data structures they
    //     refer to will have been destroyed.
    //
    // = SEE ALSO
    //     <OTC_RWPayload>, <OTC_ROPayload>, <OTC_DOMNode>, <OTC_SVBroker>
{
  public:

    			~OTC_SVPayload();

    // = CREATION

			OTC_SVPayload();
				// Creates a new data model containing only a
				// root node. The root node will be created
				// with no specific type and will have an
				// empty value. If you want the root node to
				// have a name, you must explicitly set it.

    			OTC_SVPayload(OTC_SVPayload const& theObject);
				// Creates a copy of <theObject>.

    			OTC_SVPayload(OTC_ROPayload const& theObject);
				// Creates a copy of the subtree rooted at
				// <theObject>. If <theObject> is not a valid
				// reference, the result will be the same as
				// if the default constructor is used.

    // = ASSIGNMENT

    OTC_SVPayload&	operator=(OTC_SVPayload const& theObject);
				// Causes the current data model to be
				// destroyed and replaced with a copy of
				// <theObject>.

    OTC_SVPayload&	operator=(OTC_ROPayload const& theObject);
				// Causes the current data model to be
				// destroyed and replaced with a copy of the
				// subtree rooted at <theObject>. If
				// <theObject> is not a valid reference, the
				// result will be the same as when the
				// default constructor is used.

    // = NAME

    void		setName(char const* theName);
				// Explicitly sets the name of the root node
				// to <theName>.

  private:

    OTC_Cluster*	cluster_;
    				// Memory cluster from which all memory
				// is allocated.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SVPYLOAD_HH */
