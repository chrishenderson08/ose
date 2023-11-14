#ifndef ONS_AGENT_BINARY_HH
#define ONS_AGENT_BINARY_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
//
// = FILENAME
//     agent/binary.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/rwpyload.hh>
#include <OTC/text/string.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_Binary : public OTC_String
    // = TITLE
    //     Type to encapsulate binary strings when using messaging system.
    //
    // = DESCRIPTION
    //     The <ONS_Binary> class is a derived version of the <OTC_String>
    //     class provided purely as a means of declaring that a string
    //     contains binary data, such that when the messaging system encodes
    //     a payload, that it will apply "base64" encoding to the string. It
    //     is only necessary to use this when sending binary messages via the
    //     messaging system. Binary values will quite happily decode into a
    //     normal <OTC_String> instance.
{
  public:

    // = INITIALISATION

			ONS_Binary(char const* theString=0)
			  : OTC_String(theString) {}
				// Creates a copy of <theString>.

			ONS_Binary(char theChar, u_int theNum=1)
			  : OTC_String(theChar,theNum) {}
				// Creates a string holding <theNum>
				// instances of <theChar>.

			ONS_Binary(char const* theString, u_int theNum)
			  : OTC_String(theString,theNum) {}
				// Creates a copy of the first <theNum>
				// characters of <theString>. Raises an
				// exception if <theString> is <0> but
				// <theLength> is greater than <0>.

                        ONS_Binary(OTC_String const& theString)
                          : OTC_String(theString) {}
                                // Creates a delayed copy of <theString>.

#if 0
    friend OSE_EXPORT void	operator<<=(
    			 OTC_RWPayload& thePayload,
			 ONS_Binary const& theString
			);
				// Assigns <theString> into <thePayload> in
				// "base64" encoding.
#endif
};

/* ------------------------------------------------------------------------- */

inline void operator<<=(OTC_RWPayload thePayload, ONS_Binary const& theString)
{
  thePayload.assign(theString,"xsd:base64Binary");
}

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_BINARY_HH */
