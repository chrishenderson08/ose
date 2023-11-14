#ifndef OTC_DEBUG_TAGINFO_HH
#define OTC_DEBUG_TAGINFO_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/taginfo.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_TagInfo
    // = TITLE
    //     Maintain name and state of a named trace switch.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_TagInfo> is used to store the name and trace level for
    //     instances of the <OTC_TraceTag> class. Instances of the
    //     class are never destroyed.
    //
    // = SEE ALSO
    //     <OTC_TraceTag>
{
  public:

    friend class	OTC_TraceInfoDummy;
				// To shut up GNU C++.

    // = CONSTRUCTION

			OTC_TagInfo(
			 char const* theName, int theLevel,
			 OTC_TagInfo* theTagInfo=0
			)
			  : name_(theName), level_(theLevel),
			    next_(theTagInfo)
				{}
				// Links this class in front of <theTagInfo>,
				// setting the switch name to <theName> and
				// the initial trace level of the named
				// switch to <theLevel>.

    // = QUERY/CHANGE

    char const*		name() const
				{ return name_; }
				// Returns the name of the switch.

    int			level() const
				{ return level_; }
				// Returns the trace level of the named
				// switch.

    void		setLevel(int theLevel)
				{ level_ = theLevel; }
				// Sets the trace level of the named switch.

    // = ITERATION

    OTC_TagInfo*	next() const
				{ return next_; }
				// Returns a pointer to the switch description
				// object following this one.

  private:

			~OTC_TagInfo() {}
				// Should never be required.

			OTC_TagInfo(OTC_TagInfo const&);
				// Do not define an implementation for this.

    OTC_TagInfo&	operator=(OTC_TagInfo const&);
				// Do not define an implementation for this.

    char const*		name_;
				// The name of the switch.

    int			level_;
				// Trace level of the named switch.

    OTC_TagInfo*	next_;
				// Pointer to next switch description object.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_TAGINFO_HH */
