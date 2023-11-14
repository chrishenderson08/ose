#ifndef OTC_COLLCTN_AVLLNODE_HH
#define OTC_COLLCTN_AVLLNODE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/avllnode.hh
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

#include <OTC/collctn/avlnode.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/avllnode.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Link;

class OSE_EXPORT OTC_AVLLinkNode : public OTC_AVLNode
    // = TITLE
    //     AVL tree node for holding a reference to a link in a list.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is a derived version of <OTC_AVLNode> and allows
    //     the node to contain a reference to a link within a linked
    //     list. This is used where an AVL tree is being used to build
    //     an index on a linked list.
    //
    // = SEE ALSO
    //     <OTC_AVLNode>, <OTC_AVLTree>, <OTC_Link>
{
  public:

    // = DESTRUCTION

			~OTC_AVLLinkNode();
				// This does not invoke <kill()> on the
				// link held. This must be done explicitly
				// by the user of the class if required.

    // = INITIALISATION

			OTC_AVLLinkNode(OTC_Link* theLink);
				// Creates a node to hold <theLink>.

    OTC_Link*		link()
				{ return link_; }
				// Returns the link held.

    OTC_Link const*	link() const
				{ return link_; }
				// Returns the link held.

    void		setLink(OTC_Link* theLink)
				{ link_ = theLink; }
				// Sets the link held to <theLink>.

  private:

			OTC_AVLLinkNode(OTC_AVLLinkNode const&);
				// Do not define an implementation for this.

    OTC_AVLLinkNode&	operator=(OTC_AVLLinkNode const&);
				// Do not define an implementation for this.

    OTC_Link*		link_;
				// The actual link.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_AVLLNODE_HH */
