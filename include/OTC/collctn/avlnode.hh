#ifndef OTC_COLLCTN_AVLNODE_HH
#define OTC_COLLCTN_AVLNODE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/avlnode.hh
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
#pragma interface "OTC/collctn/avlnode.hh"
#endif
#endif

#ifdef index
#undef index
#endif

/* ------------------------------------------------------------------------- */

class OTC_AVLTree;

class OSE_EXPORT OTC_AVLNode : public OTC_MPObject
    // = TITLE
    //	   Base class for nodes within an AVL tree.
    //
    // = CLASS TYPE
    //	   Abstract
    //
    // = DESCRIPTION
    //	   This class is the base class for nodes within an AVL tree.
    //	   All nodes to be placed into such a tree must be derived
    //	   from this class.
    //
    //	   It is this class which encapsulates all the operations which
    //	   can be performed on a tree and nodes within it. Of these
    //	   operations, those which involve insertion or deletion of nodes
    //	   will automatically rebalance the tree as required.
    //
    // = SEE ALSO
    //	   <OTC_AVLTree>
{
    friend		class OTC_AVLTree;

  public:

    // = DESTRUCTION

    virtual		~OTC_AVLNode();
				// Recursively deletes the left and
				// right subtrees of this node.
				// A node should only be explicitly
				// deleted if it is a root node, or
				// not contained within a tree at all.
				// If this condition is violated,
				// an exception is generated.

    // = QUERY

    OTC_AVLNode*	father()
				{ return father_; }
				// Returns the father of this node.
				// If this node is the root node within
				// a tree, or isn't contained within
				// a tree, then <0> is returned.

    OTC_AVLNode const*	father() const
				{ return father_; }
				// Returns the father of this node.
				// If this node is the root node within
				// a tree, or isn't contained within
				// a tree, then <0> is returned.

    OTC_AVLTree*	tree()
				{ return tree_; }
				// If the node is contained within a tree,
				// the pointer to the tree is returned,
				// otherwise <0> is returned.

    OTC_AVLTree const*	tree() const
				{ return tree_; }
				// If the node is contained within a tree,
				// the pointer to the tree is returned,
				// otherwise <0> is returned.

    bool		isRoot() const
				{ return father() == 0; }
				// Returns <true> if this node is a root node
				// of a tree. Will also return <true> if the
				// node is not contained within a tree at
				// all.

    OTC_AVLNode*	left()
				{ return left_; }
				// Returns the node which is the root
				// of the left subtree of this node, or
				// <0> if there is no left subtree.

    OTC_AVLNode const*	left() const
				{ return left_; }
				// Returns the node which is the root
				// of the left subtree of this node, or
				// <0> if there is no left subtree.

    OTC_AVLNode*	right()
				{ return right_; }
				// Returns the node which is the root
				// of the right subtree of this node, or
				// <0> if there is no right subtree.

    OTC_AVLNode const*	right() const
				{ return right_; }
				// Returns the node which is the root
				// of the right subtree of this node, or
				// <0> if there is no right subtree.

    bool		isLeft() const
				{
				  return (father_ == 0) ? false
				   : (father_->left_ == this);
				}
				// Returns <true> if this node is the root
				// node of the left subtree of its father
				// node. This will always return <false> if
				// this node is a root node, or is not
				// contained within a tree.

    bool		isRight() const
				{
				  return (father_ == 0) ? false
				   : (father_->right_ == this);
				}
				// Returns <true> if this node is the root
				// node of the right subtree of its father
				// node. This will always return <false> if
				// this node is a root node, or is not
				// contained within a tree.

    OTC_AVLNode*	brother()
				{ return brother_(); }
				// Returns the brother node of this node.
				// The brother node is the node at the same
				// level, in the alternate subtree of the
				// father. If this node is the root node,
				// or there is no brother node, then <0>
				// is returned.

    OTC_AVLNode const*	brother() const
				{ return brother_(); }
				// Returns the brother node of this node.
				// The brother node is the node at the same
				// level, in the alternate subtree of the
				// father. If this node is the root node,
				// or there is no brother node, then <0>
				// is returned.

    u_int		level() const;
				// Returns the level of this node in the
				// tree. The root node has a level of <0>.

    u_int		count() const
				{ return count_; }
				// Returns the count of the number of nodes
				// in the tree, plus one, to the left of this
				// node. Thus for a node with an empty
				// left subtree, this will return <1>.

    u_int		index() const;
				// Returns the index of this node in the
				// tree based on an inorder traversal.

    u_int		height() const;
				// Returns the height of the tree rooted
				// at this node.

    OTC_AVLNode*	first()
				{ return first_(); }
				// Returns the first node, base on an inorder
				// traversal, in the tree rooted at this
				// node.

    OTC_AVLNode const*	first() const
				{ return first_(); }
				// Returns the first node, base on an inorder
				// traversal, in the tree rooted at this
				// node.

    OTC_AVLNode*	last()
				{ return last_(); }
				// Returns the last node, base on an inorder
				// traversal, in the tree rooted at this
				// node.

    OTC_AVLNode const*	last() const
				{ return last_(); }
				// Returns the last node, base on an inorder
				// traversal, in the tree rooted at this
				// node.

    OTC_AVLNode*	prev()
				{ return prev_(); }
				// Returns the previous node, base on an
				// inorder traversal, in the tree rooted at
				// this node.

    OTC_AVLNode const*	prev() const
				{ return prev_(); }
				// Returns the previous node, base on an
				// inorder traversal, in the tree rooted at
				// this node.

    OTC_AVLNode*	next()
				{ return next_(); }
				// Returns the next node, base on an inorder
				// traversal, in the tree rooted at this
				// node.

    OTC_AVLNode const*	next() const
				{ return next_(); }
				// Returns the next node, base on an inorder
				// traversal, in the tree rooted at this
				// node.

    // = MODIFICATION

    void		addBefore(OTC_AVLNode* theNode);
				// Adds <theNode> before this node in the
				// tree, based on an inorder traversal. Doing
				// this, will automatcially result in
				// the tree being rebalanced. Note that
				// due to the nature of balanced trees,
				// <theNode> will not necessarily end up
				// being the left node of this node.
				// <theNode> must not already be in a
				// tree; if it is, an exception is raised.

    void		addAfter(OTC_AVLNode* theNode);
				// Adds <theNode> after this node in the
				// tree, based on an inorder traversal.
				// Doing this, will automatcially result in
				// the tree being rebalanced. Note that
				// due to the nature of balanced trees,
				// <theNode> will not necessarily end up
				// being the right node of this node.
				// <theNode> must not already be in a
				// tree; if it is, an exception is raised.

    void		swap(OTC_AVLNode* theNode);
				// Swap the location of <theNode> and
				// this node. This node must be in a tree,
				// if it isn't, an exception is raised.

    void		unlink();
				// Unlinks this node from the tree and
				// adjusts the balance of the tree
				// accordingly. Note that the node is
				// not deleted, this is the responsibility
				// of the user after it has been unlinked.

  protected:

			OTC_AVLNode();
				// Initialises everything. Can only be
				// called from derived class.

  private:

			OTC_AVLNode(OTC_AVLNode const&);
				// Do not define an implementation for this.

    OTC_AVLNode&	operator=(OTC_AVLNode const&);
				// Do not define an implementation for this.

    void		addBalance_();
				// Rebalances the tree after an insertion.

    void		unlinkBalance_();
				// Rebalances the tree after a deletion.

    void		rotateLeft_();

    void		rotateRight_();

    void		leftBalance_();

    void		rightBalance_();

    void		setTree_(OTC_AVLTree* theTree)
				{ tree_ = theTree; }
				// Sets the tree that this node is contained
				// in to <theTree>.

    void		setFather_(OTC_AVLNode* theNode)
				{ father_ = theNode; }
				// Sets the father of this node to <theNode>.

    void		setLeft_(OTC_AVLNode* theNode)
				{ left_ = theNode; }
				// Sets the left child of this node to
				// <theNode>.

    void		setRight_(OTC_AVLNode* theNode)
				{ right_ = theNode; }
				// Sets the right child of this node to
				// <theNode>.

    void		setBalance_(int theBalance)
				{ balance_ = theBalance; }
				// Sets the balance factor of this node
				// to <theBalance>.

    void		setCount_(u_int theCount)
				{ count_ = theCount; }
				// Sets the left node count of this
				// node to <theCount>.

    OTC_AVLNode*	brother_() const;
				// Returns the brother node of this node.
				// The brother node is the node at the same
				// level, in the alternate subtree of the
				// father. If this node is the root node,
				// or there is no brother node, then <0>
				// is returned.

    OTC_AVLNode*        first_() const;
                                // Returns the first node, base on an inorder
                                // traversal, in the tree rooted at this
                                // node.

    OTC_AVLNode*        last_() const;
                                // Returns the last node, base on an inorder
                                // traversal, in the tree rooted at this
                                // node.

    OTC_AVLNode*        prev_() const;
                                // Returns the previous node, base on an
                                // inorder traversal, in the tree rooted at
                                // this node.

    OTC_AVLNode*        next_() const;
                                // Returns the next node, base on an inorder
                                // traversal, in the tree rooted at this
                                // node.

    OTC_AVLTree*	tree_;
				// The tree that this node is contained in.

    OTC_AVLNode*	father_;
				// The father of this node.

    OTC_AVLNode*	left_;
				// The left child of this node.

    OTC_AVLNode*	right_;
				// The right child of this node.

    int			balance_;
				// The balance factor of this node.

    u_int		count_;
				// The left node count of this node.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_AVLNODE_HH */
