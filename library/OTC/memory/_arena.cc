/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/_arena.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/arena.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Arena theArena;

  tracer() << "blockSize=" << theArena.blockSize() << endl;
  tracer() << "slop=" << theArena.slop() << endl;
  tracer() << "align=" << theArena.align() << endl;

  tracer() << "freeBlocks=" << theArena.freeBlocks() << endl;
  tracer() << "fullBlocks=" << theArena.fullBlocks() << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Arena theArena(1);

  tracer() << "blockSize=" << theArena.blockSize() << endl;
  tracer() << "slop=" << theArena.slop() << endl;
  tracer() << "align=" << theArena.align() << endl;

  theArena.allocate(1);

  tracer() << "freeBlocks=" << theArena.freeBlocks() << endl;
  tracer() << "fullBlocks=" << theArena.fullBlocks() << endl;
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Arena theArena(1);

  tracer() << "blockSize=" << theArena.blockSize() << endl;
  tracer() << "slop=" << theArena.slop() << endl;
  tracer() << "align=" << theArena.align() << endl;

  theArena.allocate(theArena.blockSize()-theArena.slop()-1);

  tracer() << "freeBlocks=" << theArena.freeBlocks() << endl;
  tracer() << "fullBlocks=" << theArena.fullBlocks() << endl;
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Arena theArena(1);

  tracer() << "blockSize=" << theArena.blockSize() << endl;
  tracer() << "slop=" << theArena.slop() << endl;
  tracer() << "align=" << theArena.align() << endl;

  theArena.allocate(theArena.blockSize()-theArena.slop()+1);

  tracer() << "freeBlocks=" << theArena.freeBlocks() << endl;
  tracer() << "fullBlocks=" << theArena.fullBlocks() << endl;
}

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Arena theArena(1);

  tracer() << "blockSize=" << theArena.blockSize() << endl;
  tracer() << "slop=" << theArena.slop() << endl;
  tracer() << "align=" << theArena.align() << endl;

  theArena.allocate(theArena.blockSize()+1);

  tracer() << "freeBlocks=" << theArena.freeBlocks() << endl;
  tracer() << "fullBlocks=" << theArena.fullBlocks() << endl;
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Arena theArena(1);

  tracer() << "blockSize=" << theArena.blockSize() << endl;
  tracer() << "slop=" << theArena.slop() << endl;
  tracer() << "align=" << theArena.align() << endl;

  theArena.allocate(theArena.blockSize()-theArena.slop()+1);
  theArena.allocate(theArena.blockSize()-theArena.slop()+1);
  theArena.allocate(theArena.blockSize()-theArena.slop()+1);

  tracer() << "freeBlocks=" << theArena.freeBlocks() << endl;
  tracer() << "fullBlocks=" << theArena.fullBlocks() << endl;
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_Arena theArena(1);

  tracer() << "blockSize=" << theArena.blockSize() << endl;
  tracer() << "slop=" << theArena.slop() << endl;
  tracer() << "align=" << theArena.align() << endl;

  theArena.allocate(theArena.blockSize()-theArena.slop()+1);
  theArena.allocate(theArena.blockSize()-theArena.slop()+1);
  theArena.allocate(theArena.blockSize()-theArena.slop()+1);

  theArena.release(2);

  tracer() << "freeBlocks=" << theArena.freeBlocks() << endl;
  tracer() << "fullBlocks=" << theArena.fullBlocks() << endl;
}

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_Arena theArena(1);

  tracer() << "blockSize=" << theArena.blockSize() << endl;
  tracer() << "slop=" << theArena.slop() << endl;
  tracer() << "align=" << theArena.align() << endl;

  theArena.allocate(theArena.blockSize()-theArena.slop()+1);
  theArena.allocate(theArena.blockSize()-theArena.slop()+1);
  theArena.allocate(theArena.blockSize()-theArena.slop()+1);

  theArena.release(2);

  theArena.allocate(theArena.blockSize()-theArena.slop()+1);

  tracer() << "freeBlocks=" << theArena.freeBlocks() << endl;
  tracer() << "fullBlocks=" << theArena.fullBlocks() << endl;
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
