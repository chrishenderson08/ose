/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otccbitset.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1994 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/cbitset.hh"
#endif
#endif

#include <OTC/collctn/cbitset.hh>

/* ------------------------------------------------------------------------- */
u_char OTC_CBitSet::globCounts_[256] =
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
OTC_CBitSet::OTC_CBitSet(char const* theString)
{
  memset(set_,0,32);

  if (theString != 0)
  {
    while (*theString != EOS)
      set(*theString++);
  }
}

/* ------------------------------------------------------------------------- */
OTC_CBitSet::OTC_CBitSet(char const* theString, u_int theNum)
{
  OTCLIB_ENSURE_FN((theString != 0 || theNum == 0),
   "OTC_CBitSet(char const*, u_int)",
   "null pointer but non zero count");

  memset(set_,0,32);

  if (theString != 0 && theNum != 0)
  {
    while (theNum-- != 0)
      set(*theString++);
  }
}

/* ------------------------------------------------------------------------- */
u_int OTC_CBitSet::count() const
{
  int theCount = 0;
  for (int i=0; i<32; i++)
    theCount += globCounts_[set_[i]];

  return theCount;
}

/* ------------------------------------------------------------------------- */
void OTC_CBitSet::reset(OTC_Range const& theRange)
{
  OTCLIB_ENSURE_FN((theRange.lower() >= 0 && theRange.upper() <= 257),
   "OTC_CBitSet::reset(OTC_Range const&)",
   "invalid range");

  for (int i=theRange.lower(); i<theRange.upper(); i++)
    set_[byte(i)] &= u_char(~mask(i));
}

/* ------------------------------------------------------------------------- */
void OTC_CBitSet::set(OTC_Range const& theRange)
{
  OTCLIB_ENSURE_FN((theRange.lower() >= 0 && theRange.upper() <= 257),
   "OTC_CBitSet::set(OTC_Range const&)",
   "invalid range");

  for (int i=theRange.lower(); i<theRange.upper(); i++)
    set_[byte(i)] |= mask(i);
}

/* ------------------------------------------------------------------------- */
void OTC_CBitSet::complement()
{
  for (u_int i=0; i<32; i++)
    set_[i] = u_char(~set_[i]);
}

/* ------------------------------------------------------------------------- */
void OTC_CBitSet::complement(OTC_Range const& theRange)
{
  OTCLIB_ENSURE_FN((theRange.lower() >= 0 && theRange.upper() <= 257),
   "OTC_CBitSet::complement(OTC_Range const&)",
   "invalid range");

  for (int i=theRange.lower(); i<theRange.upper(); i++)
    set_[byte(i)] ^= mask(i);
}

/* ------------------------------------------------------------------------- */
OTC_CBitSet OTC_CBitSet::operator~() const
{
  OTC_CBitSet bitset(*this);
  bitset.complement();

  return bitset;
}

/* ------------------------------------------------------------------------- */
OTC_CBitSet OTC_CBitSet::operator&(OTC_CBitSet const& theSet)
{
  OTC_CBitSet bitset;
  for (u_int i=0; i<32; i++)
    bitset.set_[i] = u_char(set_[i] & theSet.set_[i]);

  return bitset;
}

/* ------------------------------------------------------------------------- */
OTC_CBitSet& OTC_CBitSet::operator&=(OTC_CBitSet const& theSet)
{
  for (u_int i=0; i<32; i++)
    set_[i] &= theSet.set_[i];

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_CBitSet OTC_CBitSet::operator|(OTC_CBitSet const& theSet)
{
  OTC_CBitSet bitset;
  for (u_int i=0; i<32; i++)
    bitset.set_[i] = u_char(set_[i] | theSet.set_[i]);

  return bitset;
}

/* ------------------------------------------------------------------------- */
OTC_CBitSet& OTC_CBitSet::operator|=(OTC_CBitSet const& theSet)
{
  for (u_int i=0; i<32; i++)
    set_[i] |= theSet.set_[i];

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_CBitSet OTC_CBitSet::operator^(OTC_CBitSet const& theSet)
{
  OTC_CBitSet bitset;
  for (u_int i=0; i<32; i++)
    bitset.set_[i] = u_char(set_[i] ^ theSet.set_[i]);

  return bitset;
}

/* ------------------------------------------------------------------------- */
OTC_CBitSet& OTC_CBitSet::operator^=(OTC_CBitSet const& theSet)
{
  for (u_int i=0; i<32; i++)
    set_[i] ^= theSet.set_[i];

  return *this;
}

/* ------------------------------------------------------------------------- */
