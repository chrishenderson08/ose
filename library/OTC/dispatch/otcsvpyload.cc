/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcsvpyload.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/svpyload.hh"
#endif
#endif

#include <OTC/dispatch/svpyload.hh>

/* ------------------------------------------------------------------------- */
OTC_SVPayload::~OTC_SVPayload()
{
  delete cluster_;
}

/* ------------------------------------------------------------------------- */
OTC_SVPayload::OTC_SVPayload()
{
  cluster_ = new OTC_Cluster;
  OTCLIB_ASSERT_M(cluster_ != 0);

  OTC_DOMNode* theNode;
  theNode = new (*cluster_) OTC_DOMNode;

  OTC_RWPayload theReference(theNode);

  OTC_RWPayload::operator=(theReference);
}

/* ------------------------------------------------------------------------- */
OTC_SVPayload::OTC_SVPayload(OTC_SVPayload const& theObject)
{
  cluster_ = new OTC_Cluster;
  OTCLIB_ASSERT_M(cluster_ != 0);

  OTC_DOMNode* theNode;
  theNode = theObject.resolve();

  if (theNode != 0)
    theNode = theNode->clone(cluster_);
  else
    theNode = new (*cluster_) OTC_DOMNode;

  OTC_RWPayload theReference(theNode);

  OTC_RWPayload::operator=(theReference);
}

/* ------------------------------------------------------------------------- */
OTC_SVPayload::OTC_SVPayload(OTC_ROPayload const& theObject)
{
  cluster_ = new OTC_Cluster;
  OTCLIB_ASSERT_M(cluster_ != 0);

  OTC_DOMNode* theNode;
  theNode = theObject.resolve();

  if (theNode != 0)
    theNode = theNode->clone(cluster_);
  else
    theNode = new (*cluster_) OTC_DOMNode;

  OTC_RWPayload theReference(theNode);

  OTC_RWPayload::operator=(theReference);
}

/* ------------------------------------------------------------------------- */
OTC_SVPayload& OTC_SVPayload::operator=(OTC_SVPayload const& theObject)
{
  if (theObject.peek() == peek())
    return *this;

  OTC_Cluster* tmpCluster;
  tmpCluster = cluster_;

  cluster_ = new OTC_Cluster;
  OTCLIB_ASSERT_M(cluster_ != 0);

  OTC_DOMNode* theNode;
  theNode = theObject.resolve();

  if (theNode != 0)
    theNode = theNode->clone(cluster_);
  else
    theNode = new (*cluster_) OTC_DOMNode;

  OTC_RWPayload theReference(theNode);

  OTC_RWPayload::operator=(theReference);

  delete tmpCluster;

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_SVPayload& OTC_SVPayload::operator=(OTC_ROPayload const& theObject)
{
  if (theObject.peek() == peek())
    return *this;

  OTC_Cluster* tmpCluster;
  tmpCluster = cluster_;

  cluster_ = new OTC_Cluster;
  OTCLIB_ASSERT_M(cluster_ != 0);

  OTC_DOMNode* theNode;
  theNode = theObject.resolve();

  if (theNode != 0)
    theNode = theNode->clone(cluster_);
  else
    theNode = new (*cluster_) OTC_DOMNode;

  OTC_RWPayload theReference(theNode);

  OTC_RWPayload::operator=(theReference);

  delete tmpCluster;

  return *this;
}

/* ------------------------------------------------------------------------- */
void OTC_SVPayload::setName(char const* theName)
{
  root()->setProperty("name",theName);
}

/* ------------------------------------------------------------------------- */
