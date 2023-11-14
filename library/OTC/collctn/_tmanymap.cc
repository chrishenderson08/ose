/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_tmanymap.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/tmanymap.hh>
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

  OTC_TManyMap<int,int> theMap1;

  tracer() << "population [1] = " << theMap1.population() << endl;
  tracer() << "populationKey [1] = " << theMap1.populationKey() << endl;

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

  tracer() << "pairs [1] =" << flush;

  thePairs = theMap1.pairs();
  while (thePairs.isValid())
  {
    tracer() << " (" << thePairs.key()
     << ',' << thePairs.item() << ")" << flush;

    thePairs.next();
  }

  tracer() << endl;

  OTC_TManyMap<int,int> theMap2(theMap1,OTCLIB_SHALLOWCOPY);

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
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_TManyMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;

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

  for (i=9; i>=0; i--)
  {
    for (j=3; j>=0; j--)
    {
      tracer() << "add(" << i << "," << j << ")" << endl;
      theMap1.add(i,j);
    }
  }

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;

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
      tracer() << "contains(" << i << "," << j << ") = "
       << theMap1.contains(i,j) << endl;
    }
  }

  tracer() << "removeAll" << endl;

  theMap1.removeAll();

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;

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

  OTC_TManyMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;

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

  OTC_TManyMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;

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

  OTC_TManyMap<int,int> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;

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

  OTC_TManyMap<int,double> theMap1;

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;

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
      tracer() << "contains(" << i << "," << j << ") = "
       << theMap1.contains(i,j) << endl;
    }
  }

  tracer() << "removeAll" << endl;

  theMap1.removeAll();

  tracer() << "population = " << theMap1.population() << endl;
  tracer() << "populationKey = " << theMap1.populationKey() << endl;

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

// = TEST 8
//     Test of adding iterator.

void test8()
{
  OTC_Tracer tracer("void test8()");

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

  OTC_TManyMap<int,double> map;

  map.add(pairs.pairs());

  tracer() << "map.population() = " << map.population() << endl;
  tracer() << "map.isEmpty() = " << map.isEmpty() << endl;

  iter = map.pairs();
  tracer() << "map.pairs() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " (" << iter.key() << ":" << iter.item() << ")" << flush;
  tracer() << endl;
}

// = TEST 9
//     Test of items with supplied rank function.

int test9_rank(char const* const& s1, char const* const& s2)
{
  int l1 = strlen(s1);
  int l2 = strlen(s2);

  if (l1 <= l2)
    return OTC_Rank::rank(s1,l1,s2,l1);

  return OTC_Rank::rank(s1,l1,s2,l2);
}

void test9()
{
  OTC_Tracer tracer("void test9()");

  char buf1[1024];
  ifstream in("/usr/dict/words");

  OTC_TManyMap<char const*,int> map;

  int i = 0;
  while (in.good())
  {
    in >> buf1;
    if (!in.fail())
    {
      if (!map.containsKey(buf1))
      {
	i++;
	int j = -i;
        char* buf2;
        buf2 = new char[strlen(buf1)+1];
        strcpy(buf2,buf1);
        map.add(buf2,i);
        buf2 = new char[strlen(buf1)+1];
        strcpy(buf2,buf1);
        map.add(buf2,j);
      }
    }
  }

  cout << "loaded" << endl;
  cout << "> " << flush;

  OTC_Iterator<int> items;
  OTC_PairIterator<char const*,int> pairs;

  while (cin.good())
  {
    cin >> buf1;
    if (!cin.fail())
    {
      items = map.items(&buf1[0],test9_rank);
      while (items.isValid())
      {
        tracer() << items.item() << endl;
        items.next();
      }
      pairs = map.pairsKey(&buf1[0],test9_rank);
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
