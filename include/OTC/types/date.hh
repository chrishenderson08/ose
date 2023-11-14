#ifndef OTC_TYPES_DATE_HH
#define OTC_TYPES_DATE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     types/date.hh
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

#include <OTC/types/duration.hh>
#include <OTC/collctn/hashactn.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/text/string.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/types/date.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_Day
    // = TITLE
    //	   The days of the week.
{
  OTCLIB_SUNDAY=0,
  OTCLIB_MONDAY=1,
  OTCLIB_TUESDAY=2,
  OTCLIB_WEDNESDAY=3,
  OTCLIB_THURSDAY=4,
  OTCLIB_FRIDAY=5,
  OTCLIB_SATURDAY=6
};

enum OTC_Month
    // = TITLE
    //	   The months of the year.
{
  OTCLIB_JANUARY=1,
  OTCLIB_FEBRUARY=2,
  OTCLIB_MARCH=3,
  OTCLIB_APRIL=4,
  OTCLIB_MAY=5,
  OTCLIB_JUNE=6,
  OTCLIB_JULY=7,
  OTCLIB_AUGUST=8,
  OTCLIB_SEPTEMBER=9,
  OTCLIB_OCTOBER=10,
  OTCLIB_NOVEMBER=11,
  OTCLIB_DECEMBER=12
};

enum OTC_TimeZone
{
  OTCLIB_GMTIME,
  OTCLIB_LOCALTIME
};

