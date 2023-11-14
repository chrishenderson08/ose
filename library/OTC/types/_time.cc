/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     types/_time.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/types/time.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

#if defined(HAVE_NL_LANGINFO)
#include <locale.h>
#endif

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Date tmp1;
  OTC_Time date1(tmp1);

  tracer() << "date1 = " << date1.nameOfDayOfWeek() << ", ";
  tracer() << date1.nameOfMonth() << " ";
  tracer() << date1.day() << " ";
  tracer() << date1.year() << " (";
  tracer() << date1.day() << "/";
  tracer() << date1.month() << "/";
  tracer() << date1.year() << ")";
  tracer() << endl;

  OTC_Time time1;

  char fchar = tracer().fill('0');

  tracer() << "time1 (ISO) = " << time1.asString() << endl;
  tracer() << "time1 (ISO) = " << time1 << endl;
  tracer() << "time1 = " << time1.nameOfDayOfWeek() << ", ";
  tracer() << time1.nameOfMonth() << " ";
  tracer() << time1.day() << " ";
  tracer() << time1.year() << " (";
  tracer() << time1.day() << "/";
  tracer() << time1.month() << "/";
  tracer() << time1.year() << ") ";
  tracer().width(2);
  tracer() << time1.hour() << ":";
  tracer().width(2);
  tracer() << time1.minute() << ":";
  tracer().width(2);
  tracer() << time1.second();
  tracer() << endl;

  tracer().fill(fchar);

  OTC_Duration dur1;

  tracer() << "dur1 = time1 - date1" << endl;

  dur1 = time1 - date1;

  tracer() << "dur1.sign() - " << dur1.sign() << endl;
  tracer() << "dur1.days() - " << dur1.days() << endl;
  tracer() << "dur1.hours() - " << dur1.hours() << endl;
  tracer() << "dur1.minutes() - " << dur1.minutes() << endl;
  tracer() << "dur1.seconds() - " << dur1.seconds() << endl;

  tracer() << "date1.addDays(1)" << endl;

  date1.addDays(1);

  tracer() << "date1 = " << date1.nameOfDayOfWeek() << ", ";
  tracer() << date1.nameOfMonth() << " ";
  tracer() << date1.day() << " ";
  tracer() << date1.year() << " (";
  tracer() << date1.day() << "/";
  tracer() << date1.month() << "/";
  tracer() << date1.year() << ")";
  tracer() << endl;

  tracer() << "dur1 = time1 - date1" << endl;

  dur1 = time1 - date1;

  tracer() << "dur1.sign() - " << dur1.sign() << endl;
  tracer() << "dur1.days() - " << dur1.days() << endl;
  tracer() << "dur1.hours() - " << dur1.hours() << endl;
  tracer() << "dur1.minutes() - " << dur1.minutes() << endl;
  tracer() << "dur1.seconds() - " << dur1.seconds() << endl;

  tracer() << "date1.addDays(1)" << endl;

  date1.addDays(1);

  tracer() << "date1 = " << date1.nameOfDayOfWeek() << ", ";
  tracer() << date1.nameOfMonth() << " ";
  tracer() << date1.day() << " ";
  tracer() << date1.year() << " (";
  tracer() << date1.day() << "/";
  tracer() << date1.month() << "/";
  tracer() << date1.year() << ")";
  tracer() << endl;

  tracer() << "dur1 = time1 - date1" << endl;

  dur1 = time1 - date1;

  tracer() << "dur1.sign() - " << dur1.sign() << endl;
  tracer() << "dur1.days() - " << dur1.days() << endl;
  tracer() << "dur1.hours() - " << dur1.hours() << endl;
  tracer() << "dur1.minutes() - " << dur1.minutes() << endl;
  tracer() << "dur1.seconds() - " << dur1.seconds() << endl;

  tracer() << "date1.addDays(-4)" << endl;

  date1.addDays(-4);

  tracer() << "date1 = " << date1.nameOfDayOfWeek() << ", ";
  tracer() << date1.nameOfMonth() << " ";
  tracer() << date1.day() << " ";
  tracer() << date1.year() << " (";
  tracer() << date1.day() << "/";
  tracer() << date1.month() << "/";
  tracer() << date1.year() << ")";
  tracer() << endl;

  tracer() << "dur1 = time1 - date1" << endl;

  dur1 = time1 - date1;

  tracer() << "dur1.sign() - " << dur1.sign() << endl;
  tracer() << "dur1.days() - " << dur1.days() << endl;
  tracer() << "dur1.hours() - " << dur1.hours() << endl;
  tracer() << "dur1.minutes() - " << dur1.minutes() << endl;
  tracer() << "dur1.seconds() - " << dur1.seconds() << endl;
}

void test2()
{
  OTC_Tracer tracer("void test2()");
 
  OTC_Date theDate(27,2,1995);
  cout << "date = " << theDate << endl;
 
  OTC_Time theTime1(theDate,23,59,50);
  cout << "time1 = " << theTime1 << endl;
 
  OTC_Time theTime2(theTime1);
  theTime2.addSeconds(10);
  cout << "time2 (time1 + 10sec) = " << theTime2 << endl;
 
  cout << "time2 - time1 = " << theTime2 - theTime1 << endl;
 
  cout << "date(time1) = " << OTC_Date(theTime1) << endl;
  cout << "date(time2) = " << OTC_Date(theTime2) << endl;
 
  cout << "date(time1) - date = " << OTC_Date(theTime1) - theDate << endl;
  cout << "date(time2) - date = " << OTC_Date(theTime2) - theDate << endl;
 
  cout << "time1 - date = " << theTime1 - theDate << endl;
  cout << "time2 - date = " << theTime2 - theDate << endl;
}

void test3()
{
  OTC_Tracer tracer("test3");

  while (cin.good())
  {
    OTC_String theTimeString = OTC_String::getline(cin);
    if (!cin.fail())
    {
      if (OTC_Time::isValidTime(theTimeString))
      {
        tracer() << "okay - ";
        OTC_Time theTime(theTimeString);
        tracer() << theTime << endl;
      }
      else
      {
        tracer() << "bad format" << endl;
      }
    }
  }
}

void test4()
{
  OTC_Tracer tracer("test4");

#if defined(HAVE_NL_LANGINFO)
  setlocale(LC_ALL,"");
#endif

  while (cin.good())
  {
    OTC_String theTimeString;
    OTC_String theFormat;

    theTimeString.readToken(cin);

    cin >> ws;
    theFormat.readLine(cin);

    if (!cin.fail())
    {
      if (OTC_Time::isValidTime(theTimeString))
      {
        tracer() << "okay - ";
        OTC_Time theTime(theTimeString);
        tracer() << theTime.asString(theFormat) << endl;
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
  OTC_Tracer tracer("test5()");

  OTC_Time theTime;

  tracer() << "time = " << theTime << endl;
  tracer() << "time - 86399 secs = " << theTime.minusSeconds(86399) << endl; 
  tracer() << "time - 86401 secs = " << theTime.minusSeconds(86401) << endl; 
  tracer() << "time - 172799 secs = " << theTime.minusSeconds(172799) << endl; 
  tracer() << "time - 172801 secs = " << theTime.minusSeconds(172801) << endl; 
  tracer() << "time - 259199 secs = " << theTime.minusSeconds(259199) << endl; 
  tracer() << "time - 259201 secs = " << theTime.minusSeconds(259201) << endl; 
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5
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
