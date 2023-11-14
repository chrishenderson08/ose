#ifndef OTC_TEXT_PATTERN_HH
#define OTC_TEXT_PATTERN_HH

//! \file OTC/text/pattern.hh
//! \ingroup text_manipulation
//!
//! \brief Support for pattern matching using regular expressions.

//! This file is part of the OSE C++ class library.
//!
//! Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/pattern.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

//! \enum OTC_PatternType
//!
//! Specifies the type of regular expression.

enum OTC_PatternType
{
  OTCLIB_REGEX_BASIC = 0,	///< Compile obsolete (basic) REs.
  OTCLIB_REGEX_EXTENDED = 1	///< Compile modern (extended) REs.
};

//! \enum OTC_CompileOptions
//!
//! Specifies options when compiling a regular expression.

enum OTC_CompileOptions
{
  OTCLIB_REGEX_IGNORECASE = 2,	///< Ignore case when matching.
  OTCLIB_REGEX_MULTILINE = 8	///< Newline-sensitive matching.
};

//! \enum OTC_MatchOptions
//!
//! Specifies options when matching against a regular expression.

enum OTC_MatchOptions
{
  OTCLIB_REGEX_NOTBOL = 1,	///< Start of string doesn't match BOL.
  OTCLIB_REGEX_NOTEOL = 2	///< End of string doesn't match EOL.
};

struct OTC_CompiledPattern;
struct OTC_MatchedPattern;

//! \class OTC_Pattern pattern.hh OTC/text/pattern.hh 
//! \ingroup text_manipulation
//!
//! \brief Pattern matching class utilising regular expressions.
//!
//! The \c OTC_Pattern class provides a wrapper around the core functionality
//! of the most recent version of Henry Spencer's implementation of the
//! regular expression routines specified in the POSIX 1003.2 standard.
//!
//! Instead of instantiating instances of this class, the derived classes \c
//! OTC_BasicRegex and \c OTC_ExtendedRegex should be used. This class and
//! specifically its derivaties should be used in preference to \c OTC_Regex
//! and \c OTC_Regexp, these latter classes having being deprecated.
//!
//! \see OTC_BasicRegex, OTC_ExtendedRegex
//!
//! \author Graham Dumpleton

class OSE_EXPORT OTC_Pattern
{
  public:

    			~OTC_Pattern();

    //! \name Pattern Construction

    //@{

			OTC_Pattern(
			 OTC_PatternType theType,
			 int theFlags,
			 char const* thePattern
			);
				///< Creates a pattern matching object for \a
				///< thePattern where \a theType indicates
				///< either basic or extended regular syntax
				///< is used. \a theFlags indicates
				///< additional properties which should be
				///< compiled into the pattern matcher. \a
				///< theFlags should be constructed by
				///< bitwise OR of values defined by the
				///< enum \c OTC_CompileOptions or left as 0.

			OTC_Pattern(
			 OTC_PatternType theType,
			 int theFlags,
			 char const* thePattern,
			 size_t theLength
			);
				///< Creates a pattern matching object for \a
				///< thePattern of \a theLength where \a
				///< theType indicates either basic or
				///< extended regular syntax is used. \a
				///< theFlags indicates additional
				///< properties which should be compiled into
				///< the pattern matcher. \a theFlags should
				///< be constructed by bitwise OR of values
				///< defined by the enum \c OTC_CompileOptions
				///< or left as 0. As the length of the
				///< pattern is defined explicitly, the
				///< pattern may contain embedded null
				///< characters.

			OTC_Pattern(
			 OTC_PatternType theType,
			 int theFlags,
			 OTC_String const& thePattern
			);
				///< Creates a pattern matching object for \a
				///< thePattern where \a theType indicates
				///< either basic or extended regular syntax
				///< is used. \a theFlags indicates
				///< additional properties which should be
				///< compiled into the pattern matcher. \a
				///< theFlags should be constructed by
				///< bitwise OR of values defined by the
				///< enum \c OTC_CompileOptions or left as 0.
				///< As the pattern is defined by an instance
				///< of \c OTC_String, the pattern may contain
				///< embedded null characters.

