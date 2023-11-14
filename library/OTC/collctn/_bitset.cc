/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_bitset.cc
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

#include <OTC/debug/tracer.hh>
#include <OTC/collctn/bitset.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

ostream& operator<<(ostream& outs, OTC_BitSet const& theSet)
{
  for (int i=theSet.range().lower(); i<theSet.range().upper(); i++)
  {
    if (theSet[i] != 0)
      outs << '1';
    else
      outs << '0';
  }
  return outs;
}

ostream& operator<<(ostream& outs, OTC_Range const& theRange)
{
  outs << "[" << theRange.lower() << ":" << theRange.upper() << "]";
  return outs;
}

void test2()
{
  OTC_Tracer tracer("void test1()");

  OTC_BitSet bitset1(12);
  OTC_BitSet bitset2(0,12);
  OTC_BitSet bitset3(OTC_Range(0,12));
  OTC_BitSet bitset4(bitset3);
  OTC_BitSet bitset5(bitset4,OTCLIB_SHALLOWCOPY);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;
  tracer() << "bitset3 = " << bitset3 << endl;
  tracer() << "bitset4 = " << bitset4 << endl;
  tracer() << "bitset5 = " << bitset5 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;
  tracer() << "bitset3.range() = " << bitset3.range() << endl;
  tracer() << "bitset4.range() = " << bitset4.range() << endl;
  tracer() << "bitset5.range() = " << bitset5.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;
  tracer() << "bitset3.count() = " << bitset3.count() << endl;
  tracer() << "bitset4.count() = " << bitset4.count() << endl;
  tracer() << "bitset5.count() = " << bitset5.count() << endl;

  tracer() << "(bitset1 == bitset5) = " << (bitset1 == bitset5) << endl;
  tracer() << "(bitset4 == bitset5) = " << (bitset4 == bitset5) << endl;
}

void test3()
{
  OTC_Tracer tracer("test3()");

  OTC_BitSet bitset1(12);
  OTC_BitSet bitset2(0,12);
  OTC_BitSet bitset3(OTC_Range(0,12));
  OTC_BitSet bitset4(bitset3);
  OTC_BitSet bitset5(bitset4,OTCLIB_SHALLOWCOPY);

  bitset1.complement();
  bitset2.complement();
  bitset3.complement();
  bitset4.complement();
  bitset5.complement();

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;
  tracer() << "bitset3 = " << bitset3 << endl;
  tracer() << "bitset4 = " << bitset4 << endl;
  tracer() << "bitset5 = " << bitset5 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;
  tracer() << "bitset3.range() = " << bitset3.range() << endl;
  tracer() << "bitset4.range() = " << bitset4.range() << endl;
  tracer() << "bitset5.range() = " << bitset5.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;
  tracer() << "bitset3.count() = " << bitset3.count() << endl;
  tracer() << "bitset4.count() = " << bitset4.count() << endl;
  tracer() << "bitset5.count() = " << bitset5.count() << endl;

  tracer() << "(bitset1 == bitset5) = " << (bitset1 == bitset5) << endl;
  tracer() << "(bitset4 == bitset5) = " << (bitset4 == bitset5) << endl;
}

void test4()
{
  OTC_Tracer tracer("test4()");

  OTC_BitSet bitset1(12);
  OTC_BitSet bitset2(0,12);
  OTC_BitSet bitset3(OTC_Range(0,12));
  OTC_BitSet bitset4(bitset3);
  OTC_BitSet bitset5(bitset4,OTCLIB_SHALLOWCOPY);

  int i;

  for (i=bitset1.range().lower(); i<bitset1.range().upper(); i+=2)
    bitset1.complement(i);
  for (i=bitset2.range().lower()+1; i<bitset2.range().upper(); i+=2)
    bitset2.complement(i,1);

  bitset3.complement(bitset3.range().lower()+1,bitset3.range().length()-2);

  bitset4.complement(bitset4.range().lower()+1,bitset4.range().length()-2);
  bitset5.complement(bitset5.range().lower()+2,bitset5.range().length()-4);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;
  tracer() << "bitset3 = " << bitset3 << endl;
  tracer() << "bitset4 = " << bitset4 << endl;
  tracer() << "bitset5 = " << bitset5 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;
  tracer() << "bitset3.range() = " << bitset3.range() << endl;
  tracer() << "bitset4.range() = " << bitset4.range() << endl;
  tracer() << "bitset5.range() = " << bitset5.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;
  tracer() << "bitset3.count() = " << bitset3.count() << endl;
  tracer() << "bitset4.count() = " << bitset4.count() << endl;
  tracer() << "bitset5.count() = " << bitset5.count() << endl;

  tracer() << "(bitset1 == bitset5) = " << (bitset1 == bitset5) << endl;
  tracer() << "(bitset4 == bitset5) = " << (bitset4 == bitset5) << endl;
}

