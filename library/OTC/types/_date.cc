/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     types/_date.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/types/date.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

#if defined(HAVE_NL_LANGINFO)
#include <locale.h>
#endif

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Date date;
  tracer() << date.day() << "/" << date.month() << "/" << date.year() << endl;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Date date(1,1,1);
  tracer() << date.day() << "/" << date.month() << "/" << date.year() << endl;
  date.subtractDays(1);
  tracer() << date.day() << "/" << date.month() << "/" << date.year() << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Date d2;

  while (cin.good())
  {
    int day, month, year;
    cin >> day >> month >> year;
    if (!cin.fail())
    {
      tracer() << "d2 = " << d2.nameOfDayOfWeek() << ", ";
      tracer() << d2.nameOfMonth() << " (";
      tracer() << d2.day() << "/";
      tracer() << d2.month() << "/";
      tracer() << d2.year() << ")";
      tracer() << endl;

      OTC_Date d1(day,month,year);

      tracer() << "d1 (ISO) = " << d1.asString() << endl;
      tracer() << "d1 (ISO) = " << d1 << endl;
      tracer() << "d1 = " << d1.nameOfDayOfWeek() << ", ";
      tracer() << d1.nameOfMonth() << " (";
      tracer() << d1.day() << "/";
      tracer() << d1.month() << "/";
      tracer() << d1.year() << ")";
      tracer() << endl;

      OTC_Duration d3 = d1 - d2;

      tracer() << "d3.sign() - " << d3.sign() << endl;
      tracer() << "d3.days() - " << d3.days() << endl;
      tracer() << "d3.hours() - " << d3.hours() << endl;
      tracer() << "d3.minutes() - " << d3.minutes() << endl;
      tracer() << "d3.seconds() - " << d3.seconds() << endl;

      d2 = d1;
    }
  }
}

void test4()
{
  OTC_Tracer tracer("test4");

  while (cin.good())
  {
    OTC_String theDateString = OTC_String::getline(cin);
    if (!cin.fail())
    {
      if (OTC_Date::isValidDate(theDateString))
      {
	tracer() << "okay - ";
	OTC_Date theDate(theDateString);
	tracer() << theDate << endl; 
      }
      else
      {
	tracer() << "bad format" << endl;
      }
    }
  }
}

void test5()
{
  OTC_Tracer tracer("test5");

  while (cin.good())
  {
    int year;
    cin >> year;

    if (!cin.fail())
    {
      OTC_Date date(1,1,year);
      date.subtractDays(10);

      tracer().fill('0');

      for (int i=0; i<20; i++)
      {
	tracer() << date.asString() << " ";
	tracer().width(2);
	tracer() << date.weekOfYear() << " ";
	tracer() << date.nameOfDayOfWeek() << endl;

	date.addDays(1);
      }
    }
  }
}

void test6()
{
  OTC_Tracer tracer("test6");

#if defined(HAVE_NL_LANGINFO)
  setlocale(LC_ALL,"");
#endif

  while (cin.good())
  {
    int year;
    cin >> year;
    cin >> ws;

    OTC_String format;
    format.readLine(cin);

    if (!cin.fail())
    {
      OTC_Date date(1,1,year);
      date.subtractDays(10);

      for (int i=0; i<20; i++)
      {
	tracer() << date.asString(format) << endl;

	date.addDays(1);
      }
    }
  }
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  while (cin.good())
  {
    int jday;
    cin >> jday;
    if (!cin.fail())
    {
      OTC_Date d1(jday);
      tracer() << " (";
      tracer() << d1.day() << "/";
      tracer() << d1.month() << "/";
      tracer() << d1.year() << ")";
      tracer() << endl;
    }
  }
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  while (cin.good())
  {
    int day, month, year;
    cin >> day >> month >> year;
    if (!cin.fail())
    {
      OTC_Date date(day,month,year);

      tracer() << date << endl;
      tracer() << "isWeekDay = " << date.isWeekDay() << endl;
      tracer() << "isFirstDayOfMonth = " << date.isFirstDayOfMonth() << endl;
      tracer() << "isLastDayOfMonth = " << date.isLastDayOfMonth() << endl;
      tracer() << "isFirstDayOfYear = " << date.isFirstDayOfYear() << endl;
      tracer() << "isLastDayOfYear = " << date.isLastDayOfYear() << endl;
      tracer() << "isLeapYear = " << date.isLeapYear() << endl;
    }
  }
}

void test9()
{
  OTC_Tracer tracer("void test9()");

  while (cin.good())
  {
    int day, month, year, dayofweek;
    cin >> day >> month >> year >> dayofweek;
    if (!cin.fail())
    {
      OTC_Date date(day,month,year);

      dayofweek %= 7;

      tracer() << date << endl;
      tracer() << "dayofweek = " << dayofweek << endl;
      tracer() << "previous = " << date.previous(dayofweek) << endl;
      tracer() << "following = " << date.following(dayofweek) << endl;
    }
  }
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
  test9
};

/* ------------------------------------------------------------------------- */

void testTerminate()
{
  exit(-1);
}

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
