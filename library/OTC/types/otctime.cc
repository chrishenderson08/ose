/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     types/otctime.cc
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
#pragma implementation "OTC/types/time.hh"
#endif
#endif

#include <OTC/types/time.hh>

#include <OTC/stdlib/iostream.h>
#include <OTC/stdlib/strstream.h>

#include <string.h>

#if defined(HAVE_NL_LANGINFO)
//#include <nl_types.h>
#include <langinfo.h>
#endif

/* ------------------------------------------------------------------------- */
OTC_Time::OTC_Time(OTC_TimeZone theTimeZone)
  : OTC_Date((long)0)
{
  setTime(theTimeZone);
}

/* ------------------------------------------------------------------------- */
OTC_Time::OTC_Time(OTC_Time const& theTime)
 : OTC_Date(theTime)
{
  seconds_ = theTime.seconds_;
}

/* ------------------------------------------------------------------------- */
OTC_Time::OTC_Time(int theHour, int theMin, int theSec)
  : OTC_Date((long)0)
{
  setTime(theHour,theMin,theSec);
}

/* ------------------------------------------------------------------------- */
OTC_Time::OTC_Time(
 OTC_Date const& theDate,
 int theHour,
 int theMin,
 int theSec
)
  : OTC_Date((long)0)
{
  setTime(theDate,theHour,theMin,theSec);
}

/* ------------------------------------------------------------------------- */
OTC_Time::OTC_Time(time_t theSeconds, OTC_TimeZone theTimeZone)
  : OTC_Date((long)0)
{
  if (theTimeZone == OTCLIB_LOCALTIME)
  {
#if defined(_REENTRANT)
    tm theTimeStruct;
    localtime_r(&theSeconds,&theTimeStruct);
    tm* theTime = &theTimeStruct;
#else
    tm* theTime = localtime(&theSeconds);
#endif
    OTC_Date theDate(theTime->tm_mday,theTime->tm_mon+1, theTime->tm_year+1900);
    setTime(theDate,theTime->tm_hour,theTime->tm_min,theTime->tm_sec);
  }
  else
  {
#if defined(_REENTRANT)
    tm theTimeStruct;
    gmtime_r(&theSeconds,&theTimeStruct);
    tm* theTime = &theTimeStruct;
#else
    tm* theTime = gmtime(&theSeconds);
#endif
    OTC_Date theDate(theTime->tm_mday,theTime->tm_mon+1,theTime->tm_year+1900);
    setTime(theDate,theTime->tm_hour,theTime->tm_min,theTime->tm_sec);
  }
}

