#ifndef OTC_TEXT_REGEXP_HH
#define OTC_TEXT_REGEXP_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/regexp.hh
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

#include <OTC/thread/nrmutex.hh>
#include <OTC/collctn/range.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/regexp.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

struct OSE_EXPORT OTC_RegexpStruct
{
  public:

    char const**	startp;
    char const**	endp;
    char		regstart;
    char		reganch;
    char*		regmust;
    int			regmlen;
    char*		program;
};

class OSE_EXPORT OTC_Regexp
    // = TITLE
    //	   Class to perform <egrep> style pattern matching.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     This class can be used to determine if some string matches a
    //     particular regular expression. In addition, information can be
    //     obtained about the match, so that string substitutions can be
    //     undertaken. The pattern style is like that for <egrep>.
    //
    // = SEE ALSO
    //     <OTC_Pattern>
{
  private:

    char const*		regparse;

    int			regnpar;

    char		regdummy;

    char*		regcode;

    long		regsize;

    char const*		reginput;

    char const*		regbol;

    char const**	regstartp;

    char const**	regendp;

    OTC_RegexpStruct*	regcomp(char const*);

    int			regexec(OTC_RegexpStruct*, char const*);

    void		regerror(char const* mesg)
				{ error_ = mesg; }

    char*		reg(int, int*);

    char*		regbranch(int*);

    char*		regpiece(int*);

    char*		regatom(int*);

    char*		regnode(char);

    char*		regnext(char*);

    void		regc(char);

    void		reginsert(char, char*);

    void		regtail(char*, char*);

    void		regoptail(char*, char*);

    int			regtry(OTC_RegexpStruct*, char const*);

    int			regmatch(char*);

    int			regrepeat(char*);

  public:

			~OTC_Regexp();

    // = CONSTRUCTION

			OTC_Regexp(
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

    // = MATCHING

    bool		match(char const* theString);
				// Returns <true> if the pattern
				// matched against <theString>. If <theString>
				// is <0> then <false> is always returned.

    u_int		start(u_int theIndex=0) const;
				// Returns the index of the first character
				// in the tagged portion of the matched
				// string indicated by <theIndex>.

    size_t		length(u_int theIndex=0) const;
				// Returns the length of the matched tagged
				// portion of the string indicated by
				// <theIndex>.

    OTC_Range		range(u_int theIndex=0) const;
				// Returns a range object for the matched
				// tagged portion of the string indicated by
				// <theIndex>.

    // = ERRORS

    bool		isValid() const;
				// Returns <true> if the pattern was
				// valid.

    char const*		error() const
				{ return error_; }
				// If the pattern was not valid, returns a
				// string describing the problem with the
				// pattern.

    // = PATTERNS
    //	   Some commonly used regular expression are defined as constant
    //	   character strings. These are:

    static OTC_Regexp&	whiteSpace();
				// Matches white space.

    static OTC_Regexp&	optWhiteSpace();
				// Optionally matches white space.

    static OTC_Regexp&	nonWhiteSpace();
				// Matches non-white space.

    static OTC_Regexp&	integerValue();
				// Matches integer number.

    static OTC_Regexp&	realValue();
				// Matches double number.

    static OTC_Regexp&	alphabetic();
				// Matches alpha characters.

    static OTC_Regexp&	lowerCase();
				// Matches lower case characters.

    static OTC_Regexp&	upperCase();
				// Matches upper case characters.

    static OTC_Regexp&	alphaNumeric();
				// Matches alphanumeric characters.

    static OTC_Regexp&	identifier();
				// Matches an identifier.

    static OTC_Regexp&	matchingQuotes();
				// Matches string delineated with double
				// quotes. Doesn't handle <\"> in
				// the string.

    static OTC_Regexp&	quotedString();
				// Matches string delineated with double
				// quotes. Handles <\"> in string.

  protected:

    // = COMPILATION

    void		compile();
				// Compiles the pattern.

  private:

			OTC_Regexp(OTC_Regexp const&);

    OTC_Regexp&		operator=(OTC_Regexp const&);

    char*		pattern_;
    				// Copy of the pattern.

    char const*		string_;
				// The string being matched against.

    OTC_RegexpStruct*	regexp_;
				// Information on the compiled pattern.

    char const*		error_;
				// The error message if the pattern is not
				// valid.

    u_int		maxSubPatterns_;
				// The maximum number of sub patterns
				// permitted plus one.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static OTC_Regexp*	whiteSpace_;
				// Matches white space.

    static OTC_Regexp*	optWhiteSpace_;
				// Optionally matches white space.

    static OTC_Regexp*	nonWhiteSpace_;
				// Matches non-white space.

    static OTC_Regexp*	integerValue_;
				// Matches integer number.

    static OTC_Regexp*	realValue_;
				// Matches double number.

    static OTC_Regexp*	alphabetic_;
				// Matches alpha characters.

    static OTC_Regexp*	lowerCase_;
				// Matches lower case characters.

    static OTC_Regexp*	upperCase_;
				// Matches upper case characters.

    static OTC_Regexp*	alphaNumeric_;
				// Matches alphanumeric characters.

    static OTC_Regexp*	identifier_;
				// Matches an identifier.

    static OTC_Regexp*	matchingQuotes_;
				// Matches string delineated with double
				// quotes. Doesn't handle <"> in
				// the string.

    static OTC_Regexp*	quotedString_;
				// Matches string delineated with double
				// quotes. Handles <"> in string.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_REGEXP_HH */
