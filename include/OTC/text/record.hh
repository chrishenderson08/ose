#ifndef OTC_TEXT_RECORD_HH
#define OTC_TEXT_RECORD_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/record.hh
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

#include <OTC/text/string.hh>
#include <OTC/collctn/list.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/record.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Record
    // = TITLE
    //     Class to perform splitting of strings.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_Record> class is for splitting up a record which
    //     is stored as a string into its separate fields. Individual fields
    //     are separated by a delimiter. Depending on which constructor
    //     is used, identification of what constitutes a field delimiter will
    //     be dealt with in different ways. The original string is accessable
    //     as field <0>. The individual fields once the string has been split
    //     are accessable from field <1> upwards.
    //
    // = NOTES
    //     To handle different ways of interpreting what a delimiter is,
    //     the implementation is a generic solution and thus may occur
    //     more overhead than is necessary. If a single character delimiter
    //     in the style of <strtok()> is required, consider instead using
    //     the <OTC_VRecord> class.
    //
    // = SEE ALSO
    //     <OTC_VRecord>
{
  public:

    // = INITIALISATION

			OTC_Record() {}
				// Creates a record which doesn't have
				// any fields.

			OTC_Record(OTC_String const& theString);
				// Splits <theString> using whitespace as
				// the field separator, ie. in the style
				// of AWK. Note that leading and trailing
				// whitespace are ignored.

			OTC_Record(OTC_String const& theString, char theFS);
				// Splits <theString> using <theFS> as the
				// field separator. Note that this is done in
				// the style of AWK, ie. multiple instances
				// of <theFS> together are not bunched
				// together as one field separator. If this
				// behaviour is required then use the
				// constructor taking the field separator as
				// a string. Also, leading and trailing
				// instances
				// of the field separator are not discarded.
				// If <theFS> is empty then an exception is
				// generated.

			OTC_Record(
			 OTC_String const& theString,
			 OTC_String const& theFS
			);
				// Splits <theString> using any of the
				// characters in <theFS> as the field
				// separator, ie. in the style of
				// <strtok(3)>. Note: that if multiple
				// instances of field separator characters
				// are found together then they are
				// interpreted as one field separator.
				// Leading and trailing instances of the
				// field separator are discarded.
				// If <theFS> is <0> then an exception is
				// generated.

			OTC_Record(OTC_Record const& theRecord);
				// Copies the contents of <theRecord>
				// to this class.

    OTC_Record&		operator=(OTC_Record const& theRecord);
				// Copies the contents of <theRecord>
				// to this class.

    // = QUERY

    u_int		numFields() const
				{ return fields_.population(); }
				// The number of fields in the string.

    OTC_String const&	field(u_int theIndex) const;
				// Returns the field in the string given by
				// <theIndex>. Attempting to access a field
				// which doesn't exist will result in an
				// exception.

    OTC_String const&	operator[](u_int theIndex) const
				{ return field(theIndex); }
				// Returns the field in the string given by
				// <theIndex>.

    OTC_Iterator<OTC_String>	fields(
				 OTC_Direction theDirection=OTCLIB_FORWARD
				) const
                                { return fields_.items(theDirection); }
				// Returns an iterator over the fields in
				// the record. By default the iteration
				// will proceed from the first field to
				// the last. An explicit direction can be
				// set by supplying <theDirection>, being
				// set to either <OTCLIB_FORWARD> or
				// <OTCLIB_BACKWARD>.

  private:

    void		split_(OTC_String const& theFS, bool isAwk);
				// Splits <record_> into fields using
				// characters in <theFS> as field separators.

    OTC_String		record_;
				// The original string being split.

    OTC_List<OTC_String>	fields_;
				// The fields into which the string was split.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_RECORD_HH */
