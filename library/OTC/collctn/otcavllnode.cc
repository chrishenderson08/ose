/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcavllnode.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/avllnode.hh"
#endif
#endif

#include <OTC/collctn/avllnode.hh>
#include <OTC/collctn/link.hh>

/* ------------------------------------------------------------------------- */
OTC_AVLLinkNode::~OTC_AVLLinkNode()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_AVLLinkNode::OTC_AVLLinkNode(OTC_Link* theLink)
  : link_(theLink)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