void test5()
{
  OTC_Tracer tracer("test5()");

  OTC_BitSet bitset1(12);
  OTC_BitSet bitset2(12);

  bitset1.complement();
  bitset2.complement(0,12);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;

  tracer() << "(bitset1 == bitset2) = " << (bitset1 == bitset2) << endl;
}

void test6()
{
  OTC_Tracer tracer("test6()");

  OTC_BitSet bitset1(12);
  OTC_BitSet bitset2(12);
  OTC_BitSet bitset3(12);

  bitset1.complement();
  bitset2.set();
  bitset3.set(0,12);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;
  tracer() << "bitset3 = " << bitset3 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;
  tracer() << "bitset3.range() = " << bitset3.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;
  tracer() << "bitset3.count() = " << bitset3.count() << endl;

  tracer() << "(bitset1 == bitset2) = " << (bitset1 == bitset2) << endl;
  tracer() << "(bitset1 == bitset3) = " << (bitset1 == bitset3) << endl;
}

void test7()
{
  OTC_Tracer tracer("test7()");

  OTC_BitSet bitset1(12);
  OTC_BitSet bitset2(12);
  OTC_BitSet bitset3(12);

  bitset1.complement();
  bitset2.complement();
  bitset3.complement();

  bitset1.complement();
  bitset2.reset();
  bitset3.reset(0,12);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;
  tracer() << "bitset3 = " << bitset3 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;
  tracer() << "bitset3.range() = " << bitset3.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;
  tracer() << "bitset3.count() = " << bitset3.count() << endl;

  tracer() << "(bitset1 == bitset2) = " << (bitset1 == bitset2) << endl;
  tracer() << "(bitset1 == bitset3) = " << (bitset1 == bitset3) << endl;
}

void test8()
{
  OTC_Tracer tracer("test8()");

  OTC_BitSet bitset1(12);

  int i;

  for (i=bitset1.range().lower(); i<bitset1.range().upper(); i+=2)
    bitset1.complement(i);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "~bitset1 = " << ~bitset1 << endl;
}

void test9()
{
  OTC_Tracer tracer("test9()");

  OTC_BitSet bitset1(12);
  OTC_BitSet bitset2(8);

  tracer() << "(bitset1 == bitset2) = " << (bitset1 == bitset2) << endl;

  bitset1.set(4,4);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;

  tracer() << "(bitset1 == bitset2) = " << (bitset1 == bitset2) << endl;

  bitset2.set(4,4);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;

  tracer() << "(bitset1 == bitset2) = " << (bitset1 == bitset2) << endl;
}

void test10()
{
  OTC_Tracer tracer("test10()");

  OTC_BitSet bitset1(0,16);
  OTC_BitSet bitset2(-11,16);

  bitset1.complement(4,12);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;

  OTC_BitSet bitset3 = bitset1 | bitset2;

  tracer() << "bitset3 (bitset1 | bitset2) = " << bitset3 << endl;

  tracer() << "bitset3.range() = " << bitset3.range() << endl;
  tracer() << "bitset3.count() = " << bitset3.count() << endl;

  bitset2.complement(-10,15);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;

  OTC_BitSet bitset4 = bitset1 ^ bitset2;

  tracer() << "bitset4 (bitset1 ^ bitset2) = " << bitset4 << endl;

  tracer() << "bitset4.range() = " << bitset4.range() << endl;
  tracer() << "bitset4.count() = " << bitset4.count() << endl;

  OTC_BitSet bitset5 = bitset1 & bitset2;

  tracer() << "bitset5 (bitset1 & bitset2) = " << bitset5 << endl;

  tracer() << "bitset5.range() = " << bitset5.range() << endl;
  tracer() << "bitset5.count() = " << bitset5.count() << endl;
}

