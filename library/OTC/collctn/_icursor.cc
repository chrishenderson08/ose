/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_icursor.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/holder.hh>
#include <OTC/collctn/iterator.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_LinkList* list = new OTC_LinkList;

  OTC_Holder<int>* holder;
  for (int i=0; i<10; i++)
  {
    holder = new OTC_Holder<int>(i);
    list->addLast(holder);
  }

  OTC_Iterator<int> iter = 0;
  iter = new OTC_ItemCursor< int,OTC_Holder<int> >(list);
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << iter.item() << endl;
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
