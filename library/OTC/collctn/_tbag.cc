/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_tbag.cc
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

#include <OTC/collctn/tbag.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/fstream.h>

#include <stdlib.h>
#include <string.h>

extern long tst_random();
extern void tst_srandom(long, long);

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test properties of an empty bag.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_TBag<int> bag;

  tracer() << "bag.population() = " << bag.population() << endl;
  tracer() << "bag.isEmpty() = " << bag.isEmpty() << endl;

  tracer() << "bag.removeAll()" << endl;
  bag.removeAll();

  tracer() << "bag.population() = " << bag.population() << endl;
  tracer() << "bag.isEmpty() = " << bag.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;
  iter = bag.items();
  tracer() << "iter.isValid() = " << iter.isValid() << endl;
}

// = TEST 3
//     Test of addition. Use iterator to check.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_TBag<int> bag;

  tst_srandom(3,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    bag.add(value);
  }

  tracer() << "bag.population() = " << bag.population() << endl;
  tracer() << "bag.isEmpty() = " << bag.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;
  iter = bag.items();
  tracer() << "bag.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment operator.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_TBag<int> bag1;

  tst_srandom(4,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    bag1.add(value);
  }

  tracer() << "bag1.population() = " << bag1.population() << endl;
  tracer() << "bag1.isEmpty() = " << bag1.isEmpty() << endl;

  tracer() << "bag2(bag1)" << endl;
  OTC_TBag<int> bag2(bag1);

  tracer() << "bag1.population() = " << bag1.population() << endl;
  tracer() << "bag1.isEmpty() = " << bag1.isEmpty() << endl;

  tracer() << "bag2.population() = " << bag2.population() << endl;
  tracer() << "bag2.isEmpty() = " << bag2.isEmpty() << endl;

  tracer() << "bag2 = bag2" << endl;
  bag2 = bag2;

  tracer() << "bag2.population() = " << bag2.population() << endl;
  tracer() << "bag2.isEmpty() = " << bag2.isEmpty() << endl;

  tracer() << "bag2.removeAll()" << endl;
  bag2.removeAll();

  tracer() << "bag2.population() = " << bag2.population() << endl;
  tracer() << "bag2.isEmpty() = " << bag2.isEmpty() << endl;

  tracer() << "bag2 = bag1" << endl;
  bag2 = bag1;

  tracer() << "bag2.population() = " << bag2.population() << endl;
  tracer() << "bag2.isEmpty() = " << bag2.isEmpty() << endl;
}

// = TEST 5
//     Test of contains and frequency.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_TBag<int> bag;

  tst_srandom(5,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    bag.add(value);
  }

  OTC_Iterator<int> iter = 0;
  iter = bag.items();
  tracer() << "bag frequencies =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
  {
    int value = iter.item();
    if (!bag.contains(value))
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << value << "/" << bag.frequency(value) << flush;
    value = -value - 1;
    if (bag.contains(value))
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 6
//     Test of explicit removal.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_TBag<int> bag;

  tst_srandom(6,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    bag.add(value);
  }

  tracer() << "bag.population() = " << bag.population() << endl;
  tracer() << "bag.isEmpty() = " << bag.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;
  iter = bag.items();
  for (iter.reset(); iter.isValid(); iter.next())
  {
    int value = iter.item();
    int freq = bag.frequency(value);
    while (freq > 0)
    {
      bag.remove(value);
      freq--;
    }
  }

  tracer() << "bag.population() = " << bag.population() << endl;
  tracer() << "bag.isEmpty() = " << bag.isEmpty() << endl;

  iter = bag.items();
  tracer() << "bag items =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of adding iterator.

void test7()
{
  OTC_Tracer tracer("void test7()");

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

  OTC_TBag<int> bag;

  bag.add(list.items());

  tracer() << "bag.population() = " << bag.population() << endl;
  tracer() << "bag.isEmpty() = " << bag.isEmpty() << endl;

  iter = bag.items();
  tracer() << "bag.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 8
//     Test of items with supplied rank function.

int test8_rank(char const* const& s1, char const* const& s2)
{
  int l1 = strlen(s1);
  int l2 = strlen(s2);

  if (l1 <= l2)
    return OTC_Rank::rank(s1,l1,s2,l1);

  return OTC_Rank::rank(s1,l1,s2,l2);
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  char buf1[1024];
  ifstream in("/usr/dict/words");

  OTC_TBag<char const*> bag;

  while (in.good())
  {
    in >> buf1;
    if (!in.fail())
    {
      if (!bag.contains(buf1))
      {
        char* buf2;
        buf2 = new char[strlen(buf1)+1];
        strcpy(buf2,buf1);
        bag.add(buf2);
      }
    }
  }

  cout << "loaded" << endl;
  cout << "> " << flush;

  OTC_Iterator<char const*> items;

  while (cin.good())
  {
    cin >> buf1;
    if (!cin.fail())
    {
      items = bag.items((char const*)buf1,test8_rank);
      while (items.isValid())
      {
        tracer() << items.item() << endl;
        items.next();
      }
      cout << "> " << flush;
    }
  }

  items = bag.items();
  while (items.isValid())
  {
    delete [] (char*)items.item();
    items.next();
  }
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
  test8
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
