/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/_align.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1997-1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/align.hh>

#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */

int main()
{
  cout << "CHAR " << OTC_Alignment::ofChar() << endl;
  cout << "SHORT " << OTC_Alignment::ofShort() << endl;
  cout << "INT " << OTC_Alignment::ofInt() << endl;
  cout << "LONG " << OTC_Alignment::ofLong() << endl;
  cout << "FLOAT " << OTC_Alignment::ofFloat() << endl;
  cout << "DOUBLE " << OTC_Alignment::ofDouble() << endl;
  cout << "STRUCT " << OTC_Alignment::ofStruct() << endl;
  cout << "WPTR " << OTC_Alignment::ofWPtr() << endl;
  cout << "BPTR " << OTC_Alignment::ofBPtr() << endl;

  return 0;
}

/* ------------------------------------------------------------------------- */