/* ------------------------------------------------------------------------- */
OTC_Time::OTC_Time(char const* theTimeString)
  : OTC_Date((long)0)
{
  int theDay = 0;
  int theMonth = 0;
  int theYear = 0;
  int theHour = 0;
  int theMin = 0;
  int theSec = 0;

  convertToTime(theTimeString,theDay,theMonth,theYear,theHour,theMin,theSec);

  OTCLIB_ENSURE_FN((theDay && theMonth && theYear),
   "OTC_Time::OTC_Time(char const*)",
   "invalid format for time");

  OTC_Date tmpDate(theDay,theMonth,theYear);
  setTime(tmpDate,theHour,theMin,theSec);
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::isValidTime(int theHour, int theMin, int theSec)
{
  if (theHour < 0 || theHour >= 24)
    return false;

  if (theMin < 0 || theMin >= 60)
    return false;

  if (theSec < 0 || theSec >= 60)
    return false;

  return true;
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::isValidTime(char const* theTimeString)
{
  int theDay = 0;
  int theMonth = 0;
  int theYear = 0;
  int theHour = 0;
  int theMin = 0;
  int theSec = 0;

  convertToTime(theTimeString,theDay,theMonth,theYear,theHour,theMin,theSec);

  if (!theDay && !theMonth && !theYear && !theHour && !theMin && !theSec)
    return false;

  bool theResult = true;

  theResult = theResult && isValidDate(theDay,theMonth,theYear);
  theResult = theResult && isValidTime(theHour,theMin,theSec);

  return theResult;
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::convertToTime(
 char const* theTimeString,
 int& theDay,
 int& theMonth,
 int& theYear,
 int& theHour,
 int& theMin,
 int& theSec
)
{
  if (theTimeString == 0)
  {
    theYear = theMonth = theDay = 0;
    theHour = theMin = theSec = 0;
    return false;
  }

  theYear = 0;
  theMonth = 0;
  theDay = 0;
  theHour = 0;
  theMin = 0;
  theSec = 0;

  // Try YYYY-MM-DDThh:mm:ss.

  istrstream ins((char*)theTimeString,strlen(theTimeString));
  ins.unsetf(ios::skipws);

  char theSeparator1 = 0;
  char theSeparator2 = 0;
  char theSeparator3 = 0;
  char theSeparator4 = 0;
  char theSeparator5 = 0;

  ins >> dec >> theYear;
  ins >> theSeparator1;
  ins >> dec >> theMonth;
  ins >> theSeparator2;
  ins >> dec >> theDay;
  ins >> theSeparator3;
  ins >> dec >> theHour;
  ins >> theSeparator4;
  ins >> dec >> theMin;
  ins >> theSeparator5;
  ins >> dec >> theSec;

  if (!ins.fail() && theSeparator1 == '-' && theSeparator2 == '-'
   && theSeparator3 == 'T' && theSeparator4 == ':' && theSeparator5 == ':'
  )
  {
    return true;
  }

  theYear = theMonth = theDay = 0;
  theHour = theMin = theSec = 0;

  return false;
}

/* ------------------------------------------------------------------------- */
int OTC_Time::hour() const
{
  return seconds_ / (60*60);
}

/* ------------------------------------------------------------------------- */
int OTC_Time::minute() const
{
  return (seconds_ / 60) % 60;
}

/* ------------------------------------------------------------------------- */
int OTC_Time::second() const
{
  return seconds_ % 60;
}

/* ------------------------------------------------------------------------- */
OTC_Time OTC_Time::plusSeconds(int theNum) const
{
  OTC_Time theTime(*this);
  theTime.addSeconds(theNum);
  return theTime;
}

/* ------------------------------------------------------------------------- */
OTC_Time OTC_Time::plusDuration(OTC_Duration const& theDuration) const
{
  OTC_Time theTime(*this);
  theTime.addDuration(theDuration);
  return theTime;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Time::asString(char const* theFormat) const
{
  if (theFormat == 0)
  {
    OTC_String tmpString = OTC_Length(47);
    ostrstream theStream(tmpString.buffer(),tmpString.capacity());

    int tmpDay;
    int tmpMonth;
    int tmpYear;

    OTC_Date::calendarDate(jday(),tmpDay,tmpMonth,tmpYear);

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
    theStream << tmpDay << 'T';
    theStream.width(2);
    theStream << hour() << ':';
    theStream.width(2);
    theStream << minute() << ':';
    theStream.width(2);
    theStream << second() << ends;

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
      "Sun", "Mon", "Tue", "Wed",
      "Thu", "Fri", "Sat",
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

	  case 'c':
	  {
	    // Time as "Tue Dec 31 23:59:59 1996".

#if defined(HAVE_NL_LANGINFO)
            char const* format;
            format = (char const*)nl_langinfo(D_T_FMT);
            theStream << asString(format);
#else
	    char buf[64];
	    sprintf(buf,"%s %s %2d %02d:%02d:%02d %d",days_a[dayOfWeek()],
	     months_a[monthOfYear()-1],dayOfMonth(),hour(),minute(),
	     second(),year());
	    theStream << buf;
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

	  case 'H':
	  {
	    // Hour (24-hour clock) [0,23]; single digits
	    // are preceded by 0.

	    char buf[16];
	    sprintf(buf,"%02d",hour());
	    theStream << buf;

	    break;
	  }

	  case 'I':
	  {
	    // Hour (12-hour clock) [1,12]; single digits
	    // are preceded by 0.

	    int theHour = hour();

	    if (theHour == 0)
	      theHour = 12;
	    else if (theHour > 12)
	      theHour -= 12;

	    char buf[16];
	    sprintf(buf,"%02d",theHour);
	    theStream << buf;

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

	  case 'k':
	  {
	    // Hour (24-hour clock) [0,23]; single digits
	    // are preceded by a blank.

	    char buf[16];
	    sprintf(buf,"%2d",hour());
	    theStream << buf;

	    break;
	  }

	  case 'l':
	  {
	    // Hour (12-hour clock) [1,12]; single digits
	    // are preceded by a blank.

	    int theHour = hour();

	    if (theHour == 0)
	      theHour = 12;
	    else if (theHour > 12)
	      theHour -= 12;

	    char buf[16];
	    sprintf(buf,"%2d",theHour);
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

	  case 'M':
	  {
	    // Minute [00,59]; leading zero is permitted
	    // but not required.

	    char buf[16];
	    sprintf(buf,"%02d",minute());
	    theStream << buf;

	    break;
	  }

	  case 'n':
	  {
	    // Insert a new line.

	    theStream << '\n';
	    break;
	  }

	  case 'p':
	  {
	    // AM or PM.

#if defined(HAVE_NL_LANGINFO)
	    if (hour() < 12)
	      theStream << (char const*)nl_langinfo(AM_STR);
	    else
	      theStream << (char const*)nl_langinfo(PM_STR);
#else
	    if (hour() < 12)
	      theStream << "AM";
	    else
	      theStream << "PM";
#endif

	    break;
	  }

	  case 'r':
	  {
	    // Time as %I:%M:%S %p.

	    int theHour = hour();

	    if (theHour == 0)
	      theHour = 12;
	    else if (theHour > 12)
	      theHour -= 12;

	    char buf[32];
#if defined(HAVE_NL_LANGINFO)
	    if (hour() < 12)
	    {
	      sprintf(buf,"%02d:%02d:%02d %s",theHour,minute(),second(),
	       (char const*)nl_langinfo(AM_STR));
	    }
	    else
	    {
	      sprintf(buf,"%02d:%02d:%02d %s",theHour,minute(),second(),
	       (char const*)nl_langinfo(PM_STR));
	    }
#else
	    if (hour() < 12)
	      sprintf(buf,"%02d:%02d:%02d AM",theHour,minute(),second());
	    else
	      sprintf(buf,"%02d:%02d:%02d PM",theHour,minute(),second());
#endif
	    theStream << buf;

	    break;
	  }

	  case 'R':
	  {
	    // Time as %H:%M.

	    char buf[12];
	    sprintf(buf,"%02d:%02d",hour(),minute());
	    theStream << buf;

	    break;
	  }

	  case 'S':
	  {
	    // Seconds [00,59].

	    char buf[16];
	    sprintf(buf,"%02d",second());
	    theStream << buf;

	    break;
	  }

	  case 't':
	  {
	    // Insert a tab.

	    theStream << '\t';
	    break;
	  }

	  case 'T':
	  {
	    // Time as %H:%M:%S.

	    char buf[16];
	    sprintf(buf,"%02d:%02d:%02d",hour(),minute(),second());
	    theStream << buf;

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

	    if (theDate > *this)
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
	    // Date as "Tue Dec 31 1996".

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

	  case 'X':
	  {
	    // Time as "23:59:59".

#if defined(HAVE_NL_LANGINFO)
            char const* format;
            format = (char const*)nl_langinfo(T_FMT);
            theStream << asString(format);
#else
	    char buf[32];
	    sprintf(buf,"%02d:%02d:%02d",hour(),minute(),second());
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
OSE_EXPORT ostream& operator<<(ostream& theStream, OTC_Time const& theTime)
{
  int tmpDay;
  int tmpMonth;
  int tmpYear;

  OTC_Date::calendarDate(theTime.jday(),tmpDay,tmpMonth,tmpYear);

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
  theStream << tmpDay << 'T';
  theStream.width(2);
  theStream << theTime.hour() << ':';
  theStream.width(2);
  theStream << theTime.minute() << ':';
  theStream.width(2);
  theStream << theTime.second();

  theStream.fill(fchar);

  return theStream;
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::operator<(OTC_Time const& theTime) const
{
  if (OTC_Date::operator<(theTime))
    return true;

  else if (OTC_Date::operator==(theTime))
    return seconds_ < theTime.seconds_;

  else
    return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::operator<=(OTC_Time const& theTime) const
{
  if (OTC_Date::operator<(theTime))
    return true;

  else if (OTC_Date::operator==(theTime))
    return seconds_ <= theTime.seconds_;

  else
    return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::operator>(OTC_Time const& theTime) const
{
  if (OTC_Date::operator>(theTime))
    return true;

  else if (OTC_Date::operator==(theTime))
    return seconds_ > theTime.seconds_;

  else
    return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::operator>=(OTC_Time const& theTime) const
{
  if (OTC_Date::operator>(theTime))
    return true;

  else if (OTC_Date::operator==(theTime))
    return seconds_ >= theTime.seconds_;

  else
    return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::operator==(OTC_Time const& theTime) const
{
  if (OTC_Date::operator!=(theTime))
    return false;

  else
    return seconds_ == theTime.seconds_;
}

/* ------------------------------------------------------------------------- */
bool OTC_Time::operator!=(OTC_Time const& theTime) const
{
  if (OTC_Date::operator!=(theTime))
    return true;

  else
    return seconds_ != theTime.seconds_;
}

/* ------------------------------------------------------------------------- */
void OTC_Time::setTime(OTC_TimeZone theTimeZone)
{
  time_t theClock = ::time(0);
  if (theTimeZone == OTCLIB_LOCALTIME)
  {
#if defined(_REENTRANT)
    tm theTimeStruct;
    localtime_r(&theClock,&theTimeStruct);
    tm* theTime = &theTimeStruct;
#else
    tm* theTime = localtime(&theClock);
#endif
    setDate(theTime->tm_mday,theTime->tm_mon+1,theTime->tm_year+1900);
    seconds_ = (theTime->tm_hour*60*60)+(theTime->tm_min*60)+theTime->tm_sec;
  }
  else
  {
#if defined(_REENTRANT)
    tm theTimeStruct;
    tm* theTime = gmtime_r(&theClock,&theTimeStruct);
#else
    tm* theTime = gmtime(&theClock);
#endif
    setDate(theTime->tm_mday,theTime->tm_mon+1,theTime->tm_year+1900);
    seconds_ = (theTime->tm_hour*60*60)+(theTime->tm_min*60)+theTime->tm_sec;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Time::setTime(OTC_Time const& theTime)
{
  setDate(theTime);
  seconds_ = theTime.seconds_;
}

/* ------------------------------------------------------------------------- */
void OTC_Time::setTime(int theHour, int theMin, int theSec)
{
  setDate();
  seconds_ = (theHour*60*60)+(theMin*60)+theSec;
}

/* ------------------------------------------------------------------------- */
void OTC_Time::setTime(
 OTC_Date const& theDate,
 int theHour,
 int theMin,
 int theSec
)
{
  setDate(theDate);
  seconds_ = (theHour*60*60)+(theMin*60)+theSec;
}

/* ------------------------------------------------------------------------- */
OTC_Time& OTC_Time::addSeconds(int theNum)
{
  seconds_ += theNum;
  if (seconds_ >= (24*60*60))
  {
    addDays(seconds_/(24*60*60));
    seconds_ = seconds_ % (24*60*60);
  }
  else if (seconds_ < 0)
  {
    addDays((seconds_/(24*60*60))-1);
    seconds_ = (24*60*60) + (seconds_ % (24*60*60));
  }
  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_Time& OTC_Time::addMinutes(int theNum)
{
  return addSeconds(60*theNum);
}

/* ------------------------------------------------------------------------- */
OTC_Time& OTC_Time::addHours(int theNum)
{
  return addSeconds(60*60*theNum);
}

/* ------------------------------------------------------------------------- */
OTC_Time& OTC_Time::addDuration(OTC_Duration const& theDuration)
{
  int theSign = theDuration.sign();

  addSeconds(theSign*theDuration.seconds());
  addMinutes(theSign*theDuration.minutes());
  addHours(theSign*theDuration.hours());
  addDays(theSign*theDuration.days());

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Time::operator-(OTC_Time const& theTime) const
{
  OTC_Duration d(0);

  if (this == &theTime || *this == theTime)
    return d;

  d = OTC_Date::operator-(theTime);
  d.subtractDuration(0,theTime.hour(),theTime.minute(),theTime.second());
  d.addDuration(0,hour(),minute(),second());
  return d;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Time::operator-(OTC_Date const& theDate) const
{
  OTC_Duration d(0);

  d = OTC_Date::operator-(theDate);
  d.addDuration(0,hour(),minute(),second());

  return d;
}

/* ------------------------------------------------------------------------- */
int OTC_Time::rank(OTC_Time const& theTime) const
{
  int theRank = OTC_Date::rank(theTime);

  if (theRank == 0)
    theRank = seconds_ - theTime.seconds_;

  return theRank;
}

/* ------------------------------------------------------------------------- */
