/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_tindex.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/tindex.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_TIndex<int> tmpIndex;

  tracer() << "population = " << tmpIndex.population() << endl;
  tracer() << "isEmpty = " << tmpIndex.isEmpty() << endl;

  int const ZERO = 0;

  tracer() << "contains(0) = " << tmpIndex.contains(ZERO) << endl;
  tracer() << "entry(0) = " << (int)tmpIndex.entry(ZERO) << endl;

  tracer() << "removeAll" << endl;
  tmpIndex.removeAll();

  tracer() << "population = " << tmpIndex.population() << endl;
  tracer() << "isEmpty = " << tmpIndex.isEmpty() << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_TIndex<int> tmpIndex;

  int i;

  OTC_TIndexEntry<int>* tmpEntry;

  for (i=20; i>=-20; i-=2)
  {
    tmpEntry = tmpIndex.insert(i);
    tracer() << "insert(" << i << ") = ";
    tracer() << (int)tmpEntry->item();
    tracer() << endl;
  }

  tracer() << "population = " << tmpIndex.population() << endl;
  tracer() << "isEmpty = " << tmpIndex.isEmpty() << endl;

  for (i=20; i>=-20; i-=1)
  {
    tracer() << "contains(" << i << ") = ";
    tracer() << tmpIndex.contains(i) << endl;
    tmpEntry = tmpIndex.entry(i);
    tracer() << "entry(" << i << ") = ";
    if (tmpEntry != 0)
      tracer() << (int)tmpEntry->item();
    else
      tracer() << "X";
    tracer() << endl;
  }

  tracer() << "removeAll" << endl;
  tmpIndex.removeAll();

  tracer() << "population = " << tmpIndex.population() << endl;
  tracer() << "isEmpty = " << tmpIndex.isEmpty() << endl;

  for (i=20; i>=-20; i-=1)
  {
    tracer() << "contains(" << i << ") = ";
    tracer() << tmpIndex.contains(i) << endl;
    tmpEntry = tmpIndex.entry(i);
    tracer() << "entry(" << i << ") = ";
    if (tmpEntry != 0)
      tracer() << (int)tmpEntry->item();
    else
      tracer() << "X";
    tracer() << endl;
  }
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_TIndex<int> tmpIndex;

  int i;

  OTC_TIndexEntry<int>* tmpEntry;

  for (i=20; i>=-20; i-=2)
  {
    tmpEntry = tmpIndex.insert(i);
    tmpEntry->setItem((void*)i);

    tracer() << "insert(" << i << ") = ";
    tracer() << (int)tmpEntry->item();
    tracer() << endl;
  }

  tracer() << "population = " << tmpIndex.population() << endl;
  tracer() << "isEmpty = " << tmpIndex.isEmpty() << endl;

  for (i=20; i>=-20; i-=1)
  {
    tracer() << "contains(" << i << ") = ";
    tracer() << tmpIndex.contains(i) << endl;
    tmpEntry = tmpIndex.entry(i);
    tracer() << "entry(" << i << ") = ";
    if (tmpEntry != 0)
      tracer() << (int)tmpEntry->item();
    else
      tracer() << "X";
    tracer() << endl;
  }

  for (i=20; i>=-20; i-=4)
  {
    tmpEntry = tmpIndex.entry(i);
    if (tmpEntry != 0)
    {
      tracer() << "removeEntry(" << i << ")" << endl;
      tmpEntry->removeEntry();
      tracer() << "population = " << tmpIndex.population() << endl;
    }
  }

  tracer() << "isEmpty = " << tmpIndex.isEmpty() << endl;

  for (i=20; i>=-20; i-=1)
  {
    tracer() << "contains(" << i << ") = ";
    tracer() << tmpIndex.contains(i) << endl;
    tmpEntry = tmpIndex.entry(i);
    tracer() << "entry(" << i << ") = ";
    if (tmpEntry != 0)
      tracer() << (int)tmpEntry->item();
    else
      tracer() << "X";
    tracer() << endl;
  }
}

void test5()
{
  OTC_Tracer tracer("void test5()");
 
  OTC_TIndex<int> index;
 
  int i;
 
  for (i=-150000; i<=150000; i+=3)
    index.insert(i);

  tracer() << "population = " << index.population() << endl;
 
  for (i=-150000; i<=150000; i+=3)
    index.remove(i);

  tracer() << "population = " << index.population() << endl;
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
  test4();
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5
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
