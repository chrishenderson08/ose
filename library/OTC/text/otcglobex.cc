/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/otcglobex.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/globex.hh"
#endif
#endif

#include <OTC/text/globex.hh>

#include <string.h>

#define OTCLIB_GXFALSE 0
#define OTCLIB_GXTRUE 1
#define OTCLIB_GXABORT 2     /* end of search indicator */

/* ------------------------------------------------------------------------- */
OTC_Globex::~OTC_Globex()
{
  delete [] pattern_;
}

/* ------------------------------------------------------------------------- */
OTC_Globex::OTC_Globex(char const* thePattern)
  : valid_(true),
    error_(0),
    length_(0)
{
  OTCLIB_ENSURE_FN((thePattern != 0),
   "OTC_Globex::OTC_Globex(char const*)",
   "invalid argument");

  pattern_ = new char[strlen(thePattern)+1];
  OTCLIB_ASSERT_M(pattern_ != 0);
  strcpy(pattern_,thePattern);

  compile();
}

/* ------------------------------------------------------------------------- */
void OTC_Globex::compile()
{
  char const* thePattern = pattern_;

  while (*thePattern != EOS)
  {
    switch (*thePattern++)
    {
      case '?':
      case '*':
      case '[':
	break;

      case '\\':
	if (*thePattern++ == EOS)
	{
	  error_ = "trailing \\";
	  valid_ = false;
	  return;
	}
    }
  }
  valid_ = true;
}

/* ------------------------------------------------------------------------- */
bool OTC_Globex::match(char const* theString)
{
  if (!isValid() || theString == 0)
  {
    length_ = 0;

    return false;
  }
  else
  {
    int res = doMatch(pattern_,theString);
    bool matched = false;
    if (res == OTCLIB_GXTRUE)
      matched = true;

    if (matched)
      length_ = strlen(theString);
    else
      length_ = 0;

    return matched;
  }
}

/* ------------------------------------------------------------------------- */
bool OTC_Globex::isValid() const
{
  return valid_;
}

/* ------------------------------------------------------------------------- */
char const* OTC_Globex::error() const
{
  return error_;
}

/* ------------------------------------------------------------------------- */
int OTC_Globex::doMatch(char const* p, char const* t)
{
    register char range_start, range_end;  /* start and end in range */

    int invert;             /* is this [..] or [!..] */
    int member_match;       /* have I matched the [..] construct? */
    int loop;               /* should I terminate? */

    for ( ; *p; p++, t++ ) {

        /* if this is the end of the text then this is the end of the match */
        if (!*t) {
            return ( *p == '*' && *++p == '\0' )
	     ? OTCLIB_GXTRUE : OTCLIB_GXABORT;
        }

        /* determine and react to pattern type */
        switch ( *p ) {

            /* single any character match */
            case '?':
                break;

            /* multiple any character match */
            case '*':
                return doMatchStar (p, t);

            /* [..] construct, single member/exclusion character match */
            case '[': {

                /* move to beginning of range */
                p++;

                /* check if this is a member match or exclusion match */
                invert = OTCLIB_GXFALSE;
                if ( *p == '!' || *p == '^') {
                    invert = OTCLIB_GXTRUE;
                    p++;
                }

                /* if closing bracket here or at range start then we have a
                   malformed pattern */
                if ( *p == ']' ) {
                    return OTCLIB_GXABORT;
                }

                member_match = OTCLIB_GXFALSE;
                loop = OTCLIB_GXTRUE;

                while ( loop ) {

                    /* if end of construct then loop is done */
                    if (*p == ']') {
                        loop = OTCLIB_GXFALSE;
                        continue;
                    }

                    /* matching a '!', '^', '-', '\' or a ']' */
                    if ( *p == '\\' ) {
                        range_start = range_end = *++p;
                    }
                    else {
                        range_start = range_end = *p;
                    }

                    /* if end of pattern then bad pattern (Missing ']') */
                    if (!range_start)
                        return OTCLIB_GXABORT;

                    /* check for range bar */
                    if (*++p == '-') {

                        /* get the range end */
                        range_end = *++p;

                        /* if end of pattern or construct then bad pattern */
                        if (range_end == '\0' || range_end == ']')
                            return OTCLIB_GXABORT;

                        /* special character range end */
                        if (range_end == '\\')
                            range_end = *++p;

                        /* move just beyond this range */
                        p++;
                    }

                    /* if the text character is in range then match found.
                       make sure the range letters have the proper
                       relationship to one another before comparison */
                    if ( range_start < range_end  ) {
                        if (*t >= range_start && *t <= range_end) {
                            member_match = OTCLIB_GXTRUE;
                            loop = OTCLIB_GXFALSE;
                        }
                    }
                    else {
                        if (*t >= range_end && *t <= range_start) {
                            member_match = OTCLIB_GXTRUE;
                            loop = OTCLIB_GXFALSE;
                        }
                    }
                }

                /* if there was a match in an exclusion set then no match */
                /* if there was no match in a member set then no match */
                if ((invert && member_match) ||
                   !(invert || member_match))
                    return OTCLIB_GXFALSE;

                /* if this is not an exclusion then skip the rest of the [...]
                    construct that already matched. */
                if (member_match) {
                    while (*p != ']') {

                        /* bad pattern (Missing ']') */
                        if (!*p)
                            return OTCLIB_GXABORT;

                        /* skip exact match */
                        if (*p == '\\') {
                            p++;
                        }

                        /* move to next pattern char */
                        p++;
                    }
                }

                break;
            }

            /* next character is quoted and must match exactly */
            case '\\':

                /* move pattern pointer to quoted char and fall through */
                p++;

            /* must match this character exactly */
            default:
                if (*p != *t)
                    return OTCLIB_GXFALSE;
        }
    }

    /* if end of text not reached then the pattern fails */
    return !*t;
}

/* ------------------------------------------------------------------------- */
int OTC_Globex::doMatchStar (char const* p, char const* t)
{
    register int match;
    register int nextp;

    /* pass over existing ? and * in pattern */
    while ( *p == '?' || *p == '*' ) {

        /* take one char for each ? */
        if ( *p == '?' ) {

            /* if end of text then no match */
            if ( !*t++ ) {
                return OTCLIB_GXABORT;
            }
        }

        /* move to next char in pattern */
        p++;
    }

    /* if end of pattern we have matched regardless of text left */
    if ( !*p ) {
        return OTCLIB_GXTRUE;
    }

    /* get the next character to match which must be a literal or '[' */
    nextp = *p;
    if ( nextp == '\\' )
        nextp = p[1];

    /* Continue until we run out of text or definite result seen */
    match = OTCLIB_GXFALSE;
    while ( match == OTCLIB_GXFALSE ) {

        /* a precondition for matching is that the next character
           in the pattern match the next character in the text or that
           the next pattern char is the beginning of a range.  Increment
           text pointer as we go here */
        if ( nextp == *t || nextp == '[' ) {
            match = doMatch(p, t);
        }

        /* if the end of text is reached then no match */
        if ( !*t++ ) match = OTCLIB_GXABORT;
    }

    /* return result */
    return match;
}

/* ------------------------------------------------------------------------- */
