#ifndef OTC_TEXT_EXTENDRE_HH
#define OTC_TEXT_EXTENDRE_HH

//! \file OTC/text/extendre.hh
//! \ingroup text_manipulation
//!
//! \brief Support for pattern matching using extended regular expressions.

//! This file is part of the OSE C++ class library.
//!
//! Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

#include <OTC/text/pattern.hh>

/* ------------------------------------------------------------------------- */

//! \class OTC_ExtendedRegex extendre.hh OTC/text/extendre.hh 
//! \ingroup text_manipulation
//!
//! \brief Pattern matching class utilising extended regular expressions.
//!
//! \see OTC_Pattern, OTC_BasicRegex
//!
//! \author Graham Dumpleton

class OSE_EXPORT OTC_ExtendedRegex : public OTC_Pattern
{
  public:

    //! \name Pattern Construction

    //@{

			OTC_ExtendedRegex(
			 int theFlags,
			 char const* thePattern
			)
			  : OTC_Pattern(OTCLIB_REGEX_EXTENDED,
			     theFlags,thePattern) {}
				///< Creates a pattern matching object for \a
				///< thePattern where the extended regular
				///< expression syntax is used. \a theFlags
				///< indicates additional properties which
				///< should be compiled into the pattern
				///< matcher. \a theFlags should be
				///< constructed by bitwise OR of values
				///< defined by the enum \c OTC_CompileOptions
				///< or left as 0.

			OTC_ExtendedRegex(
			 int theFlags,
			 char const* thePattern,
			 size_t theLength
			)
			  : OTC_Pattern(OTCLIB_REGEX_EXTENDED,
			     theFlags,thePattern,theLength) {}
				///< Creates a pattern matching object for \a
				///< thePattern of \a theLength where the
				///< extended regular expression syntax is
				///< used. \a theFlags indicates additional
				///< properties which should be compiled into
				///< the pattern matcher. \a theFlags should
				///< be constructed by bitwise OR of values
				///< defined by the enum \c OTC_CompileOptions
				///< or left as 0. As the length of the
				///< pattern is defined explicitly, the
				///< pattern may contain embedded null
				///< characters.

			OTC_ExtendedRegex(
			 int theFlags,
			 OTC_String const& thePattern
			)
			  : OTC_Pattern(OTCLIB_REGEX_EXTENDED,
			     theFlags,thePattern) {}
				///< Creates a pattern matching object for \a
				///< thePattern where the extended regular
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

			OTC_ExtendedRegex(char const* thePattern)
			  : OTC_Pattern(OTCLIB_REGEX_EXTENDED,0,thePattern) {}
				///< Creates a pattern matching object for \a
				///< thePattern where the extended regular
				///< expression syntax is used.

			OTC_ExtendedRegex(
			 char const* thePattern,
			 u_int theLength
			)
			  : OTC_Pattern(OTCLIB_REGEX_EXTENDED,
			     0,thePattern,theLength) {}
				///< Creates a pattern matching object for \a
				///< thePattern of \a theLength where the
				///< extended regular expression syntax is
				///< used. As the length of the pattern is
				///< defined explicitly, the pattern may
				///< contain embedded null characters.

			OTC_ExtendedRegex(OTC_String const& thePattern)
			  : OTC_Pattern(OTCLIB_REGEX_EXTENDED,0,thePattern) {}
				///< Creates a pattern matching object for \a
				///< thePattern where the extended regular
				///< expression syntax is used. As the pattern
				///< is defined by an instance of \c
				///< OTC_String, the pattern may contain
				///< embedded null characters.

    //@}

  private:

			OTC_ExtendedRegex(OTC_ExtendedRegex const&);
				///< Do not define an implementation for this.

    OTC_ExtendedRegex&	operator=(OTC_ExtendedRegex const&);
				///< Do not define an implementation for this.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_EXTENDRE_HH */
