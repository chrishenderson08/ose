/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/tstrandom.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//
// ============================================================================
*/

#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */

// Based on RNG from NIH library. The NIH version was apparently
// based on the following article:
//
// Pierre L'ecuyer, "Efficient and Portable Combined Random Number
// Generators", Commun. ACM 31, 6 (June 1988), 742-749.

const long s1max = 2147483562;
const long s2max = 2147483398;

static long s1 = 1;
static long s2 = 1;

inline int ABS(long val) { return (val<0) ? -val : val; }

void tst_srandom(long seed1, long seed2)
{
  s1 = seed1;
  s2 = seed2;

  if (s1 < 1 || s1 > s1max) s1 = (ABS(s1) % s1max) + 1;
  if (s2 < 1 || s2 > s2max) s2 = (ABS(s2) % s2max) + 1;
}

long tst_random()
{
  long Z,k;

  k = s1/53668;
  s1 = 40014 * (s1 - k * 53668) - k * 12211;
  if (s1 < 0) s1 += s1max + 1;

  k = s2/52774;
  s2 = 40692 * (s2 - k * 52774) - k * 3791;
  if (s2 < 0) s2 += s2max + 1;

  Z = s1 - s2;
  if (Z < 1) Z += s1max;

  return Z;
}

/* ------------------------------------------------------------------------- */
