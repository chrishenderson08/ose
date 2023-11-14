/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcdomprpty.cc
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
#pragma implementation "OTC/text/domprpty.hh"
#endif
#endif

#include <OTC/text/domprpty.hh>
#include <OTC/text/domnode.hh>

#include <OTC/memory/cluster.hh>

#include <string.h>

/* ------------------------------------------------------------------------- */
OTC_DOMProperty::~OTC_DOMProperty()
{
  unlink_();
}

/* ------------------------------------------------------------------------- */
void OTC_DOMProperty::unlink_()
{
  if (node_ != 0)
  {
    if (prev_ == 0)
      node_->firstProperty_ = next_;
    else
      prev_->next_ = next_;

    if (next_ == 0)
      node_->lastProperty_ = prev_;
    else
      next_->prev_ = prev_;
  }
}

/* ------------------------------------------------------------------------- */
OTC_DOMProperty::OTC_DOMProperty(
 OTC_DOMNode* theNode,
 char const* theName,
 char const* theValue
)
  : node_(theNode), prev_(0), next_(0),
    name_(0), value_(0), valueLength_(0)
{
  if (theName != 0 && *theName != EOS)
  {
    size_t theLength = strlen(theName) + 1;
    name_ = OTC_Cluster::of(this)->allocate(theLength);
    OTC_Copy::copy(name_,theName,theLength);
  }

  if (theValue != 0 && *theValue != EOS)
  {
    valueLength_ = strlen(theValue);
    value_ = OTC_Cluster::of(this)->allocate(valueLength_+1);
    OTC_Copy::copy(value_,theValue,valueLength_);
    value_[valueLength_] = EOS;
  }

  if (node_ != 0)
  {
    if (node_->lastProperty_ == 0)
    {
      node_->firstProperty_ = this;
      node_->lastProperty_ = this;
    }
    else
    {
      prev_ = node_->lastProperty_;
      node_->lastProperty_->next_ = this; 
      node_->lastProperty_ = this;
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_DOMProperty::setValue(char const* theValue)
{
  if (theValue != 0 && *theValue != EOS)
  {
    valueLength_ = strlen(theValue);
    value_ = OTC_Cluster::of(this)->allocate(valueLength_+1);
    OTC_Copy::copy(value_,theValue,valueLength_);
    value_[valueLength_] = EOS;
  }
  else
    value_ = 0;
}

/* ------------------------------------------------------------------------- */
void OTC_DOMProperty::setValue(char const* theValue, size_t theLength)
{
  OTCLIB_ENSURE_FN((theValue != 0 || theLength == 0),
   "OTC_DOMProperty::setValue(char const*,size_t)",
   "invalid value string");

  if (theLength != 0)
  {
    valueLength_ = theLength;
    value_ = OTC_Cluster::of(this)->allocate(valueLength_+1);
    OTC_Copy::copy(value_,theValue,valueLength_);
    value_[valueLength_] = EOS;
  }
  else
    value_ = 0;
}

/* ------------------------------------------------------------------------- */
