/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/_osstream.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/osstream.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_String theString;

  theString += "123";

  OTC_OSStream theStream(theString);

  theStream << "4";
  theStream << 5;
  theStream << 6.0;

  theString += "789";

  tracer() << theString << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_String theString;

  theString += "123";

  OTC_OSStream theStream(theString,OTCLIB_BUFFERED);

  theStream << "4";
  theStream << 5;
  theStream << 6.0;

  theStream << flush;

  theString += "789";

  tracer() << theString << endl;
}

void test1()
{
  test2();
  test3();
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
