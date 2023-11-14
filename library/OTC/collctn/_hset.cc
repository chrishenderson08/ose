/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_hset.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/hset.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

extern long tst_random();
extern void tst_srandom(long, long);

/* -------------------------------------------------------------------------- */

// = TEST 2
//     Test of properties of an empty set.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_HSet<int> set;

  tracer() << "set.population() = " << set.population() << endl;
  tracer() << "set.isEmpty() = " << set.isEmpty() << endl;

  tracer() << "set.removeAll()" << endl;
  set.removeAll();

  tracer() << "set.population() = " << set.population() << endl;
  tracer() << "set.isEmpty() = " << set.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;
  iter = set.items();
  tracer() << "iter.isValid() = " << iter.isValid() << endl;
}

// = TEST 3
//     Test of addition. Use iterator to check.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_HSet<int> set;

  for (int i=0; i<100; i++)
    set.add(i);

  tracer() << "set.population() = " << set.population() << endl;
  tracer() << "set.isEmpty() = " << set.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;
  iter = set.items();
  tracer() << "set.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment operator.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_HSet<int> set1;

  for (int i=0; i<100; i++)
    set1.add(i);

  tracer() << "set1.population() = " << set1.population() << endl;
  tracer() << "set1.isEmpty() = " << set1.isEmpty() << endl;

  tracer() << "set2(set1)" << endl;
  OTC_HSet<int> set2(set1);

  tracer() << "set1.population() = " << set1.population() << endl;
  tracer() << "set1.isEmpty() = " << set1.isEmpty() << endl;

  tracer() << "set2.population() = " << set2.population() << endl;
  tracer() << "set2.isEmpty() = " << set2.isEmpty() << endl;

  tracer() << "set2 = set2" << endl;
  set2 = set2;

  tracer() << "set2.population() = " << set2.population() << endl;
  tracer() << "set2.isEmpty() = " << set2.isEmpty() << endl;

  tracer() << "set2.removeAll()" << endl;
  set2.removeAll();

  tracer() << "set2.population() = " << set2.population() << endl;
  tracer() << "set2.isEmpty() = " << set2.isEmpty() << endl;

  tracer() << "set2 = set1" << endl;
  set2 = set1;

  tracer() << "set2.population() = " << set2.population() << endl;
  tracer() << "set2.isEmpty() = " << set2.isEmpty() << endl;
}

// = TEST 5
//     Test of contains.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_HSet<int> set;

  for (int i=0; i<100; i++)
    set.add(i);

  OTC_Iterator<int> iter = 0;
  iter = set.items();
  tracer() << "set items =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
  {
    int value1 = iter.item();
    if (!set.contains(value1))
      tracer() << "XXX - this should never happen" << endl;
    int value2 = set.item(value1);
    if (value1 != value2)
      tracer() << "XXX - this should never happen" << endl;
    int const* value3 = set.pItem(value1);
    if (!value3 || value1 != *value3)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << value1 << flush;
    value1 = -value1 - 1;
    if (set.contains(value1))
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 6
//     Test of explicit removal.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_HSet<int> set;

  for (int i=0; i<100; i++)
    set.add(i);

  tracer() << "set.population() = " << set.population() << endl;
  tracer() << "set.isEmpty() = " << set.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;
  iter = set.items();
  for (iter.reset(); iter.isValid(); iter.next())
  {
    int value = iter.item();
    set.remove(value);
  }

  tracer() << "set.population() = " << set.population() << endl;
  tracer() << "set.isEmpty() = " << set.isEmpty() << endl;

  iter = set.items();
  tracer() << "set.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of replacement.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_HSet<int> set;

  int i;

  for (i=0; i<10; i++)
    set.add(i,OTCLIB_REPLACE);

  OTC_Iterator<int> iter;

  iter = set.items();
  tracer() << "set.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  for (i=9; i>=0; i--)
    set.add(i,OTCLIB_REPLACE);

  iter = set.items();
  tracer() << "set.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 8
//     Test of a large set.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_HSet<int> set;

  u_int population = 0;
  tst_srandom(11,0);
  for (int i=0; i<2500; i++)
  {
    int value = int(tst_random());
    if (!set.contains(value))
    {
      set.add(value);
      population++;
    }
  }

  tracer() << "population = " << population << endl;
  tracer() << "set.population() = " << set.population() << endl;
  tracer() << "set.isEmpty() = " << set.isEmpty() << endl;

  set.removeAll();
}

// = TEST 9
//     Test of adding iterator.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_Deque<int> list;

  tst_srandom(3,0);
  for (int i=0; i<10; i++)
  {
    int value = int(tst_random() % 50);
    list.addLast(value);
  }

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;

  iter = list.items();
  tracer() << "list.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  OTC_HSet<int> set;

  set.add(list.items(),OTCLIB_REPLACE);

  tracer() << "set.population() = " << set.population() << endl;
  tracer() << "set.isEmpty() = " << set.isEmpty() << endl;

  iter = set.items();
  tracer() << "set.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 1
//     All tests.

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
}

/* -------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5,
  test6,
  test7,
  test8,
  test9
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