class OSE_EXPORT OTC_Date
    // = TITLE
    //	   Class for representing a date.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   Class for representing dates. The class uses Julian Day Numbers
    //	   and will convert between them and either Gregorian and Julian
    //	   calendar dates as appropriate. Julian Day Numbers (JDN) are used
    //	   by astronomers as a date/time measure independent of calendars and
    //	   convenient for computing the elapsed time between dates. The JDN
    //	   for any date/time is the number of days (including fractional
    //	   days) elapsed since noon, 1 Jan 4713 BC. Julian Day Numbers were
    //	   originated by Joseph Scaliger in 1582 and named after his father
    //	   Julius, not after Julius Caesar. They are not related to the
    //	   Julian calendar.
    //
    //     On a 32 bit system, the class is capable of handling dates from
    //     approximately 7450 BC through 1465000 AD. This will be dependent
    //     a bit on the floating point implementation. Years BC are given
    //     (and returned) as negative numbers. Note that there is no year 0
    //     BC; the day before 1 Jan 1 AD is 31 Dec 1 BC. Note also that 1 BC,
    //     5 BC, etc. are leap years.
    //     
    //	   Pope Gregory XIII decreed that the Julian calendar would end on 4
    //	   Oct 1582 AD and that the next day would be 15 Oct 1582 in the
    //	   Gregorian Calendar. The only other change is that centesimal
    //	   years (years ending in 00) would no longer be leap years unless
    //	   divisible by 400. Britain and its possessions and colonies
    //	   continued to use the Julian calendar up until 2 Sep 1752, when the
    //	   next day became 14 Sep 1752 in the Gregorian Calendar. This class
    //	   uses Pope Gregory's convention.
    //
    //     The ISO 8601:1988 extended format for representing dates as
    //     strings is "CCYY-MM-DD". For example the 25th December 1995 would
    //     be represented as "1995-12-25". This format is used when converting
    //     the date to a string. Note though that ISO 8601:1988 standard
    //     doesn't really talk about years before year 1. In this case, just
    //     output a negative sign before the year although it may not be
    //     strictly correct.
    //
    //     When reading in a string and converting it to a date, the string
    //     format is expected to be that of the extended date format of ISO
    //     8601:1988. Note though that the conversion routine allows some
    //     variance from the standard in that it doesn't expect each
    //     component to be padded out with leading zeros.
    //     
    // = NOTES
    //	   The code uses routines which are in the public domain.
    //
    // = SEE ALSO
    //	   <OTC_Duration>
{
  public:

    // = INITIALISATION

			OTC_Date(OTC_TimeZone theTimeZone=OTCLIB_LOCALTIME);
				// Initialises this to todays date. A value
				// of <OTCLIB_LOCALTIME> for <theTimeZone>
				// will result in the date being adjusted
				// corresponding to whatever the local
				// timezone of the machine is set to. To
				// leave the date unadjusted, use the
                                // value <OTCLIB_GMTIME> for <theTimeZone>.

			OTC_Date(OTC_Date const& theDate)
			  : date_(theDate.date_) {}
				// Initialises the date to that of <theDate>.

			OTC_Date(int theDay, int theMonth, int theYear);
				// Initialises the date to that corresponding
				// to <theDay> of <theMonth> of <theYear>.
				// A negative year is interpreted as BC. A
				// zero value for the year will result in
				// an exception being raised.

			OTC_Date(char const* theDateString);
				// Initialises the date to that described
				// by <theDateString>. The only format for
				// <theDateString> which is currently
				// understood is that described for dates in
				// ISO 8601:1988. If <theDateString> isn't in
				// a valid format, an exception will be
				// raised.

			OTC_Date(long theDate)
				{ date_ = theDate; }
				// Initialises the date given the Julian
				// day number <theDate>.

    // = CLASS OPERATIONS

    static OTC_Date     currentDate(OTC_TimeZone theTimeZone=OTCLIB_LOCALTIME)
                                {
				  OTC_Date theDate(theTimeZone);
				  return theDate;
				}
				// Returns the current date. A value
				// of <OTCLIB_LOCALTIME> for <theTimeZone>
				// will result in the date being adjusted
				// corresponding to whatever the local
				// timezone of the machine is set to. To
				// leave the date unadjusted, use the
                                // value <OTCLIB_GMTIME> for <theTimeZone>.

    static bool		isValidDate(int theDay, int theMonth, int theYear);
				// Returns <true> if <theDay>, <theMonth> and
				// <theYear> describes a valid date. Note
				// that this will not work properly if the
				// date described is outside the bounds of
				// what the date class is capable of
				// handling. The function only really
				// determines if what you have given for
				// <theDay> and <theMonth> are valid.

    static bool		isValidDate(char const* theDateString);
				// Returns <true> if the contents of
				// <theDateString> describe a valid date. The
				// only format for <theDateString>
				// which is currently understood is that
				// described for dates in ISO 8601:1988.
				// Once the string is interpreted, the
				// <isValidDate()> function above is used
				// to determine if the day of month and
				// month are valid.

    static long		julianDate(int theDay, int theMonth, int theYear);
				// Returns the Julian day number for
				// <theDay> of <theMonth> of <theYear>.

    static void		calendarDate(
			 long theDate,
			 int& theDay,
			 int& theMonth,
			 int& theYear
			);
				// Sets <theDay>, <theMonth> and <theYear>
				// corresponding to the Julian day number
				// <theDate>.

    static bool		convertToDate(
			 char const* theDateString,
			 int& theDay,
			 int& theMonth,
			 int& theYear
			);
				// Sets <theDay>, <theMonth> and <theYear>
				// corresponding to the date described by
				// the contents of <theDateString>. The only
				// format for <theDateString> which is
				// currently understood is the extended date
				// format in ISO 8601:1988. <true> is
				// returned if the string was in the format
				// expected, else <false> is returned.
				// Note that the values returned are as
				// they appear in the string and are not
				// normalised or validated.

    // = RELATIONAL OPERATORS

    bool		operator==(OTC_Date const& theDate) const
				{ return date_ == theDate.date_; }
				// Returns <true> if this date is the same as
				// <theDate>.

    bool		operator!=(OTC_Date const& theDate) const
				{ return date_ != theDate.date_; }
				// Returns <true> if this date is not the
				// same as <theDate>.

    bool		operator<(OTC_Date const& theDate) const
				{ return date_ < theDate.date_; }
				// Returns <true> if this date comes before
				// <theDate>.

    bool		operator<=(OTC_Date const& theDate) const
				{ return date_ <= theDate.date_; }
				// Returns <true> if this date comes before
				// or is equal to <theDate>.

    bool		operator>(OTC_Date const& theDate) const
				{ return date_ > theDate.date_; }
				// Returns <true> if this date comes after
				// <theDate>.

    bool		operator>=(OTC_Date const& theDate) const
				{ return date_ >= theDate.date_; }
				// Returns <true> if this date comes after or
				// is equal to <theDate>.

    // = QUERY

    long		jday() const
				{ return date_; }
				// Returns the date as its Julian day number.

    int			day() const;
				// Returns the day of the month.

    int			month() const;
				// Returns the month of the year.

    int			year() const;
				// Returns the year.

    int			dayOfWeek() const
				{ return int((date_ + 1) % 7); }
				// Returns the day of the week. Use the
				// enumeration <OTC_Day> to determine
				// which day. Ie., <OTCLIB_SUNDAY>,
				// <OTCLIB_MONDAY>, etc.

    int			dayOfMonth() const
				{ return day(); }
				// Returns the day of the month. This is
				// the same as calling <day()>.

    int			dayOfYear() const;
				// Returns the day of the year which this
				// date falls on.

    int			daysInYear() const;
				// Returns the number of days in the year
				// in which this date falls.

    int			weekOfYear() const;
				// Returns the week of the year in which the
				// day falls. If the week (Monday to Sunday)
				// containing January 1 has four or more days
				// in the new year, then it is week 1;
				// otherwise it is week 53 of the previous
				// year, and the next week is week 1. This
				// interpretation is that given in the ISO
				// 8601 standard. The interpretation is quite
				// bizzare, as it means that the last week
				// of the previous year could change from
				// being week 52 to week 53 part way through
				// it.

    int			monthOfYear() const
				{ return month(); }
				// Returns the month of the year. This is
				// the same as calling <month()>.

    OTC_String const&	nameOfDayOfWeek() const;
				// Returns the day of the week as a string.

    OTC_String const&	nameOfMonth() const;
				// Returns the name of the month as a string.

    bool		isBetween(
			 OTC_Date const& theDate1,
			 OTC_Date const& theDate2
			) const;
				// Returns <true> if this date lies between
				// <theDate1> or <theDate2> or is the same as
				// either of those dates. The order of the
				// dates makes no difference to the result.

    bool		isWeekDay() const;
				// Returns <true> if this date is a weekday.

    bool		isFirstDayOfMonth() const;
				// Returns <true> if this date is the first
				// day of the month.

    bool		isLastDayOfMonth() const;
				// Returns <true> if this date is the last
				// day of the month.

    bool		isFirstDayOfYear() const
				{ return (*this) == firstDayOfYear(); }
				// Returns <true> if this date is the first
				// day of the year.

    bool		isLastDayOfYear() const
				{ return (*this) == lastDayOfYear(); }
				// Returns <true> if this date is the last
				// day of the year.

    bool		isLeapYear() const;
				// Returns <true> if this date falls in a
				// leap year.

    OTC_Date		following(int theDay) const;
				// Returns the date of the following <theDay>.

    OTC_Date		previous(int theDay) const;
				// Returns the date of the previous <theDay>.

    OTC_Date		firstDayOfWeek() const;
				// Returns the date of the first day of
				// the week in which this date falls.
				// Ie. Sunday.

    OTC_Date		lastDayOfWeek() const;
				// Returns the date of the last day of
				// the week in which this date falls.
				// Ie. Saturday.

    OTC_Date		firstDayOfMonth() const;
				// Returns the date of the first day of
				// the month in which this date falls.

    OTC_Date		lastDayOfMonth() const;
				// Returns the date of the last day of
				// the month in which this date falls.

    OTC_Date		firstDayOfYear() const;
				// Returns the date of the first day of
				// the year in which this date falls.

    OTC_Date		lastDayOfYear() const;
				// Returns the date of the last day of
				// the year in which this date falls.

    int			daysTill(OTC_Date const& theDate) const
				{ return theDate.date_ - date_; }
				// Returns the number of days between this
				// date and <theDate>.

    OTC_Date		plusDays(int theNum) const
				{ return date_ + theNum; }
				// Returns the date of the day <theNum> days
				// following this date.

    OTC_Date		minusDays(int theNum) const
				{ return date_ - theNum; }
				// Returns the date of the day <theNum> days
				// previous to this date.

    OTC_Date		plusWeeks(int theNum)
				{ return plusDays(7*theNum); }
				// Returns the date of the day <theNum> weeks
				// following this date.

    OTC_Date		minusWeeks(int theNum) const
				{ return minusDays(7*theNum); }
				// Returns the date of the day <theNum> weeks
				// previous to this date.

    // = TEXTUAL REPRESENTATION

    OTC_String		asString(char const* theFormat=0) const;
				// Returns the date as a string. The format
				// used for the date when converting it to a
				// string, is extended format defined by ISO
				// 8601:1988 if <theFormat> is <0>, that is
				// "CCYY-MM-DD", else <theFormat> is
				// interpreted as per <strftime()> except
				// those aspects of <strftime()> pertaining
				// to time.

#if 0

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& theStream,
				 OTC_Date const& theDate
				);
				// Outputs the date onto <theStream>. The
				// format used for the date when displaying
				// it, is that defined by the extended format
				// in ISO 8601:1988.

