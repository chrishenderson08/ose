/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     types/otcduration.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/types/duration.hh"
#endif
#endif

#include <OTC/types/duration.hh>

#include <OTC/stdlib/iostream.h>
#include <OTC/stdlib/strstream.h>

#include <string.h>

/* ------------------------------------------------------------------------- */
OTC_Duration::OTC_Duration()
  : sign_(1),
    days_(0),
    hours_(0),
    minutes_(0),
    seconds_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Duration::OTC_Duration(
 int theDays,
 int theHours,
 int theMinutes,
 int theSeconds
)
  : sign_(1),
    days_(theDays),
    hours_(theHours),
    minutes_(theMinutes),
    seconds_(theSeconds)
{
  normalise();
}

/* ------------------------------------------------------------------------- */
OTC_Duration::OTC_Duration(char const* theDurationString)
  : sign_(1),
    days_(0),
    hours_(0),
    minutes_(0),
    seconds_(0)
{

  bool theResult;
  theResult = convertToDuration(theDurationString,
   days_,hours_,minutes_,seconds_);

  OTCLIB_ENSURE_FN((theResult != false),
   "OTC_Duration::OTC_Duration(char const*)",
   "invalid format for duration");

  normalise();
}

/* ------------------------------------------------------------------------- */
OTC_Duration::OTC_Duration(OTC_Duration const& theDuration)
  : sign_(theDuration.sign_),
    days_(theDuration.days_),
    hours_(theDuration.hours_),
    minutes_(theDuration.minutes_),
    seconds_(theDuration.seconds_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::convertToDuration(
 char const* theDurationString,
 int& theDays,
 int& theHours,
 int& theMinutes,
 int& theSeconds
)
{
  if (theDurationString == 0)
  {
    theDays = theHours = theMinutes = theSeconds = 0;

    return false;
  }

  int theResult;

  theResult = sscanf(theDurationString,
   "-P%uDT%uH%uM%uS",&theDays,&theHours,&theMinutes,&theSeconds);

  if (theResult == 4)
  {
    theDays = -theDays;
    theHours = -theHours;
    theMinutes = -theMinutes;
    theSeconds = -theSeconds;

    return true;
  }

  theResult = sscanf(theDurationString,
   "P%uDT%uH%uM%uS",&theDays,&theHours,&theMinutes,&theSeconds);

  return (theResult == 4);
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::isValidDuration(char const* theDurationString)
{
  int theDays = 0;
  int theHours = 0;
  int theMinutes = 0;
  int theSeconds = 0;

  return convertToDuration(theDurationString,
   theDays,theHours,theMinutes,theSeconds);
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::isZeroLength() const
{
  return !days_ && !hours_ && !minutes_ && !seconds_;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Duration::plusDuration(OTC_Duration const& theDuration) const
{
  OTC_Duration tmpDuration(*this);
  tmpDuration.addDuration(theDuration);
  return tmpDuration;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Duration::minusDuration(OTC_Duration const& theDuration) const
{
  OTC_Duration tmpDuration(*this);
  tmpDuration.subtractDuration(theDuration);
  return tmpDuration;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Duration::plusDuration(
 int theDays,
 int theHours,
 int theMinutes,
 int theSeconds
) const
{
  OTC_Duration tmpDuration(*this);
  tmpDuration.addDuration(theDays,theHours,theMinutes,theSeconds);
  return tmpDuration;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Duration::minusDuration(
 int theDays,
 int theHours,
 int theMinutes,
 int theSeconds
) const
{
  OTC_Duration tmpDuration(*this);
  tmpDuration.subtractDuration(theDays,theHours,theMinutes,theSeconds);
  return tmpDuration;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Duration::asString() const
{
  char buf[256];
  ostrstream theStream(buf,sizeof(buf));

  theStream << *this << ends;

  return OTC_String(buf);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& operator<<(
 ostream& theStream,
 OTC_Duration const& theDuration
)
{
  if (theDuration.isNegativeDuration())
    theStream << "-";
  theStream << 'P';
  theStream << theDuration.days() << "DT";
  theStream << theDuration.hours() << 'H';
  theStream << theDuration.minutes() << 'M';
  theStream << theDuration.seconds() << 'S';

  return theStream;
}

/* ------------------------------------------------------------------------- */
void OTC_Duration::setDuration(OTC_Duration const& theDuration)
{
  if (this != &theDuration)
  {
    sign_ = theDuration.sign_;
    days_ = theDuration.days_;
    hours_ = theDuration.hours_;
    minutes_ = theDuration.minutes_;
    seconds_ = theDuration.seconds_;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Duration::setDuration(
 int theDays,
 int theHours,
 int theMinutes,
 int theSeconds
)
{
  days_ = theDays;
  hours_ = theHours;
  minutes_ = theMinutes;
  seconds_ = theSeconds;

  normalise();
}

/* ------------------------------------------------------------------------- */
OTC_Duration& OTC_Duration::addDuration(OTC_Duration const& theDuration)
{
  int theSign = theDuration.sign_;

  days_ = (sign_ * days_) + (theSign * theDuration.days_);
  hours_ = (sign_ * hours_) + (theSign * theDuration.hours_);
  minutes_ = (sign_ * minutes_) + (theSign * theDuration.minutes_);
  seconds_ = (sign_ * seconds_) + (theSign * theDuration.seconds_);

  normalise();

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_Duration& OTC_Duration::subtractDuration(OTC_Duration const& theDuration)
{
  int theSign = theDuration.sign_;

  days_ = (sign_ * days_) - (theSign * theDuration.days_);
  hours_ = (sign_ * hours_) - (theSign * theDuration.hours_);
  minutes_ = (sign_ * minutes_) - (theSign * theDuration.minutes_);
  seconds_ = (sign_ * seconds_) - (theSign * theDuration.seconds_);

  normalise();

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_Duration& OTC_Duration::addDuration(
 int theDays,
 int theHours,
 int theMinutes,
 int theSeconds
)
{
  OTC_Duration tmpDuration(theDays,theHours,theMinutes,theSeconds);
  return addDuration(tmpDuration);
}

/* ------------------------------------------------------------------------- */
OTC_Duration& OTC_Duration::subtractDuration(
 int theDays,
 int theHours,
 int theMinutes,
 int theSeconds
)
{
  OTC_Duration tmpDuration(theDays,theHours,theMinutes,theSeconds);
  return subtractDuration(tmpDuration);
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::operator==(OTC_Duration const& theDuration) const
{
  return (days_ == theDuration.days_) &&
	 (hours_ == theDuration.hours_) &&
	 (minutes_ == theDuration.minutes_) &&
	 (seconds_ == theDuration.seconds_);
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::operator!=(OTC_Duration const& theDuration) const
{
  return !OTC_Duration::operator==(theDuration);
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::operator<=(OTC_Duration const& theDuration) const
{
  return !OTC_Duration::operator>(theDuration);
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::operator<(OTC_Duration const& theDuration) const
{
  if (days_ < theDuration.days_)
    return true;

  else if (days_ > theDuration.days_)
    return false;

  if (hours_ < theDuration.hours_)
    return true;

  else if (hours_ > theDuration.hours_)
    return false;

  if (minutes_ < theDuration.minutes_)
    return true;

  else if (minutes_ > theDuration.minutes_)
    return false;

  if (seconds_ < theDuration.seconds_)
    return true;

  return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::operator>=(OTC_Duration const& theDuration) const
{
  return !OTC_Duration::operator<(theDuration);
}

/* ------------------------------------------------------------------------- */
bool OTC_Duration::operator>(OTC_Duration const& theDuration) const
{
  if (days_ > theDuration.days_)
    return true;

  else if (days_ < theDuration.days_)
    return false;

  if (hours_ > theDuration.hours_)
    return true;

  else if (hours_ < theDuration.hours_)
    return false;

  if (minutes_ > theDuration.minutes_)
    return true;

  else if (minutes_ < theDuration.minutes_)
    return false;

  if (seconds_ > theDuration.seconds_)
    return true;

  return false;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Duration::operator-() const
{
  OTC_Duration tmpDuration(*this);
  tmpDuration.sign_ = -tmpDuration.sign_;
  return tmpDuration;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Duration::operator*(int theNum) const
{
  OTC_Duration tmpDuration(*this);
  tmpDuration *= theNum;
  return tmpDuration;
}

/* ------------------------------------------------------------------------- */
OTC_Duration& OTC_Duration::operator*=(int theNum)
{
  days_ *= (sign_ * theNum);
  hours_ *= (sign_ * theNum);
  minutes_ *= (sign_ * theNum);
  seconds_ *= (sign_ * theNum);

  normalise();

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_Duration OTC_Duration::operator/(u_int theNum) const
{
  OTC_Duration tmpDuration(*this);
  tmpDuration /= theNum;
  return tmpDuration;
}

/* ------------------------------------------------------------------------- */
OTC_Duration& OTC_Duration::operator/=(u_int theNum)
{
  OTCLIB_ENSURE_FN((theNum != 0),
   "OTC_Duration::operator/=(u_int)",
   "zero divisor");

  double theRes;

  theRes = (double)seconds_ / theNum;

  seconds_ = int(theRes);

  theRes = (double)minutes_ / theNum;

  minutes_ = int(theRes);
  seconds_ += int((theRes - int(theRes)) * 60);

  theRes = (double)hours_ / theNum;

  hours_ = int(theRes);
  minutes_ += int((theRes - int(theRes)) * 60);

  theRes = (double)days_ / theNum;

  days_ = int(theRes);
  hours_ += int((theRes - int(theRes)) * 24);

  normalise();

  return *this;
}

/* ------------------------------------------------------------------------- */
int OTC_Duration::operator/(OTC_Duration const& theDuration) const
{
  OTCLIB_ENSURE_FN((!theDuration.isZeroLength()),
   "OTC_Duration::operator/(OTC_Duration const&)",
   "zero length duration");

  int theExtra = 0;

  if (theDuration.days_ != 0)
    return days_ / theDuration.days_;

  if (days_ != 0)
    theExtra += (24 * days_);

  if (theDuration.hours_ != 0)
    return (hours_ + theExtra) / theDuration.hours_;

  theExtra *= 60;
  if (hours_ != 0)
    theExtra += (60 * hours_);

  if (theDuration.minutes_ != 0)
    return (minutes_ + theExtra) / theDuration.minutes_;

  theExtra *= 60;
  if (minutes_ != 0)
    theExtra += (60 * minutes_);

  return (seconds_ + theExtra) / theDuration.seconds_;
}

/* ------------------------------------------------------------------------- */
void OTC_Duration::normalise()
{
  int theDiv;

  // Default sign.

  sign_ = 1;

  // Bring values within range.
  // Values may still be negative.

  theDiv = seconds_ / 60;
  seconds_ %= 60;
  minutes_ += theDiv;

  theDiv = minutes_ / 60;
  minutes_ %= 60;
  hours_ += theDiv;

  theDiv = hours_ / 24;
  hours_ %= 24;
  days_ += theDiv;

  if (days_ || hours_ || minutes_ || seconds_)
  {
    // Not all zero.

    if (days_ <= 0 && hours_ <= 0 && minutes_ <= 0 && seconds_ <= 0)
    {
      // All negative or zero.

      sign_ = -1;
      days_ = -days_;
      hours_ = -hours_;
      minutes_ = -minutes_;
      seconds_ = -seconds_;
    }
    else if (days_ < 0 || hours_ < 0 || minutes_ < 0 || seconds_ < 0)
    {
      // Mixture of negative and positive.

      if (seconds_ < 0)
      {
	minutes_ -= 1;
	seconds_ += 60;
      }
      if (minutes_ < 0)
      {
	hours_ -= 1;
	minutes_ += 60;
      }
      if (hours_ < 0)
      {
	days_ -= 1;
	hours_ += 24;
      }

      if (days_ < 0)
      {
	// Negative days, other positive.
	// Need to convert to all negative.

	days_ += 1;
	hours_ -= 24;

	if (minutes_ > 0 || seconds_ > 0)
	{
	  hours_ += 1;
	  minutes_ -= 60;

	  if (seconds_ > 0)
	  {
	    minutes_ += 1;
	    seconds_ -= 60;
	  }
	}

	sign_ = -1;

	days_ = -days_;
	hours_ = -hours_;
	minutes_ = -minutes_;
	seconds_ = -seconds_;
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
int OTC_Duration::rank(OTC_Duration const& theDuration) const
{
  int theRank;

  theRank = days_ - theDuration.days_;

  if (theRank != 0)
    return theRank;

  theRank = hours_ - theDuration.hours_;

  if (theRank != 0)
    return theRank;

  theRank = minutes_ - theDuration.minutes_;

  if (theRank != 0)
    return theRank;

  theRank = seconds_ - theDuration.seconds_;

  return theRank;
}

/* ------------------------------------------------------------------------- */
int OTC_Duration::hash() const
{
  return days_ ^ hours_ ^ minutes_ ^ seconds_;
}

/* ------------------------------------------------------------------------- */
