/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_huniqmap.cc
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

#include <OTC/collctn/huniqmap.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

extern long tst_random();
extern void tst_srandom(long, long);

/* ------------------------------------------------------------------------- */

// = TEST 2
//      Test properties of an empty map.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_HUniqMap<int,int> map;

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  tracer() << "map.removeAll()" << endl;
  map.removeAll();

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  OTC_Iterator<int> keys = 0;
  keys = map.keys();
  tracer() << "keys.isValid() = " << keys.isValid() << endl;

  OTC_Iterator<int> items = 0;
  items = map.items();
  tracer() << "items.isValid() = " << items.isValid() << endl;
}

// = TEST 3
//     Test of addition. Use iterator to check.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_HUniqMap<int,int> map;

  for (int i=0; i<100; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  OTC_Iterator<int> keys = 0;
  keys = map.keys();
  tracer() << "map.keys() =" << flush;
  for (keys.reset(); keys.isValid(); keys.next())
    tracer() << " " << keys.item() << flush;
  tracer() << endl;

  OTC_Iterator<int> items = 0;
  items = map.items();
  tracer() << "map.items() =" << flush;
  for (items.reset(); items.isValid(); items.next())
    tracer() << " " << items.item() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment operator.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_HUniqMap<int,int> map1;

  for (int i=0; i<100; i++)
    map1.add(i,i);

  tracer() << "map1.population() = " << map1.population() << endl;
  tracer() << "map1.isEmpty() = " << map1.isEmpty() << endl;

  tracer() << "map2(map1)" << endl;
  OTC_HUniqMap<int,int> map2(map1);

  tracer() << "map1.population() = " << map1.population() << endl;
  tracer() << "map1.isEmpty() = " << map1.isEmpty() << endl;

  tracer() << "map2.population() = " << map2.population() << endl;
  tracer() << "map2.isEmpty() = " << map2.isEmpty() << endl;

  tracer() << "map2 = map2" << endl;
  map2 = map2;

  tracer() << "map2.population() = " << map2.population() << endl;
  tracer() << "map2.isEmpty() = " << map2.isEmpty() << endl;

  tracer() << "map2.removeAll()" << endl;
  map2.removeAll();

  tracer() << "map2.population() = " << map2.population() << endl;
  tracer() << "map2.isEmpty() = " << map2.isEmpty() << endl;

  tracer() << "map2 = map1" << endl;
  map2 = map1;

  tracer() << "map2.population() = " << map2.population() << endl;
  tracer() << "map2.isEmpty() = " << map2.isEmpty() << endl;
}

// = TEST 5
//     Test of contains and item.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_HUniqMap<int,int> map;

  for (int i=0; i<100; i++)
    map.add(i,i);

  OTC_Iterator<int> iter = 0;
  iter = map.keys();
  tracer() << "map items =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
  {
    int value = iter.item();
    if (!map.containsKey(value) || !map.containsItem(value))
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << map.item(value) << "/";
    tracer() << map.item(value) << flush;
    value = -value - 1;
    if (map.containsKey(value) || map.containsItem(value))
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 6
//     Test of explicit removal.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_HUniqMap<int,int> map;

  int i;

  for (i=0; i<100; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  OTC_Iterator<int> keys = 0;
  keys = map.keys();
  for (keys.reset(); keys.isValid(); keys.next())
  {
    int value = keys.item();
    map.removeKey(value);
  }

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  keys = map.keys();
  tracer() << "map.keys() =" << flush;
  for (keys.reset(); keys.isValid(); keys.next())
    tracer() << " " << keys.item() << flush;
  tracer() << endl;

  OTC_Iterator<int> items = 0;
  items = map.items();
  tracer() << "map.items() =" << flush;
  for (items.reset(); items.isValid(); items.next())
    tracer() << " " << items.item() << flush;
  tracer() << endl;

  for (i=0; i<100; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  items = map.items();
  for (items.reset(); items.isValid(); items.next())
  {
    int value = items.item();
    map.removeItem(value);
  }

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  keys = map.keys();
  tracer() << "map.keys() =" << flush;
  for (keys.reset(); keys.isValid(); keys.next())
    tracer() << " " << keys.item() << flush;
  tracer() << endl;

  items = map.items();
  tracer() << "map.items() =" << flush;
  for (items.reset(); items.isValid(); items.next())
    tracer() << " " << items.item() << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of large map.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_HUniqMap<int,int> map;

  for (int i=0; i<2500; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  tracer() << "map.removeAll()" << endl;
  map.removeAll();

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;
}

// = TEST 8
//     Test of pairs().

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_HUniqMap<int,double> map;

  for (int i=0; i<100; i++)
  {
    double j = (double)i / 2;
    map.add(i,j);
  }

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  OTC_PairIterator<int,double> pairs = 0;
  pairs = map.pairs();
  tracer() << "map.pairs() =" << flush;
  for (pairs.reset(); pairs.isValid(); pairs.next())
    tracer() << " (" << pairs.key() << "," << pairs.item() << ")" << flush;
  tracer() << endl;
}

// = TEST 9
//     Test of adding iterator.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_HMap<int,double> pairs;

  tst_srandom(3,0);
  for (int i=0; i<10; i++)
  {
    int value = int(tst_random() % 50);
    pairs.add(value,double(value)/2,OTCLIB_REPLACE);
  }

  tracer() << "pairs.population() = " << pairs.population() << endl;
  tracer() << "pairs.isEmpty() = " << pairs.isEmpty() << endl;

  OTC_PairIterator<int,double> iter = 0;

  iter = pairs.pairs();
  tracer() << "pairs.pairs() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " (" << iter.key() << ":" << iter.item() << ")" << flush;
  tracer() << endl;

  OTC_HUniqMap<int,double> map;

  map.add(pairs.pairs());

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  iter = map.pairs();
  tracer() << "map.pairs() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " (" << iter.key() << ":" << iter.item() << ")" << flush;
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