    //@}

    //! \name Pattern Compilation

    //@{

    bool		isValid() const
    				{ return regex_ != 0; }
				///< Returns \c true if the pattern was able
				///< to be compiled.

    char const*		error() const
    				{ return error_ ? error_ : ""; }
				///< Returns details about the last error to
				///< have occurred. This will either be that
				///< which pertains to the original
				///< compilation of the regular expression, or
				///< a problem when matching against the
				///< pattern. Will return an empty string if
				///< the last operation didn't result in an
				///< error.

    //@}

    //! \name Pattern Match

    //@{

    bool		match(
    			 int theFlags,
			 char const* theString,
			 size_t theLength
			);
				///< Returns \c true if \a theString of \a
				///< theLength matches the compiled pattern.
				///< \a theFlags indicates options which
				///< affect the matching process. \a theFlags
				///< should be constructed by bitwise OR of
				///< values defined by the enum \c
				///< OTC_MatchOptions or left as 0. As the
				///< length of the string is defined
				///< explicitly, the string may contain
				///< embedded null characters.

    bool		match(
			 char const* theString,
			 size_t theLength
			)
				{ return match(0,theString,theLength); }
				///< Returns \c true if \a theString of \a
				///< theLength matches the compiled pattern.
				///< As the length of the string is defined
				///< explicitly, the string may contain
				///< embedded null characters.

    bool		match(int theFlags, char const* theString);
				///< Returns \c true if \a theString matches
				///< the compiled pattern. \a theFlags
				///< indicates options which affect the
				///< matching process. \a theFlags should be
				///< constructed by bitwise OR of values
				///< defined by the enum \c OTC_MatchOptions
				///< or left as 0.

    bool		match(char const* theString)
    				{ return match(0,theString); }
				///< Returns \c true if \a theString matches
				///< the compiled pattern.

    bool		match(int theFlags, OTC_String const& theString)
    				{
				  return match(theFlags,
				   theString.string(),theString.length());
				}
				///< Returns \c true if \a theString matches
				///< the compiled pattern. \a theFlags
				///< indicates options which affect the
				///< matching process. \a theFlags should be
				///< constructed by bitwise OR of values
				///< defined by the enum \c OTC_MatchOptions
				///< or left as 0. As the string is specified
				///< using \c OTC_String, the string may
				///< contain embedded null characters.

    bool		match(OTC_String const& theString)
    				{
				  return match(0,theString.string(),
				   theString.length());
				}
				///< Returns \c true if \a theString matches
				///< the compiled pattern. As the string is
				///< specified using \c OTC_String, the string
				///< may contain embedded null characters.

    //@}

    //! \name Sub Patterns

    //@{

    u_int		count() const;
				///< Returns the number of sub patterns which
				///< occurred in the

    int			start(u_int theIndex=0) const;
				///< Returns the index into the string where
				///< the match began for the tagged sub
				///< pattern indicated by \a theIndex. Returns
				///< -1 if the corresponding sub pattern
				///< didn't match. A value of 0 for \a
				///< theIndex actually returns the index into
				///< the string where any match of the whole
				///< pattern began.

    size_t		length(u_int theIndex=0) const;
				///< Returns the length of the match for the
				///< tagged sub pattern indicated by \a
				///< theIndex.

    OTC_Range		range(u_int theIndex=0) const;
				///< Returns a range object representing the
				///< portion of the string that matched for
				///< the tagged sub pattern indicated by \a
				///< theIndex.

    //@}

  private:

			OTC_Pattern(OTC_Pattern const&);
				///< Do not define an implementation for this.

    OTC_Pattern&	operator=(OTC_Pattern const&);
				///< Do not define an implementation for this.

    OTC_CompiledPattern*	regex_;
				///< The compiled regular expression matcher.

    OTC_MatchedPattern*	match_;
				///< Details character ranges within string
				///< that patterns and sub patterns matched in
				///< last attempted pattern match.

    char*		error_;
				///< Details of the last error which occurred.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_PATTERN_HH */
