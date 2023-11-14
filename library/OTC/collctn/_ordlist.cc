/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_ordlist.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/ordlist.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/fstream.h>

#include <stdlib.h>
#include <string.h>

extern long tst_random();
extern void tst_srandom(long, long);

#ifdef index
#undef index
#endif

/* ------------------------------------------------------------------------- */

// = TEST 2
//      Test properties of an empty list.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_OrderedList<int> list;

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  OTC_Iterator<int> iter(list.items());
  tracer() << "iter.isValid() = " << iter.isValid() << endl;
}

// = TEST 3
//     Test of addition. Use iterator to check ordering.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_OrderedList<int> list;

  tst_srandom(3,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    list.add(value);
  }

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;
  iter = list.items();
  tracer() << "list.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_OrderedList<int> list1;

  tst_srandom(4,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    list1.add(value);
  }

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  OTC_OrderedList<int> list2(list1);

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  tracer() << "list1.removeAll()" << endl;
  list1.removeAll();

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;
  iter = list2.items();
  tracer() << "list2.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  tracer() << "list2 = list2" << endl;
  list2 = list2;

  tracer() << "list1 = list2" << endl;
  list1 = list2;

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  tracer() << "list1.removeAll()" << endl;
  list1.removeAll();

  tracer() << "list2 = list1" << endl;
  list2 = list1;

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;
}

// = TEST 5
//     Test of looking at and removing items from the head of the list.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_OrderedList<int> list;

  tst_srandom(5,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    list.add(value);
  }

  tracer() << "list items =" << flush;
  for (u_int j=500; j>0; j--)
  {
    if (j != list.population())
      tracer() << "XXX - this should never happen" << endl;
    int lookat = list.first();
    list.removeFirst();
    tracer() << " " << lookat << flush;
  }
  tracer() << endl;
}

// = TEST 6
//     Test of looking at and removing items from the tail of the list.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_OrderedList<int> list;

  tst_srandom(6,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    list.add(value);
  }

  tracer() << "list items =" << flush;
  for (u_int j=500; j>0; j--)
  {
    if (j != list.population())
      tracer() << "XXX - this should never happen" << endl;
    int lookat = list.last();
    list.removeLast();
    tracer() << " " << lookat << flush;
  }
  tracer() << endl;
}

// = TEST 7
//     Test of contains and frequency.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_OrderedList<int> list;

  tst_srandom(7,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    list.add(value);
  }

  OTC_Iterator<int> iter = 0;
  iter = list.items();
  tracer() << "list frequencies =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
  {
    int value = iter.item();
    if (!list.contains(value))
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << value << "/" << list.frequency(value) << flush;
    value = -value - 1;
    if (list.contains(value))
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 8
//     Test of removing explicit items.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_OrderedList<int> list;

  tst_srandom(8,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    list.add(value);
  }

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  while (!list.isEmpty())
  {
    int value = list.first();
    int freq = list.frequency(value);
    while (freq > 0)
    {
      list.remove(value);
      freq--;
    }
    if (!list.isEmpty() && list.first() == value)
      tracer() << "XXX - this should never happen" << endl;
  }

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;
}

// = TEST 9
//     Test of index returned by add().

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_OrderedList<int> list;

  tst_srandom(3,0);
  for (int i=0; i<25; i++)
  {
    int value = int(tst_random() % 10);
    u_int theIndex = list.add(value);
    tracer() << "index(" << value << ") = " << theIndex << endl;
    OTC_Iterator<int> iter = 0;
    iter = list.items();
    tracer() << "list.items() =" << flush;
    for (iter.reset(); iter.isValid(); iter.next())
      tracer() << " " << iter.item() << flush;
    tracer() << endl;
  }
}

// = TEST 10
//     Test of index().

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_OrderedList<int> list;

  tst_srandom(3,0);
  for (int i=0; i<25; i++)
  {
    int value = int(tst_random() % 10);
    list.add(value);
    tracer() << "index(" << value << ",OTCLIB_ANY) = ";
    tracer() << list.index(value,OTCLIB_ANY) << endl;
    tracer() << "index(" << value << ",OTCLIB_FIRST) = ";
    tracer() << list.index(value,OTCLIB_FIRST) << endl;
    tracer() << "index(" << value << ",OTCLIB_LAST) = ";
    tracer() << list.index(value,OTCLIB_LAST) << endl;
    OTC_Iterator<int> iter = 0;
    iter = list.items();
    tracer() << "list.items() =" << flush;
    for (iter.reset(); iter.isValid(); iter.next())
      tracer() << " " << iter.item() << flush;
    tracer() << endl;
  }
}