void test11()
{
  OTC_Tracer tracer("test11()");

  OTC_BitSet bitset1(1);
  OTC_BitSet bitset2(1);

  bitset2.set();

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;

  tracer() << "(bitset1 | bitset1) = " << (bitset1 | bitset1) << endl;
  tracer() << "(bitset1 ^ bitset1) = " << (bitset1 ^ bitset1) << endl;
  tracer() << "(bitset1 & bitset1) = " << (bitset1 & bitset1) << endl;
  tracer() << "(bitset1 | bitset2) = " << (bitset1 | bitset2) << endl;
  tracer() << "(bitset1 ^ bitset2) = " << (bitset1 ^ bitset2) << endl;
  tracer() << "(bitset1 & bitset2) = " << (bitset1 & bitset2) << endl;
  tracer() << "(bitset2 | bitset2) = " << (bitset2 | bitset2) << endl;
  tracer() << "(bitset2 ^ bitset2) = " << (bitset2 ^ bitset2) << endl;
  tracer() << "(bitset2 & bitset2) = " << (bitset2 & bitset2) << endl;
}

void test12()
{
  OTC_Tracer tracer("test12()");

  OTC_BitSet bitset1(12);
  OTC_BitSet bitset2(0,12);
  OTC_BitSet bitset3(OTC_Range(0,12));
  OTC_BitSet bitset4(bitset3);
  OTC_BitSet bitset5(bitset4,OTCLIB_SHALLOWCOPY);

  int i;

  for (i=bitset1.range().lower(); i<bitset1.range().upper(); i+=2)
    bitset1.complement(i);
  for (i=bitset2.range().lower()+1; i<bitset2.range().upper(); i+=2)
    bitset2.complement(i,1);

  bitset3.complement(bitset3.range().lower()+1,bitset3.range().length()-2);

  bitset4.complement(bitset4.range().lower()+1,bitset4.range().length()-2);
  bitset5.complement(bitset5.range().lower()+2,bitset5.range().length()-4);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset2 = " << bitset2 << endl;
  tracer() << "bitset3 = " << bitset3 << endl;
  tracer() << "bitset4 = " << bitset4 << endl;
  tracer() << "bitset5 = " << bitset5 << endl;

  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset2.range() = " << bitset2.range() << endl;
  tracer() << "bitset3.range() = " << bitset3.range() << endl;
  tracer() << "bitset4.range() = " << bitset4.range() << endl;
  tracer() << "bitset5.range() = " << bitset5.range() << endl;

  tracer() << "bitset1.count() = " << bitset1.count() << endl;
  tracer() << "bitset2.count() = " << bitset2.count() << endl;
  tracer() << "bitset3.count() = " << bitset3.count() << endl;
  tracer() << "bitset4.count() = " << bitset4.count() << endl;
  tracer() << "bitset5.count() = " << bitset5.count() << endl;

  tracer() << "bitset1.hash() = " << bitset1.hash() << endl;
  tracer() << "bitset2.hash() = " << bitset2.hash() << endl;
  tracer() << "bitset3.hash() = " << bitset3.hash() << endl;
  tracer() << "bitset4.hash() = " << bitset4.hash() << endl;
  tracer() << "bitset5.hash() = " << bitset5.hash() << endl;

  tracer() << "bitset1.rank(bitset2) = " << bitset1.rank(bitset2) << endl;
  tracer() << "bitset2.rank(bitset1) = " << bitset2.rank(bitset1) << endl;
  tracer() << "bitset1.rank(bitset3) = " << bitset1.rank(bitset3) << endl;
  tracer() << "bitset1.rank(bitset4) = " << bitset1.rank(bitset4) << endl;
  tracer() << "bitset4.rank(bitset5) = " << bitset4.rank(bitset5) << endl;
}

void test13()
{
  OTC_Tracer tracer("test13()");

  OTC_BitSet bitset1(0,8192);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset1.count() = " << bitset1.count() << endl;

  bitset1.set(bitset1.range().lower()+2048,2048);
  bitset1.set(bitset1.range().lower()+6144,2048);

  tracer() << "bitset1 = " << bitset1 << endl;
  tracer() << "bitset1.range() = " << bitset1.range() << endl;
  tracer() << "bitset1.count() = " << bitset1.count() << endl;
}

void test1()
{
  OTC_Tracer tracer("test1()");

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
  test13();
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
