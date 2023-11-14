/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_tdualmap.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/tdualmap.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/fstream.h>

#include <stdlib.h>
#include <string.h>

extern long tst_random();
extern void tst_srandom(long, long);

/* ------------------------------------------------------------------------- */

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_TDualMap<int,int> theMap1;

  tracer() << "population [1] = " << theMap1.population() << endl;
  tracer() << "populationKey [1] = " << theMap1.populationKey() << endl;
  tracer() << "populationItem [1] = " << theMap1.populationItem() << endl;

  tracer() << "pairs [1] =" << flush;

  OTC_PairIterator<int,int> thePairs;
  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  tracer() << "removeAll [1]" << endl;

  theMap1.removeAll();

  tracer() << "population [1] = " << theMap1.population() << endl;
  tracer() << "populationKey [1] = " << theMap1.populationKey() << endl;
  tracer() << "populationItem [1] = " << theMap1.populationItem() << endl;

  tracer() << "pairs [1] =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  OTC_TDualMap<int,int> theMap2(theMap1,OTCLIB_SHALLOWCOPY);

  tracer() << "population [2] = " << theMap2.population() << endl;

  tracer() << "pairs [2]" << flush;

  thePairs = theMap2.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  int ZERO = 0;

  tracer() << "containsKey(0) [1] = " << theMap1.containsKey(ZERO) << endl;
  tracer() << "frequencyKey(0) [1] = " << theMap1.frequencyKey(ZERO) << endl;
  tracer() << "containsItem(0) [1] = " << theMap1.containsItem(ZERO) << endl;
  tracer() << "frequencyItem(0) [1] = " << theMap1.frequencyItem(ZERO) << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_TDualMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  OTC_PairIterator<int,int> thePairs;
  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  int i;
  int j;

  for (i=0; i<10; i++)
  {
    for (j=0; j<4; j++)
    {
      tracer() << "add(" << i << "," << j << ")" << endl;
      theMap1.add(i,j);
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  for (i=-1; i<11; i++)
  {
    tracer() << "containsKey(" << i << ") = "
     << theMap1.containsKey(i) << endl;

    tracer() << "frequencyKey(" << i << ") = "
     << theMap1.frequencyKey(i) << endl;

    for (j=-1; j<5; j++)
    {
      tracer() << "containsItem(" << j << ") = "
       << theMap1.containsItem(j) << endl;

      tracer() << "contains(" << i << "," << j << ") = "
       << theMap1.contains(i,j) << endl;

      tracer() << "frequencyItem(" << j << ") = "
       << theMap1.frequencyItem(j) << endl;
    }
  }

  tracer() << "removeAll" << endl;

  theMap1.removeAll();

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_TDualMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  OTC_PairIterator<int,int> thePairs;
  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  int i;
  int j;

  for (i=0; i<10; i++)
  {
    for (j=0; j<4; j++)
    {
      tracer() << "add(" << i << "," << j << ")" << endl;
      theMap1.add(i,j);
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  for (i=1; i<9; i++)
  {
    for (j=1; j<3; j++)
    {
      tracer() << "remove(" << i << "," << j << ")" << endl;
      theMap1.remove(i,j);
    }
  }

  for (i=-1; i<11; i++)
  {
    tracer() << "containsKey(" << i << ") = "
     << theMap1.containsKey(i) << endl;

    tracer() << "frequencyKey(" << i << ") = "
     << theMap1.frequencyKey(i) << endl;

    for (j=-1; j<5; j++)
    {
      tracer() << "contains(" << i << "," << j << ") = "
       << theMap1.contains(i,j) << endl;
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_TDualMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  OTC_PairIterator<int,int> thePairs;
  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  int i;
  int j;

  for (i=0; i<10; i++)
  {
    for (j=0; j<4; j++)
    {
      tracer() << "add(" << i << "," << j << ")" << endl;
      theMap1.add(i,j);
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  for (i=1; i<9; i++)
  {
    tracer() << "removeKey(" << i << ")" << endl;
    theMap1.removeKey(i);
  }

  for (i=-1; i<11; i++)
  {
    tracer() << "containsKey(" << i << ") = "
     << theMap1.containsKey(i) << endl;

    tracer() << "frequencyKey(" << i << ") = "
     << theMap1.frequencyKey(i) << endl;

    for (j=-1; j<5; j++)
    {
      tracer() << "contains(" << i << "," << j << ") = "
       << theMap1.contains(i,j) << endl;
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;
}

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_TDualMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  OTC_PairIterator<int,int> thePairs;
  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  int i;
  int j;

  for (i=0; i<10; i++)
  {
    for (j=0; j<4; j++)
    {
      tracer() << "add(" << i << "," << j << ")" << endl;
      theMap1.add(i,j);
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  for (i=-1; i<11; i++)
  {
    tracer() << "pairsKey(" << i << ") =" << flush;

    thePairs = theMap1.pairsKey(i);
    while (thePairs.isValid())
    {
      tracer() << " (" << thePairs.key()
       << ',' << thePairs.item() << ")" << flush;

      thePairs.next();
    }

    tracer() << endl;
  }

  OTC_Iterator<int> theItems;

  for (i=-1; i<11; i++)
  {
    tracer() << "items(" << i << ") =" << flush;

    theItems = theMap1.items(i);
    while (theItems.isValid())
    {
      tracer() << " (" << theItems.item() << ")" << flush;

      theItems.next();
    }

    tracer() << endl;
  }
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_TDualMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  OTC_PairIterator<int,int> thePairs;
  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  int i;
  int j;

  for (i=0; i<10; i++)
  {
    for (j=0; j<4; j++)
    {
      tracer() << "add(" << i << "," << j << ")" << endl;
      theMap1.add(i,j);
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  for (j=1; j<3; j++)
  {
    tracer() << "removeItem(" << j << ")" << endl;
    theMap1.removeItem(j);
  }

  for (i=-1; i<11; i++)
  {
    for (j=-1; j<5; j++)
    {
      tracer() << "contains(" << i << "," << j << ") = "
       << theMap1.contains(i,j) << endl;
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_TDualMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  OTC_PairIterator<int,int> thePairs;
  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  int i;
  int j;

  for (i=0; i<10; i++)
  {
    for (j=0; j<4; j++)
    {
      tracer() << "add(" << i << "," << j << ")" << endl;
      theMap1.add(i,j);
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  for (j=-1; j<5; j++)
  {
    tracer() << "pairsItem(" << j << ") =" << flush;

    thePairs = theMap1.pairsItem(j);
    while (thePairs.isValid())
    {
      tracer() << " (" << thePairs.key()
       << ',' << thePairs.item() << ")" << flush;

      thePairs.next();
    }

    tracer() << endl;
  }

  OTC_Iterator<int> theKeys;

  for (j=-1; j<5; j++)
  {
    tracer() << "keys(" << j << ") =" << flush;

    theKeys = theMap1.keys(j);
    while (theKeys.isValid())
    {
      tracer() << " (" << theKeys.item() << ")" << flush;

      theKeys.next();
    }

    tracer() << endl;
  }
}

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_TDualMap<int,double> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  OTC_PairIterator<int,double> thePairs;
  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  int i;
  double j;

  for (i=0; i<10; i++)
  {
    for (j=0; j<4; j++)
    {
      tracer() << "add(" << i << "," << j << ")" << endl;
      theMap1.add(i,j);
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  for (i=-1; i<11; i++)
  {
    tracer() << "containsKey(" << i << ") = "
     << theMap1.containsKey(i) << endl;

    tracer() << "frequencyKey(" << i << ") = "
     << theMap1.frequencyKey(i) << endl;

    for (j=-1; j<5; j++)
    {
      tracer() << "containsItem(" << j << ") = "
       << theMap1.containsItem(j) << endl;

      tracer() << "contains(" << i << "," << j << ") = "
       << theMap1.contains(i,j) << endl;

      tracer() << "frequencyItem(" << j << ") = "
       << theMap1.frequencyItem(j) << endl;
    }
  }

  tracer() << "removeAll" << endl;

  theMap1.removeAll();

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;
  tracer() << "populationItem = " << theMap1.populationItem() << endl;

  tracer() << "pairs =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;
}

// = TEST 10
//     Test of adding iterator.

void test10()
{
  OTC_Tracer tracer("void test10()");

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

  OTC_TDualMap<int,double> map;

  map.add(pairs.pairs());

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  iter = map.pairs();
  tracer() << "map.pairs() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " (" << iter.key() << ":" << iter.item() << ")" << flush;
  tracer() << endl;
}

// = TEST 11
//     Test of items with supplied rank function.

int test11_rank1(char const* const& s1, char const* const& s2)
{
  int l1 = strlen(s1);
  int l2 = strlen(s2);

  if (l1 <= l2)
    return OTC_Rank::rank(s1,l1,s2,l1);

  return OTC_Rank::rank(s1,l1,s2,l2);
}

int test11_rank2(int const& i1, int const& i2)
{
  return i1 - i2;
}

void test11()
{
  OTC_Tracer tracer("void test11()");

  char buf1[1024];
  ifstream in("/usr/dict/words");

  OTC_TDualMap<char const*,int> map;

  int i = 0;
  while (in.good())
  {
    in >> buf1;
    if (!in.fail())
    {
      if (!map.containsKey(buf1))
      {
	i++;
	int j = strlen(buf1);
	int k = -j;
        char* buf2;
        buf2 = new char[j+1];
        strcpy(buf2,buf1);
        map.add(buf2,j);
        buf2 = new char[j+1];
        strcpy(buf2,buf1);
        map.add(buf2,k);
      }
    }
  }

  cout << "loaded" << endl;
  cout << "> " << flush;

  OTC_Iterator<char const*> keys;
  OTC_Iterator<int> items;
  OTC_PairIterator<char const*,int> pairs;

  while (cin.good())
  {
    cin >> buf1;
    if (!cin.fail())
    {
      items = map.items((char const*)buf1,test11_rank1);
      while (items.isValid())
      {
        tracer() << items.item() << endl;
        items.next();
      }
      pairs = map.pairsKey((char const*)buf1,test11_rank1);
      while (pairs.isValid())
      {
        tracer() << pairs.key() << " " << pairs.item() << endl;
        pairs.next();
      }
      int j = strlen(buf1);
      keys = map.keys(j,test11_rank2);
      while (keys.isValid())
      {
        tracer() << keys.item() << endl;
        keys.next();
      }
      pairs = map.pairsItem(j,test11_rank2);
      while (pairs.isValid())
      {
        tracer() << pairs.key() << " " << pairs.item() << endl;
        pairs.next();
      }
      cout << "> " << flush;
    }
  }

  pairs = map.pairs();
  while (pairs.isValid())
  {
    delete [] (char*)pairs.key();
    pairs.next();
  }
}


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
}

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
  test11
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
