#define regcomp otclib_regcomp
#define regerror otclib_regerror
#define regexec otclib_regexec
#define regfree otclib_regfree

#define regex_t otclib_regex_t
#define regmatch_t otclib_regmatch_t

#define POSIX_MISTAKE

#include "regex1.h"

#if defined(__cplusplus)
struct OTC_CompiledPattern : public regex_t {};
struct OTC_MatchedPattern : public regmatch_t {};
#endif
