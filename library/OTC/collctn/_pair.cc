/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_pair.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/pair.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

OTC_Pair<double,int> mod2(double theValue)
{
  int theValue2 = int(theValue)%2;
  OTC_Pair<double,int> theResult(theValue,theValue2);
  return theResult;
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  double i = 0;
  int j = 0;
  OTC_Pair<double,int> theResult(i,j);
  for (i=0; i<4; i++)
  {
    theResult = mod2(i);
    tracer() << theResult.first() << " " << theResult.second() << endl;
  }
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1
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
