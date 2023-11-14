/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otclisti.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/listi.hh"
#endif
#endif

#include <OTC/collctn/listi.hh>
#include <OTC/collctn/avllnode.hh>

/* ------------------------------------------------------------------------- */
OTC_ListI::~OTC_ListI()
{
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
OTC_ListI::OTC_ListI()
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();
}

/* ------------------------------------------------------------------------- */
OTC_ListI::OTC_ListI(OTC_ListI const& theList)
{
  list_ = theList.list_;
  list_->reference();

  index_ = theList.index_;
  index_->reference();
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_ListI::first() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_ListI::first()",
   "list is empty");

  return list_->first();
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_ListI::last() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_ListI::last()",
   "list is empty");

  return list_->last();
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_ListI::item(int theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex >= 0 && theIndex < (int)index_->population()),
   "OTC_ListI::item(int)",
   "index out of range");

  if (theIndex == 0)
    return first();

  else if (theIndex == (int)index_->population()-1)
    return last();

  OTC_AVLNode* theNode = index_->node(theIndex);
  return ((OTC_AVLLinkNode*)theNode)->link();
}

/* ------------------------------------------------------------------------- */
void OTC_ListI::addBeforeItem(OTC_Link* theLink, int theIndex)
{
  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_ListI::addBeforeItem(OTC_Link*, int)",
   "invalid link");
  OTCLIB_ENSURE_FN((theIndex >= 0 && theIndex <= (int)index_->population()),
   "OTC_ListI::addBeforeItem(OTC_Link*, int)",
   "index out of range");

  OTC_AVLLinkNode* theLinkNode;
  theLinkNode = new OTC_AVLLinkNode(theLink);
  OTCLIB_ASSERT_M(theLinkNode);

  if (theIndex == 0 && population() == 0)
  {
    list_->addFirst(theLink);
    index_->addRoot(theLinkNode);
  }
  else
  {
    if (theIndex == (int)population())
    {
      OTC_AVLLinkNode* tmpLinkNode;
      tmpLinkNode = (OTC_AVLLinkNode*)index_->node(theIndex-1);
      OTCLIB_ASSERT(tmpLinkNode != 0);
      tmpLinkNode->link()->addAfter(theLink);
      tmpLinkNode->addAfter(theLinkNode);
    }
    else
    {
      OTC_AVLLinkNode* tmpLinkNode;
      tmpLinkNode = (OTC_AVLLinkNode*)index_->node(theIndex);
      OTCLIB_ASSERT(tmpLinkNode != 0);
      tmpLinkNode->link()->addBefore(theLink);
      tmpLinkNode->addBefore(theLinkNode);
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_ListI::replaceItem(OTC_Link* theLink, int theIndex)
{
  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_ListI::replaceItem(OTC_Link*, int)",
   "invalid link");
  OTCLIB_ENSURE_FN((theIndex >= 0 && theIndex < (int)index_->population()),
   "OTC_ListI::replaceItem(OTC_Link*, int)",
   "index out of range");

  OTC_AVLLinkNode* theLinkNode;

  theLinkNode = (OTC_AVLLinkNode*)index_->node(theIndex);
  theLinkNode->link()->addBefore(theLink);
  theLinkNode->link()->kill();
  theLinkNode->setLink(theLink);
}

/* ------------------------------------------------------------------------- */
void OTC_ListI::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
void OTC_ListI::removeItem(int theIndex, OTC_Protection theProtection)
{
  if (theProtection == OTCLIB_UNSAFE)
  {
    if (theIndex < 0 || theIndex >= (int)index_->population())
      return;
  }

  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_ListI::removeItem(int, OTC_Protection)",
   "list is empty");
  OTCLIB_ENSURE_FN((theIndex >= 0 && theIndex < (int)index_->population()),
   "OTC_List::removeItem(int, OTC_Protection)",
   "index out of range");

  OTC_AVLNode* theNode = index_->node(theIndex);
  OTCLIB_ASSERT(theNode != 0);

  theNode->unlink();
  ((OTC_AVLLinkNode*)theNode)->link()->kill();
  delete theNode;
}

/* ------------------------------------------------------------------------- */
void OTC_ListI::removeRange(
 int theStart,
 u_int theLength,
 OTC_Protection theProtection
)
{
  if (theProtection == OTCLIB_UNSAFE)
  {
    if (theStart < 0 || theStart+theLength > index_->population())
      return;
  }

  OTCLIB_ENSURE_FN((theStart>=0 && theStart+theLength<=index_->population()),
   "OTC_ListI::removeRange(int, u_int, OTC_Protection)",
   "invalid range");

  while (theLength-- > 0)
    removeItem(theStart+theLength);
}

/* ------------------------------------------------------------------------- */
