/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcdomnode.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/domnode.hh"
#endif
#endif

#include <OTC/text/domnode.hh>

#include <OTC/memory/cluster.hh>

#include <string.h>

/* ------------------------------------------------------------------------- */
OTC_DOMNode::~OTC_DOMNode()
{
  removeAllChildren();
  removeAllProperties();

  unlink();
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode::OTC_DOMNode(
 char const* theName,
 char const* theContent
)
  : type_(0), name_(0), content_(0), contentLength_(0),
    parent_(0), nextSibling_(0), prevSibling_(0), firstProperty_(0),
    lastProperty_(0), firstChild_(0), lastChild_(0), numChildren_(0)
{
  if (theName != 0 && *theName != EOS)
  {
    size_t theLength = strlen(theName);
    name_ = OTC_Cluster::of(this)->allocate(theLength+1);
    OTC_Copy::copy(name_,theName,theLength+1);
  }

  if (theContent != 0 && *theContent != EOS)
  {
    contentLength_ = strlen(theContent);
    content_ = OTC_Cluster::of(this)->allocate(contentLength_+1);
    OTC_Copy::copy(content_,theContent,contentLength_+1);
  }
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode::OTC_DOMNode(
 int theType,
 char const* theName,
 char const* theContent
)
  : type_(theType), name_(0), content_(0), contentLength_(0),
    parent_(0), nextSibling_(0), prevSibling_(0), firstProperty_(0),
    lastProperty_(0), firstChild_(0), lastChild_(0), numChildren_(0)
{
  if (theName != 0 && *theName != EOS)
  {
    size_t theLength = strlen(theName);
    name_ = OTC_Cluster::of(this)->allocate(theLength+1);
    OTC_Copy::copy(name_,theName,theLength+1);
  }

  if (theContent != 0 && *theContent != EOS)
  {
    contentLength_ = strlen(theContent);
    content_ = OTC_Cluster::of(this)->allocate(contentLength_+1);
    OTC_Copy::copy(content_,theContent,contentLength_+1);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_DOMNode::setName(char const* theName)
{
  if (theName != 0)
  {
    size_t theLength = strlen(theName);
    name_ = OTC_Cluster::of(this)->allocate(theLength+1);
    OTC_Copy::copy(name_,theName,theLength+1);
  }
  else
    name_ = 0;
}

/* ------------------------------------------------------------------------- */
void OTC_DOMNode::setName(char const* theName, size_t theLength)
{
  OTCLIB_ENSURE_FN((theName != 0 || theLength == 0),
   "OTC_DOMNode::setName(char const*,size_t)",
   "invalid string");

  if (theLength != 0)
  {
    name_ = OTC_Cluster::of(this)->allocate(theLength+1);
    OTC_Copy::copy(name_,theName,theLength);
    name_[theLength] = EOS;
  }
  else
    name_ = 0;
}

/* ------------------------------------------------------------------------- */
void OTC_DOMNode::setContent(char const* theContent)
{
  if (theContent != 0)
  {
    contentLength_ = strlen(theContent);
    content_ = OTC_Cluster::of(this)->allocate(contentLength_+1);
    OTC_Copy::copy(content_,theContent,contentLength_+1);
  }
  else
  {
    content_ = 0;
    contentLength_ = 0;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_DOMNode::setContent(char const* theContent, size_t theLength)
{
  OTCLIB_ENSURE_FN((theContent != 0 || theLength == 0),
   "OTC_DOMNode::setContent(char const*,size_t)",
   "invalid string");

  if (theLength != 0)
  {
    contentLength_ = theLength;
    content_ = OTC_Cluster::of(this)->allocate(contentLength_+1);
    OTC_Copy::copy(content_,theContent,contentLength_);
    content_[contentLength_] = EOS;
  }
  else
  {
    content_ = 0;
    contentLength_ = 0;
  }
}

/* ------------------------------------------------------------------------- */
OTC_DOMProperty* OTC_DOMNode::setProperty(
 char const* theName,
 char const* theValue
)
{
  OTCLIB_ENSURE_FN((theName != 0 && *theName != EOS),
   "OTC_DOMNode::setProperty(char const*,char const*)",
   "invalid name");

  OTC_DOMProperty* theProperty;
  theProperty = property(theName);

  if (theProperty == 0)
  {
    theProperty = new (*OTC_Cluster::of(this))
     OTC_DOMProperty(this,theName,theValue);
  }
  else
    theProperty->setValue(theValue);

  return theProperty;
}

/* ------------------------------------------------------------------------- */
OTC_DOMProperty* OTC_DOMNode::setProperty(
 char const* theName,
 char const* theValue,
 size_t theLength
)
{
  OTCLIB_ENSURE_FN((theName != 0 && *theName != EOS),
   "OTC_DOMNode::setProperty(char const*,char const*)",
   "invalid name");

  OTC_DOMProperty* theProperty;
  theProperty = property(theName);

  if (theProperty == 0)
  {
    theProperty = new (*OTC_Cluster::of(this))
     OTC_DOMProperty(this,theName);
    theProperty->setValue(theValue,theLength);
  }
  else
    theProperty->setValue(theValue);

  return theProperty;
}

/* ------------------------------------------------------------------------- */
void OTC_DOMNode::removeProperty(char const* theName)
{
  OTCLIB_ENSURE_FN((theName != 0 && *theName != EOS),
   "OTC_DOMNode::setProperty(char const*,char const*)",
   "invalid name");

  OTC_DOMProperty* theProperty;
  theProperty = property(theName);

  if (theProperty != 0)
    theProperty->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_DOMNode::removeAllProperties()
{
  while (firstProperty_ != 0)
    firstProperty_->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_DOMNode::unlink()
{
  if (parent_ != 0)
  {
    if (parent_->firstChild_ == this)
      parent_->firstChild_ = nextSibling_;
    if (parent_->lastChild_ == this)
      parent_->lastChild_ = prevSibling_;

    parent_->numChildren_--;
  }

  parent_ = 0;

  if (prevSibling_ != 0)
    prevSibling_->nextSibling_ = nextSibling_;

  if (nextSibling_ != 0)
    nextSibling_->prevSibling_ = prevSibling_;

  prevSibling_ = 0;
  nextSibling_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::clone(OTC_Cluster* theCluster) const
{
  OTC_DOMNode* theNode;
  OTC_DOMProperty* theProperty;

  if (theCluster == 0)
    theCluster = OTC_Cluster::of(this);

  theNode = new (*theCluster) OTC_DOMNode;

  theNode->setType(type_);
  theNode->setName(name_);
  theNode->setContent(content_,contentLength_);

  theProperty = firstProperty_;
  while (theProperty != 0)
  {
    theNode->setProperty(
     theProperty->name().string(),theProperty->value());

    theProperty = theProperty->next();
  }

  OTC_DOMNode* theChild;

  theChild = firstChild_;
  while (theChild != 0)
  {
    theNode->insertChild(theChild->clone(theCluster));

    theChild = theChild->nextSibling();
  }

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::cloneChildren(OTC_Cluster* theCluster) const
{
  OTC_DOMNode* theNode;

  if (theCluster == 0)
    theCluster = OTC_Cluster::of(this);

  theNode = new (*theCluster) OTC_DOMNode;

  OTC_DOMNode* theChild;

  theChild = firstChild_;
  while (theChild != 0)
  {
    theNode->insertChild(theChild->clone(theCluster));

    theChild = theChild->nextSibling();
  }

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::createChild(
 char const* theName,
 char const* theContent
)
{
  OTC_DOMNode* theNode;
  theNode = new (*OTC_Cluster::of(this)) OTC_DOMNode;
  
  theNode->setName(theName);
  theNode->setContent(theContent);

  return insertChild(theNode);
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::createChild(
 int theType,
 char const* theName,
 char const* theContent
)
{
  OTC_DOMNode* theNode;
  theNode = new (*OTC_Cluster::of(this)) OTC_DOMNode;
  
  theNode->setType(theType);
  theNode->setName(theName);
  theNode->setContent(theContent);

  return insertChild(theNode);
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::insertChild(
 OTC_DOMNode* theNewChild,
 OTC_DOMNode* theOldChild
)
{
  OTCLIB_ENSURE_FN((theNewChild != 0),
   "OTC_DOMNode::insertChild(OTC_DOMNode*,OTC_DOMNode*)",
   "invalid child object");

  OTCLIB_ENSURE_FN((theOldChild == 0 || theOldChild->parent() == this),
   "OTC_DOMNode::insertChild(OTC_DOMNode*,OTC_DOMNode*)",
   "old child is not a child of this node");

  OTC_DOMNode* theNode;
  theNode = this;

  while (theNode != 0)
  {
    OTCLIB_ENSURE_FN((theNode != theNewChild),
     "OTC_DOMNode::insertChild(OTC_DOMNode*,OTC_DOMNode*)",
     "new child is an ancestor of this node");

    theNode = theNode->parent();
  }

  theNewChild->unlink();

  theNewChild->parent_ = this;

  if (firstChild_ == 0)
  {
    firstChild_ = theNewChild;
    lastChild_ = theNewChild;
  }
  else
  {
    if (theOldChild == 0)
    {
      theNewChild->prevSibling_ = lastChild_;
      lastChild_->nextSibling_ = theNewChild;
      lastChild_ = theNewChild;
    }
    else
    {
      if (theOldChild->prevSibling_ != 0)
        theOldChild->prevSibling_->nextSibling_ = theNewChild;
      theNewChild->prevSibling_ = theOldChild->prevSibling_;
      theNewChild->nextSibling_ = theOldChild;
      theOldChild->prevSibling_ = theNewChild;
    }
  }

  numChildren_++;

  return theNewChild;
}

/* ------------------------------------------------------------------------- */
void OTC_DOMNode::removeAllChildren()
{
  while (firstChild_ != 0)
    firstChild_->destroy();

  numChildren_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::child_(u_int theIndex) const
{
  if (theIndex >= numChildren_)
    return 0;

  if (theIndex == 0)
    return firstChild_;

  if (theIndex == numChildren_)
    return lastChild_;

  OTC_DOMNode* theChild;
  theChild = firstChild_;

  while (theIndex-- > 0)
    theChild = theChild->nextSibling();

  return theChild;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findNode_(char const* theName) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findNode_(char const*)",
   "invalid name string");

  OTC_DOMNode* theNode;
  theNode = (OTC_DOMNode*)this;

  if (theNode->name() == theName)
    return theNode;

  OTC_DOMNode* theResult = 0;

  theNode = theNode->firstChild();
  while (theNode != 0)
  {
    theResult = theNode->findNode_(theName);
    if (theResult != 0)
      return theResult;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findNode_(char const* theName, size_t theLength) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findNode_(char const*,size_t)",
   "invalid name string");

  OTC_DOMNode* theNode;
  theNode = (OTC_DOMNode*)this;

  if (theNode->name().rank(theName,theLength) == 0)
    return theNode;

  OTC_DOMNode* theResult = 0;

  theNode = theNode->firstChild();
  while (theNode != 0)
  {
    theResult = theNode->findNode_(theName,theLength);
    if (theResult != 0)
      return theResult;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMProperty* OTC_DOMNode::property_(char const* theName) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::property_(char const*)",
   "invalid name string");

  OTC_DOMProperty* theProperty;
  theProperty = firstProperty_;

  while (theProperty != 0)
  {
    if (theProperty->name() == theName)
      break;

    theProperty = theProperty->next();
  }

  return theProperty;
}

/* ------------------------------------------------------------------------- */
OTC_DOMProperty* OTC_DOMNode::property_(
 char const* theName,
 char const* theValue
) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::property_(char const*,char const*)",
   "invalid name string");
  OTCLIB_ENSURE_FN((theValue != 0),
   "OTC_DOMNode::property_(char const*,char const*)",
   "invalid value string");

  OTC_DOMProperty* theProperty;
  theProperty = firstProperty_;

  while (theProperty != 0)
  {
    if (theProperty->name() == theName &&
     theProperty->value() == theValue)
    {
      break;
    }

    theProperty = theProperty->next();
  }

  return theProperty;
}

/* ------------------------------------------------------------------------- */
OTC_DOMProperty* OTC_DOMNode::property_(
 char const* theName,
 char const* theValue,
 size_t theLength
) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::property_(char const*,char const*,size_t)",
   "invalid name string");
  OTCLIB_ENSURE_FN((theValue != 0),
   "OTC_DOMNode::property_(char const*,char const*,size_t)",
   "invalid value string");

  OTC_DOMProperty* theProperty;
  theProperty = firstProperty_;

  while (theProperty != 0)
  {
    if (theProperty->name() == theName &&
     theProperty->value().rank(theValue,theLength) == 0)
    {
      break;
    }

    theProperty = theProperty->next();
  }

  return theProperty;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findProperty_(char const* theName) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findProperty_(char const*)",
   "invalid name string");

  OTC_DOMNode* theNode;
  theNode = (OTC_DOMNode*)this;

  if (theNode->property(theName) != 0)
    return theNode;

  OTC_DOMNode* theResult = 0;

  theNode = theNode->firstChild();
  while (theNode != 0)
  {
    theResult = theNode->findProperty_(theName);
    if (theResult != 0)
      return theResult;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findProperty_(
 char const* theName,
 char const* theValue
) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findProperty_(char const*,char const*)",
   "invalid name string");
  OTCLIB_ENSURE_FN((theValue != 0),
   "OTC_DOMNode::findProperty_(char const*,char const*)",
   "invalid value string");

  OTC_DOMNode* theNode;
  theNode = (OTC_DOMNode*)this;

  if (theNode->property(theName,theValue) != 0)
    return theNode;

  OTC_DOMNode* theResult = 0;

  theNode = theNode->firstChild();
  while (theNode != 0)
  {
    theResult = theNode->findProperty_(theName,theValue);
    if (theResult != 0)
      return theResult;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findProperty_(
 char const* theName,
 char const* theValue,
 size_t theLength
) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findProperty_(char const*,char const*,size_t)",
   "invalid name string");
  OTCLIB_ENSURE_FN((theValue != 0),
   "OTC_DOMNode::findProperty_(char const*,char const*,size_t)",
   "invalid value string");

  OTC_DOMNode* theNode;
  theNode = (OTC_DOMNode*)this;

  if (theNode->property(theName,theValue,theLength) != 0)
    return theNode;

  OTC_DOMNode* theResult = 0;

  theNode = theNode->firstChild();
  while (theNode != 0)
  {
    theResult = theNode->findProperty_(theName,theValue,theLength);
    if (theResult != 0)
      return theResult;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findChild_(char const* theName) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findChild_(char const*)",
   "invalid name string");

  OTC_DOMNode* theNode;

  theNode = (OTC_DOMNode*)firstChild();
  while (theNode != 0)
  {
    if (theNode->name() == theName)
      return theNode;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findChild_(
 char const* theName,
 size_t theLength
) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findChild_(char const*,size_t)",
   "invalid name string");

  OTC_DOMNode* theNode;

  theNode = (OTC_DOMNode*)firstChild();
  while (theNode != 0)
  {
    if (theNode->name().rank(theName,theLength) == 0)
      return theNode;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findSibling_(char const* theName) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findSibling_(char const*)",
   "invalid name string");

  OTC_DOMNode* theNode;

  theNode = (OTC_DOMNode*)nextSibling();
  while (theNode != 0)
  {
    if (theNode->name() == theName)
      return theNode;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_DOMNode::findSibling_(
 char const* theName,
 size_t theLength
) const
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_DOMNode::findSibling_(char const*,size_t)",
   "invalid name string");

  OTC_DOMNode* theNode;

  theNode = (OTC_DOMNode*)nextSibling();
  while (theNode != 0)
  {
    if (theNode->name().rank(theName,theLength) == 0)
      return theNode;

    theNode = theNode->nextSibling();
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
