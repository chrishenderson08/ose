/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcbitset.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/bitset.hh"
#endif
#endif

#include <OTC/collctn/bitset.hh>

/* ------------------------------------------------------------------------- */
u_char OTC_BitSet::globCounts_[256] =
{
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

/* ------------------------------------------------------------------------- */
OTC_BitSet::~OTC_BitSet()
{
  if (set_ != 0)
  {
    if (set_[size_] == 0)
      delete [] set_;
    else
      set_[size_]--;
  }
}

/* ------------------------------------------------------------------------- */
int OTC_BitSet::rank(OTC_BitSet const& theSet) const
{
  if (*this == theSet)
    return 0;

  return count() - theSet.count();
}

/* ------------------------------------------------------------------------- */
int OTC_BitSet::hash() const
{
  return count();
}

/* ------------------------------------------------------------------------- */
OTC_BitSet::OTC_BitSet(OTC_BitSet const& theSet)
  : range_(theSet.range()), size_(theSet.size_), set_(0)
{
  if (theSet.set_ != 0)
  {
    set_ = new u_char[size_+1];
    OTCLIB_ASSERT_M(set_ != 0);
    memcpy(set_,theSet.set_,size_);

    set_[size_] = 0;
  }
}

/* ------------------------------------------------------------------------- */
OTC_BitSet::OTC_BitSet(OTC_BitSet const& theSet, OTC_ShallowCopy)
  : range_(theSet.range()), size_(theSet.size_), set_(theSet.set_)
{
  if (set_ != 0)
    set_[size_]++;
}

/* ------------------------------------------------------------------------- */
OTC_BitSet::OTC_BitSet(u_int theLength)
  : range_(0,theLength), size_(bytes(theLength)), set_(0)
{
  if (size_ > 0)
  {
    set_ = new u_char[size_+1];
    OTCLIB_ASSERT_M(set_ != 0);
    memset(set_,0,size_);

    set_[size_] = 0;
  }
}

/* ------------------------------------------------------------------------- */
OTC_BitSet::OTC_BitSet(int theLower, u_int theLength)
  : range_(theLower,theLength), size_(bytes(theLength)), set_(0)
{
  if (size_ > 0)
  {
    set_ = new u_char[size_+1];
    OTCLIB_ASSERT_M(set_ != 0);
    memset(set_,0,size_);

    set_[size_] = 0;
  }
}

/* ------------------------------------------------------------------------- */
OTC_BitSet::OTC_BitSet(OTC_Range const& theRange)
  : range_(theRange), size_(bytes(theRange.length())), set_(0)
{
  if (size_ > 0)
  {
    set_ = new u_char[size_+1];
    OTCLIB_ASSERT_M(set_ != 0);
    memset(set_,0,size_);

    set_[size_] = 0;
  }
}

/* ------------------------------------------------------------------------- */
OTC_BitSet& OTC_BitSet::operator=(OTC_BitSet const& theSet)
{
  if (&theSet == this)
    return *this;

  if (theSet.range().length() !=  range_.length())
  {
    if (set_[size_] == 0)
      delete [] set_;
    else
      set_[size_]--;

    range_ = theSet.range();
    size_ = theSet.size_;

    set_ = new u_char[size_+1];
    OTCLIB_ASSERT_M(set_ != 0);
    memcpy(set_,theSet.set_,size_);

    set_[size_] = 0;
  }
  else
  {
    range_ = theSet.range();
    memcpy(set_,theSet.set_,size_);
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
u_int OTC_BitSet::count() const
{
  if (set_ == 0)
    return 0;

  // Check all but last byte using a loop. Need to check last byte
  // explicitly as we may not be using all bits in that byte and extra
  // bits could contain garbage.

  u_int theCount = 0;

  for (int i=0; i<size_-1; i++)
    theCount += globCounts_[set_[i]];

  // This mess calculates a mask for those bits being used in the
  // last byte.

  u_char mask = u_char(~(0xff << (range_.length() % 8)));

  theCount += globCounts_[set_[size_-1] & mask];

  return theCount;
}

/* ------------------------------------------------------------------------- */
bool OTC_BitSet::operator==(OTC_BitSet const& theSet) const
{
  if (&theSet == this)
    return true;

  if (set_ == theSet.set_)
    return true;

  if (count() != theSet.count())
    return false;

  for (int i=range_.lower(); i<range_.upper(); i++)
  {
    if (test(i) != 0)
    {
      if (!theSet.range().contains(i))
	return false;

      if (theSet.test(i) == 0)
	return false;
    }
    else
    {
      if (theSet.range().contains(i) && theSet.test(i) != 0)
	return false;
    }
  }

  return true;
}

/* ------------------------------------------------------------------------- */
void OTC_BitSet::reset(OTC_Range const& theRange)
{
  OTCLIB_ENSURE_FN((theRange.isSubRange(range_)),
   "OTC_BitSet::reset(OTC_Range const&)",
   "invalid range");

  for (int i=theRange.lower(); i<theRange.upper(); i++)
    set_[byte(i)] &= u_char(~mask(i));
}

/* ------------------------------------------------------------------------- */
void OTC_BitSet::set(OTC_Range const& theRange)
{
  OTCLIB_ENSURE_FN((theRange.isSubRange(range_)),
   "OTC_BitSet::set(OTC_Range const&)",
   "invalid range");

  for (int i=theRange.lower(); i<theRange.upper(); i++)
    set_[byte(i)] |= mask(i);
}

/* ------------------------------------------------------------------------- */
void OTC_BitSet::complement()
{
  for (int i=0; i<size_; i++)
    set_[i] = u_char(~set_[i]);
}

/* ------------------------------------------------------------------------- */
void OTC_BitSet::complement(OTC_Range const& theRange)
{
  OTCLIB_ENSURE_FN((theRange.isSubRange(range_)),
   "OTC_BitSet::complement()",
   "invalid range");

  for (int i=theRange.lower(); i<theRange.upper(); i++)
    set_[byte(i)] ^= mask(i);
}

/* ------------------------------------------------------------------------- */
OTC_BitSet OTC_BitSet::operator~() const
{
  OTC_BitSet bitset(*this);
  bitset.complement();

  return OTC_BitSet(bitset,OTCLIB_SHALLOWCOPY);
}

/* ------------------------------------------------------------------------- */
OTC_BitSet OTC_BitSet::operator&(OTC_BitSet const& theSet)
{
  int theLower;
  int theUpper;

  theLower = range_.lower();
  theUpper = range_.upper();

  if (theSet.range_.lower() < theLower)
    theLower = theSet.range_.lower();

  if (theSet.range_.upper() > theUpper)
    theUpper = theSet.range_.upper();

  OTC_BitSet bitset(theLower,theUpper-theLower);

  if (range_.isSubRange(theSet.range_))
  {
    theLower = range_.lower();
    theUpper = range_.upper();
  }
  else if (theSet.range_.isSubRange(range_))
  {
    theLower = theSet.range_.lower();
    theUpper = theSet.range_.upper();
  }
  else
  {
    theLower = range_.lower();
    theUpper = range_.upper();

    if (theSet.range_.lower() > theLower)
      theLower = theSet.range_.lower();

    if (theSet.range_.upper() < theUpper)
      theUpper = theSet.range_.upper();
  }

  for (int i=theLower; i<theUpper; i++)
  {
    if ((test(i) != 0) && (theSet.test(i) != 0))
      bitset.set(i);
  }

  return OTC_BitSet(bitset,OTCLIB_SHALLOWCOPY);
}

/* ------------------------------------------------------------------------- */
OTC_BitSet& OTC_BitSet::operator&=(OTC_BitSet const& theSet)
{
  *this = *this & theSet;

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_BitSet OTC_BitSet::operator|(OTC_BitSet const& theSet)
{
  int theLower;
  int theUpper;

  theLower = range_.lower();
  theUpper = range_.upper();

  if (theSet.range_.lower() < theLower)
    theLower = theSet.range_.lower();

  if (theSet.range_.upper() > theUpper)
    theUpper = theSet.range_.upper();

  OTC_BitSet bitset(theLower,theUpper-theLower);

  for (int i=theLower; i<theUpper; i++)
  {
    if ((range_.contains(i) && test(i) != 0) ||
     (theSet.range_.contains(i) && theSet.test(i) != 0))
    {
      bitset.set(i);
    }
  }

  return OTC_BitSet(bitset,OTCLIB_SHALLOWCOPY);
}

/* ------------------------------------------------------------------------- */
OTC_BitSet& OTC_BitSet::operator|=(OTC_BitSet const& theSet)
{
  *this = *this | theSet;

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_BitSet OTC_BitSet::operator^(OTC_BitSet const& theSet)
{
  int theLower;
  int theUpper;

  theLower = range_.lower();
  theUpper = range_.upper();

  if (theSet.range_.lower() < theLower)
    theLower = theSet.range_.lower();

  if (theSet.range_.upper() > theUpper)
    theUpper = theSet.range_.upper();

  OTC_BitSet bitset(theLower,theUpper-theLower);

  for (int i=theLower; i<theUpper; i++)
  {
    if (range_.contains(i))
    {
      if (theSet.range_.contains(i))
      {
	if ((test(i) != 0) ^ (theSet.test(i) != 0))
	  bitset.set(i);
      }
      else
      {
	if (test(i) != 0)
	  bitset.set(i);
      }
    }
    else
    {
      if (theSet.test(i) != 0)
	bitset.set(i);
    }
  }

  return OTC_BitSet(bitset,OTCLIB_SHALLOWCOPY);
}

/* ------------------------------------------------------------------------- */
OTC_BitSet& OTC_BitSet::operator^=(OTC_BitSet const& theSet)
{
  *this = *this ^ theSet;

  return *this;
}

/* ------------------------------------------------------------------------- */
