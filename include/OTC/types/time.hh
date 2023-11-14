#ifndef OTC_TYPES_TIME_HH
#define OTC_TYPES_TIME_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     types/time.hh
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

#include <OTC/types/date.hh>

#include <time.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/types/time.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Time : public OTC_Date
    // = TITLE
    //	   Class maintaining information about a day and the time of day.
    //	   
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   Maintains time information down to a resolution of one second.
    //	   The class does not track anything related to timezones or whether
    //	   daylight savings time is in operation. It exists purely to record
    //	   a time value. Times can be from 00:00:00, 1 Jan 4713 BC thru to
    //	   some time in the distant future.
    //
    //     The ISO 8601:1988 extended format for representing date/times as
    //     strings is "CCYY-MM-DDThh:mm:ss". For example 12 noon, 25th
    //     December 1995, would be represented as "1995-12-25T12:00:00".
    //     This format is used when converting the date/time to a string.
    //     Note though that ISO 8601:1988 standard doesn't really talk about
    //     years before year 1. In this case, just output a negative sign
    //     before the year although it may not be strictly correct.
    //     
    //     When reading in a string and converting it to a date/time, the
    //     string format is expected to be that of the extended date/time
    //     format of ISO 8601:1988. Note though that the conversion routine
    //     allows some variance from the standard in that it doesn't expect
    //     each component to be padded out with leading zeros.
    //     
    // = SEE ALSO
    //	   <OTC_Date>, <OTC_Duration>
{
  public:

    // = INITIALISATION

			OTC_Time(OTC_TimeZone theTimeZone=OTCLIB_LOCALTIME);
				// Initialises the time to the current time.
				// A value of <OTCLIB_LOCALTIME> for
				// <theTimeZone> will result in the time
				// being adjusted corresponding to whatever
				// the local timezone of the machine is set
				// to. To leave the time unadjusted, use the
				// value <OTCLIB_GMTIME> for <theTimeZone>.

			OTC_Time(OTC_Time const& theTime);
				// Initialises the time to that specified by
				// <theTime>.

			OTC_Time(
			 time_t theSeconds,
			 OTC_TimeZone theTimeZone=OTCLIB_LOCALTIME
			);
				// Initialises the time to <theSeconds> since
				// the computers epoch, generally 00:00:00
				// UTC, January 1, 1970. A value of
				// <OTCLIB_LOCALTIME> for <theTimeZone> will
				// result in the time being adjusted
				// corresponding to whatever the local
				// timezone of the machine is set to.
				// To leave the time unadjusted, use the
				// value <OTCLIB_GMTIME> for <theTimeZone>.

			OTC_Time(int theHour, int theMin, int theSec);
				// Initialises the time to today and with
				// the hour set to <theHour>, the minute
				// set to <theMin> and seconds to <theSec>.

			OTC_Time(
			 OTC_Date const& theDate,
			 int theHour=0,
			 int theMin=0,
			 int theSec=0
			);
				// Initialises the time to that specified by
				// <theDate> and with the hour set to
				// <theHour>, the minute set to <theMin> and
				// seconds to <theSec>.

			OTC_Time(char const* theTimeString);
				// Initialises the time to that described by
				// <theTimeString>. The only format for
				// <theTimeString> which is accepted is that
				// described for the string representation of
				// times in ISO 8601:1988. An exception will
				// be raised if <theTimeString> is in an
				// incorrect format.

    // = CLASS OPERATIONS

    static OTC_Time	currentTime(OTC_TimeZone theTimeZone=OTCLIB_LOCALTIME)
				{
				  OTC_Time theTime(theTimeZone);
				  return theTime;
				}
				// Returns the current time. A value of
				// <OTCLIB_LOCALTIME> for <theTimeZone> will
				// result in the time being adjusted
				// corresponding to whatever the local
				// timezone of the machine is set to. To
				// leave the time unadjusted, use the value
				// <OTCLIB_GMTIME> for <theTimeZone>.

    static bool		isValidTime(int theHour, int theMin, int theSec);
				// Returns <true> if the arguments are within
				// valid ranges.

    static bool		isValidTime(char const* theTimeString);
				// Returns <true> if <theTimeString>
				// describes a valid time.

    static bool		convertToTime(
			 char const* theTimeString,
			 int& theDay,
			 int& theMonth,
			 int& theYear,
			 int& theHour,
			 int& theMin,
			 int& theSec
			);
				// Sets <theDay>, <theMonth>, <theYear>,
				// <theHour>, <theMin> and <theSec>
				// corresponding to the time described by the
				// contents of <theTimeString>. The only
				// format for <theTimeString> which is
				// currently understood is the extended time
				// format in ISO 8601:1988. <true> is
				// returned if the string was in the format
				// expected, else <false> is returned.
				// Note that the values returned are as
                                // they appear in the string and are not
                                // normalised or validated.

    // = QUERY

    int			hour() const;
				// Returns the number of hours using 24 hour
				// time, <0-23>.

    int			minute() const;
				// Returns the number of minutes, <0-59>.

    int			second() const;
				// Returns the number of seconds, <0-59>.

    OTC_Time		plusSeconds(int theNum) const;
				// Returns this time with <theNum> seconds
				// added.

    OTC_Time		minusSeconds(int theNum) const
				{ return plusSeconds(-theNum); }
				// Returns this time with <theNum> seconds
				// subtracted.

    OTC_Time		plusMinutes(int theNum) const
				{ return plusSeconds(60*theNum); }
				// Returns this time with <theNum> minutes
				// added.

    OTC_Time		minusMinutes(int theNum) const
				{ return minusSeconds(60*theNum); }
				// Returns this time with <theNum> minutes
				// subtracted.

    OTC_Time		plusHours(int theNum) const
				{ return plusSeconds(60*60*theNum); }
				// Returns this time with <theNum> hours
				// added.

    OTC_Time		minusHours(int theNum) const
				{ return minusSeconds(60*60*theNum); }
				// Returns this time with <theNum> hours
				// subtracted.

    OTC_Time		plusDuration(OTC_Duration const& theDuration) const;
				// Returns this time with <theDuration>
				// added.

    OTC_Time		minusDuration(OTC_Duration const& theDuration) const
				{ return plusDuration(-theDuration); }
				// Returns this time with <theDuration>
				// subtracted.

    // = TEXTUAL REPRESENTATION

    OTC_String		asString(char const* theFormat=0) const;
				// Returns the time as a string. The format
				// used for the time when converting it to a
				// string, is that defined by ISO 8601:1988
				// if <theFormat> is <0>, else <theFormat> is
				// interpreted as per <strftime()> except
				// those aspects of <strftime()> pertaining
				// to time which cannot be handled such as
				// timezones.

#if 0

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& theStream,
				 OTC_Time const& theTime
				);
				// Outputs the time onto <theStream>. The
				// format used for the time when displaying
				// it, is that defined by ISO 8601:1988.

