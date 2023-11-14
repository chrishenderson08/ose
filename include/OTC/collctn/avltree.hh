#ifndef OTC_COLLCTN_AVLTREE_HH
#define OTC_COLLCTN_AVLTREE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/avltree.hh
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
#include <OTC/refcnt/resource.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/avltree.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_AVLTree : public OTC_Resource
    // = TITLE
    //	   The root class of a height balanced AVL tree.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class is the root class of a height balanced AVL tree.
    //	   It is this class which holds the root node in the tree and
    //	   maintains the population count for the tree.
    //	   
    //	   The generic AVL tree structure does not actually provide the
    //	   capability to hold any items. Instead it only maintains the
    //	   balance of the tree and ensures the integrity of the tree
    //	   structure. In order for the tree structure to actually hold
    //	   items, a specific node class must be derived from the
    //	   <OTC_AVLNode> class. An example of such a class is the
    //	   <OTC_AVLLinkNode> class, which holds a pointer to a link
    //	   within a linked list.
    //	   
    // = SEE ALSO
    //	   <OTC_AVLNode>, <OTC_AVLLinkNode>
{
    friend		class OTC_AVLNode;

  public:

    // = DESTRUCTION

			~OTC_AVLTree();
				// Deletes all nodes in the tree.

			OTC_AVLTree();
				// Creates an empty tree. When the tree
				// is empty, the user must explicitly add
				// the initial root node.

    bool		isEmpty() const
				{ return population_ == 0; }
				// Returns <true> if the tree is empty.

    u_int		population() const
				{ return population_; }
				// Returns the number of nodes in the tree.

    void		removeAll();
				// Removes and deletes all nodes in the tree.

    OTC_AVLNode*	root()
				{ return root_; }
				// Returns the root node, or <0> if the tree
				// is empty.

    OTC_AVLNode const*	root() const
				{ return root_; }
				// Returns the root node, or <0> if the tree
				// is empty.

    void		addRoot(OTC_AVLNode* theNode);
				// Adds the initial root node to the tree
				// and sets the population count to <1>.
				// If this is invoked when the tree is not
				// empty, an exception is raised.

    int			depth() const;
				// Returns the depth of the tree. A tree
				// with a single root node is regarded as
				// having a depth of <0>. If the tree is
				// empty, then <-1> is returned.

    OTC_AVLNode*	node(u_int theIndex)
				{ return node_(theIndex); }
				// If the tree is empty, then <0> is returned.
				// Otherwise the node with index <theIndex>,
				// based on an inorder traversal, is returned.
				// Indexes start at <0>.

    OTC_AVLNode const*	node(u_int theIndex) const
				{ return node_(theIndex); }
				// If the tree is empty, then <0> is returned.
				// Otherwise the node with index <theIndex>,
				// based on an inorder traversal, is returned.
				// Indexes start at <0>.

    OTC_AVLNode*	first()
				{ return first_(); }
				// Returns the first node, base on an inorder
				// traversal, in the tree.

    OTC_AVLNode const*	first() const
				{ return first_(); }
				// Returns the first node, base on an inorder
				// traversal, in the tree.

    OTC_AVLNode*	last()
				{ return last_(); }
				// Returns the last node, base on an inorder
				// traversal, in the tree.

    OTC_AVLNode const*	last() const
				{ return last_(); }
				// Returns the last node, base on an inorder
				// traversal, in the tree.

  private:

			OTC_AVLTree(OTC_AVLTree const&);
				// Do not define an implementation for this.

    OTC_AVLTree&	operator=(OTC_AVLTree const&);
				// Do not define an implementation for this.

    void		setRoot_(OTC_AVLNode* theNode)
				{ root_ = theNode; }
				// Sets the root node to <theNode>.

    void		setPopulation_(u_int thePopulation)
				{ population_ = thePopulation; }
				// Sets the population to <thePopulation>.

    OTC_AVLNode*	node_(u_int theIndex) const;
				// If the tree is empty, then <0> is returned.
				// Otherwise the node with index <theIndex>,
				// based on an inorder traversal, is returned.
				// Indexes start at <0>.

    OTC_AVLNode*	first_() const;
				// Returns the first node, base on an inorder
				// traversal, in the tree.

    OTC_AVLNode*	last_() const;
				// Returns the last node, base on an inorder
				// traversal, in the tree.

    OTC_AVLNode*	root_;
				// Pointer to the root node of the tree.

    u_int		population_;
				// The number of nodes in the tree.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_AVLTREE_HH */
