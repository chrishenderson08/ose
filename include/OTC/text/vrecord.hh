#ifndef OTC_TEXT_VRECORD_HH
#define OTC_TEXT_VRECORD_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/vrecord.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/refcnt/count.hh>
#include <OTC/text/string.hh>
#include <OTC/collctn/cbitset.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_VRecord
    // = TITLE
    //     Class to perform splitting of strings.
    //
    // = DESCRIPTION
    //     The <OTC_VRecord> class is for splitting up a record which is
    //     stored as a string into its separate fields. Individual fields are
    //     separated by a single character, from a defined set, called the
    //     field delimiter. The field delimiter may be any character
    //     including a null character. Two adjacent instances of the
    //     delimiter are taken to indicate an empty field between them. The
    //     original string will be copied and each instance of a delimiter
    //     changed to a null character. If fields have embedded nulls, you
    //     will need to ensure you request what the length of the field was,
    //     otherwise, you can rely on the fact that the field will be null
    //     terminated. The individual fields once the string has been split
    //     are accessable from field <1> upwards. Unlike <OTC_Record>, the
    //     original string is not available through this class.
    //
    // = SEE ALSO
    //     <OTC_Record>
{
  public:

			~OTC_VRecord();
				// If this is the last instance referencing
				// the record and index, delete them.

    // = INITIALISATION

			OTC_VRecord();
				// Creates a record which doesn't have
				// any fields.

			OTC_VRecord(
			 OTC_String const& theString,
			 char theDelimiter
			);
				// Copies <theString> and makes individual
				// fields separated by <theDelimiter>
				// accessable by integral index positions.

			OTC_VRecord(
			 char const* theString,
			 char theDelimiter
			);
				// Copies <theString> and makes individual
				// fields separated by <theDelimiter>
				// accessable by integral index positions.

			OTC_VRecord(
			 char const* theString,
			 u_int theLength,
			 char theDelimiter
			);
				// Copies <theString> and makes individual
				// fields separated by <theDelimiter>
				// accessable by integral index positions.
				// The number of characters in <theString>
				// should be given by <theLength>. <theString>
				// does not need to be null terminated.

			OTC_VRecord(
			 OTC_String const& theString,
			 OTC_String const& theDelimiters
			);
				// Copies <theString> and makes individual
				// fields separated by <theDelimiters>
				// accessable by integral index positions.

			OTC_VRecord(
			 char const* theString,
			 OTC_String const& theDelimiters
			);
				// Copies <theString> and makes individual
				// fields separated by <theDelimiters>
				// accessable by integral index positions.

			OTC_VRecord(
			 char const* theString,
			 u_int theLength,
			 OTC_String const& theDelimiters
			);
				// Copies <theString> and makes individual
				// fields separated by <theDelimiter>
				// accessable by integral index positions.
				// The number of characters in <theString>
				// should be given by <theLength>. <theString>
				// does not need to be null terminated.

			OTC_VRecord(OTC_VRecord const& theRecord);
				// Creates a new record which references
				// the same data as <theRecord>.

			OTC_VRecord(
			 OTC_VRecord const& theRecord,
			 u_int theStart,
			 u_int theLength
			);
				// Creates a new record which references
				// a slice of the data in <theRecord>.
				// The slice is the range starting at
				// <theStart> offset and with <theLength>.
				// The same underlying data is used.
				// If the range for the slice is invalid
				// an exception will be raised.
				// Note that indexes given by <theStart>
				// commence at <1> and not <0>.

    // = ASSIGNMENT

    OTC_VRecord&	operator=(OTC_VRecord const& theRecord);
				// Makes this record reference the same
				// data as <theRecord>.

    // = QUERY

    u_int		numFields() const
				{ return numFields_; }
				// Return the number of fields in the
				// record.

    char const*		field(u_int theIndex) const;
				// Returns the field at position <theIndex>.
				// Note that indexes for fields start at <1>
				// and not <0>. If <0> or an out of range
				// index is supplied, an exception will be
				// raised.

    char const*		operator[](u_int theIndex) const
				{ return field(theIndex); }
				// Returns the field at position <theIndex>.

    u_int		length(u_int theIndex) const;
				// Returns the length of the field at
				// position <theIndex>. Note that indexes for
				// fields start at <1> and not <0>. If <0> or
				// an out of range index is supplied, an
				// exception will be raised.

    OTC_VRecord		slice(u_int theStart, u_int theLength) const;
				// Returns a new record which is a slice out
				// of the original record. The slice is the
				// range starting at <theStart> offset and
				// with <theLength>. The same underlying data
				// is used. If the range for the slice is
				// invalid an exception will be raised.
				// Note that indexes given by <theStart>
				// commence at <1> and not <0>.

  private:

    void		split_(
			 char const* theString,
			 u_int theLength,
			 OTC_CBitSet theDelimiters
			);
				// Splits the record into its separate
				// fields.

    OTC_Count*		refcnt_;
				// The record and index may be shared
				// between multiple instances of this
				// class. This variable records the
				// reference count for the data.

    char*		data_;
				// Copy of original record. The field
				// delimiters have been replaced with
				// null characters and a terminating
				// null added to the string.

    char**		index_;
				// Index onto all fields in the record.

    u_int		offset_;
				// Offset into index which we regard as
				// position <0>.

    u_int		numFields_;
				// The number of fields in the part of
				// the record we are interested in.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_VRECORD_HH */
