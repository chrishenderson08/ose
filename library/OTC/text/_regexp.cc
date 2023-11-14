/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/_regexp.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/regexp.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test precompiled patterns are valid.

void test2()
{
  OTC_Tracer tracer("void test2()");

  tracer() << "OTC_Regexp::whiteSpace().pattern() = ";
  tracer() << OTC_Regexp::whiteSpace().pattern() << endl;
  tracer() << "OTC_Regexp::whiteSpace().isValid() = ";
  tracer() << OTC_Regexp::whiteSpace().isValid() << endl;

  tracer() << "OTC_Regexp::optWhiteSpace().pattern() = ";
  tracer() << OTC_Regexp::optWhiteSpace().pattern() << endl;
  tracer() << "OTC_Regexp::optWhiteSpace().isValid() = ";
  tracer() << OTC_Regexp::optWhiteSpace().isValid() << endl;

  tracer() << "OTC_Regexp::nonWhiteSpace().pattern() = ";
  tracer() << OTC_Regexp::nonWhiteSpace().pattern() << endl;
  tracer() << "OTC_Regexp::nonWhiteSpace().isValid() = ";
  tracer() << OTC_Regexp::nonWhiteSpace().isValid() << endl;

  tracer() << "OTC_Regexp::alphabetic().pattern() = ";
  tracer() << OTC_Regexp::alphabetic().pattern() << endl;
  tracer() << "OTC_Regexp::alphabetic().isValid() = ";
  tracer() << OTC_Regexp::alphabetic().isValid() << endl;

  tracer() << "OTC_Regexp::lowerCase().pattern() = ";
  tracer() << OTC_Regexp::lowerCase().pattern() << endl;
  tracer() << "OTC_Regexp::lowerCase().isValid() = ";
  tracer() << OTC_Regexp::lowerCase().isValid() << endl;

  tracer() << "OTC_Regexp::upperCase().pattern() = ";
  tracer() << OTC_Regexp::upperCase().pattern() << endl;
  tracer() << "OTC_Regexp::upperCase().isValid() = ";
  tracer() << OTC_Regexp::upperCase().isValid() << endl;

  tracer() << "OTC_Regexp::alphaNumeric().pattern() = ";
  tracer() << OTC_Regexp::alphaNumeric().pattern() << endl;
  tracer() << "OTC_Regexp::alphaNumeric().isValid() = ";
  tracer() << OTC_Regexp::alphaNumeric().isValid() << endl;

  tracer() << "OTC_Regexp::identifier().pattern() = ";
  tracer() << OTC_Regexp::identifier().pattern() << endl;
  tracer() << "OTC_Regexp::identifier().isValid() = ";
  tracer() << OTC_Regexp::identifier().isValid() << endl;

  tracer() << "OTC_Regexp::matchingQuotes().pattern() = ";
  tracer() << OTC_Regexp::matchingQuotes().pattern() << endl;
  tracer() << "OTC_Regexp::matchingQuotes().isValid() = ";
  tracer() << OTC_Regexp::matchingQuotes().isValid() << endl;

  tracer() << "OTC_Regexp::integerValue().pattern() = ";
  tracer() << OTC_Regexp::integerValue().pattern() << endl;
  tracer() << "OTC_Regexp::integerValue().isValid() = ";
  tracer() << OTC_Regexp::integerValue().isValid() << endl;

  tracer() << "OTC_Regexp::realValue().pattern() = ";
  tracer() << OTC_Regexp::realValue().pattern() << endl;
  tracer() << "OTC_Regexp::realValue().isValid() = ";
  tracer() << OTC_Regexp::realValue().isValid() << endl;

  tracer() << "OTC_Regexp::quotedString().pattern() = ";
  tracer() << OTC_Regexp::quotedString().pattern() << endl;
  tracer() << "OTC_Regexp::quotedString().isValid() = ";
  tracer() << OTC_Regexp::quotedString().isValid() << endl;
}

