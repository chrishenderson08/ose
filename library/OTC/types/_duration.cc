/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     types/_duration.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/types/duration.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Duration d2;
  OTC_Duration d3;

  while (!cin.eof())
  {
    int theDays, theHours, theMinutes, theSeconds;
    cin >> theDays >> theHours >> theMinutes >> theSeconds;
    if (!cin.fail())
    {
      OTC_Duration d1(theDays,theHours,theMinutes,theSeconds);

      tracer() << "d2.sign() - " << d2.sign() << endl;
      tracer() << "d2.days() - " << d2.days() << endl;
      tracer() << "d2.hours() - " << d2.hours() << endl;
      tracer() << "d2.minutes() - " << d2.minutes() << endl;
      tracer() << "d2.seconds() - " << d2.seconds() << endl;

      tracer() << "d1 (ISO) = " << d1.asString() << endl;
      tracer() << "d1 (ISO) = " << d1 << endl;
      tracer() << "d1.sign() - " << d1.sign() << endl;
      tracer() << "d1.days() - " << d1.days() << endl;
      tracer() << "d1.hours() - " << d1.hours() << endl;
      tracer() << "d1.minutes() - " << d1.minutes() << endl;
      tracer() << "d1.seconds() - " << d1.seconds() << endl;

      theHours += 24 * theDays;
      theMinutes += 60 * theHours;
      theSeconds += 60 * theMinutes;

      tracer() << "seconds (raw) = " << theSeconds << endl;

      theDays = d1.days();
      theHours = d1.hours();
      theMinutes = d1.minutes();
      theSeconds = d1.seconds();

      theHours += 24 * theDays;
      theMinutes += 60 * theHours;
      theSeconds += 60 * theMinutes;

      theSeconds = d1.sign() * theSeconds;

      tracer() << "seconds (normalised) = " << theSeconds << endl;

      tracer() << "zero length = " << d1.isZeroLength() << endl;
      tracer() << "is positive = " << d1.isPositiveDuration() << endl;
      tracer() << "is negative = " << d1.isNegativeDuration() << endl;

      tracer() << "(d1 == d2) = " << (d1 == d2) << endl;
      tracer() << "(d1 != d2) = " << (d1 != d2) << endl;
      tracer() << "(d1 < d2) = " << (d1 < d2) << endl;
      tracer() << "(d1 <= d2) = " << (d1 <= d2) << endl;
      tracer() << "(d1 > d2) = " << (d1 > d2) << endl;
      tracer() << "(d1 >= d2) = " << (d1 >= d2) << endl;

      tracer() << "d3 = -d1" << endl;

      d3 = -d1;

      tracer() << "d3.sign() - " << d3.sign() << endl;
      tracer() << "d3.days() - " << d3.days() << endl;
      tracer() << "d3.hours() - " << d3.hours() << endl;
      tracer() << "d3.minutes() - " << d3.minutes() << endl;
      tracer() << "d3.seconds() - " << d3.seconds() << endl;

      tracer() << "d3 = d1 + d2" << endl;

      d3 = d1 + d2;

      tracer() << "d3.sign() - " << d3.sign() << endl;
      tracer() << "d3.days() - " << d3.days() << endl;
      tracer() << "d3.hours() - " << d3.hours() << endl;
      tracer() << "d3.minutes() - " << d3.minutes() << endl;
      tracer() << "d3.seconds() - " << d3.seconds() << endl;

      tracer() << "d3 = d1 - d2" << endl;

      d3 = d1 - d2;

      tracer() << "d3.sign() - " << d3.sign() << endl;
      tracer() << "d3.days() - " << d3.days() << endl;
      tracer() << "d3.hours() - " << d3.hours() << endl;
      tracer() << "d3.minutes() - " << d3.minutes() << endl;
      tracer() << "d3.seconds() - " << d3.seconds() << endl;

      tracer() << "d3 = d2.days() * d1" << endl;

      d3 = d2.days() * d1;

      tracer() << "d3.sign() - " << d3.sign() << endl;
      tracer() << "d3.days() - " << d3.days() << endl;
      tracer() << "d3.hours() - " << d3.hours() << endl;
      tracer() << "d3.minutes() - " << d3.minutes() << endl;
      tracer() << "d3.seconds() - " << d3.seconds() << endl;

      if (d2.days() != 0)
      {
	tracer() << "d3 = d1 / d2.days()" << endl;

	d3 = d1 / d2.days();

	tracer() << "d3.sign() - " << d3.sign() << endl;
	tracer() << "d3.days() - " << d3.days() << endl;
	tracer() << "d3.hours() - " << d3.hours() << endl;
	tracer() << "d3.minutes() - " << d3.minutes() << endl;
	tracer() << "d3.seconds() - " << d3.seconds() << endl;

	tracer() << "d3 seconds (div d2.days()) = ";
	tracer() << theSeconds / d2.days() << endl;

	theDays = d3.days();
	theHours = d3.hours();
	theMinutes = d3.minutes();
	theSeconds = d3.seconds();

	theHours += 24 * theDays;
	theMinutes += 60 * theHours;
	theSeconds += 60 * theMinutes;

	theSeconds = d3.sign() * theSeconds;

	tracer() << "d3 seconds (normalised) = " << theSeconds << endl;
      }

      if (!d2.isZeroLength())
	tracer() << "d1 / d2 = " << d1 / d2 << endl;

      tracer() << "d1.rank(d2) = " << d1.rank(d2) << endl;

      d2 = d1;
    }
  }
}

void test2()
{
  OTC_Tracer tracer("test2");

  while (cin.good())
  {
    OTC_String theDurationString = OTC_String::getline(cin);
    if (!cin.fail())
    {
      if (OTC_Duration::isValidDuration(theDurationString))
      {
        tracer() << "okay - ";
        OTC_Duration theDuration(theDurationString);
        tracer() << theDuration.sign() << " * " << theDuration << endl;
      }
      else
      {
        tracer() << "bad format" << endl;
      }
    }
  }
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2
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
