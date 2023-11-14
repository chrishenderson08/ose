/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     types/otcdate.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/types/date.hh"
#endif
#endif

#include <OTC/types/date.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/iostream.h>
#include <OTC/stdlib/strstream.h>

#include <time.h>
#include <string.h>

#if defined(HAVE_NL_LANGINFO)
//#include <nl_types.h>
#include <langinfo.h>
#endif

#include <math.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Date::mutex_;

/* ------------------------------------------------------------------------- */
OTC_Date::OTC_Date(OTC_TimeZone theTimeZone)
{
  setDate(theTimeZone);
}

/* ------------------------------------------------------------------------- */
OTC_Date::OTC_Date(int theDay, int theMonth, int theYear)
{
  date_ = julianDate(theDay,theMonth,theYear);
}

/* ------------------------------------------------------------------------- */
OTC_Date::OTC_Date(char const* theDateString)
{
  int theDay = 0;
  int theMonth = 0;
  int theYear = 0;

  convertToDate(theDateString,theDay,theMonth,theYear);

  OTCLIB_ENSURE_FN((theDay && theMonth && theYear),
   "OTC_Date::OTC_Date(char const*)",
   "invalid format for date");

  date_ = julianDate(theDay,theMonth,theYear);
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::isValidDate(int theDay, int theMonth, int theYear)
{
  if (theYear == 0)
    return false;

  int tmpDay;
  int tmpMonth;
  int tmpYear;

  long tmpDate = julianDate(theDay,theMonth,theYear);
  calendarDate(tmpDate,tmpDay,tmpMonth,tmpYear);

  return (theDay == tmpDay) && (theMonth == tmpMonth) && (theYear == tmpYear);
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::isValidDate(char const* theDateString)
{
  if (theDateString == 0)
    return false;

  int theDay = 0;
  int theMonth = 0;
  int theYear = 0;

  convertToDate(theDateString,theDay,theMonth,theYear);

  if (!theDay && !theMonth && !theYear)
    return false;

  return isValidDate(theDay,theMonth,theYear);
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::convertToDate(
 char const* theDateString,
 int& theDay,
 int& theMonth,
 int& theYear
)
{
  if (theDateString == 0)
  {
    theYear = theMonth = theDay = 0;
    return false;
  }

  theYear = 0;
  theMonth = 0;
  theDay = 0;

  // Try format YYYY-MM-DD.

  istrstream ins1((char*)theDateString,strlen(theDateString));
  ins1.unsetf(ios::skipws);

  char theSeparator1 = 0;
  char theSeparator2 = 0;

  ins1 >> dec >> theYear;
  ins1 >> theSeparator1;
  ins1 >> dec >> theMonth;
  ins1 >> theSeparator2;
  ins1 >> dec >> theDay;

  if (!ins1.fail() && theSeparator1 == '-' && theSeparator2 == '-')
    return true;

  theYear = theMonth = theDay = 0;

  return false;
}

/* ------------------------------------------------------------------------- */
int OTC_Date::day() const
{
  int theDay, theMonth, theYear;
  OTC_Date::calendarDate(date_,theDay,theMonth,theYear);
  return theDay;
}

/* ------------------------------------------------------------------------- */
int OTC_Date::dayOfYear() const
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  calendarDate(date_,tmpDay,tmpMonth,tmpYear);
  OTC_Date tmpDate(1,1,tmpYear);
  return tmpDate.daysTill(*this) + 1;
}

/* ------------------------------------------------------------------------- */
int OTC_Date::month() const
{
  int theDay, theMonth, theYear;
  OTC_Date::calendarDate(date_,theDay,theMonth,theYear);
  return theMonth;
}

/* ------------------------------------------------------------------------- */
int OTC_Date::year() const
{
  int theDay, theMonth, theYear;
  OTC_Date::calendarDate(date_,theDay,theMonth,theYear);
  return theYear;
}

/* ------------------------------------------------------------------------- */
int OTC_Date::daysInYear() const
{
  if (isLeapYear())
    return 366;

  else
  {
    int tmpDay;
    int tmpMonth;
    int tmpYear;

    calendarDate(date_,tmpDay,tmpMonth,tmpYear);

    if (tmpYear == 1582)
      return 355;

    else
      return 365;
  }
}

/* ------------------------------------------------------------------------- */
int OTC_Date::weekOfYear() const
{
  int theSimpleWeekOfYear = 0;

  theSimpleWeekOfYear = ((dayOfYear() + 6 -
   (dayOfWeek() ? (dayOfWeek() - 1) : 6)) / 7) + 1;

  int theDiff = 1;
  int theDiffDayOfWeek;

  theDiffDayOfWeek = (dayOfYear() - 2) % 7 - dayOfWeek();
  if (theDiffDayOfWeek < 0)
    theDiffDayOfWeek += 7;

  // Note these day of week numbers have no
  // correspondance to normal day of week
  // numbers. Do not really fully understand
  // how this works.

  if (theDiffDayOfWeek >= 2 && theDiffDayOfWeek <= 4)
    theDiff = 0;

  int theWeekOfYear = theSimpleWeekOfYear - theDiff;

  if (theWeekOfYear == 0)
    theWeekOfYear = 53;

  return theWeekOfYear;
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Date::nameOfDayOfWeek() const
{
  // Use local statics, as we cannot rely on initialisation of
  // global statics in a shared library on certain platforms.
  // Also, do not assign OTC_CString directly in static declaration
  // as it triggers a bug in cfront compilers.

  static int gInit = 0;
  static OTC_String gSUN;
  static OTC_String gMON;
  static OTC_String gTUE;
  static OTC_String gWED;
  static OTC_String gTHU;
  static OTC_String gFRI;
  static OTC_String gSAT;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gInit == 0)
  {
    gSUN = OTC_CString("Sunday");
    gMON = OTC_CString("Monday");
    gTUE = OTC_CString("Tuesday");
    gWED = OTC_CString("Wednesday");
    gTHU = OTC_CString("Thursday");
    gFRI = OTC_CString("Friday");
    gSAT = OTC_CString("Saturday");
    gInit = 1;
  }

  xxxMutex.release();

  mutex_.unlock();

  switch (dayOfWeek())
  {
    case OTCLIB_SUNDAY:
      return gSUN;

    case OTCLIB_MONDAY:
      return gMON;

    case OTCLIB_TUESDAY:
      return gTUE;

    case OTCLIB_WEDNESDAY:
      return gWED;

    case OTCLIB_THURSDAY:
      return gTHU;

    case OTCLIB_FRIDAY:
      return gFRI;

    case OTCLIB_SATURDAY:
      return gSAT;
  }

  return OTC_String::undefinedString();
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Date::nameOfMonth() const
{
  // Use local statics, as we cannot rely on initialisation of 
  // global statics in a shared library on certain platforms.
  // Also, do not assign OTC_CString directly in static declaration
  // as it triggers a bug in cfront compilers.

  static int gInit = 0;
  static OTC_String gJAN;
  static OTC_String gFEB;
  static OTC_String gMAR;
  static OTC_String gAPR;
  static OTC_String gMAY;
  static OTC_String gJUN;
  static OTC_String gJUL;
  static OTC_String gAUG;
  static OTC_String gSEP;
  static OTC_String gOCT;
  static OTC_String gNOV;
  static OTC_String gDEC;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gInit == 0)
  {
    gJAN = OTC_CString("January");
    gFEB = OTC_CString("February");
    gMAR = OTC_CString("March");
    gAPR = OTC_CString("April");
    gMAY = OTC_CString("May");
    gJUN = OTC_CString("June");
    gJUL = OTC_CString("July");
    gAUG = OTC_CString("August");
    gSEP = OTC_CString("September");
    gOCT = OTC_CString("October");
    gNOV = OTC_CString("November");
    gDEC = OTC_CString("December");
    gInit = 1;
  }

  xxxMutex.release();

  mutex_.unlock();

  switch(monthOfYear())
  {
    case OTCLIB_JANUARY:
      return gJAN;

    case OTCLIB_FEBRUARY:
      return gFEB;

    case OTCLIB_MARCH:
      return gMAR;

    case OTCLIB_APRIL:
      return gAPR;

    case OTCLIB_MAY:
      return gMAY;

    case OTCLIB_JUNE:
      return gJUN;

    case OTCLIB_JULY:
      return gJUL;

    case OTCLIB_AUGUST:
      return gAUG;

    case OTCLIB_SEPTEMBER:
      return gSEP;

    case OTCLIB_OCTOBER:
      return gOCT;

    case OTCLIB_NOVEMBER:
      return gNOV;

    case OTCLIB_DECEMBER:
      return gDEC;
  }

  return OTC_String::undefinedString();
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::isBetween(
 OTC_Date const& theDate1,
 OTC_Date const& theDate2
) const
{
  if (theDate1.date_ < theDate2.date_)
    return (date_ >= theDate1.date_) && (date_ <= theDate2.date_);

  return (date_ >= theDate2.date_) && (date_ <= theDate1.date_);
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::isWeekDay() const
{
  switch (dayOfWeek())
  {
    case OTCLIB_SATURDAY:
    case OTCLIB_SUNDAY:
    {
      return false;
    }
  }

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::isFirstDayOfMonth() const
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  calendarDate(date_,tmpDay,tmpMonth,tmpYear);

  return tmpDay == 1;
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::isLastDayOfMonth() const
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  calendarDate(date_+1,tmpDay,tmpMonth,tmpYear);

  return tmpDay == 1;
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::isLeapYear() const
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  calendarDate(date_,tmpDay,tmpMonth,tmpYear);
  tmpDay = 29;
  tmpMonth = 2;
  return isValidDate(tmpDay,tmpMonth,tmpYear);
}

/* ------------------------------------------------------------------------- */
OTC_Date OTC_Date::following(int theDay) const
{
  OTCLIB_ASSERT(theDay >= 0 && theDay < 7);

  int theNum = theDay - dayOfWeek();
  if (theNum > 0)
    return plusDays(theNum);

  else
    return plusDays(7+theNum);
}

/* ------------------------------------------------------------------------- */
OTC_Date OTC_Date::previous(int theDay) const
{
  OTCLIB_ASSERT(theDay >= 0 && theDay < 7);

  int theNum = dayOfWeek() - theDay;
  if (theNum > 0)
    return minusDays(theNum);

  else
    return minusDays(7+theNum);
}

/* ------------------------------------------------------------------------- */
OTC_Date OTC_Date::firstDayOfWeek() const
{
  OTC_Date aDate(date_+1);
  return aDate.previous(OTCLIB_SUNDAY);
}

/* ------------------------------------------------------------------------- */
OTC_Date OTC_Date::lastDayOfWeek() const
{
  OTC_Date aDate(date_-1);
  return aDate.following(OTCLIB_SATURDAY);
}

/* ------------------------------------------------------------------------- */
OTC_Date OTC_Date::firstDayOfMonth() const
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  calendarDate(date_,tmpDay,tmpMonth,tmpYear);
  return OTC_Date(1,tmpMonth,tmpYear);
}

/* ------------------------------------------------------------------------- */
OTC_Date OTC_Date::lastDayOfMonth() const
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  calendarDate(date_,tmpDay,tmpMonth,tmpYear);

  OTC_Date tmpDate(1,tmpMonth+1,tmpYear);
  return tmpDate.minusDays(1);
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Date::asString(char const* theFormat) const
{
  if (theFormat == 0)
  {
    OTC_String tmpString = OTC_Length(31);
    ostrstream theStream(tmpString.buffer(),tmpString.capacity());

    int tmpDay;
    int tmpMonth;
    int tmpYear;

    OTC_Date::calendarDate(date_,tmpDay,tmpMonth,tmpYear);

    char fchar = theStream.fill('0');

    if (tmpYear < 0)
    {
      theStream << '-';
      theStream.width(4);
      theStream << -tmpYear << '-';
    }
    else
    {
      theStream.width(4);
      theStream << tmpYear << '-';
    }

    theStream.width(2);
    theStream << tmpMonth << '-';
    theStream.width(2);
    theStream << tmpDay << ends;

    theStream.fill(fchar);

    int theLength = strlen(tmpString.buffer());
    tmpString.length(theLength);

    return tmpString;
  }
  else
  {
#if defined(HAVE_NL_LANGINFO)
    static int days_a_i[] =
    {
      ABDAY_1, ABDAY_2, ABDAY_3, ABDAY_4, ABDAY_5, ABDAY_6, ABDAY_7
    };
#else
    static char *days_a[] =
    {
      "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
    };
#endif

#if defined(HAVE_NL_LANGINFO)
    static int days_l_i[] =
    {
      DAY_1, DAY_2, DAY_3, DAY_4, DAY_5, DAY_6, DAY_7
    };
#else
    static char *days_l[] =
    {
      "Sunday", "Monday", "Tuesday", "Wednesday",
      "Thursday", "Friday", "Saturday",
    };
#endif

#if defined(HAVE_NL_LANGINFO)
    static int months_a_i[] =
    {
      ABMON_1, ABMON_2, ABMON_3, ABMON_4, ABMON_5, ABMON_6,
      ABMON_7, ABMON_8, ABMON_9, ABMON_10, ABMON_11, ABMON_12
    };
#else
    static char *months_a[] =
    {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
    };
#endif

#if defined(HAVE_NL_LANGINFO)
    static int months_l_i[] =
    {
      MON_1, MON_2, MON_3, MON_4, MON_5, MON_6,
      MON_7, MON_8, MON_9, MON_10, MON_11, MON_12
    };
#else
    static char *months_l[] =
    {
      "January", "February", "March", "April", "May", "June",
      "July", "August", "September", "October", "November", "December",
    };
#endif

    ostrstream theStream;

    while (*theFormat != EOS)
    {
      if (*theFormat != '%')
      {
	theStream << *theFormat;
      }
      else
      {
	theFormat++;
	if (*theFormat == EOS)
	  return OTC_String::nullString();

	switch (*theFormat)
	{
	  case '%':
	  {
	    theStream << '%';
	    break;
	  }

	  case 'a':
	  {
	    // Abbreviated weekday name.

#if defined(HAVE_NL_LANGINFO)
	    theStream << (char const*)nl_langinfo(days_a_i[dayOfWeek()]);
#else
	    theStream << days_a[dayOfWeek()];
#endif
	    break;
	  }

	  case 'A':
	  {
	    // Full weekday name.

#if defined(HAVE_NL_LANGINFO)
	    theStream << (char const*)nl_langinfo(days_l_i[dayOfWeek()]);
#else
	    theStream << days_l[dayOfWeek()];
#endif
	    break;
	  }

	  case 'b':
	  {
	    // Abbreviated month name.

#if defined(HAVE_NL_LANGINFO)
	    theStream << (char const*)nl_langinfo(months_a_i[monthOfYear()-1]);
#else
	    theStream << months_a[monthOfYear()-1];
#endif
	    break;
	  }

	  case 'B':
	  {
	    // Full month name.

#if defined(HAVE_NL_LANGINFO)
	    theStream << (char const*)nl_langinfo(months_l_i[monthOfYear()-1]);
#else
	    theStream << months_l[monthOfYear()-1];
#endif
	    break;
	  }

	  case 'C':
	  {
	    // Century number.

	    char buf[16];
	    sprintf(buf,"%02d",year()/100);

	    theStream << buf;
	    break;
	  }

	  case 'd':
	  {
	    // Day of month [01,31].

	    char buf[16];
	    sprintf(buf,"%02d",dayOfMonth());

	    theStream << buf;
	    break;
	  }

	  case 'D':
	  {
	    // Date as "%m/%d/%y".

	    char buf[16];
	    sprintf(buf,"%02d/%02d/%02d",monthOfYear(),
	     dayOfMonth(),year()%100);

	    theStream << buf;
	    break;
	  }

	  case 'e':
	  {
	    // Day of month [ 1,31], blank padded.

	    char buf[16];
	    sprintf(buf,"%2d",dayOfMonth());

	    theStream << buf;
	    break;
	  }

          case 'h':
          {
            // Abbreviated month name.

#if defined(HAVE_NL_LANGINFO)
	    theStream << (char const*)nl_langinfo(months_a_i[monthOfYear()-1]);
#else
            theStream << months_a[monthOfYear()-1];
#endif
            break;
          }

	  case 'j':
	  {
	    // Day of the year [001,366].

	    char buf[16];
	    sprintf(buf,"%03d",dayOfYear());

	    theStream << buf;
	    break;
	  }

	  case 'm':
	  {
	    // Month of year [01,12].

	    char buf[16];
	    sprintf(buf,"%02d",monthOfYear());

	    theStream << buf;
	    break;
	  }

	  case 'n':
	  {
	    // Insert a new line.

	    theStream << '\n';
	    break;
	  }

	  case 't':
	  {
	    // Insert a tab.

	    theStream << '\t';
	    break;
	  }

	  case 'u':
	  {
	    // Weekday as a decimal number [1,7].
	    // With 1 representing Sunday.

	    theStream << dayOfWeek()+1;
	    break;
	  }

	  case 'U':
	  {
	    // Week number of year as a decimal number
	    // [00,53], with Sunday as the first day of
	    // week 1.

	    OTC_Date theDate =
	     firstDayOfYear().minusDays(1).following(OTCLIB_SUNDAY);

	    if (*this < theDate)
	    {
	      theStream << "00";
	    }
	    else
	    {
	      int theWeekOfYear = 0;

	      theWeekOfYear = (dayOfYear() + 6 -
	       dayOfWeek()) / 7;

	      char buf[16];
	      sprintf(buf,"%02d",theWeekOfYear);
	      theStream << buf;
	    }

	    break;
	  }

	  case 'V':
	  {
	    // Week number as per ISO 8601.

	    char buf[16];
	    sprintf(buf,"%02d",weekOfYear());
	    theStream << buf;

	    break;
	  }

	  case 'w':
	  {
	    // Weekday as a decimal number [0,6].
	    // With 0 representing Sunday.

	    theStream << dayOfWeek();
	    break;
	  }

	  case 'W':
	  {
	    // Week number of year as a decimal number
	    // [00,53], with Monday as the first day of
	    // week 1.

	    int theWeekOfYear = 0;

	    theWeekOfYear = (dayOfYear() + 6 -
	     (dayOfWeek() ? (dayOfWeek() - 1) : 6)) / 7;

	    char buf[16];
	    sprintf(buf,"%02d",theWeekOfYear);
	    theStream << buf;

	    break;
	  }

	  case 'x':
	  {
	    // Date as "%m/%d/%y".

#if defined(HAVE_NL_LANGINFO)
	    char const* format;
	    format = (char const*)nl_langinfo(D_FMT);
	    theStream << asString(format);
#else
	    char buf[16];
	    sprintf(buf,"%02d/%02d/%02d",monthOfYear(),
	     dayOfMonth(),year()%100);
	    theStream << buf;
#endif
	    break;
	  }

	  case 'y':
	  {
	    // Year within century [00,99].

	    char buf[16];
	    sprintf(buf,"%02d",year()%100);
	    theStream << buf;

	    break;
	  }

	  case 'Y':
	  {
	    // Year, including the century.

	    theStream << year();

	    break;
	  }

	  default:
	  {
	    theStream << '%';
	    theStream << *theFormat;
	    break;
	  }
	}
      }

      theFormat++;
    }

    theStream << ends;

    char* theString;
    theString = theStream.str();

    OTC_String tmpString = theString;

    delete [] theString;

    return tmpString;
  }
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& operator<<(ostream& theStream, OTC_Date const& theDate)
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  OTC_Date::calendarDate(theDate.jday(),tmpDay,tmpMonth,tmpYear);

  char fchar = theStream.fill('0');

    if (tmpYear < 0)
    {
      theStream << '-';
      theStream.width(4);
      theStream << -tmpYear << '-';
    }
    else
    {
      theStream.width(4);
      theStream << tmpYear << '-';
    }

  theStream.width(2);
  theStream << tmpMonth << '-';
  theStream.width(2);
  theStream << tmpDay;

  theStream.fill(fchar);

  return theStream;
}

/* ------------------------------------------------------------------------- */
OTC_Date OTC_Date::firstDayOfYear() const
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  calendarDate(date_,tmpDay,tmpMonth,tmpYear);
  return OTC_Date(1,1,tmpYear);
}

/* ------------------------------------------------------------------------- */
OTC_Date OTC_Date::lastDayOfYear() const
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  calendarDate(date_,tmpDay,tmpMonth,tmpYear);

  OTC_Date tmpDate(1,1,tmpYear+1);
  return tmpDate.minusDays(1);
}

/* ------------------------------------------------------------------------- */
void OTC_Date::setDate(OTC_TimeZone theTimeZone)
{
  time_t theClock = time(0);
  if (theTimeZone == OTCLIB_LOCALTIME)
  {
#if defined(_REENTRANT)
    tm theTimeStruct;
    localtime_r(&theClock,&theTimeStruct);
    tm* theDate = &theTimeStruct;
#else
    tm* theDate = localtime(&theClock);
#endif
    date_ = julianDate(theDate->tm_mday,theDate->tm_mon+1,
     theDate->tm_year+1900);
  }
  else
  {
#if defined(_REENTRANT)
    tm theTimeStruct;
    gmtime_r(&theClock,&theTimeStruct);
    tm* theDate = &theTimeStruct;
#else
    tm* theDate = gmtime(&theClock);
#endif
    date_ = julianDate(theDate->tm_mday,theDate->tm_mon+1,
     theDate->tm_year+1900);
  }
}

/* ------------------------------------------------------------------------- */
bool OTC_Date::setDate(int theDay, int theMonth, int theYear)
{
  date_ = julianDate(theDay,theMonth,theYear);
  return isValidDate(theDay,theMonth,theYear);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_Duration OTC_Date::operator-(OTC_Date const& theDate) const
{
  OTC_Duration theDuration(int(date_ - theDate.date_));
  return theDuration;
}

/* ------------------------------------------------------------------------- */
/* jdn.c -- Julian Day Number computation
**
** public domain Julian Day Number functions
**
** Based on formulae originally posted by
**    Tom Van Flandern / Washington, DC / metares@well.sf.ca.us
**       in the UseNet newsgroup sci.astro.
**    Reposted 14 May 1991 in FidoNet C Echo conference by
**       Paul Schlyter (Stockholm)
** Minor corrections, added JDN to julian, and recast into C by
**    Raymond Gardner  Englewood, Colorado
**
** Synopsis:
**      long ymd_to_jdn(int year, int month, int day, int julian_flag)
**      void jdn_to_ymd(long jdn, int *year, int *month, int *day,
**                                                      int julian_flag)
**      year is negative if BC
**      if julian_flag is >  0, use Julian calendar
**      if julian_flag is == 0, use Gregorian calendar
**      if julian_flag is <  0, routines decide based on date
**
** These routines convert Gregorian and Julian calendar dates to and 
** from Julian Day Numbers.  Julian Day Numbers (JDN) are used by 
** astronomers as a date/time measure independent of calendars and 
** convenient for computing the elapsed time between dates.  The JDN 
** for any date/time is the number of days (including fractional 
** days) elapsed since noon, 1 Jan 4713 BC.  Julian Day Numbers were 
** originated by Joseph Scaliger in 1582 and named after his father 
** Julius, not after Julius Caesar.  They are not related to the 
** Julian calendar. 
** 
** For dates from 1 Jan 4713 BC thru 12 Dec Feb 32766 AD, ymd_to_jdn() 
** will give the JDN for noon on that date.  jdn_to_ymd() will compute 
** the year, month, and day from the JDN.  Years BC are given (and 
** returned) as negative numbers.  Note that there is no year 0 BC; 
** the day before 1 Jan 1 AD is 31 Dec 1 BC.  Note also that 1 BC, 
** 5 BC, etc. are leap years.
** 
** Pope Gregory XIII decreed that the Julian calendar would end on 
** 4 Oct 1582 AD and that the next day would be 15 Oct 1582 in the 
** Gregorian Calendar.  The only other change is that centesimal 
** years (years ending in 00) would no longer be leap years 
** unless divisible by 400.  Britain and its possessions and 
** colonies continued to use the Julian calendar up until 2 Sep 
** 1752, when the next day became 14 Sep 1752 in the Gregorian 
** Calendar.  These routines can be compiled to use either 
** convention.  By default, the British convention will be used.  
** Simply #define PAPAL to use Pope Gregory's convention. 
** 
** Each routine takes, as its last argument, a flag to indicate 
** whether to use the Julian or Gregorian calendar convention.  If 
** this flag is negative, the routines decide based on the date 
** itself, using the changeover date described in the preceding 
** paragraph.  If the flag is zero, Gregorian conventions will be used, 
** and if the flag is positive, Julian conventions will be used. 
*/

#define PAPAL 1

#ifdef PAPAL                    /* Pope Gregory XIII's decree */
#define LASTJULDATE 15821004L   /* last day to use Julian calendar */
#define LASTJULJDN  2299160L    /* jdn of same */
#else                           /* British-American usage */
#define LASTJULDATE 17520902L   /* last day to use Julian calendar */
#define LASTJULJDN  2361221L    /* jdn of same */
#endif


/* ------------------------------------------------------------------------- */
long OTC_Date::julianDate(int d, int m, int y)
// long ymd_to_jdn(int y, int m, int d, int julian)
{
	OTCLIB_ENSURE_FN((y != 0),
	 "OTC_Date::julianDate()",
	 "invalid year zero");

	int julian = -1;

        long jdn;

        if (julian < 0)         /* set Julian flag if auto set */
                julian = (((y * 100L) + m) * 100 + d  <=  LASTJULDATE);

        if (y < 0)              /* adjust BC year */
                y++;

        if (julian)
                jdn = 367L * y - 7 * (y + 5001L + (m - 9) / 7) / 4
                + 275 * m / 9 + d + 1729777L;
        else
                jdn = (long)(d - 32076)
                + 1461L * (y + 4800L + (m - 14) / 12) / 4
                + 367 * (m - 2 - (m - 14) / 12 * 12) / 12
                - 3 * ((y + 4900L + (m - 14) / 12) / 100) / 4
                + 1;            /* correction by rdg */

        return jdn;
}

/* ------------------------------------------------------------------------- */
void OTC_Date::calendarDate(long jdn, int& dd, int& mm, int& yy)
// void jdn_to_ymd(long jdn, int *yy, int *mm, int *dd, int julian)
{
	int julian = -1;

        long x, z, m, d, y;
        long daysPer400Years = 146097L;
        long fudgedDaysPer4000Years = 1460970L + 31;

        if (julian < 0)                 /* set Julian flag if auto set */
                julian = (jdn <= LASTJULJDN);

        x = jdn + 68569L;
        if ( julian )
        {
                x += 38;
                daysPer400Years = 146100L;
                fudgedDaysPer4000Years = 1461000L + 1;
        }
        z = 4 * x / daysPer400Years;
        x = x - (daysPer400Years * z + 3) / 4;
        y = 4000 * (x + 1) / fudgedDaysPer4000Years;
        x = x - 1461 * y / 4 + 31;
        m = 80 * x / 2447;
        d = x - 2447 * m / 80;
        x = m / 11;
        m = m + 2 - 12 * x;
        y = 100 * (z - 49) + y + x;

        yy = (int)y;
        mm = (int)m;
        dd = (int)d;

        if (yy <= 0)                   /* adjust BC years */
                (yy)--;
}

/* ------------------------------------------------------------------------- */
