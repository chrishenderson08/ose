/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/_atomic.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/thread/atomic.hh>

#include <OTC/stdlib/iostream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_NRMutex mutex;
  OTC_Atomic i = 0;

  cout << "i = " << i << endl;
  cout << "get(i) = " << otclib_atomic_get(i,mutex) << endl;
  cout << "i = " << i << endl;
  cout << "inc(i) = " << otclib_atomic_inc(i,mutex) << endl;
  cout << "i = " << i << endl;
  cout << "get(i) = " << otclib_atomic_get(i,mutex) << endl;
  cout << "dec(i) = " << otclib_atomic_dec(i,mutex) << endl;
  cout << "i = " << i << endl;
  cout << "get(i) = " << otclib_atomic_get(i,mutex) << endl;
  cout << "i = " << i << endl;
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
