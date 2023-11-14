#ifndef OTC_TEXT_REGEX_HH
#define OTC_TEXT_REGEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/regex.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1992 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/thread/nrmutex.hh>
#include <OTC/collctn/range.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/regex.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Regex
    // = TITLE
    //	   Class for performing regular expression matches.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class can be used to determine if some string matches a
    //	   particular regular expression. In addition, information can be
    //	   obtained about the match, so that string substitutions can be
    //	   undertaken. The pattern style is like that for <ex> or <grep>.
    //	   
    // = NOTES
    //	   This uses the regex routines in v07i021 of volume 7
    //	   of comp.sources.misc.
{
  public:

			~OTC_Regex();

    // = INITIALISATION

			OTC_Regex(
			 char const* thePattern,
			 u_int theMaxSubPatterns=9
			);
                                // Compiles <thePattern>. <theMaxSubPatterns>
                                // changes the maximum number of sub patterns
                                // allowed in the pattern. The default number
                                // of permitted sub patterns "9". Note that
                                // if you try and set this to a value less
                                // than "9", it will be forcibly set to "9".
                                // Also, there is still a maximum you can
                                // set, that being "49".

    // = QUERY

    char const*		pattern() const
				{ return pattern_; }
				// Returns the pattern.

  protected:

    // = PATTERN COMPILATION

    void		compile()
				{ valid_ = re_comp(pattern()); }
				// Compiles the pattern.

  public:

    // = ERRORS

    bool		isValid() const;
				// Returns <true> if the pattern is valid.

    // = PATTERN MATCHING

    bool		match(char const* theString);
				// Returns <true> if <theString> matches the
				// most recently compiled pattern. If no
				// pattern had been compiled then <false> is
				// returned. If <theString> is <0> then
				// <false> is always returned.

    // The following functions return information about the area of
    // a string which a pattern matched with. A value of <0> for <theIndex>
    // indicates matches with the complete pattern. A value greater than
    // zero indicates matches with subpatterns as designated by the
    // <\(\)> notation.
    //
    // All functions return <0> if no match has occurred.

    u_int		start(u_int theIndex=0) const;
				// Returns the index into the string where
				// the match began for the tagged portion
				// indicated by <theIndex>.

    size_t		length(u_int theIndex=0) const;
				// Returns the length of the match for the
				// tagged portion indicated by <theIndex>.

    OTC_Range		range(u_int theIndex=0) const;
				// Returns a range object for the matched
				// tagged portion of the string indicated by
				// <theIndex>.

    // = CUSTOMISATION

    void		modw(char const* theString)
				{ re_modw(theString); }
				// The characters in the null terminated
				// string <theString> are added to the set of
				// characters identified as being in a word.
				// If <theString> is null or is zero length,
				// then the set of characters is reset to
				// the default.

    // = ERRORS

    char const*		error() const
				{ return errMesg_; }
				// Returns a error message describing why the
				// pattern compilation failed.

  protected:

    bool		re_comp(char const* pat);
				// Performs the actual compilation
				// of a pattern.

    int			re_exec(char const* lp);
				// Performs the actual matching of
				// a pattern against a string.

    void		re_modw(char const* s);
				// Performs the actual work of adding
				// characters to set the set of characters
				// identified as being in a word.

    int			re_subs(char const* src, char* dst);

    void		re_fail(char const* msg, char op);
				// Invoked when an internal error occurs
				// <re_exec()>. <msg> points to a description
				// of the error, and <op> indicates where in
				// the automaton the error occurred.

  private:

			OTC_Regex(OTC_Regex const&);

    OTC_Regex&		operator=(OTC_Regex const&);

    char*		pattern_;
    				// Copy of the pattern.

    int*		tagstk;
				// Sub-pattern tag stack.

    u_char*		dfa;
				// Automaton.

    bool		sta;
				// Set to <true> if there is a pattern
				// currently compiled.

    u_char*		bittab;
				// Bit table for CCL.

    void		chset(u_char c);
				// Sets info in bit table.

    char const*		pmatch(char const* lp, u_char* ap);
				// Does the hard work of determining if
				// a pattern matches.

    static char const	defchrtyp[];
				// Default classification table for word
				// boundary operators. A copy of this is
				// placed in <chrtyp> when the class is
				// constructed.

    static u_char const	deftab[];
				// Not sure.

    char*		chrtyp;
				// Holds the classification table for word
				// boundary operators. This can be modified
				// by using <re_modw()>.

    char const*		bol;
				// Points to the start of the string
				// the pattern is being matched against.

    char const**	bopat;
				// Array of pointers in to the string,
				// indicating where matched patterns
				// commenced.

    char const**	eopat;
				// Array of pointers in to the string,
				// indicating where matched patterns
				// ended.

    char const*		errMesg_;
				// Points to a message describing why a
				// pattern compilation failed.

    void		badpat(char const* theErrMesg);
				// Logs <theErrMesg> as being the
				// reason that a pattern compilation failed.
				// Also set <dfa[0]> to <END> indicating
				// that there is no pattern.

    bool		valid_;
				// Is <true> if pattern compiled correctly.

    u_int		maxSubPatterns_;
				// The maximum number of sub patterns
				// permitted plus one.

  public:

    // = BUILTIN REGULAR EXPRESSIONS
    //	   Some commonly used regular expressions are provided in
    //     precompiled form. When threads are being used, the only
    //     safe thing to do here is get the string representing the
    //     pattern as if two threads using the same class instance
    //     to match strings will cause problems.

    static OTC_Regex&	whiteSpace();
				// Matches white space.

    static OTC_Regex&	optWhiteSpace();
				// Optionally matches white space.

    static OTC_Regex&	nonWhiteSpace();
				// Matches non-white space.

    static OTC_Regex&	alphabetic();
				// Matches alpha characters.

    static OTC_Regex&	lowerCase();
				// Matches lower case characters.

    static OTC_Regex&	upperCase();
				// Matches upper case characters.

    static OTC_Regex&	alphaNumeric();
				// Matches alphanumeric characters.

    static OTC_Regex&	identifier();
				// Matches an identifier.

    static OTC_Regex&	matchingQuotes();
				// Matches string delineated with double
				// quotes. Note that this does not ignore
				// a quote preceded by a slosh, instead
				// that quote will be seen as the terminating
				// quote.

  private:

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static OTC_Regex*	whiteSpace_;
				// Matches white space.

    static OTC_Regex*	optWhiteSpace_;
				// Optionally matches white space.

    static OTC_Regex*	nonWhiteSpace_;
				// Matches non-white space.

    static OTC_Regex*	alphabetic_;
				// Matches alpha characters.

    static OTC_Regex*	lowerCase_;
				// Matches lower case characters.

    static OTC_Regex*	upperCase_;
				// Matches upper case characters.

    static OTC_Regex*	alphaNumeric_;
				// Matches alphanumeric characters.

    static OTC_Regex*	identifier_;
				// Matches an identifier.

    static OTC_Regex*	matchingQuotes_;
				// Matches string delineated with double
				// quotes.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_REGEX_HH */
