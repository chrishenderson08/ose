/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcavltree.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/avltree.hh"
#endif
#endif

#include <OTC/collctn/avltree.hh>

/* ------------------------------------------------------------------------- */
OTC_AVLTree::~OTC_AVLTree()
{
  removeAll();
}

/* ------------------------------------------------------------------------- */
OTC_AVLTree::OTC_AVLTree()
  : root_(0), population_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_AVLTree::removeAll()
{
  if (root_ != 0)
    delete root_;
  root_ = 0;
}

/* ------------------------------------------------------------------------- */
void OTC_AVLTree::addRoot(OTC_AVLNode* theNode)
{
  OTCLIB_ENSURE_FN((root() == 0),
   "OTC_AVLTree::addRoot(OTC_AVLNode*)",
   "this tree already has a root node");
  OTCLIB_ENSURE_FN((theNode != 0),
   "OTC_AVLTree::addRoot(OTC_AVLNode*)",
   "must have valid object to use as root node");
  OTCLIB_ENSURE_FN((theNode->tree() == 0),
   "OTC_AVLTree::addRoot(OTC_AVLNode*)",
   "root node is already in a tree");

  root_ = theNode;
  theNode->setTree_(this);
  population_ = 1;
}

/* ------------------------------------------------------------------------- */
int OTC_AVLTree::depth() const
{
  if (isEmpty())
    return -1;

  return root()->height() - 1;
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode* OTC_AVLTree::node_(u_int theIndex) const
{
  if (theIndex >= population())
    return 0;

  theIndex++;
  OTC_AVLNode const* theNode = root();
  u_int fullCount = 0;
  u_int tmpCount = theNode->count();
  while ((fullCount+tmpCount) != theIndex)
  {
    if (theIndex > (fullCount+tmpCount))
    {
      theNode = theNode->right();
      fullCount += tmpCount;
      tmpCount = theNode->count();
    }
    else
    {
      theNode = theNode->left();
      tmpCount = theNode->count();
    }
  }
  return (OTC_AVLNode*)theNode;
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode* OTC_AVLTree::first_() const
{
  if (root() != 0)
    return root()->first_();

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_AVLNode* OTC_AVLTree::last_() const
{
  if (root() != 0)
    return root()->last_();

  return 0;
}

/* ------------------------------------------------------------------------- */
