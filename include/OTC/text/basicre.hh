#ifndef OTC_TEXT_BASICRE_HH
#define OTC_TEXT_BASICRE_HH

//! \file OTC/text/basicre.hh
//! \ingroup text_manipulation
//!
//! \brief Support for pattern matching using basic regular expressions.

//! This file is part of the OSE C++ class library.
//!
//! Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

#include <OTC/text/pattern.hh>

/* ------------------------------------------------------------------------- */

//! \class OTC_BasicRegex basicre.hh OTC/text/basicre.hh 
//! \ingroup text_manipulation
//!
//! \brief Pattern matching class utilising basic regular expressions.
//!
//! \see OTC_Pattern, OTC_ExtendedRegex
//!
//! \author Graham Dumpleton

class OSE_EXPORT OTC_BasicRegex : public OTC_Pattern
{
  public:

    //! \name Pattern Construction

    //@{

			OTC_BasicRegex(
			 int theFlags,
			 char const* thePattern
			)
			  : OTC_Pattern(OTCLIB_REGEX_BASIC,
			     theFlags,thePattern) {}
				///< Creates a pattern matching object for \a
				///< thePattern where the basic regular
				///< expression syntax is used. \a theFlags
				///< indicates additional properties which
				///< should be compiled into the pattern
				///< matcher. \a theFlags should be
				///< constructed by bitwise OR of values
				///< defined by the enum \c OTC_CompileOptions
				///< or left as 0.

			OTC_BasicRegex(
			 int theFlags,
			 char const* thePattern,
			 size_t theLength
			)
			  : OTC_Pattern(OTCLIB_REGEX_BASIC,
			     theFlags,thePattern,theLength) {}
				///< Creates a pattern matching object for \a
				///< thePattern of \a theLength where the
				///< basic regular expression syntax is used.
				///< \a theFlags indicates additional
				///< properties which should be compiled into
				///< the pattern matcher. \a theFlags should
				///< be constructed by bitwise OR of values
				///< defined by the enum \c OTC_CompileOptions
				///< or left as 0. As the length of the
				///< pattern is defined explicitly, the
				///< pattern may contain embedded null
				///< characters.

			OTC_BasicRegex(
			 int theFlags,
			 OTC_String const& thePattern
			)
			  : OTC_Pattern(OTCLIB_REGEX_BASIC,
			     theFlags,thePattern) {}
				///< Creates a pattern matching object for \a
				///< thePattern where the basic regular
				///< expression syntax is used. \a theFlags
				///< indicates additional properties which
				///< should be compiled into the pattern
				///< matcher. \a theFlags should be
				///< constructed by bitwise OR of values
				///< defined by the enum \c OTC_CompileOptions
				///< or left as 0. As the pattern is defined
				///< by an instance of \c OTC_String, the
				///< pattern may contain embedded null
				///< characters.

			OTC_BasicRegex(char const* thePattern)
			  : OTC_Pattern(OTCLIB_REGEX_BASIC,0,thePattern) {}
				///< Creates a pattern matching object for \a
				///< thePattern where the basic regular
				///< expression syntax is used.

			OTC_BasicRegex(
			 char const* thePattern,
			 size_t theLength
			)
			  : OTC_Pattern(OTCLIB_REGEX_BASIC,
			     0,thePattern,theLength) {}
				///< Creates a pattern matching object for \a
				///< thePattern of \a theLength where the
				///< basic regular expression syntax is used.
				///< As the length of the pattern is defined
				///< explicitly, the pattern may contain
				///< embedded null characters.

			OTC_BasicRegex(OTC_String const& thePattern)
			  : OTC_Pattern(OTCLIB_REGEX_BASIC,
			     theFlags,thePattern) {}
				///< Creates a pattern matching object for \a
				///< thePattern where the basic regular
				///< expression syntax is used. As the pattern
				///< is defined by an instance of \c
				///< OTC_String, the pattern may contain
				///< embedded null characters.

    //@}

  private:

			OTC_BasicRegex(OTC_BasicRegex const&);
				///< Do not define an implementation for this.

    OTC_BasicRegex&	operator=(OTC_BasicRegex const&);
				///< Do not define an implementation for this.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_BASICRE_HH */