#endif

    // = MODIFIERS

    void		setDate(OTC_TimeZone theTimeZone=OTCLIB_LOCALTIME);
				// Sets the date to todays date. A value
				// of <OTCLIB_LOCALTIME> for <theTimeZone>
				// will result in the date being adjusted
				// corresponding to whatever the local
				// timezone of the machine is set to. To
				// leave the date unadjusted, use the
                                // value <OTCLIB_GMTIME> for <theTimeZone>.

    void		setDate(OTC_Date const& theDate)
				{ date_ = theDate.date_; }
				// Sets the date to the defined by
				// <theDate>.

    bool		setDate(int theDay, int theMonth, int theYear);
				// Sets the date to that described by
				// <theDay>, <theMonth> and <theYear>.
				// Returns <true> if it was a valid date.

    OTC_Date&		operator=(OTC_Date const& theDate)
				{ setDate(theDate); return *this; }
				// Sets this date to that defined by
				// <theDate>.

    OTC_Date&		addDays(int theNum)
				{ date_ += theNum; return *this; }
				// Adds <theNum> days to this date and
				// then returns the new date.

    OTC_Date&		subtractDays(int theNum)
				{ return addDays(-theNum); }
				// Deducts <theNum> days from this date and
				// then returns the new date.

    OTC_Date&		addWeeks(int theNum)
				{ return addDays(7*theNum); }
				// Add <theNum> weeks to the date and then
				// return the date.

    OTC_Date&		subtractWeeks(int theNum)
				{ return addWeeks(-theNum); }
				// Subtract <theNum> weeks to the date and
				// then return the date.

    // = ARITHMETIC

    OTC_Duration	operator-(OTC_Date const& theDate) const;
				// Returns the duration between this
				// date and <theDate>. Note that a negative
				// duration indicates that <theDate> is
				// in the future with respect to this date.

    // = RANKING

    int			rank(OTC_Date const& theDate) const
				{ return date_ - theDate.date_; }
				// Returns a value less than, equal to,
				// or greater than zero corresponding to
				// how this date compares to <theDate>.

    // = HASHING

    int			hash() const
				{ return int(date_); }
				// Returns a hash value for this date.

    // = DATES

    static OTC_Date	now()
				{ return OTC_Date(); }
				// Returns the current date.

  private:

    long		date_;
				// The date as a Julian day number.

    static OTC_NRMutex	mutex_;
				// Lock for threads.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_RankActions<OTC_Date>
{
  public:
    static int		rank(OTC_Date const& theDate1, OTC_Date const& theDate2)
				{ return theDate1.rank(theDate2); }
};

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_HashActions<OTC_Date>
{
  public:
    static int		hash(OTC_Date const& theDate)
				{ return theDate.hash(); }
};

/* ------------------------------------------------------------------------- */

OSE_EXPORT ostream& operator<<(ostream& theStream, OTC_Date const& theDate);

/* ------------------------------------------------------------------------- */

#endif /* OTC_TYPES_DATE_HH */
