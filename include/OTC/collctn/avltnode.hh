#ifndef OTC_COLLCTN_AVLTNODE_HH
#define OTC_COLLCTN_AVLTNODE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/avltnode.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/avlnode.hh>
#include <OTC/collctn/avltree.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/avltnode.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_AVLTreeNode : public OTC_AVLNode
    // = TITLE
    //     AVL tree node for holding tree index.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is a derived version of <OTC_AVLNode> and contains a
    //     futher AVL tree.
    //     
    // = SEE ALSO
    //     <OTC_AVLNode>, <OTC_AVLTree>
{
  public:

    // = DESTRUCTION

			~OTC_AVLTreeNode();
				// Deletes the AVL tree. Whether the nodes
				// in the tree delete what they reference
				// will be up to those nodes.

    // = INITIALISATION

			OTC_AVLTreeNode();
				// Creates an empty AVL tree.

    // = QUERY

    OTC_AVLTree*	index()
				{ return &index_; }
				// Returns a pointer to the tree.

    OTC_AVLTree const*	index() const
				{ return &index_; }
				// Returns a pointer to the tree.

  private:

			OTC_AVLTreeNode(OTC_AVLTreeNode const&);
				// Do not define an implementation for this.

    OTC_AVLTreeNode&	operator=(OTC_AVLTreeNode const&);
				// Do not define an implementation for this.

    OTC_AVLTree		index_;
				// The tree.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_AVLTNODE_HH */
