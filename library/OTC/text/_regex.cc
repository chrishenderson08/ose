/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/_regex.cc
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

#include <OTC/text/regex.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test precompiled patterns are valid.

void test2()
{
  OTC_Tracer tracer("void test2()");

  tracer() << "OTC_Regex::whiteSpace().pattern() = ";
  tracer() << OTC_Regex::whiteSpace().pattern() << endl;
  tracer() << "OTC_Regex::whiteSpace().isValid() = ";
  tracer() << OTC_Regex::whiteSpace().isValid() << endl;

  tracer() << "OTC_Regex::optWhiteSpace().pattern() = ";
  tracer() << OTC_Regex::optWhiteSpace().pattern() << endl;
  tracer() << "OTC_Regex::optWhiteSpace().isValid() = ";
  tracer() << OTC_Regex::optWhiteSpace().isValid() << endl;

  tracer() << "OTC_Regex::nonWhiteSpace().pattern() = ";
  tracer() << OTC_Regex::nonWhiteSpace().pattern() << endl;
  tracer() << "OTC_Regex::nonWhiteSpace().isValid() = ";
  tracer() << OTC_Regex::nonWhiteSpace().isValid() << endl;

  tracer() << "OTC_Regex::alphabetic().pattern() = ";
  tracer() << OTC_Regex::alphabetic().pattern() << endl;
  tracer() << "OTC_Regex::alphabetic().isValid() = ";
  tracer() << OTC_Regex::alphabetic().isValid() << endl;

  tracer() << "OTC_Regex::lowerCase().pattern() = ";
  tracer() << OTC_Regex::lowerCase().pattern() << endl;
  tracer() << "OTC_Regex::lowerCase().isValid() = ";
  tracer() << OTC_Regex::lowerCase().isValid() << endl;

  tracer() << "OTC_Regex::upperCase().pattern() = ";
  tracer() << OTC_Regex::upperCase().pattern() << endl;
  tracer() << "OTC_Regex::upperCase().isValid() = ";
  tracer() << OTC_Regex::upperCase().isValid() << endl;

  tracer() << "OTC_Regex::alphaNumeric().pattern() = ";
  tracer() << OTC_Regex::alphaNumeric().pattern() << endl;
  tracer() << "OTC_Regex::alphaNumeric().isValid() = ";
  tracer() << OTC_Regex::alphaNumeric().isValid() << endl;

  tracer() << "OTC_Regex::identifier().pattern() = ";
  tracer() << OTC_Regex::identifier().pattern() << endl;
  tracer() << "OTC_Regex::identifier().isValid() = ";
  tracer() << OTC_Regex::identifier().isValid() << endl;

  tracer() << "OTC_Regex::matchingQuotes().pattern() = ";
  tracer() << OTC_Regex::matchingQuotes().pattern() << endl;
  tracer() << "OTC_Regex::matchingQuotes().isValid() = ";
  tracer() << OTC_Regex::matchingQuotes().isValid() << endl;
}

// = TEST 3
//     Test of whitespace pattern.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Regex& pattern = OTC_Regex::whiteSpace();
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

  OTC_Regex& pattern = OTC_Regex::optWhiteSpace();
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

  OTC_Regex& pattern = OTC_Regex::nonWhiteSpace();
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

  OTC_Regex& pattern = OTC_Regex::alphabetic();
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

  OTC_Regex& pattern = OTC_Regex::lowerCase();
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

  OTC_Regex& pattern = OTC_Regex::upperCase();
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

  OTC_Regex& pattern = OTC_Regex::alphaNumeric();
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

  OTC_Regex& pattern = OTC_Regex::identifier();
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

  OTC_Regex& pattern = OTC_Regex::matchingQuotes();
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
//     Test of range().

void test12()
{
  OTC_Tracer tracer("void test12()");

  char const* s1 = "ab\\(cd\\)ef";
  tracer() << "pattern1 = " << s1 << endl;
  OTC_Regex pattern1(s1);
  tracer() << "pattern1.match(\"abcdef\") = ";
  tracer() << pattern1.match("abcdef") << endl;

  tracer() << "pattern1.start() = " << pattern1.start() << endl;
  tracer() << "pattern1.length() = " << pattern1.length() << endl;

  tracer() << "pattern1.range().lower() = ";
  tracer() << pattern1.range().lower() << endl;
  tracer() << "pattern1.range().length() = ";
  tracer() << pattern1.range().length() << endl;

  tracer() << "pattern1.start(1) = " << pattern1.start(1) << endl;
  tracer() << "pattern1.length(1) = " << pattern1.length(1) << endl;

  tracer() << "pattern1.range(1).lower() = ";
  tracer() << pattern1.range(1).lower() << endl;
  tracer() << "pattern1.range(1).length() = ";
  tracer() << pattern1.range(1).length() << endl;
}

// = TEST 13
//     Test of large number of sub patterns.

void test13()
{
  OTC_Tracer tracer("void test13()");

  char const* s1 = "\\(1*\\)\\(2*\\)\\(3*\\)\\(4*\\)\\(5*\\)"
   "\\(6*\\)\\(7*\\)\\(8*\\)\\(9*\\)\\(0*\\)";

  OTC_Regex pattern1(s1);

  if (!pattern1.isValid())
    tracer() << "error1 = " << pattern1.error() << endl;

  OTC_Regex pattern2(s1,10);

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
