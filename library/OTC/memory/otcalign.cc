/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/otcalign.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/memory/align.hh"
#endif
#endif

#include <OTC/memory/align.hh>

/* ------------------------------------------------------------------------- */

struct OTC_Align1
{
  char a;
};

struct OTC_Align2
{
  double a0; char c0;
  char c1;
  double a00; char c00;
  short s1;
  double a2; char c2;
  int i1;
  double a3; char c3;
  long l1;
  double a4; char c4;
  float f1;
  double a6; char c5;
  double d1;
  double a7; char c6;
  char* p1;
  double a8; char c7;
  struct OTC_Align2* p2;
  double a9; char c8;
  struct OTC_Align1 oo;
  double a10; char c9;
};

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofChar()
{
  OTC_Align2 oo;
  return (size_t)&oo.c1-(size_t)&oo.c0;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofShort()
{
  OTC_Align2 oo;
  return (size_t)&oo.s1-(size_t)&oo.c00;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofInt()
{
  OTC_Align2 oo;
  return (size_t)&oo.i1-(size_t)&oo.c2;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofLong()
{
  OTC_Align2 oo;
  return (size_t)&oo.l1-(size_t)&oo.c3;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofFloat()
{
  OTC_Align2 oo;
  return (size_t)&oo.f1-(size_t)&oo.c4;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofDouble()
{
  OTC_Align2 oo;
  return (size_t)&oo.d1-(size_t)&oo.c5;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofStruct()
{
  OTC_Align2 oo;
  return (size_t)&oo.oo-(size_t)&oo.c8;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofWPtr()
{
  OTC_Align2 oo;
  return (size_t)&oo.p2-(size_t)&oo.c7;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Alignment::ofBPtr()
{
  OTC_Align2 oo;
  return (size_t)&oo.p1-(size_t)&oo.c6;
}

/* ------------------------------------------------------------------------- */
