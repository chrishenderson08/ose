/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_itmap.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/itmap.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//      Test properties of an empty map.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_ITMap<int,int> map;

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  tracer() << "map.removeAll()" << endl;
  map.removeAll();

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  tracer() << "map.isValid() = " << map.isValid() << endl;
}

// = TEST 3
//     Test of addition. Use iterator to check.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_ITMap<int,int> map;

  for (int i=0; i<100; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  tracer() << "map.keys() =" << flush;
  for (map.resetFirst(); map.isValid(); map.next())
    tracer() << " " << map.key() << flush;
  tracer() << endl;

  tracer() << "map.items() =" << flush;
  for (map.resetFirst(); map.isValid(); map.next())
    tracer() << " " << map.item() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment operator.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_ITMap<int,int> map1;

  for (int i=0; i<100; i++)
    map1.add(i,i);

  tracer() << "map1.population() = " << map1.population() << endl;
  tracer() << "map1.isEmpty() = " << map1.isEmpty() << endl;

  tracer() << "map2(map1)" << endl;
  OTC_ITMap<int,int> map2(map1);

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

  OTC_ITMap<int,int> map;

  for (int i=0; i<100; i++)
    map.add(i,i);

  tracer() << "map items =" << flush;
  for (map.resetFirst(); map.isValid(); map.next())
  {
    int value = map.key();
    if (!map.contains(value))
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << map.item(value) << flush;
    value = -value - 1;
    if (map.contains(value))
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 6
//     Test of explicit removal.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_ITMap<int,int> map;

  for (int i=0; i<100; i++)
    map.add(i,i);

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  for (map.resetFirst(); map.isValid(); map.next())
  {
    int value = map.key();
    map.remove(value);
  }

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  tracer() << "map.keys() =" << flush;
  for (map.resetFirst(); map.isValid(); map.next())
    tracer() << " " << map.key() << flush;
  tracer() << endl;

  tracer() << "map.items() =" << flush;
  for (map.resetFirst(); map.isValid(); map.next())
    tracer() << " " << map.item() << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of large map.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_ITMap<int,int> map;

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
 
  OTC_ITMap<int,double> map;
 
  for (int i=0; i<100; i++)
  {
    double j = (double)i / 2;
    map.add(i,j);
  }
 
  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;
 
  tracer() << "map.pairs() =" << flush;
  for (map.resetFirst(); map.isValid(); map.next())
    tracer() << " (" << map.key() << "," << map.item() << ")" << flush;
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