// = TEST 3
//     Test of whitespace pattern.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Regexp& pattern = OTC_Regexp::whiteSpace();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"ABcd\") = " << pattern.match("ABcd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12 Cd\") = " << pattern.match("12 Cd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 4
//     Test of optional whitespace pattern.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Regexp& pattern = OTC_Regexp::optWhiteSpace();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"ABcd\") = " << pattern.match("ABcd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12 Cd\") = " << pattern.match("12 Cd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 5
//     Test of nonwhitespace pattern.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Regexp& pattern = OTC_Regexp::nonWhiteSpace();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"ABcd\") = " << pattern.match("ABcd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12 Cd\") = " << pattern.match("12 Cd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 6
//     Test of alphabetic pattern.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Regexp& pattern = OTC_Regexp::alphabetic();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"ABcd\") = " << pattern.match("ABcd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12 Cd\") = " << pattern.match("12 Cd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 7
//     Test of lowercase pattern.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Regexp& pattern = OTC_Regexp::lowerCase();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"ABcd\") = " << pattern.match("ABcd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12 Cd\") = " << pattern.match("12 Cd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 8
//     Test of uppercase pattern.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_Regexp& pattern = OTC_Regexp::upperCase();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"ABcd\") = " << pattern.match("ABcd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12 Cd\") = " << pattern.match("12 Cd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 9
//     Test of alphanumeric pattern.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_Regexp& pattern = OTC_Regexp::alphaNumeric();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"ABcd\") = " << pattern.match("ABcd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12Cd\") = " << pattern.match("12Cd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 10
//     Test of identifier pattern.

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_Regexp& pattern = OTC_Regexp::identifier();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"ABcd\") = " << pattern.match("ABcd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12Cd\") = " << pattern.match("12Cd") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"Cd12\") = " << pattern.match("Cd12") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 11
//     Test of matchingQuotes pattern.

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_Regexp& pattern = OTC_Regexp::matchingQuotes();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  char const* s1 = "A\"Bc\"d";
  tracer() << "s1 = " << s1 << endl;
  tracer() << "pattern.match(s1) = ";
  tracer() << pattern.match(s1) << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  char const* s2 = "12\"Cd";
  tracer() << "s2 = " << s2 << endl;
  tracer() << "pattern.match(s2) = ";
  tracer() << pattern.match(s2) << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  char const* s3 = "\"Cd\" \"12\"";
  tracer() << "s3 = " << s3 << endl;
  tracer() << "pattern.match(s3) = ";
  tracer() << pattern.match(s3) << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  char const* s4 = "\"Cd\\\"\\\"12\"";
  tracer() << "s4 = " << s4 << endl;
  tracer() << "pattern.match(s4) = ";
  tracer() << pattern.match(s4) << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 12
//     Test of integervalue pattern.

void test12()
{
  OTC_Tracer tracer("void test12()");

  OTC_Regexp& pattern = OTC_Regexp::integerValue();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12.34\") = " << pattern.match("12.34") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12ab\") = " << pattern.match("12ab") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 13
//     Test of real value pattern.

void test13()
{
  OTC_Tracer tracer("void test13()");

  OTC_Regexp& pattern = OTC_Regexp::realValue();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12\") = " << pattern.match("12") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"-12\") = " << pattern.match("-12") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"12.34\") = " << pattern.match("12.34") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"-12.34\") = " << pattern.match("-12.34") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\".34\") = " << pattern.match(".34") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"-.34\") = " << pattern.match("-.34") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"3e1\") = " << pattern.match("3e1") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"3E1\") = " << pattern.match("3E1") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"3E-1\") = " << pattern.match("3E-1") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"3E+1\") = " << pattern.match("3E+1") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 14
//     Test of quoted string pattern.

void test14()
{
  OTC_Tracer tracer("void test14()");

  OTC_Regexp& pattern = OTC_Regexp::quotedString();
  tracer() << "pattern = " << pattern.pattern() << endl;

  tracer() << "pattern.match(\"\") = " << pattern.match("") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\" \") = " << pattern.match(" ") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  tracer() << "pattern.match(\"1234\") = " << pattern.match("1234") << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  char const* s1 = "A\"Bc\"d";
  tracer() << "s1 = " << s1 << endl;
  tracer() << "pattern.match(s1) = ";
  tracer() << pattern.match(s1) << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  char const* s2 = "12\"Cd";
  tracer() << "s2 = " << s2 << endl;
  tracer() << "pattern.match(s2) = ";
  tracer() << pattern.match(s2) << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  char const* s3 = "\"Cd\" \"12\" \"34\" 56";
  tracer() << "s3 = " << s3 << endl;
  tracer() << "pattern.match(s3) = ";
  tracer() << pattern.match(s3) << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;

  char const* s4 = "\"Cd\\\"\\\"12\" \"34\" 56";
  tracer() << "s4 = " << s4 << endl;
  tracer() << "pattern.match(s4) = ";
  tracer() << pattern.match(s4) << endl;
  tracer() << "pattern.start() = " << pattern.start() << endl;
  tracer() << "pattern.length() = " << pattern.length() << endl;
}

// = TEST 15
//     Test of large number of sub patterns.

void test15()
{
  OTC_Tracer tracer("void test15()");

  char const* s1 = "(1*)(2*)(3*)(4*)(5*)(6*)(7*)(8*)(9*)(0*)";

  OTC_Regexp pattern1(s1);

  if (!pattern1.isValid())
    tracer() << "error1 = " << pattern1.error() << endl;

  OTC_Regexp pattern2(s1,10);

  if (!pattern2.isValid())
    tracer() << "error2 = " << pattern2.error() << endl;

  tracer() << "match = " << pattern2.match("11223344556677889900") << endl;

  for (u_int i=0; i<=10; i++)
    tracer() << "p" << i << " = " << pattern2.start(i) << endl;
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
  test13();
  test14();
  test15();
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
  test13,
  test14,
  test15
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
