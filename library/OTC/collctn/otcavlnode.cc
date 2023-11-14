/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcavlnode.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/avlnode.hh"
#endif
#endif

#include <OTC/collctn/avlnode.hh>
#include <OTC/collctn/avltree.hh>

#ifdef index
#undef index
#endif

/* ------------------------------------------------------------------------- */
enum
{
  OTCLIB_LEFTHEAVY = -2,
  OTCLIB_LEFTHIGH = -1,
  OTCLIB_EQUALHEIGHT = 0,
  OTCLIB_RIGHTHIGH = 1,
  OTCLIB_RIGHTHEAVY = 2
};

/* ------------------------------------------------------------------------- */
OTC_AVLNode::OTC_AVLNode()
  : tree_(0), father_(0), left_(0), right_(0),
    balance_(OTCLIB_EQUALHEIGHT), count_(1)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode::~OTC_AVLNode()
{
  OTCLIB_ENSURE_FN((isRoot()),
   "OTC_AVLNode::~OTC_AVLNode()",
   "only root node can be deleted");

  if (left_ != 0)
  {
    left_->setFather_(0);
    left_->setTree_(0);
    delete left_;
  }
  if (right_ != 0)
  {
    right_->setFather_(0);
    right_->setTree_(0);
    delete right_;
  }

  if (tree_ != 0)
  {
    tree_->setRoot_(0);
    tree_->setPopulation_(0);
  }
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode* OTC_AVLNode::brother_() const
{
  if (isRoot())
    return 0;

  if (isLeft())
    return (OTC_AVLNode*)father()->right();

  if (isRight())
    return (OTC_AVLNode*)father()->left();

  return 0;
}

/* ------------------------------------------------------------------------- */
u_int OTC_AVLNode::level() const
{
  u_int theLevel = 0;
  OTC_AVLNode const* theNode = father();
  while (theNode != 0)
  {
    theNode = theNode->father();
    theLevel++;
  }
  return theLevel;
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::addBefore(OTC_AVLNode* theNode)
{
  OTCLIB_ENSURE_FN((tree() != 0),
   "OTC_AVLNode::addBefore(OTC_AVLNode*)",
   "this node must be in a tree");
  OTCLIB_ENSURE_FN((theNode != 0),
   "OTC_AVLNode::addBefore(OTC_AVLNode*)",
   "must have valid object to insert");
  OTCLIB_ENSURE_FN((theNode->tree() == 0),
   "OTC_AVLNode::addBefore(OTC_AVLNode*)",
   "node is already in a tree");

  if (left() != 0)
  {
    OTC_AVLNode* theParent = left();
    OTC_AVLNode* tmpNode = theParent->right();
    while (tmpNode != 0)
    {
      theParent = tmpNode;
      tmpNode = tmpNode->right();
    }
    theParent->addAfter(theNode);
  }
  else
  {
    theNode->setBalance_(OTCLIB_EQUALHEIGHT);
    theNode->setTree_(tree());
    theNode->setFather_(this);
    theNode->setCount_(1);
    setLeft_(theNode);
    setBalance_(balance_-1);

    setCount_(count()+1);
    OTC_AVLNode* tmpNode = father();
    bool childIsLeft = isLeft();
    while (tmpNode != 0)
    {
      if (childIsLeft != false)
	tmpNode->setCount_(tmpNode->count()+1);
      childIsLeft = tmpNode->isLeft();
      tmpNode = tmpNode->father();
    }

    addBalance_();
    tree()->population_++;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::addAfter(OTC_AVLNode* theNode)
{
  OTCLIB_ENSURE_FN((tree() != 0),
   "OTC_AVLNode::addAfter(OTC_AVLNode*)",
   "this node must be in a tree");
  OTCLIB_ENSURE_FN((theNode != 0),
   "OTC_AVLNode::addAfter(OTC_AVLNode*)",
   "must have valid object to insert");
  OTCLIB_ENSURE_FN((theNode->tree() == 0),
   "OTC_AVLNode::addAfter(OTC_AVLNode*)",
   "node is already in a tree");

  if (right() != 0)
  {
    OTC_AVLNode* theParent = right();
    OTC_AVLNode* tmpNode = theParent->left();
    while (tmpNode != 0)
    {
      theParent = tmpNode;
      tmpNode = tmpNode->left();
    }
    theParent->addBefore(theNode);
  }
  else
  {
    theNode->setBalance_(OTCLIB_EQUALHEIGHT);
    theNode->setTree_(tree());
    theNode->setFather_(this);
    theNode->setCount_(1);
    setRight_(theNode);
    setBalance_(balance_+1);

    OTC_AVLNode* tmpNode = father();
    bool childIsLeft = isLeft();
    while (tmpNode != 0)
    {
      if (childIsLeft != false)
	tmpNode->setCount_(tmpNode->count()+1);
      childIsLeft = tmpNode->isLeft();
      tmpNode = tmpNode->father();
    }

    addBalance_();
    tree()->population_++;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::swap(OTC_AVLNode* theNode)
{
  OTCLIB_ENSURE_FN((tree() != 0),
   "OTC_AVLNode::swap(OTC_AVLNode*)",
   "this node must be in a tree");
  OTCLIB_ENSURE_FN((theNode != 0),
   "OTC_AVLNode::swap(OTC_AVLNode*)",
   "must have a valid object to insert");

  if (father() == 0)
    tree()->setRoot_(theNode);

  if (theNode->father() == 0 && theNode->tree() != 0)
    theNode->tree()->setRoot_(this);

  OTC_AVLTree* theTree = theNode->tree();
  theNode->setTree_(tree());
  setTree_(theTree);

  int theBalance = theNode->balance_;
  theNode->setBalance_(balance_);
  setBalance_(theBalance);

  u_int theCount = theNode->count();
  theNode->setCount_(count());
  setCount_(theCount);

  if (father() == theNode || theNode->father() == this)
  {
    // One is the direct child of the other.

    OTC_AVLNode* topNode = 0;
    OTC_AVLNode* bottomNode = 0;

    if (father() == theNode)
    {
      topNode = theNode;
      bottomNode = this;
    }
    else
    {
      topNode = this;
      bottomNode = theNode;
    }

    OTC_AVLNode* theFather = topNode->father();

    topNode->setFather_(bottomNode);
    bottomNode->setFather_(theFather);

    if (theFather != 0)
    {
      if (theFather->right() == topNode)
	theFather->setRight_(bottomNode);
      else if (theFather->left() == topNode)
	theFather->setLeft_(bottomNode);
    }

    OTC_AVLNode* leftNode = bottomNode->left();
    OTC_AVLNode* rightNode = bottomNode->right();

    if (topNode->left() == bottomNode)
    {
      bottomNode->setLeft_(topNode);
      bottomNode->setRight_(topNode->right());
      if (topNode->right() != 0)
	topNode->right()->setFather_(bottomNode);
    }
    else if (topNode->right() == bottomNode)
    {
      bottomNode->setRight_(topNode);
      bottomNode->setLeft_(topNode->left());
      if (topNode->left() != 0)
	topNode->left()->setFather_(bottomNode);
    }

    topNode->setLeft_(leftNode);
    if (leftNode != 0)
      leftNode->setFather_(topNode);
    topNode->setRight_(rightNode);
    if (rightNode != 0)
      rightNode->setFather_(topNode);
  }
  else
  {
    // No father child relationship.

    OTC_AVLNode* theFather = theNode->father();

    if (father_ == 0)
    {
      theNode->setFather_(0);
    }
    else
    {
      if (father_->left() == this)
	father_->setLeft_(theNode);
      else if (father_->right() == this)
	father_->setRight_(theNode);
      if (theNode != 0)
	theNode->setFather_(father_);
    }

    if (theFather == 0)
    {
      setFather_(0);
    }
    else
    {
      if (theFather->left() == theNode)
	theFather->setLeft_(this);
      else if (theFather->right() == theNode)
	theFather->setRight_(this);
      setFather_(theFather);
    }

    OTC_AVLNode* theLeft = theNode->left();
    OTC_AVLNode* theRight = theNode->right();

    theNode->setLeft_(left());
    if (left() != 0)
      left()->setFather_(theNode);
    theNode->setRight_(right());
    if (right() != 0)
      right()->setFather_(theNode);

    setLeft_(theLeft);
    if (theLeft != 0)
      theLeft->setFather_(this);
    setRight_(theRight);
    if (theRight != 0)
      theRight->setFather_(this);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::unlink()
{
  OTCLIB_ENSURE_FN((tree() != 0),
   "OTC_AVLNode::unlink()",
   "this node must be in a tree");

  if (left() != 0 && right() != 0)
  {
    OTC_AVLNode* theParent = left();
    OTC_AVLNode* theNode = theParent->right();
    while (theNode != 0)
    {
      theParent = theNode;
      theNode = theNode->right();
    }
    theParent->swap(this);
  }

  OTC_AVLNode* tmpNode = father();
  bool childIsLeft = isLeft();
  while (tmpNode != 0)
  {
    if (childIsLeft != false)
      tmpNode->setCount_(tmpNode->count() - 1);
    childIsLeft = tmpNode->isLeft();
    tmpNode = tmpNode->father();
  }

  OTCLIB_ASSERT(left() == 0 || right() == 0);

  OTC_AVLNode* childNode = 0;

  if (left() != 0)
    childNode = left();
  else if (right() != 0)
    childNode = right();

  OTC_AVLNode* fatherNode = father();

  if (fatherNode == 0)
  {
    if (childNode != 0)
    {
      tree()->setRoot_(childNode);
      childNode->setFather_(0);
    }
    else
      tree()->setRoot_(0);
  }
  else
  {
    if (fatherNode->left() == this)
    {
      fatherNode->setLeft_(childNode);
      fatherNode->setBalance_(fatherNode->balance_+1);
    }
    else if (fatherNode->right() == this)
    {
      fatherNode->setRight_(childNode);
      fatherNode->setBalance_(fatherNode->balance_-1);
    }
    if (childNode != 0)
      childNode->setFather_(fatherNode);
  }

  if (fatherNode != 0)
    fatherNode->unlinkBalance_();

  tree()->population_--;

  setTree_(0);
  setFather_(0);
  setBalance_(OTCLIB_EQUALHEIGHT);
  setCount_(1);
  setRight_(0);
  setLeft_(0);
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::addBalance_()
{
  OTCLIB_ASSERT(balance_ >= OTCLIB_LEFTHEAVY);
  OTCLIB_ASSERT(balance_ <= OTCLIB_RIGHTHEAVY);

  switch (balance_)
  {
    case OTCLIB_LEFTHEAVY:
    {
      leftBalance_();
      break;
    }

    case OTCLIB_LEFTHIGH:
    case OTCLIB_RIGHTHIGH:
    {
      if (!isRoot())
      {
	if (isLeft())
	{
	  father()->setBalance_(father()->balance_-1);
	  father()->addBalance_();
	}
	else if (isRight())
	{
	  father()->setBalance_(father()->balance_+1);
	  father()->addBalance_();
	}
      }
      break;
    }

    case OTCLIB_EQUALHEIGHT:
    {
      break;
    }

    case OTCLIB_RIGHTHEAVY:
    {
      rightBalance_();
      break;
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::unlinkBalance_()
{
  OTCLIB_ASSERT(balance_ >= OTCLIB_LEFTHEAVY);
  OTCLIB_ASSERT(balance_ <= OTCLIB_RIGHTHEAVY);

  switch (balance_)
  {
    case OTCLIB_LEFTHEAVY:
    {
      OTC_AVLNode* fatherNode = father();
      OTC_AVLNode* leftNode = left();

      switch (leftNode->balance_)
      {
	case OTCLIB_EQUALHEIGHT:
	{
	  rotateRight_();
	  setBalance_(OTCLIB_LEFTHIGH);
	  leftNode->setBalance_(OTCLIB_RIGHTHIGH);
	  break;
	}

	case OTCLIB_LEFTHIGH:
	{
	  rotateRight_();
	  setBalance_(OTCLIB_EQUALHEIGHT);
	  leftNode->setBalance_(OTCLIB_EQUALHEIGHT);
	  if (fatherNode != 0)
	  {
	    if (leftNode->isLeft())
	      fatherNode->setBalance_(fatherNode->balance_+1);
	    else if (leftNode->isRight())
	      fatherNode->setBalance_(fatherNode->balance_-1);
	    fatherNode->unlinkBalance_();
	  }
	  break;
	}

	case OTCLIB_RIGHTHIGH:
	{
	  OTC_AVLNode* leftRightNode = leftNode->right();
	  leftBalance_();
	  if (fatherNode != 0)
	  {
	    if (leftRightNode->isLeft())
	      fatherNode->setBalance_(fatherNode->balance_+1);
	    else if (leftRightNode->isRight())
	      fatherNode->setBalance_(fatherNode->balance_-1);
	    fatherNode->unlinkBalance_();
	  }
	  break;
	}
      }
      break;
    }

    case OTCLIB_LEFTHIGH:
    case OTCLIB_RIGHTHIGH:
    {
      break;
    }

    case OTCLIB_EQUALHEIGHT:
    {
      if (father() != 0)
      {
	if (isLeft())
	{
	  father()->setBalance_(father()->balance_+1);
	  father()->unlinkBalance_();
	}
	else if (isRight())
	{
	  father()->setBalance_(father()->balance_-1);
	  father()->unlinkBalance_();
	}
      }
      break;
    }

    case OTCLIB_RIGHTHEAVY:
    {
      OTC_AVLNode* fatherNode = father();
      OTC_AVLNode* rightNode = right();

      switch (rightNode->balance_)
      {
	case OTCLIB_EQUALHEIGHT:
	{
	  rotateLeft_();
	  setBalance_(OTCLIB_RIGHTHIGH);
	  rightNode->setBalance_(OTCLIB_LEFTHIGH);
	  break;
	}

	case OTCLIB_LEFTHIGH:
	{
	  OTC_AVLNode* rightLeftNode = rightNode->left();
	  rightBalance_();
	  if (fatherNode != 0)
	  {
	    if (rightLeftNode->isLeft())
	      fatherNode->setBalance_(fatherNode->balance_+1);
	    else if (rightLeftNode->isRight())
	      fatherNode->setBalance_(fatherNode->balance_-1);
	    fatherNode->unlinkBalance_();
	  }
	  break;
	}

	case OTCLIB_RIGHTHIGH:
	{
	  rotateLeft_();
	  setBalance_(OTCLIB_EQUALHEIGHT);
	  rightNode->setBalance_(OTCLIB_EQUALHEIGHT);
	  if (fatherNode != 0)
	  {
	    if (rightNode->isLeft())
	      fatherNode->setBalance_(fatherNode->balance_+1);
	    else if (rightNode->isRight())
	      fatherNode->setBalance_(fatherNode->balance_-1);
	    fatherNode->unlinkBalance_();
	  }
	  break;
	}
      }
      break;
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::rotateLeft_()
{
  OTC_AVLNode* fatherNode = father();
  OTC_AVLNode* rightNode = right();
  OTC_AVLNode* rightLeftNode = right()->left();
  setRight_(rightLeftNode);
  if (rightLeftNode != 0)
    rightLeftNode->setFather_(this);
  rightNode->setLeft_(this);
  setFather_(rightNode);
  rightNode->setFather_(fatherNode);

  if (fatherNode == 0)
    tree()->setRoot_(rightNode);
  else if (fatherNode->left() == this)
    fatherNode->setLeft_(rightNode);
  else if (fatherNode->right() == this)
    fatherNode->setRight_(rightNode);

  rightNode->setCount_(rightNode->count()+count());
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::rotateRight_()
{
  OTC_AVLNode* fatherNode = father();
  OTC_AVLNode* leftNode = left();
  OTC_AVLNode* leftRightNode = left()->right();
  setLeft_(leftRightNode);
  if (leftRightNode != 0)
    leftRightNode->setFather_(this);
  leftNode->setRight_(this);
  setFather_(leftNode);
  leftNode->setFather_(fatherNode);

  if (fatherNode == 0)
    tree()->setRoot_(leftNode);
  else if (fatherNode->left() == this)
    fatherNode->setLeft_(leftNode);
  else if (fatherNode->right() == this)
    fatherNode->setRight_(leftNode);

  setCount_(count() - leftNode->count());
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::leftBalance_()
{
  OTC_AVLNode* leftNode = left();

  OTCLIB_ASSERT(leftNode != 0);
  OTCLIB_ASSERT(leftNode->balance_ != OTCLIB_EQUALHEIGHT);

  switch (leftNode->balance_)
  {
    case OTCLIB_LEFTHIGH:
    {
      setBalance_(OTCLIB_EQUALHEIGHT);
      leftNode->setBalance_(OTCLIB_EQUALHEIGHT);
      rotateRight_();
      break;
    }

    case OTCLIB_RIGHTHIGH:
    {
      OTC_AVLNode* leftRightNode = leftNode->right();

      OTCLIB_ASSERT(leftRightNode != 0);

      switch (leftRightNode->balance_)
      {
        case OTCLIB_EQUALHEIGHT:
        {
          setBalance_(OTCLIB_EQUALHEIGHT);
          leftNode->setBalance_(OTCLIB_EQUALHEIGHT);
          break;
        }

        case OTCLIB_LEFTHIGH:
        {
          setBalance_(OTCLIB_RIGHTHIGH);
          leftNode->setBalance_(OTCLIB_EQUALHEIGHT);
          break;
        }

        case OTCLIB_RIGHTHIGH:
        {
          setBalance_(OTCLIB_EQUALHEIGHT);
          leftNode->setBalance_(OTCLIB_LEFTHIGH);
          break;
        }
      }
      leftRightNode->setBalance_(OTCLIB_EQUALHEIGHT);
      leftNode->rotateLeft_();
      rotateRight_();
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_AVLNode::rightBalance_()
{
  OTC_AVLNode* rightNode = right();

  OTCLIB_ASSERT(rightNode != 0);
  OTCLIB_ASSERT(rightNode->balance_ != OTCLIB_EQUALHEIGHT);

  switch (rightNode->balance_)
  {
    case OTCLIB_RIGHTHIGH:
    {
      setBalance_(OTCLIB_EQUALHEIGHT);
      rightNode->setBalance_(OTCLIB_EQUALHEIGHT);
      rotateLeft_();
      break;
    }

    case OTCLIB_LEFTHIGH:
    {
      OTC_AVLNode* rightLeftNode = rightNode->left();

      OTCLIB_ASSERT(rightLeftNode != 0);

      switch (rightLeftNode->balance_)
      {
        case OTCLIB_EQUALHEIGHT:
        {
          setBalance_(OTCLIB_EQUALHEIGHT);
          rightNode->setBalance_(OTCLIB_EQUALHEIGHT);
          break;
        }

        case OTCLIB_LEFTHIGH:
        {
          setBalance_(OTCLIB_EQUALHEIGHT);
          rightNode->setBalance_(OTCLIB_RIGHTHIGH);
          break;
        }

        case OTCLIB_RIGHTHIGH:
        {
          setBalance_(OTCLIB_LEFTHIGH);
          rightNode->setBalance_(OTCLIB_EQUALHEIGHT);
          break;
        }
      }
      rightLeftNode->setBalance_(OTCLIB_EQUALHEIGHT);
      rightNode->rotateRight_();
      rotateLeft_();
    }
  }
}

/* ------------------------------------------------------------------------- */
u_int OTC_AVLNode::index() const
{
  u_int theIndex = 0;
  u_int tmpCount = count();
  bool childIsLeft = isLeft();
  OTC_AVLNode const* theNode = father();
  while (theNode != 0)
  {
    if (theNode->isLeft())
    {
      if (childIsLeft == false)
      {
	theIndex += tmpCount;
	tmpCount = theNode->count();
	childIsLeft = true;
      }
    }
    else
    {
      theIndex += tmpCount;
      if (childIsLeft != false)
      {
	childIsLeft = false;
	tmpCount = 0;
      }
      else
	tmpCount = theNode->count();
    }
    theNode = theNode->father();
  }
  theIndex += tmpCount;
  return theIndex-1;
}

/* ------------------------------------------------------------------------- */
u_int OTC_AVLNode::height() const
{
  u_int theHeight = 0;
  OTC_AVLNode const* theNode = this;
  while (theNode != 0)
  {
    theHeight++;

    OTCLIB_ASSERT(theNode->balance_ >= OTCLIB_LEFTHIGH);
    OTCLIB_ASSERT(theNode->balance_ <= OTCLIB_RIGHTHIGH);

    switch (theNode->balance_)
    {
      case OTCLIB_LEFTHIGH:
      {
        theNode = theNode->left();
        break;
      }
 
      case OTCLIB_EQUALHEIGHT:
      {
        theNode = theNode->left();
        break;
      }
 
      case OTCLIB_RIGHTHIGH:
      {
        theNode = theNode->right();
        break;
      }
    }
  }
  return theHeight;
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode* OTC_AVLNode::first_() const
{
  OTC_AVLNode const* theNode = this;
  OTC_AVLNode const* theLeft = left();

  while (theLeft != 0)
  {
    theNode = theLeft;
    theLeft = theLeft->left();
  }

  return (OTC_AVLNode*)theNode;
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode* OTC_AVLNode::last_() const
{
  OTC_AVLNode const* theNode = this;
  OTC_AVLNode const* theRight = right();

  while (theRight != 0)
  {
    theNode = theRight;
    theRight = theRight->right();
  }

  return (OTC_AVLNode*)theNode;
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode* OTC_AVLNode::prev_() const
{
  if (left() != 0)
    return (OTC_AVLNode*)left()->last();

  if (isRight())
    return (OTC_AVLNode*)father();

  OTC_AVLNode const* theNode = this;
  while (theNode->isLeft())
    theNode = theNode->father();
  theNode = theNode->father();

  return (OTC_AVLNode*)theNode;
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode* OTC_AVLNode::next_() const
{
  if (right() != 0)
    return (OTC_AVLNode*)right()->first();

  if (isLeft())
    return (OTC_AVLNode*)father();

  OTC_AVLNode const* theNode = this;
  while (theNode->isRight())
    theNode = theNode->father();
  theNode = theNode->father();

  return (OTC_AVLNode*)theNode;
}

/* ------------------------------------------------------------------------- */