// = TEST 11
//     Test of item().

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_OrderedList<int> list;

  for (int i=1; i<10; i+=2)
  {
    list.add(i);
    list.add(i);
  }

  OTC_Iterator<int> iter = 0;
  iter = list.items();
  tracer() << "list.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  for (int j=0; j<=10; j++)
  {
    tracer() << "VALUE = " << j << endl;

    tracer() << "OTCLIB_FIRST && OTCLIB_LT = ";
    tracer() << list.index(j,OTCLIB_FIRST,OTCLIB_LT) << endl;
    tracer() << "OTCLIB_ANY && OTCLIB_LT = ";
    tracer() << list.index(j,OTCLIB_ANY,OTCLIB_LT) << endl;
    tracer() << "OTCLIB_LAST && OTCLIB_LT = ";
    tracer() << list.index(j,OTCLIB_LAST,OTCLIB_LT) << endl;

    tracer() << "OTCLIB_FIRST && OTCLIB_LE = ";
    tracer() << list.index(j,OTCLIB_FIRST,OTCLIB_LE) << endl;
    tracer() << "OTCLIB_ANY && OTCLIB_LE = ";
    tracer() << list.index(j,OTCLIB_ANY,OTCLIB_LE) << endl;
    tracer() << "OTCLIB_LAST && OTCLIB_LE = ";
    tracer() << list.index(j,OTCLIB_LAST,OTCLIB_LE) << endl;

    tracer() << "OTCLIB_FIRST && OTCLIB_EQ = ";
    tracer() << list.index(j,OTCLIB_FIRST,OTCLIB_EQ) << endl;
    tracer() << "OTCLIB_ANY && OTCLIB_EQ = ";
    tracer() << list.index(j,OTCLIB_ANY,OTCLIB_EQ) << endl;
    tracer() << "OTCLIB_LAST && OTCLIB_EQ = ";
    tracer() << list.index(j,OTCLIB_LAST,OTCLIB_EQ) << endl;

    tracer() << "OTCLIB_FIRST && OTCLIB_GE = ";
    tracer() << list.index(j,OTCLIB_FIRST,OTCLIB_GE) << endl;
    tracer() << "OTCLIB_ANY && OTCLIB_GE = ";
    tracer() << list.index(j,OTCLIB_ANY,OTCLIB_GE) << endl;
    tracer() << "OTCLIB_LAST && OTCLIB_GE = ";
    tracer() << list.index(j,OTCLIB_LAST,OTCLIB_GE) << endl;

    tracer() << "OTCLIB_FIRST && OTCLIB_GT = ";
    tracer() << list.index(j,OTCLIB_FIRST,OTCLIB_GT) << endl;
    tracer() << "OTCLIB_ANY && OTCLIB_GT = ";
    tracer() << list.index(j,OTCLIB_ANY,OTCLIB_GT) << endl;
    tracer() << "OTCLIB_LAST && OTCLIB_GT = ";
    tracer() << list.index(j,OTCLIB_LAST,OTCLIB_GT) << endl;
  }
}

// = TEST 12
//     Test of adding iterator.

void test12()
{
  OTC_Tracer tracer("void test12()");

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

  OTC_OrderedList<int> ordlist;

  ordlist.add(list.items());

  tracer() << "ordlist.population() = " << ordlist.population() << endl;
  tracer() << "ordlist.isEmpty() = " << ordlist.isEmpty() << endl;

  iter = ordlist.items();
  tracer() << "ordlist.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;
}

// = TEST 13
//     Test of items with supplied rank function.

int test13_rank(char const* const& s1, char const* const& s2)
{
  int l1 = strlen(s1);
  int l2 = strlen(s2);

  if (l1 <= l2)
    return OTC_Rank::rank(s1,l1,s2,l1);

  return OTC_Rank::rank(s1,l1,s2,l2);
}

void test13()
{
  OTC_Tracer tracer("void test13()");

  char buf1[1024];
  ifstream in("/usr/dict/words");

  OTC_OrderedList<char const*> list;

  while (in.good())
  {
    in >> buf1;
    if (!in.fail())
    {
      if (!list.contains(buf1))
      {
        char* buf2;
        buf2 = new char[strlen(buf1)+1];
        strcpy(buf2,buf1);
        list.add(buf2);
      }
    }
  }

  cout << "loaded" << endl;
  cout << "> " << flush;

  OTC_Iterator<char const*> items;
  OTC_Range range;

  while (cin.good())
  {
    cin >> buf1;
    if (!cin.fail())
    {
      items = list.items((char const*)buf1,test13_rank);
      while (items.isValid())
      {
        tracer() << items.item() << endl;
        items.next();
      }
      range = list.range((char const*)buf1,test13_rank);
      tracer() << range.lower() << ":" << range.length() << endl;
      cout << "> " << flush;
    }
  }

  items = list.items();
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
  test8();
  test9();
  test10();
  test11();
  test12();
}

/* ------------------------------------------------------------------------- */

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
  test9,
  test10,
  test11,
  test12,
  test13
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
