#ifndef OTC_TEXT_GLOBEX_HH
#define OTC_TEXT_GLOBEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/globex.hh
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

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/globex.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Globex
    // = TITLE
    //	   Class to perform shell style globbing on a string.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class provides the ability to determine if a string
    //	   matches a particular pattern. The pattern in this case is
    //	   in the form of a shell style globbing pattern.
    //
    // = FORMAT
    //	   In the pattern string:
    //
    // = BEGIN<INDENT>
    // = BEGIN<NOFILL>
    //	   `*' matches any sequence of characters
    //	   `?' matches any character
    //	   [SET] matches any character in the specified set,
    //	   [!SET] or [^SET] matches any character not in the specified set.
    // = END<NOFILL>
    // = END<INDENT>
    //
    //	   Note: the standard regex character '+' (one or more) should by
    //	   simulated by using "?*" which is equivalent here.
    //
    //	   A set is composed of characters or ranges; a range looks like
    //	   character hyphen character (as in 0-9 or A-Z). [0-9a-zA-Z_] is the
    //	   minimal set of characters allowed in the [..] pattern construct.
    //
    //	   To suppress the special syntactic significance of any of
    //	   `[]*?!^-\', and match the character exactly, precede it with a
    //	   `\'.
    //
    // = SOURCE
    //	   This class is based on public domain routines written by
    //	   J. Kercheval (johnk@wrq.com) and posted to alt.sources.
    //
    // = SEE ALSO
    //	   <OTC_Pattern>
{
  public:

			~OTC_Globex();

    // = INITIALISATION

			OTC_Globex(char const* thePattern);
				// Compiles <thePattern>.

    // = QUERY

    char const*		pattern() const
				{ return pattern_; }
				// Returns the pattern.

    // = MATCHING

    bool		match(char const* theString);
				// Returns <true> if the pattern matched
				// against <theString>. Note that if
				// <theString> is <0> then <false> will
				// always be returned.

    // = ERRORS

    bool		isValid() const;
				// Returns <true> if the pattern was valid.

    char const*		error() const;
				// If the pattern was not valid, returns a
				// messages describing what was wrong with
				// the pattern.

  protected:

    int			doMatch(
			 char const* thePattern,
			 char const* theString
			);
				// Performs match.

    int			doMatchStar(
			 char const* thePattern,
			 char const* theString
			);
				// Performs match.

    // = COMPILATION

    void		compile();
				// Compiles the pattern.

  private:

			OTC_Globex(OTC_Globex const&);

    OTC_Globex&		operator=(OTC_Globex const&);

    char*		pattern_;
				// The actual pattern.

    bool		valid_;
				// Set to <true> if the pattern is valid.

    char const*		error_;
				// If the pattern is not valid, contains
				// a string describing what is wrong.

    size_t		length_;
				// Contains the length of the matched
				// portion of the string.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_GLOBEX_HH */
