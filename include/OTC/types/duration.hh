#ifndef OTC_TYPES_DURATION_HH
#define OTC_TYPES_DURATION_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     TYPES/duration.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/hashactn.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/types/duration.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Duration
    // = TITLE
    //	   Maintains information about the duration between two times.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   Maintains information about the duration between two times.
    //	   Resolution is one second. Durations can be specified in terms
    //	   of days, hours, minutes and seconds.
{
  public:

    // = INITIALISATION

			OTC_Duration();
				// Creates a duration of zero length.

			OTC_Duration(
			 int theDays,
			 int theHours=0,
			 int theMinutes=0,
			 int theSeconds=0
			);
				// Creates a duration of length corresponding
				// to the arguments <theDays>, <theHours>,
				// <theMinutes> and <theSeconds>. The last
				// three arguments are optional. The actual
				// duration is calculated by adding the
				// values according to their respective
				// weights. For example <1> day and <-1> hour
				// will result in <23> hours.

                        OTC_Duration(char const* theDurationString);
				// Initialises the date to that described by
				// <theDurationString>. The only format for
				// <theDurationString> which is currently
				// understood is that described for dates in
				// ISO 8601:1988. If <theDurationString>
				// isn't in a valid format, an exception will
				// be raised.

			OTC_Duration(OTC_Duration const& theDuration);
				// Creates a duration of length
				// <theDuration>.

    OTC_Duration&	operator=(OTC_Duration const& theDuration)
				{ setDuration(theDuration); return *this; }
				// Sets the duration to <theDuration>.

    // = CLASS OPERATIONS

    static bool	  convertToDuration(
                         char const* theDurationString,
                         int& theDays,
                         int& theHours,
                         int& theMinutes,
                         int& theSeconds
                        );
				// Sets <theDays>, <theHours>, <theMinutes>
				// and <theSeconds> corresponding to the
				// duration described by the contents of
				// <theDurationString>. The only format for
				// <theDurationString> which is currently
				// understood is that described for dates in
				// ISO 8601:1988. <true> is returned
				// if string was in format which could be
				// parsed correctly, else <false> is
				// returned. The results passed back
				// in the reference arguments are the
				// integer values as they occur in the
				// string and have not been normalised.

    static bool	  isValidDuration(char const* theDateString);
				// Returns <true> if the contents of
				// <theDurationString> describe a valid
				// duration. The only format for
				// <theDuration> which is currently
				// understood is that described for durations
				// in ISO 8601:1988.

    // = QUERY

    bool		isZeroLength() const;
				// Returns <true> if the duration is of zero
				// length.

    bool		isPositiveDuration() const
				{ return sign_ > 0; }
				// Returns <true> if the duration is positive
				// in direction. A zero length duration is
				// take to be a duration in the positive
				// direction.

    bool		isNegativeDuration() const
				{ return sign_ < 0; }
				// Returns <true> if the duration is negative
				// in direction.

    int			sign() const
				{ return sign_; }
				// Returns the direction of the duration as
				// an integer. Value of <1> is returned if
				// duration is positive in direction and a
				// value of <-1> if the duration is negative
				// in duration.

    //	   Component parts of duration will always be positive values.
    //	   To determine if the duration is in the negative direction,
    //	   use the <sign()> member function.

    int			days() const
				{ return days_; }
				// Returns the day component of the duration.

    int			hours() const
				{ return hours_; }
				// Returns the hour component of the duration.

    int			minutes() const
				{ return minutes_; }
				// Returns the minute component of the
				// duration.

    int			seconds() const
				{ return seconds_; }
				// Returns the second component of the
				// duration.

    OTC_Duration	plusSeconds(int theNum) const
				{
				  OTC_Duration theDuration(0,0,0,theNum);
				  return plusDuration(theDuration);
				}
				// Returns this duration with <theNum>
				// seconds added.

    OTC_Duration	minusSeconds(int theNum) const
				{
				  OTC_Duration theDuration(0,0,0,theNum);
				  return minusDuration(theDuration);
				}
				// Returns this duration with <theNum>
				// seconds subtracted.

    OTC_Duration	plusMinutes(int theNum) const
				{
				  OTC_Duration theDuration(0,0,theNum,0);
				  return plusDuration(theDuration);
				}
				// Returns this duration with <theNum>
				// minutes added.

    OTC_Duration	minusMinutes(int theNum) const
				{
				  OTC_Duration theDuration(0,0,theNum,0);
				  return minusDuration(theDuration);
				}
				// Returns this duration with <theNum>
				// minutes subtracted.

    OTC_Duration	plusHours(int theNum) const
				{
				  OTC_Duration theDuration(0,theNum,0,0);
				  return plusDuration(theDuration);
				}
				// Returns this duration with <theNum> hours
				// added.

    OTC_Duration	minusHours(int theNum) const
				{
				  OTC_Duration theDuration(0,theNum,0,0);
				  return minusDuration(theDuration);
				}
				// Returns this duration with <theNum> hours
				// subtracted.

    OTC_Duration	plusDays(int theNum) const
				{
				  OTC_Duration theDuration(theNum,0,0,0);
				  return plusDuration(theDuration);
				}
				// Returns this duration with <theNum> days
				// added.

    OTC_Duration	minusDays(int theNum) const
				{
				  OTC_Duration theDuration(theNum,0,0,0);
				  return minusDuration(theDuration);
				}
				// Returns this duration with <theNum> days
				// subtracted.

    OTC_Duration	plusWeeks(int theNum) const
				{
				  OTC_Duration theDuration(7*theNum,0,0,0);
				  return plusDuration(theDuration);
				}
				// Returns this duration with <theNum> weks
				// added.

    OTC_Duration	minusWeeks(int theNum) const
				{
				  OTC_Duration theDuration(7*theNum,0,0,0);
				  return minusDuration(theDuration);
				}
				// Returns this duration with <theNum> weeks
				// subtracted.

    OTC_Duration	plusDuration(OTC_Duration const& theDuration) const;
				// Returns this duration with <theDuration>
				// added.

    OTC_Duration	minusDuration(OTC_Duration const& theDuration) const;
				// Returns this duration with <theDuration>
				// subtracted.

    OTC_Duration	plusDuration(
			 int theDays,
			 int theHours,
			 int theMinutes,
			 int theSeconds
			) const;
				// Returns this duration with the duration
				// corresponding to that defined by
				// <theDays>, <theHours>, <theMinutes> and
				// <theSeconds> added.

    OTC_Duration	minusDuration(
			 int theDays,
			 int theHours,
			 int theMinutes,
			 int theSeconds
			) const;
				// Returns this duration with the duration
				// corresponding to that defined by
				// <theDays>, <theHours>, <theMinutes> and
				// <theSeconds> subtracted.

    // = TEXTUAL REPRESENTATION

    OTC_String          asString() const;
				// Returns the duration as a string. The
				// format used for the duration when
				// converting it to a string, is that defined
				// by ISO 8601:1988.

#if 0

    friend OSE_EXPORT ostream&     operator<<(
				 ostream& theStream,
				 OTC_Duration const& theDuration
				);
				// Outputs the duration onto <theStream>. The
				// format used for the duration when
				// displaying it, is that defined by ISO
				// 8601:1988.

#endif

    // = MODIFIERS

    void		setDuration(OTC_Duration const& theDuration);
				// Sets this duration to <theDuration>.

    void		setDuration(
			 int theDays,
			 int theHours=0,
			 int theMinutes=0,
			 int theSeconds=0
			);
				// Sets the duration to a length
				// corresponding to the arguments <theDays>,
				// <theHours>, <theMinutes> and <theSeconds>.
				// The last three arguments are optional. The
				// actual duration is calculated by adding
				// the values according to their respective
				// weights. For example <1> day and <-1> hour
				// will result in <23> hours.

    OTC_Duration&	addSeconds(int theNum)
				{
				  OTC_Duration theDuration(0,0,0,theNum);
				  return addDuration(theDuration);
				}
				// Adds <theNum> seconds to this duration.

    OTC_Duration&	subtractSeconds(int theNum)
				{
				  OTC_Duration theDuration(0,0,0,theNum);
				  return subtractDuration(theDuration);
				}
				// Subtracts <theNum> seconds from this
				// duration.

    OTC_Duration&	addMinutes(int theNum)
				{
				  OTC_Duration theDuration(0,0,theNum,0);
				  return addDuration(theDuration);
				}
				// Adds <theNum> minutes to this duration.

    OTC_Duration&	subtractMinutes(int theNum)
				{
				  OTC_Duration theDuration(0,0,theNum,0);
				  return subtractDuration(theDuration);
				}
				// Subtracts <theNum> minutes from this
				// duration.

    OTC_Duration&	addHours(int theNum)
				{
				  OTC_Duration theDuration(0,theNum,0,0);
				  return addDuration(theDuration);
				}
				// Adds <theNum> hours to this duration.

    OTC_Duration&	subtractHours(int theNum)
				{
				  OTC_Duration theDuration(0,theNum,0,0);
				  return subtractDuration(theDuration);
				}
				// Subtracts <theNum> hours from this
				// duration.

    OTC_Duration&	addDays(int theNum)
				{
				  OTC_Duration theDuration(theNum,0,0,0);
				  return addDuration(theDuration);
				}
				// Adds <theNum> days to this duration.

    OTC_Duration&	subtractDays(int theNum)
				{
				  OTC_Duration theDuration(theNum,0,0,0);
				  return subtractDuration(theDuration);
				}
				// Subtracts <theNum> days from this
				// duration.

    OTC_Duration&	addWeeks(int theNum)
				{
				  OTC_Duration theDuration(7*theNum,0,0,0);
				  return addDuration(theDuration);
				}
				// Adds <theNum> weeks to this duration.

    OTC_Duration&	subtractWeeks(int theNum)
				{
				  OTC_Duration theDuration(7*theNum,0,0,0);
				  return subtractDuration(theDuration);
				}
				// Subtracts <theNum> weeks from this
				// duration.

    OTC_Duration&	addDuration(OTC_Duration const& theDuration);
				// Adds <theDuration> to this duration.

    OTC_Duration&	subtractDuration(OTC_Duration const& theDuration);
				// Subtracts <theDuration> from this duration.

    OTC_Duration&	addDuration(
			 int theDays,
			 int theHours,
			 int theMinutes,
			 int theSeconds
			);
				// Adds the duration corresponding to that
				// defined by <theDays>, <theHours>,
				// <theMinutes> and <theSeconds> to this
				// duration.

    OTC_Duration&	subtractDuration(
			 int theDays,
			 int theHours,
			 int theMinutes,
			 int theSeconds
			);
				// Subtracts the duration corresponding to
				// that defined by <theDays>, <theHours>,
				// <theMinutes> and <theSeconds> from this
				// duration.

    // = COMPARISONS
    //	   The following do not take into consideration the sign of the
    //	   duration. They compare the period of the duration only.

    bool		operator==(OTC_Duration const& theDuration) const;
				// Returns <true> if the period of this
				// duration is the same as that of
				// <theDuration>.

    bool		operator!=(OTC_Duration const& theDuration) const;
				// Returns <true> if the period of this
				// duration is not the same as that of
				// <theDuration>.

    bool		operator<=(OTC_Duration const& theDuration) const;
				// Returns <true> if the period of this
				// duration is less than or equal to that of
				// <theDuration>.

    bool		operator<(OTC_Duration const& theDuration) const;
				// Returns <true> if the period of this
				// duration is less than to that of
				// <theDuration>.

    bool		operator>=(OTC_Duration const& theDuration) const;
				// Returns <true> if the period of this
				// duration is greater than or equal to that
				// of <theDuration>.

    bool		operator>(OTC_Duration const& theDuration) const;
				// Returns <true> if the period of this
				// duration is greater than that of
				// <theDuration>.

    // = ARITHMETIC

    OTC_Duration	operator+(OTC_Duration const& theDuration) const
				{ return plusDuration(theDuration); }
				// Adds <theDuration> to this duration,
				// returning the result. This duration is not
				// modified.

    OTC_Duration&	operator+=(OTC_Duration const& theDuration)
				{ return addDuration(theDuration); }
				// Adds <theDuration> to this duration. The
				// modified value of this duration is
				// returned.

    OTC_Duration	operator-() const;
				// Returns a duration which is of the
				// opposite sign to this duration. This
				// duration is not modified.

    OTC_Duration	operator-(OTC_Duration const& theDuration) const
				{ return minusDuration(theDuration); }
				// Subtracts <theDuration> from this duration,
				// returning the result. This duration is
				// not modified.

    OTC_Duration&	operator-=(OTC_Duration const& theDuration)
				{ return subtractDuration(theDuration); }
				// Subtracts <theDuration> from this
				// duration. The modified value of this
				// duration is returned.

    OTC_Duration	operator*(int theNum) const;
				// Returns a duration which is <theNum>
				// times longer than this duration. This
				// duration is not modified.

#if 0

    friend OSE_EXPORT OTC_Duration operator*(
				 int theNum,
				 OTC_Duration const& theDuration
				)
				{ return theDuration * theNum; }
				// Returns a duration which is <theNum>
				// times longer than <theDuration>.
				// <theDuration> is not modified.

#endif

    OTC_Duration&	operator*=(int theNum);
				// Increases the length of this duration by
				// <theNum> times. The modified value of this
				// duration is returned.

    // The following return approximate results only.

    OTC_Duration	operator/(u_int theNum) const;
				// Divides this duration by the factor
				// <theNum>, returning the result. This
				// duration is not modified. Raises an
				// exception if <theNum> is <0>. Note that
				// <theNum> must be a positive value.

    OTC_Duration&	operator/=(u_int theNum);
				// Divides this duration by the factor
				// <theNum>. The modified value of this
				// duration is returned. Raises an exception
				// if <theNum> is <0>. Note that <theNum>
				// must be a positive value.

    int			operator/(OTC_Duration const& theDuration) const;
				// Returns how many times <theDuration> will
				// fit into this duration. The sign of the
				// duration is not considered. Raises an
				// exception if <theDuration> is of
				// zero length.

    // = RANKING

    int			rank(OTC_Duration const& theDuration) const;
				// Returns a value less than, equal to,
				// or greater than zero corresponding to
				// how this date compares to <theDuration>.

    // = HASHING

    int			hash() const;
				// Returns a hash value for this date.

  private:

    void		normalise();
				// Normalises the various components of the
				// duration such that they are all positive
				// and the sign bit set correspondingly.

    int			sign_;
				// The sign of the duration. Has value <1> if
				// duration is in positive direction. Has
				// value <-1> if duration is in negative
				// direction.

    int			days_;
				// The day component of the duration.

    int			hours_;
				// The hour component of the duration.

    int			minutes_;
				// The minute component of the duration.

    int			seconds_;
				// The second component of the duration.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_RankActions<OTC_Duration>
{
  public:
    static int		rank(
			 OTC_Duration const& theDuration1,
			 OTC_Duration const& theDuration2
			)
				{ return theDuration1.rank(theDuration2); }
};

OSE_TEMPLATE_SPECIALISATION
class OTC_HashActions<OTC_Duration>
{
  public:
    static int		hash(OTC_Duration const& theDuration)
				{ return theDuration.hash(); }
};

/* ------------------------------------------------------------------------- */

OSE_EXPORT ostream& operator<<(
 ostream& theStream,
 OTC_Duration const& theDuration
);

inline OTC_Duration operator*(
 int theNum,
 OTC_Duration const& theDuration
)
{ return theDuration * theNum; }

/* ------------------------------------------------------------------------- */

#endif /* OTC_TYPES_DURATION_HH */