#endif

    // = MODIFIERS

    void		setTime(OTC_TimeZone theTimeZone=OTCLIB_LOCALTIME);
				// Sets the time to the current time.
				// A value of <OTCLIB_LOCALTIME> for
				// <theTimeZone> will result in the time
				// being adjusted corresponding to whatever
				// the local timezone of the machine is set
				// to. To leave the time unadjusted, use the
				// value <OTCLIB_GMTIME> for <theTimeZone>.

    void		setTime(OTC_Time const& theTime);
				// Sets the time to that defined by
				// <theTime>.

    void		setTime(int theHour, int theMin, int theSec);
				// Sets the time to that defined
				// by <theHour>, <theMin> and <theSec>
				// within the current day. Values of
				// zero indicate midnight.

    void		setTime(
			 OTC_Date const& theDate,
			 int theHour,
			 int theMin,
			 int theSec
			);
				// Sets the time to that defined
				// by <theHour>, <theMin> and <theSec>
				// within the <theDate>.

    OTC_Time&		operator=(OTC_Time const& theTime)
				{ setTime(theTime); return *this; }
				// Sets the time to that defined by
				// <theTime>.

    OTC_Time&		addSeconds(int theNum);
				// Adds <theNum> seconds to the time.

    OTC_Time&		subtractSeconds(int theNum)
				{ return addSeconds(-theNum); }
				// Subtracts <theNum> seconds from the time.

    OTC_Time&		addMinutes(int theNum);
				// Adds <theNum> minutes to the time.

    OTC_Time&		subtractMinutes(int theNum)
				{ return addMinutes(-theNum); }
				// Subtracts <theNum> minutes from the time.

    OTC_Time&		addHours(int theNum);
				// Adds <theNum> hours to the time.

    OTC_Time&		subtractHours(int theNum)
				{ return addHours(-theNum); }
				// Subtracts <theNum> hours from the time.

    OTC_Time&		addDuration(OTC_Duration const& theDuration);
				// Adds <theDuration> to the time.

    OTC_Time&		subtractDuration(OTC_Duration const& theDuration);
				// Subtracts <theDuration> from the time.

    // = COMPARISONS

    bool		operator==(OTC_Time const& theTime) const;
				// Returns <true> if this time is the same as
				// <theTime>.

    bool		operator!=(OTC_Time const& theTime) const;
				// Returns <true> if this time is not the
				// same as <theTime>.

    bool		operator<(OTC_Time const& theTime) const;
				// Returns <true> if this time comes before
				// <theTime>.

    bool		operator<=(OTC_Time const& theTime) const;
				// Returns <true> if this time comes before
				// or is the same as <theTime>.

    bool		operator>(OTC_Time const& theTime) const;
				// Returns <true> if this time comes after
				// <theTime>.

    bool		operator>=(OTC_Time const& theTime) const;
				// Returns <true> if this time comes after or
				// is the same as <theTime>.

    // = ARITHMETIC

    OTC_Time&		operator+=(OTC_Duration const& theDuration)
				{ return addDuration(theDuration); }
				// Adds <theDuration> to the time.

    OTC_Time&		operator-=(OTC_Duration const& theDuration)
				{ return addDuration(-theDuration); }
				// Subtracts <theDuration> from the time.

    OTC_Time		operator+(OTC_Duration const& theDuration) const
				{ return plusDuration(theDuration); }
				// Returns this time with <theDuration>
				// added.

    OTC_Time		operator-(OTC_Duration const& theDuration) const
				{ return plusDuration(-theDuration); }
				// Returns this time with <theDuration>
				// subtracted.

    OTC_Duration	operator-(OTC_Time const& theTime) const;
				// Returns the duration between this
				// time and <theTime>. Note that a negative
				// duration indicates that <theTime> is
				// in the future with respect to this time.

    OTC_Duration	operator-(OTC_Date const& theDate) const;
				// Returns the duration between this
				// time and <theDate>. Note that a negative
				// duration indicates that <theDate> is
				// in the future with respect to this time.

    // = RANKING

    int			rank(OTC_Time const& theTime) const;
				// Returns a value less than, equal to,
				// or greater than zero corresponding to
				// how this time compares to <theTime>.

    // = HASHING

    int			hash() const
				{ return OTC_Date::hash() ^ int(seconds_); }
				// Returns a hash value for this time.

  private:

    int			seconds_;
				// Number of seconds since midnight.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_RankActions<OTC_Time>
{
  public:
    static int		rank(OTC_Time const& theTime1, OTC_Time const& theTime2)
				{ return theTime1.rank(theTime2); }
};

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_HashActions<OTC_Time>
{
  public:
    static int		hash(OTC_Time const& theTime)
				{ return theTime.hash(); }
};

/* ------------------------------------------------------------------------- */

OSE_EXPORT ostream& operator<<(ostream& theStream, OTC_Time const& theTime);

/* ------------------------------------------------------------------------- */

#endif /* OTC_TYPES_TIME_HH */
