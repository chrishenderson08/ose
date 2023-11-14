#ifndef OTC_TEXT_STRING_HH
#define OTC_TEXT_STRING_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/string.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993-1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/rstring.hh>
#include <OTC/text/sobject.hh>
#include <OTC/text/cstring.hh>
#include <OTC/text/tstring.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/collctn/hashactn.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/range.hh>
#include <OTC/collctn/cbitset.hh>
#include <OTC/thread/nrmutex.hh>

#include <OTC/stdlib/iostream.h>

#include <string.h>

#ifdef index
#undef index
#endif

#ifdef rindex
#undef rindex
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/string.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_SCmpType
{
  OTCLIB_EXACTMATCH,
  OTCLIB_IGNORECASE
};

class OSE_EXPORT OTC_Capacity
    // = TITLE
    //     Class for holding string capacity.
{
  public:

			OTC_Capacity(size_t theCapacity)
			  : capacity_(theCapacity) {}

    size_t		capacity() const
				{ return capacity_; }

  private:

    size_t		capacity_;
};

class OSE_EXPORT OTC_Length
    // = TITLE
    //     Class for holding string length.
{
  public:

			OTC_Length(size_t theLength)
			  : length_(theLength) {}

    size_t		length() const
				{ return length_; }

  private:

    size_t		length_;
};

class OSE_EXPORT OTC_String
    // = TITLE
    //	   A class for holding a sequence of characters.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     The <OTC_String> class is designed to hold a sequence of
    //     characters, including null characters. The class uses delayed copy
    //     to improve average performance.
    //     
    //	   The delayed copy mechanism means that when an assignment of one
    //	   string class is made to another, or when a new class is created
    //	   using another, they initially share the same underlying character
    //	   buffer. The number of instances of the string class referencing
    //	   the buffer is maintained through reference counting. Only when an
    //	   operation is performed which would modify the buffer is a private
    //	   copy made for that class. This scheme avoids unnecessary copies
    //	   being made of strings.
    //
    // = NOTES
    //	   Except where special behaviour is mentioned, a nil pointer is
    //	   dealt with as if it were a null terminated string.
    //
    // = SEE ALSO
    //     <OTC_SObject>, <OTC_CString>, <OTC_LString>, <OTC_RString>,
    //     <OTC_TString>
{
  public:

    virtual		~OTC_String();

    // = INITIALISATION

			OTC_String()
			  : data_(undefinedString().data_) {}
				// Creates an empty string with a notionally
				// undefined value. The string will behave
				// exactly like an empty string, but
				// <isUndefined()> will return true until the
				// value of the string has been set.

			OTC_String(OTC_Capacity theCapacity)
			  : data_(theCapacity.capacity(),OTCLIB_UNBUFFERED)
			  	{ data_.reallocate(0); }
				// Creates an empty string with enough
				// underlying capacity to hold a string of
				// <theCapacity>. This is equivalent to
				// the old <OTC_String> constructor
				// taking <u_int>.

			OTC_String(OTC_Length theLength)
			  : data_(theLength.length()) {}
				// Creates a string of length <theLength>.
				// The string is not initialised and
				// will be filled with garbage. This is
				// equivalent to the old <OTC_Buffer>
				// constructor taking <u_int>.

			OTC_String(
			 OTC_Length theLength,
			 OTC_Capacity theCapacity
			)
			  : data_(theCapacity.capacity(),OTCLIB_UNBUFFERED)
			  	{ data_.reallocate(theLength.length()); }
				// Creates a string of length <theLength>.
				// The string is not initialised and
				// will be filled with garbage. The
				// underlying buffer capacity will at the
				// same time be set to <theCapacity>.

			OTC_String(char const* theString)
			  : data_((theString?::strlen(theString):0))
			  	{
				  if (theString != 0)
				  {
				    OTC_Copy::copy(data_.string(),
				     theString,data_.length());
				  }
				}
				// Creates a string holding a copy of the
				// null terminated string <theString>. If
				// <theString> is <0>, an empty string is
				// created.

			OTC_String(char const* theString, size_t theNum);
				// Creates a string holding a copy of the
				// first <theNum> characters of <theString>.
				// If <theString> is <0>, and <theNum> is
				// not <0>, an exception will be raised.

			OTC_String(char theChar, size_t theNum=1)
			  : data_(theNum)
			  	{
				  memset(data_.string(),
				   theChar,data_.length());
				}
				// Creates a string containing <theNum>
				// occurrences of <theChar>. If <theNum>
				// is <0> an empty string is created.

			OTC_String(OTC_String const& theString)
			  : data_(theString.data_) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it.

			OTC_String(OTC_String const& theString, size_t theNum);
				// Creates a string holding a copy of the
				// first <theNum> characters of <theString>.
				// If the length of <theString> is less
				// than <theNum>, an exception is raised.

			OTC_String(OTC_SObject const& theString)
			  : data_(theString.rawString()) {}
				// Creates a string which references the same
				// character string obtained from
				// <theString>. A copy is only made of the
				// string when an attempt is made to modify
				// it.

			OTC_String(OTC_CString const& theString)
			  : data_(theString.data_) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it. <OTC_CString>
				// is a constant string.

			OTC_String(OTC_RString const& theString)
			  : data_(theString) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it. <OTC_RString>
				// is the raw string used to implement this
				// class.

			OTC_String(OTC_TString const& theString)
			  : data_(theString.data_) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it. Instances of
				// <OTC_TString> are returned from the
				// <operator+()> defined for strings.

    // = STREAM OPERATIONS

#if 0

    friend OSE_EXPORT istream&	operator>>(istream& ins, OTC_String& theString);
				// If the flag <ios::skipws> is set in <ins>,
				// any whitespace at the start of <ins> is
				// discarded. Characters are then read in and
				// placed into <theString>. Reading stops
				// when further whitespace or <EOF> is
				// encountered. If <ios::skipws> is not
				// defined and there is initial white space,
				// an empty string is returned. If a field
				// width is specified in <ins> it will be
				// honoured.

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& outs,
				 OTC_String const& theString
				);
				// Outputs <theString> onto <outs>. Any field
				// widths and justification flags set in
				// <outs> are honoured.

#endif

    // The following give similar functionality to <istream::get()> and
    // <istream::getline()>.

    static OTC_String	get(istream& ins, char theDelim=EOL);
				// Similiar to <istream::get()> except that
				// it is not required to specify a maximum
				// length for a buffer. In other words, any
				// number of characters up to but not
				// including <theDelim> are read from <ins>,
				// the result being returned as a string.
				// <theDelim> is left in <ins>.

    static OTC_String&	get(
			 OTC_String& theString,
			 istream& ins,
			 char theDelim=EOL
			);
				// Similiar to <istream::get()> except that
				// it is not required to specify a maximum
				// length for a buffer. In other words, any
				// number of characters up to but not
				// including <theDelim> are read from <ins>
				// and appended to <theString>. <theDelim> is
				// left in <ins>. Characters are appended to
				// <theString>. A reference to <theString> is
				// returned.

    static OTC_String	getline(istream& ins, char theDelim=EOL);
				// Similar to <istream::getline()> except
				// that it is not required to specify a
				// maximum length for a buffer. In other
				// words, any number of characters up to but
				// not including <theDelim> are read from <ins>,
				// the result being returned as a string.
				// <theDelim> is extracted from <ins>.

    static OTC_String&	getline(
			 OTC_String& theString,
			 istream& ins,
			 char theDelim=EOL
			);
				// Similar to <istream::getline()> except
				// that it is not required to specify a
				// maximum length for a buffer. In other
				// words, any number of characters up to but
				// not including <theDelim> are read from
				// <ins> and appended to <theString>.
				// <theDelim> is extracted from <ins>.
				// Characters are appended to <theString>. A
				// reference to <theString> is returned.

    // The following give similar functionality to <istream::read()>.

    static OTC_String	read(istream& ins, size_t theNum);
				// Similar to <istream::read()>. The string
				// into which the characters are read is
				// returned. The length of the string
				// returned should be checked to determine
				// how many characters were actually read.

    static OTC_String&	read(
			 OTC_String& theString,
			 istream& ins,
			 size_t theNum
			);
				// Similar to <istream::read()>. Characters
				// read in are appended to <theString>. The
				// length of the string returned should be
				// checked to determine how many characters
				// were actually read. A reference to
				// <theString> is returned.

    // The following stream functions give the same functionality as do the
    // member functions of the same name in the Rogue Wave string class.
    // These have been added to ease transition from the Rogue Wave string
    // class to this string class. The member functions do also present a
    // simpler interface than the stream functions above. Note though that
    // the value returned is the string class and not the stream from which
    // characters are being read. This is different from the Rogue Wave
    // string class.

    OTC_String&		readFile(istream& theStream);
				// Reads characters from <theStream>,
				// replacing the contents of this string
				// in the process. Reading of characters
				// from <theStream> will stop when <EOF>
				// is reached.

    OTC_String&		readLine(
			 istream& theStream,
			 bool theSkipWhite=true
			);
				// Reads characters from <theStream>,
				// replacing the contents of this string
				// in the process. Reading of characters
				// from <theStream> will stop when a newline
				// character is encountered, or the end of
				// the stream is reached. The newline
				// character is removed from the stream but
				// is not stored in this string. If
				// <theSkipWhite> is <true>, the default, any
				// leading whitespace is skipped by applying
				// the <iostream> manipulator <ws>. Note that
				// a newline is regarded as whitespace, thus
				// blank lines would be skipped.

    OTC_String&		readString(istream& theStream);
				// Reads characters from <theStream>,
				// replacing the contents of this string in
				// the process. Reading of characters from
				// <theStream> will stop when a null
				// character is encountered, or the end of
				// the stream is reached. The null character
				// is removed from the stream but is not
				// stored in this string.

    OTC_String&		readToDelim(istream& theStream, char theDelim);
				// Reads characters from <theStream>,
				// replacing the contents of this string in
				// the process. Reading of characters from
				// <theStream> will stop when the delimiter
				// character <theDelim> is encountered, or
				// the end of the stream is reached. The
				// delimiter character is removed from the
				// stream but is not stored in this string.

    OTC_String&		readToken(istream& theStream);
				// Leading whitespace in <theStream> is
				// skipped. Characters are then read from
				// <theStream>, replacing the contents of
				// this string in the process. Reading
				// of characters from <theStream> will stop
				// when further whitespace is encountered
				// or the end of the stream is reached.

    // The following extends Rogue Wave style interface to reading set
    // number of characters from a stream.

    OTC_String&		readData(istream& theStream, size_t theNum);
				// Reads characters from <theStream>,
				// replacing the contents of this string in
				// the process. Reading of characters from
				// <theStream> will stop when <theNum>
				// characters is read or the end of the
				// stream is reached. You should check the
				// length of the string to determine how
				// many characters were actually read.

    // = ACCESS
    //     It is not guaranteed that pointers returned by the following
    //     functions are valid after subsequent modifications to the
    //     underlying buffer, as modifications may result in new buffer space
    //     being allocated and the previous space being deleted. If a valid
    //     buffer is required, a copy should be made. See <duplicate()> about
    //     making copies.
    //     
    //     The return value of the following functions should not be cast
    //     from non const, to circumvent the restrictions in place to stop
    //     indirect modifications to the buffer, as the changes could effect
    //     more than one class due to the delayed copy mechanism.

    char const*		string() const
      				{ return data_.string(); }
				// Returns a pointer to the underlying buffer
				// used to hold characters. If the string
				// is empty, a pointer to an empty but null
				// terminated string will be returned.

    			operator char const*() const
			  	{ return data_.string(); }
				// Returns a pointer to the underlying buffer
				// used to hold characters. If the string
				// is empty, a pointer to an empty but null
				// terminated string will be returned.

    char const*		data() const
      				{ return data_.string(); }
				// Returns a pointer to the underlying buffer
				// used to hold characters. If the string
				// is empty, a pointer to an empty but null
				// terminated string will be returned.
				// This function is only provided to make a
				// transition from using the Rogue Wave
				// string class to this string class easier.
				// You should use <string()> instead.

    char const*		c_str() const
      				{ return data_.string(); }
				// Returns a pointer to the underlying buffer
				// used to hold characters. If the string
				// is empty, a pointer to an empty but null
				// terminated string will be returned.
				// This function is only provided to make a
				// transition from using the standard C++
				// library string class to this string class
				// or vice versa easier. You should use
				// <string()> instead.

    char const*		buffer() const
      				{ return (data_.length()==0)?0:data_.string(); }
				// Returns a pointer to the underlying buffer
				// used to hold characters. If the string
				// is empty, a <0> pointer will be returned.

    char*		buffer();
				// Returns a pointer to the underlying buffer
				// used to hold characters. If the string is
				// empty, a <0> pointer will be returned.
				// Note that this variant of the <buffer()>
				// function is only used on non const
				// instances of the class. The returned
				// pointer can be used to directly write into
				// the buffer. It is guaranteed that any
				// delayed copy will be broken before the
				// pointer is returned. If however, you
				// overwrite the end of the buffer and
				// destroy the null terminator, it is your
				// own problem.

    // WARNING. WARNING. WARNING. Ye who play with the following functions
    // better know what they are doing.

    OTC_RString const&	rawString() const
				{ return data_; }
				// Returns a reference to the raw string
				// information.

    OTC_RString&	rawString()
				{ return data_; }
				// Returns a reference to the raw string
				// information.

    // = INDEXING
    //     Whether the object is const or not, it is not possible to
    //     access the null terminator which is automatically added
    //     at the end of the string.

    char		operator[](size_t theIndex) const;
				// Returns the character at the location in
				// this string given by <theIndex>. If
				// <theIndex> is greater than or equal to the
				// length of this string, an exception is
				// raised.

    char&		operator[](size_t theIndex);
				// Returns a reference to the character at
				// the location in this string given by
				// <theIndex>. If <theIndex> is greater than
				// or equal to the length of this string, an
				// exception is raised. When this function is
				// invoked, any delayed copy will be broken
				// first.

    // = RESIZING

    void		length(size_t theLength)
      				{ data_.reallocate(theLength); }
				// Resizes the string. Old data in the string
				// is preserved for whatever range would be
				// valid in the new string. If the length of
				// the string is increased, any new portion
				// of memory is not initialised.

    void		resize(size_t theLength)
      				{ data_.reallocate(theLength); }
				// Resizes the string. Old data in the string
				// is preserved for whatever range would be
				// valid in the new string. If the length of
				// the string is increased, any new portion
				// of memory is not initialised. Kept for
				// backward compatibility with the old
				// <OTC_Buffer>, use <length(u_int)> instead.

    void		capacity(size_t theCapacity);
				// Resizes the underlying buffer to increase
				// its size so that it can hold a string of
				// length <theCapacity>. The length of the
				// current string and its contents are
				// unchanged. If the underlying buffer is
				// shared, this operation will result in the
				// buffer being reallocated, the capacity
				// being the greater of what is required
				// to hold the current string and the value
				// <theCapacity>. If the underlying buffer is
				// not shared, a reallocation will only occur
				// if <theCapacity> is greater than the
				// existing capacity.

    // = COPYING

    char*		duplicate() const;
				// Returns a copy of the underlying buffer.
				// Note that it is the users responsibility
				// to delete the copy. Only enough
				// space to hold the string and the null
				// terminator is allocated. Also, the
				// memory is always obtained from transient
				// memory in situations where a database
				// product is being used.

    OTC_String		clone() const
				{ return *this; }
				// Returns a clone of this string as a new
				// instance of an <OTC_String> object.
				// Actually, the delayed copy mechanism is
				// used, meaning that once a modification
				// is made separate memory for the string
				// will be allocated.

    // = QUERIES

    size_t		length() const
      				{ return data_.length(); }
				// Returns the length of this string.
				// The length does not include the null
				// terminator.

    size_t		size() const
      				{ return data_.length(); }
				// Provided for backward compatibility with
				// <OTC_Buffer> only. Use <length()> instead.

    bool		isEmpty() const
    				{ return data_.length() == 0; }
				// Returns <true> if the length of this
				// string is <0>.

    bool		isNull() const
    				{ return data_.length() == 0; }
				// Returns <true> if the length of this
				// string is <0>. This function is only
				// provided to ease the transition from using
				// the Rogue Wave string class to this string
				// class. Use <isEmpty()> instead.

    bool		isUndefined() const
      				{
				  return data_.string() ==
				   undefinedString().data_.string();
				}
				// Returns <true> if the value of the string
				// has not as yet been set. Note that
				// <isEmpty()> will also return <true> if the
				// string has not as yet been set.

    size_t		capacity() const
      				{ return data_.capacity(); }
				// Returns the maximum capacity of the buffer
				// being used to hold the current string. The
				// value returned does not include the cell
				// reserved for holding the null terminator.

    // = NULL STRING

    static OTC_String const&	nullString();
				// Returns a reference to an empty string.
				// Can be used when it is necessary to return
				// an empty string where the return type of
				// the function is a reference but you
				// do not have an actual instance to return.

    static OTC_String const&	nullBuffer()
				{ return nullString(); }
				// Returns a reference to an empty string.
				// Can be used when it is necessary to return
				// an empty string where the return type of
				// the function is a reference but you
				// do not have an actual instance to return.
				// Provided only for backward compatibility
				// with <OTC_Buffer>. Use <nullString()>
				// instead.

    static OTC_String const&	undefinedString();
				// Returns a reference to an empty string.
				// This string is different to that returned
				// by <nullString()>. When a string has been
				// set to the value of <undefinedString()>
				// the <isUndefinedString()> function will
				// return true.

    // = REPLACEMENT
    //     In the following functions, <theStart> is an index. Valid values
    //     for <theStart> are <0> through <length()>. Values outside this
    //     range will result in an exception. <theLength> is the number of
    //     characters to be replaced. If <theStart> plus <theLength> is
    //     greater than <length()>, an exception will be raised. If
    //     <theLength> is equal to <0>, insertion is performed at that point.
    //     Both <theStart> and <theLength> equal to <0> is equivalent to
    //     prepending something to the string. <theStart> equal to <length()>
    //     and <theLength> equal to <0> is equivalent to appending something
    //     to the string. Replacing a range of characters with a null
    //     terminated string, a zero number of characters, or a null pointer,
    //     is equivalent to removing that sequence of characters. <theStart>
    //     equal to <0> and <theLength> equal to <length()> is equivalent to
    //     assignment.
    //     
    //     Any functions where arguments of <theStart> and <theLength> are
    //     expected have also been overloaded such that an instance of the
    //     <OTC_Range> object can be supplied instead. In these cases
    //     <theStart> is equivalent to <OTC_Range::lower()> and <theLength>
    //     to <OTC_Range::length()>.

    OTC_String&		replace(
			 size_t theStart,
			 size_t theLength,
			 char theChar,
			 size_t theNum=1
			)
				{
				  replace_(theStart,theLength,
				   theChar,theNum);
				  return *this;
				}
				// Replaces <theLength> characters from
				// <theStart> with <theNum> instances of
				// <theChar>.

    OTC_String&		replace(
			 OTC_Range const& theRange,
			 char theChar,
			 size_t theNum=1
			)
				{
				  replace_(theRange.lower(),
				   theRange.length(),theChar,theNum);
				  return *this;
				}
				// Replaces <theRange> of characters with
				// <theNum> instances of <theChar>.

    OTC_String&		replace(
			 size_t theStart,
			 size_t theLength,
			 char const* theString
			)
				{
				  replace_(theStart,theLength,theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Replaces <theLength> characters from
				// <theStart> with <theString>.

    OTC_String&		replace(
			 OTC_Range const& theRange,
			 char const* theString
			)
				{
				  replace_(theRange.lower(),
				   theRange.length(),theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Replaces <theRange> of characters with
				// <theString>.

    OTC_String&		replace(
			 size_t theStart,
			 size_t theLength,
			 char const* theString,
			 size_t theNum
			)
				{
				  replace_(theStart,theLength,
				   theString,theNum);
				  return *this;
				}
				// Replaces <theLength> characters from
				// <theStart> with the first <theNum>
				// characters of <theString>.

    OTC_String&		replace(
			 OTC_Range const& theRange,
			 char const* theString,
			 size_t theNum
			)
				{
				  replace_(theRange.lower(),
				   theRange.length(),theString,theNum);
				  return *this;
				}
				// Replaces <theRange> of characters with the
				// first <theNum> characters of <theString>.

    OTC_String&		replace(
			 size_t theStart,
			 size_t theLength,
			 OTC_String const& theString
			)
				{
				  replace_(theStart,theLength,
				   theString.string(),theString.length());
				  return *this;
				}
				// Replaces <theLength> characters from
				// <theStart> with <theString>.

    OTC_String&		replace(
			 OTC_Range const& theRange,
			 OTC_String const& theString
			)
				{
				  replace_(theRange.lower(),
				   theRange.length(),theString.string(),
				   theString.length());
				  return *this;
				}
				// Replaces <theRange> of characters with
				// <theString>.

    OTC_String&		replace(
			 size_t theStart,
			 size_t theLength,
			 OTC_String const& theString,
			 size_t theNum
			)
				{
				  replace_(theStart,theLength,
				   theString,theNum);
				  return *this;
				}
				// Replaces <theLength> characters from
				// <theStart> with the first <theNum>
				// characters of <theString>. If the length
				// of <theString> is less than <theNum> an
				// exception is raised.

    OTC_String&		replace(
			 OTC_Range const& theRange,
			 OTC_String const& theString,
			 size_t theNum
			)
				{
				  replace_(theRange.lower(),
				   theRange.length(),theString,theNum);
				  return *this;
				}
				// Replaces <theRange> of characters with the
				// first <theNum> characters of <theString>.
				// If the length of <theString> is less than
				// <theNum> an exception is raised.

    // = ASSIGNMENT

    OTC_String&		assign(char theChar, size_t theNum=1)
				{
				  replace_(0,length(),theChar,theNum);
				  return *this;
				}
				// Sets this string to be a sequence of
				// <theNum> instances of the character
				// <theChar>. If <theNum> is <0> the result
				// is an empty string.

    OTC_String&		assign(char const* theString)
				{
				  replace_(0,length(),theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Sets this string to be <theString>. If
				// <theString> is <0> the result is an empty
				// string.

    OTC_String&		assign(char const* theString, size_t theNum)
				{
				  replace_(0,length(),theString,theNum);
				  return *this;
				}
				// Sets this string to be the first <theNum>
				// characters of <theString>. If <theString>
				// is <0> the result is an empty string.

    OTC_String&		assign(OTC_String const& theString)
      				{
				  data_ = theString.data_;
				  return *this;
				}
				// Sets this string to reference the same
				// character string as <theString>. A copy
				// is only made of the string when an attempt
				// is made to modify it. See description of
				// delayed copy mechanism above.

    OTC_String&		assign(OTC_String const& theString, size_t theNum);
				// Sets this string to be the first <theNum>
				// characters of <theString>. If the length
				// of <theString> is less than <theNum> an
				// exception is raised.

    // Following return a reference to this string so that assignments may be
    // chained. This makes the behaviour consistent with the standard
    // assignment operator.

    OTC_String&		operator=(char theChar)
				{
				  replace_(0,length(),theChar,1);
				  return *this;
				}
				// Sets this string to be the single
				// character <theChar>.

    OTC_String&		operator=(char const* theString)
				{
				  replace_(0,length(),theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Sets this string to be <theString>. If
				// <theString> is <0> the result is an empty
				// string.

    OTC_String&		operator=(OTC_String const& theString)
				{ assign(theString); return *this; }
				// Sets this string to reference the same
				// character string as <theString>. A copy
				// is only made of the string when an attempt
				// is made to modify it. See description of
				// delayed copy mechanism above.

    // = INSERTION
    //     Valid indexes for insertion are <0> through <length()>. An
    //     index of <length()> will result in concatenation to this string.
    //     An index outside of this range will result in an exception.

    OTC_String&		insert(size_t theIndex, char theChar, size_t theNum=1)
				{
				  replace_(theIndex,0,theChar,theNum);
				  return *this;
				}
				// Inserts <theChar> before <theIndex>
				// position of this string <theNum> times.

    OTC_String&		insert(size_t theIndex, char const* theString)
				{
				  replace_(theIndex,0,theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Inserts <theString> before <theIndex>
				// position of this string.

    OTC_String&		insert(
			 size_t theIndex,
			 char const* theString,
			 size_t theNum
			)
				{
				  replace_(theIndex,0,theString,theNum);
				  return *this;
				}
				// Inserts the first <theNum> characters of
				// <theString> before <theIndex> position of
				// this string.

    OTC_String&		insert(size_t theIndex, OTC_String const& theString)
				{
				  replace_(theIndex,0,theString.string(),
				   theString.length());
				  return *this;
				}
				// Inserts <theString> before <theIndex>
				// position of this string.

    OTC_String&		insert(
			 size_t theIndex,
			 OTC_String const& theString,
			 size_t theNum
			)
				{
				  replace_(theIndex,0,theString,theNum);
				  return *this;
				}
				// Inserts the first <theNum> characters of
				// <theString> before <theIndex> position of
				// this string. If the length of <theString>
				// is less than <theNum> an exception is
				// raised.

    // = EXTENSION

    OTC_String&		append(char theChar, size_t theNum=1)
				{
				  replace_(length(),0,theChar,theNum);
				  return *this;
				}
				// Appends <theChar> to the end of this
				// string <theNum> times.

    OTC_String&		append(char const* theString)
				{
				  replace_(length(),0,theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Appends <theString> to the end of this
				// string.

    OTC_String&		append(char const* theString, size_t theNum)
				{
				  replace_(length(),0,theString,theNum);
				  return *this;
				}
				// Appends the first <theNum> characters
				// of <theString> to the end of this string.

    OTC_String&		append(OTC_String const& theString)
				{
				  replace_(length(),0,theString.string(),
				   theString.length());
				  return *this;
				}
				// Appends <theString> to the end of this
				// string.

    OTC_String&		append(OTC_String const& theString, size_t theNum)
				{
				  replace_(length(),0,theString,theNum);
				  return *this;
				}
				// Appends the first <theNum> characters
				// of <theString> to the end of this string.
				// If the length of <theString> is less
				// than <theNum> an exception is raised.

    OTC_String&		operator+=(char theChar)
				{
				  replace_(length(),0,theChar,1);
				  return *this;
				}
				// Appends <theChar> to the end of this
				// string.

    OTC_String&		operator+=(char const* theString)
				{
				  replace_(length(),0,theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Appends <theString> to the end of this
				// string.

    OTC_String&		operator+=(OTC_String const& theString)
				{
				  replace_(length(),0,theString.string(),
				   theString.length());
				  return *this;
				}
				// Appends <theString> to the end of this
				// string.

    OTC_String&		prepend(char theChar, size_t theNum=1)
				{
				  replace_(0,0,theChar,theNum);
				  return *this;
				}
				// Prepends <theChar> to the start of this
				// string <theNum> times.

    OTC_String&		prepend(char const* theString)
				{
				  replace_(0,0,theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Prepends <theString> to the start of this
				// string.

    OTC_String&		prepend(char const* theString, size_t theNum)
				{
				  replace_(0,0,theString,theNum);
				  return *this;
				}
				// Prepends the first <theNum> characters
				// of <theString> to the start of this string.

    OTC_String&		prepend(OTC_String const& theString)
				{
				  replace_(0,0,theString.string(),
				   theString.length());
				  return *this;
				}
				// Prepends <theString> to the start of this
				// string.

    OTC_String&		prepend(OTC_String const& theString, size_t theNum)
				{
				  replace_(0,0,theString,theNum);
				  return *this;
				}
				// Prepends the first <theNum> characters
				// of <theString> to the start of this string.
				// If the length of <theString> is less
				// than <theNum> an exception is raised.

    // = CONCATENATION

#if 0

    friend OSE_EXPORT OTC_TString	operator+(
				 OTC_String const& s1,
				 OTC_String const& s2
				);
				// Returns a string which is the concatenation
				// of <s1> and <s2>.

    friend OSE_EXPORT OTC_TString	operator+(
				 OTC_String const& s1,
				 char const* s2
				);
				// Returns a string which is the concatenation
				// of <s1> and <s2>.

    friend OSE_EXPORT OTC_TString	operator+(
				 char const* s1,
				 OTC_String const& s2
				);
				// Returns a string which is the concatenation
				// of <s1> and <s2>.

    friend OSE_EXPORT OTC_TString	operator+(
				 OTC_SObject const& s1,
				 OTC_String const& s2
				);
				// Returns a string which is the concatenation
				// of <s1> and <s2>.

    friend OSE_EXPORT OTC_TString	operator+(
				 OTC_SObject const& s1,
				 char const* s2
				);
				// Returns a string which is the concatenation
				// of <s1> and <s2>.

    friend OSE_EXPORT OTC_TString	operator+(
				 OTC_String const& s1,
				 OTC_SObject const& s2
				);
				// Returns a string which is the concatenation
				// of <s1> and <s2>.

    friend OSE_EXPORT OTC_TString	operator+(
				 OTC_String const& s1,
				 char c1
				);
				// Returns a string which is the concatenation
				// of <s1> and <c1>.

    friend OSE_EXPORT OTC_TString	operator+(
				 char c1,
				 OTC_String const& s1
				);
				// Returns a string which is the concatenation
				// of <c1> and <s1>.

#endif

    // = REMOVAL

    OTC_String&		remove(size_t theStart, size_t theLength)
				{
				  replace_(theStart,theLength,EOS,0);
				  return *this;
				}
				// Removes from this string <theLength>
				// characters starting at <theStart>.

    OTC_String&		remove(OTC_Range const& theRange)
				{
				  replace_(theRange.lower(),
				   theRange.length(),EOS,0);
				  return *this;
				}
				// Removes from this string <theRange>
				// of characters.

    // = TRUNCATION

    OTC_String&		truncate()
				{ length(0); return *this; }
				// Sets the size of the string back to <0>.

    OTC_String&		truncate(size_t theIndex);
				// Removes those characters from <theIndex>
				// to the end of this string. If <theIndex>
				// is greater than the length of the string
				// an exception is raised.

    // = SUBSTRINGS

    OTC_String		section(size_t theStart, size_t theLength) const
				{ return section_(theStart,theLength); }
				// Returns a new string containing
				// <theLength> characters starting at
				// <theStart>.

    OTC_String		section(OTC_Range const& theRange) const
				{
				  return section_(theRange.lower(),
				   theRange.length());
				}
				// Returns a new string containing
				// those characters in <theRange>.

    OTC_String		between(size_t theStart, size_t theEnd) const
				{
				  return section_(theStart+1,
				   theEnd-(theStart+1));
				}
				// Returns a new string containing the
				// characters between <theStart> and <theEnd>.

    OTC_String		after(size_t theIndex) const
				{
				  return section_(theIndex+1,
				   length()-(theIndex+1));
				}
				// Returns a new string containing characters
				// appearing after <theIndex>.

    OTC_String		from(size_t theIndex) const
				{
				  return section_(theIndex,
				   length()-theIndex);
				}
				// Returns a new string containing characters
				// appearing from <theIndex> through to
				// the end of the string.

    OTC_String		before(size_t theIndex) const
				{ return section_(0,theIndex); }
				// Returns a new string containing characters
				// appearing before <theIndex>.

    OTC_String		through(size_t theIndex) const
				{ return section_(0,theIndex+1); }
				// Returns a new string containing characters
				// appearing from the start of the string
				// through <theIndex>.

    OTC_String		besides(size_t theStart, size_t theLength) const
				{ return except_(theStart,theLength); }
				// Returns a new string containing the
				// characters not appearing in the part
				// of the string from <theStart> and
				// with <theLength>.

    OTC_String		besides(OTC_Range const& theRange) const
				{
				  return except_(theRange.lower(),
				   theRange.length());
				}
				// Returns a new string containing the
				// characters not appearing in <theRange>
				// of characters.

#ifndef _MSC_VER

    OTC_String		except(size_t theStart, size_t theLength) const
				{ return except_(theStart,theLength); }
				// Returns a new string containing the
				// characters not appearing in the part
				// of the string from <theStart> and
				// with <theLength>.

    OTC_String		except(OTC_Range const& theRange) const
				{
				  return except_(theRange.lower(),
				   theRange.length());
				}
				// Returns a new string containing the
				// characters not appearing in <theRange>
				// of characters.

#endif

    // = MODIFIERS

    OTC_String&		upper(size_t theStart, size_t theLength)
				{
				  upper_(theStart,theLength);
				  return *this;
				}
				// Converts any lower case characters in the
				// range defined by <theStart> and
				// <theLength> to upper case.

    OTC_String&		upper(OTC_Range const& theRange)
				{
				  upper_(theRange.lower(),
				   theRange.length());
				  return *this;
				}
				// Converts any lower case characters in
				// <theRange> of characters to upper case.

    OTC_String&		upper(size_t theLength)
				{ upper_(0,theLength); return *this; }
				// Converts any lower case characters in the
				// first <theLength> characters of this string
				// string to upper case.

    OTC_String&		upper()
				{ upper_(0,length()); return *this; }
				// Converts any lower case characters in this
				// string to upper case.

    OTC_String&		lower(size_t theStart, size_t theLength)
				{ lower_(theStart,theLength); return *this; }
				// Converts any upper case characters in the
				// range defined by <theStart> and <theLength>
				// to lower case.

    OTC_String&		lower(OTC_Range const& theRange)
				{
				  lower_(theRange.lower(),theRange.length());
				  return *this;
				}
				// Converts any upper case characters in
				// <theRange> of characters to lower case.

    OTC_String&		lower(size_t theLength)
				{ lower_(0,theLength); return *this; }
				// Converts any upper case characters in the
				// first <theLength> characters of this string
				// string to lower case.

    OTC_String&		lower()
				{ lower_(0,length()); return *this; }
				// Converts any upper case characters in this
				// string to lower case.

    OTC_String&		lchop(size_t theNum=1)
				{
				  replace_(0,theNum,EOS,0);
				  return *this;
				}
				// Removes the first <theNum> characters
				// from this string. If that number of
				// characters does not exist, an exception
				// is raised.

    OTC_String&		rchop(size_t theNum=1)
				{
				  replace_(data_.length()-theNum,
				   theNum,EOS,0);
				  return *this;
				}
				// Removes the last <theNum> characters
				// from this string. If that number of
				// characters does not exist, an exception
				// is raised.

    OTC_String&		ljustify(size_t theWidth, char theChar=' ');
				// If there are less than <theWidth>
				// characters in the string, instances
				// of <theChar> will be added to the
				// string to fill it out to a width
				// of <theWidth>. The original string
				// will be left justified.

    OTC_String&		rjustify(size_t theWidth, char theChar=' ');
				// If there are less than <theWidth>
				// characters in the string, instances
				// of <theChar> will be added to the
				// string to fill it out to a width
				// of <theWidth>. The original string
				// will be right justified.

    OTC_String&		reverse();
				// Reverses the order of characters in
				// this string.

    // For the following functions, white space is any of SPACE, TAB,
    // RETURN, NEWLINE, FORMFEED, or the vertical tab character.

    OTC_String&		rtrim();
				// Removes trailing white space from
				// this string.

    OTC_String&		rtrim(char theChar);
				// Removes trailing instances of <theChar>
				// from the end of this string.

    OTC_String&		rtrim(OTC_CBitSet const& theCharSet);
				// Removes trailing instances of any
				// character in <theCharSet> from the end of
				// this string.

    OTC_String&		ltrim();
				// Removes leading white space from
				// this string.

    OTC_String&		ltrim(char theChar);
				// Removes leading instances of <theChar>
				// from the start of this string.

    OTC_String&		ltrim(OTC_CBitSet const& theCharSet);
				// Removes leading instances of any
				// character in <theCharSet> from the start
				// of this string.

    OTC_String&		trim()
				{ return rtrim().ltrim(); }
				// Removes trailing and leading white space
				// from this string.

    OTC_String&		trim(char theChar)
				{ return rtrim(theChar).ltrim(theChar); }
				// Removes trailing and leading instances
				// of <theChar> from this string.

    OTC_String&		trim(OTC_CBitSet const& theCharSet)
				{ return rtrim(theCharSet).ltrim(theCharSet); }
				// Removes trailing and leading instances
				// of any characters in <theCharSet> from
				// this string.

    // = SEARCHING

    int			index(
			 size_t theIndex,
			 char theChar,
			 size_t theNum=1
			) const
				{ return index_(theIndex,theChar,theNum); }
				// Returns the index of <theNum>'th instance
				// of <theChar> in this string appearing from
				// <theIndex> forward, or <-1> if there were
				// not that many instances of <theChar> in
				// the string.

    int			index(char theChar, size_t theNum=1) const
				{ return index_(0,theChar,theNum); }
				// Returns the index of <theNum>'th instance
				// of <theChar> nearest to the start of this
				// string or <-1> if there were not that
				// many instances of <theChar> in the string.

    int			rindex(char theChar, size_t theNum=1) const
				{ return rindex_(theChar,theNum); }
				// Returns the index of <theNum>'th instance
				// of <theChar> nearest to the end of this
				// string or <-1> if there were not that
				// many instances of <theChar> in the string.

    int			index(
			 size_t theIndex,
			 OTC_CBitSet const& theCharSet,
			 size_t theNum=1
			) const
				{ return index_(theIndex,theCharSet,theNum); }
				// Returns the index of <theNum>'th instance
				// of any character from <theCharSet> in this
				// string appearing from <theIndex> forward,
				// or <-1> if there weren't that many
				// instances of characters from <theCharSet>
				// in the string.

    int			index(
			 OTC_CBitSet const& theCharSet,
			 size_t theNum=1
			) const
				{ return index_(0,theCharSet,theNum); }
				// Returns the index of <theNum>'th instance
				// of any character from <theCharSet> nearest
				// to the start of this string or <-1> if
				// there were not that many instances of
				// characters from <theCharSet> in the string.

    int			rindex(
			 OTC_CBitSet const& theCharSet,
			 size_t theNum=1
			) const
				{ return rindex_(theCharSet,theNum); }
				// Returns the index of <theNum>'th instance
				// of any character from <theCharSet> nearest
				// to the end of this string or <-1> if there
				// were not that many instances of characters
				// from <theCharSet> in the string.

    int			index(
			 size_t theIndex,
			 char const* theString
			) const
				{
				  return index_(theIndex,theString,
				   (theString ? ::strlen(theString) : 0));
				}
				// Returns the index of the first instance
				// of <theString> in this string, appearing
				// beyond <theIndex>. If <theString> doesn't
				// match against this string then <-1> is
				// returned.

    int			index(char const* theString) const
				{
				  return index_(0,theString,
				   (theString ? ::strlen(theString) : 0));
				}
				// Returns the index of the first instance
				// of <theString>, in this string. If
				// <theString> doesn't match against
				// this string then <-1> is returned.

    int			index(
			 size_t theIndex,
			 char const* theString,
			 size_t theLength
			) const
				{ return index_(theIndex,theString,theLength); }
				// Returns the index of the first instance
				// of <theString> in this string, appearing
				// beyond <theIndex>. If <theString> doesn't
				// match against this string then <-1> is
				// returned. <theLength> is the length of
				// <theString>.

    int			index(
			 char const* theString,
			 size_t theLength
			) const
				{ return index_(0,theString,theLength); }
				// Returns the index of the first instance
				// of <theString>, in this string. If
				// <theString> doesn't match against
				// this string then <-1> is returned.
				// <theLength> is the length of <theString>.

    int			index(
			 size_t theIndex,
			 OTC_String const& theString
			) const
				{
				  return index_(theIndex,theString,
				   theString.length());
				}
				// Returns the index of the first instance
				// of <theString> in this string appearing
				// beyond <theIndex>. If <theString> doesn't
				// match against this string then <-1> is
				// returned.

    int			index(OTC_String const& theString) const
				{
				  return index_(0,theString,
				   theString.length());
				}
				// Returns the index of the first instance
				// of <theString> in this string. If
				// <theString> doesn't match against
				// this string then <-1> is returned.

    int			rindex(char const* theString) const
				{
				  return rindex_(theString,
				   (theString ? ::strlen(theString) : 0));
				}
				// Returns the index of the first instance
				// of <theString> nearest to the end of this
				// string or <-1> if there is no match.

    int			rindex(char const* theString, size_t theLength) const
				{ return rindex_(theString,theLength); }
				// Returns the index of the first instance
				// of <theString> nearest to the end of this
				// string or <-1> if there is no match.
				// <theLength> is the length of <theString>.

    int			rindex(OTC_String const& theString) const
				{
				  return rindex_(theString.string(),
				   theString.length());
				}
				// Returns the index of the first instance
				// of <theString> nearest to the end of this
				// string or <-1> if there is no match.

  public:

#if !defined(strchr) && !defined(strrchr)

    // Following exist to get around problems which can arise when the X
    // Windows header files start to play games by defining a macro for
    // "index" and "rindex". Things are okay if the X Windows header files
    // are included before this file as "index" and "rindex" are undefined at
    // the start of this file, however, if the X Windows header files are
    // included after this file, "index" can be redefined to "strchr" and
    // "rindex" to "strrchr" with the above functions becoming inaccessable.
    // We cheat by defining equivalents of the above functions using the
    // names that "index" and "rindex" are defined to. The following
    // functions should never be used explicitly, use those named with
    // "index" and "rindex" instead.

    int			strchr(
			 size_t theIndex,
			 char theChar,
			 size_t theNum=1
			) const
				{ return index_(theIndex,theChar,theNum); }

    int			strchr(char theChar, size_t theNum=1) const
				{ return index_(0,theChar,theNum); }

    int			strrchr(char theChar, size_t theNum=1) const
				{ return rindex_(theChar,theNum); }

    int			strchr(
			 size_t theIndex,
			 OTC_CBitSet const& theCharSet,
			 size_t theNum=1
			) const
				{ return index_(theIndex,theCharSet,theNum); }

    int			strchr(
			 OTC_CBitSet const& theCharSet,
			 size_t theNum=1
			) const
				{ return index_(0,theCharSet,theNum); }

    int			strrchr(
			 OTC_CBitSet const& theCharSet,
			 size_t theNum=1
			) const
				{ return rindex_(theCharSet,theNum); }

    int			strchr(
			 size_t theIndex,
			 char const* theString
			) const
				{
				  return index_(theIndex,theString,
				   (theString ? ::strlen(theString) : 0));
				}

    int			strchr(char const* theString) const
				{
				  return index_(0,theString,
				   (theString ? ::strlen(theString) : 0));
				}

    int			strchr(
			 size_t theIndex,
			 char const* theString,
			 size_t theLength
			) const
				{ return index_(theIndex,theString,theLength); }

    int			strchr(
			 char const* theString,
			 size_t theLength
			) const
				{ return index_(0,theString,theLength); }

    int			strchr(
			 size_t theIndex,
			 OTC_String const& theString
			) const
				{
				  return index_(theIndex,theString,
				   theString.length());
				}

    int			strchr(OTC_String const& theString) const
				{
				  return index_(0,theString,
				   theString.length());
				}

    int			strrchr(char const* theString) const
				{
				  return rindex_(theString,
				   (theString ? ::strlen(theString) : 0));
				}

    int			strrchr(char const* theString, size_t theLength) const
				{ return rindex_(theString,theLength); }

    int			strrchr(OTC_String const& theString) const
				{
				  return rindex_(theString.string(),
				   theString.length());
				}

#endif

    // = RANKING
    //     In the following functions, a return value of <-1> indicates
    //     that this string ranks less than that being compared against.
    //     A value of <0> indicates that the strings are equivalent, and a
    //     value of <1> indicates that this string ranks greater than that
    //     being compared against. Each of the <compare()> functions accept
    //     an optional argument to indicate the convention to be used to
    //     determine ordering. The three possible values for this agrument
    //     are <OTCLIB_ASCIIORDER>, <OTCLIB_ASCIIORDER_IGNORECASE> and
    //     <OTCLIB_LOCALEORDER>. If the operating system doesn't provide
    //     the <strcoll()> function, the locale ordering will be the
    //     same as ASCII ordering. Note that locale ordering using <strcoll()>
    //     will give incorrect results if the string contains embedded
    //     null characters. This is because <strcoll()> will only compare
    //     the strings up till the first embedded null character in each.
    //     The result being that anything after the first null character
    //     will be ignored. If embedded nulls are detected no warning is
    //     given that it will not work as you expect. Using locale ordering
    //     is not as efficient as ASCII ordering. If you want to be able to
    //     use locale ordering where a string is used as a key in a
    //     collection, use <OTC_LString> as the argument to the template
    //     collection.

    int			rank(
			 OTC_String const& theString,
			 OTC_SRankType theType=OTCLIB_ASCIIORDER
			) const
			{
			  return OTC_Rank::rank(data_.string(),data_.length(),
			   theString.data_.string(),theString.data_.length(),
			   theType);
			}
				// Performs a comparison between this string
				// and <theString> and return a value
				// corresponding to how the two strings
				// rank.

    int			rank(
			 char const* theString,
			 OTC_SRankType theType=OTCLIB_ASCIIORDER
			) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),theString,
				   (theString ? ::strlen(theString) : 0),
				   theType);
				}
				// Performs a comparison between this string
				// and <theString> and return a value
				// corresponding to how the two strings
				// rank.

    int			rank(
			 char const* theString,
			 size_t theLength,
			 OTC_SRankType theType=OTCLIB_ASCIIORDER
			) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),theString,theLength,theType);
				}
				// Performs a comparison between this string
				// and the first <theLength> characters of
				// <theString> and return a value
				// corresponding to how the two strings rank.

    int			rank(
			 OTC_String const& theString,
			 size_t theLength,
			 OTC_SRankType theType=OTCLIB_ASCIIORDER
			) const;
				// Performs a comparison between this string
				// and the first <theLength> characters of
				// <theString> and return a value
				// corresponding to how the two strings rank.
				// If <theString> is less than <theLength>
				// characters in length, an exception is
				// raised.

    // = HASHING

    int			hash() const
				{
				  return OTC_Hash::hash(data_.string(),
				   data_.length());
				}
				// Returns a hash value for this string.

    // = COMPARISION
    //     Note that for the <compare()> functions, a value of <0> for
    //     <theString> is not interpreted as being the same as a null
    //     terminated string. As a result, if <theString> is <0> then
    //     <false> will always be returned, even if the length of this
    //     string is <0>. Also, each of the <compare()> functions accept an
    //     optional argument to indicate whether an exact match is expected
    //     or whether case can be ignored. The two corresponding values to
    //     indicate this are <OTCLIB_EXACTMATCH> and <OTCLIB_IGNORECASE> with
    //     the default being an exact match.

    bool		compare(
			 size_t theIndex,
			 char const* theString,
			 OTC_SCmpType theType=OTCLIB_EXACTMATCH
			) const;
				// Returns <true> if <theString> is
				// equivalent to the portion of this string
				// starting at <theIndex> through to the end
				// of this string. If <theIndex> is greater
				// than the length of this string an
				// exception is raised.

    bool		compare(
			 char const* theString,
			 OTC_SCmpType theType=OTCLIB_EXACTMATCH
			) const
				{ return compare(0,theString,theType); }
				// Returns <true> if <theString> is
				// equivalent to this string.

    bool		compare(
			 size_t theIndex,
			 char const* theString,
			 size_t theNum,
			 OTC_SCmpType theType=OTCLIB_EXACTMATCH
			) const;
				// Returns <true> if <theNum> characters of
				// this string starting at <theIndex>, are
				// the same as the first <theNum> characters
				// of <theString>. If <theIndex> is greater
				// than the length of this string an
				// exception is raised. If <theIndex> plus
				// <theNum> is greater than the length of
				// this string, <false> will always be
				// returned.

    bool		compare(
			 size_t theIndex,
			 OTC_String const& theString,
			 OTC_SCmpType theType=OTCLIB_EXACTMATCH
			) const;
				// Returns <true> if <theString> is
				// equivalent to the portion of this string
				// starting at <theIndex> through to the
				// end of this string. If <theIndex> is
				// greater than the length of this string
				// an exception is raised.

    bool		compare(
			 OTC_String const& theString,
			 OTC_SCmpType theType=OTCLIB_EXACTMATCH
			) const
				{ return compare(0,theString,theType); }
				// Returns <true> if <theString> is
				// equivalent to this string.

    bool		compare(
			 size_t theIndex,
			 OTC_String const& theString,
			 size_t theNum,
			 OTC_SCmpType theType=OTCLIB_EXACTMATCH
			) const;
				// Returns <true> if <theNum> characters of
				// this string starting at <theIndex> are the
				// same as the first <theNum> characters of
				// <theString>. If <theIndex> is greater than
				// the length of this string, or <theNum> is
				// greater than the length of <theString>, an
				// exception is raised. If <theIndex> plus
				// <theNum> is greater than the length of
				// this string, <false> will always be
				// returned.

    // Following all return <true> if the particular comparison is
    // satisfied. Ordering is based on ASCII comparison and not on locale
    // ordering.

    bool		operator==(char c1) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),&c1,1) == 0;
				}

    bool		operator==(char const* s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2,
				   (s2 ? ::strlen(s2) : 0)) == 0;
				}

    bool		operator==(OTC_String const& s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2.string(),
				   s2.length()) == 0;
				}

    bool		operator!=(char c1) const
				{
				  return OTC_Rank::rank(data_.string(),
				  data_.length(),&c1,1) != 0;
				}

    bool		operator!=(char const* s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2,
				   (s2 ? ::strlen(s2) : 0)) != 0;
				}

    bool		operator!=(OTC_String const& s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2.string(),
				   s2.length()) != 0;
				}

    bool		operator<=(char c1) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),&c1,1) <= 0;
				}

    bool		operator<=(char const* s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2,
				   (s2 ? ::strlen(s2) : 0)) <= 0;
				}

    bool		operator<=(OTC_String const& s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2.string(),
				   s2.length()) <= 0;
				}

    bool		operator>=(char c1) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),&c1,1) >= 0;
				}

    bool		operator>=(char const* s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2,
				   (s2 ? ::strlen(s2) : 0)) >= 0;
				}

    bool		operator>=(OTC_String const& s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				  data_.length(),s2.string(),
				  s2.length()) >= 0;
				}

    bool		operator<(char c1) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),&c1,1) < 0;
				}

    bool		operator<(char const* s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2,
				   (s2 ? ::strlen(s2) : 0)) < 0;
				}

    bool		operator<(OTC_String const& s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2.string(),
				   s2.length()) < 0;
				}

    bool		operator>(char c1) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),&c1,1) > 0;
				}

    bool		operator>(char const* s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2,
				   (s2 ? ::strlen(s2) : 0)) > 0;
				}

    bool		operator>(OTC_String const& s2) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),s2.string(),
				   s2.length()) > 0;
				}

    friend OSE_EXPORT bool	operator==(
				 char c1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(&c1,1,
				   s2.string(),s2.length()) == 0;
				}

    friend OSE_EXPORT bool	operator==(
				 char const* s1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(s1,
				   (s1 ? strlen(s1) : 0),
				   s2.string(),s2.length()) == 0;
				}

    friend OSE_EXPORT bool	operator!=(
				 char c1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(&c1,1,
				   s2.string(),s2.length()) != 0;
				}

    friend OSE_EXPORT bool	operator!=(
				 char const* s1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(s1,
				   (s1 ? strlen(s1) : 0),
				   s2.string(),s2.length()) != 0;
				}

    friend OSE_EXPORT bool	operator<=(
				 char c1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(&c1,1,
				   s2.string(),s2.length()) <= 0;
				}

    friend OSE_EXPORT bool	operator<=(
				 char const* s1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(s1,
				   (s1 ? strlen(s1) : 0),
				   s2.string(),s2.length()) <= 0;
				}

    friend OSE_EXPORT bool	operator>=(
				 char c1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(&c1,1,
				   s2.string(),s2.length()) >= 0;
				}

    friend OSE_EXPORT bool	operator>=(
				 char const* s1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(s1,
				   (s1 ? strlen(s1) : 0),
				   s2.string(),s2.length()) >= 0;
				}

    friend OSE_EXPORT bool	operator>(
				 char c1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(&c1,1,
				   s2.string(),s2.length()) > 0;
				}

    friend OSE_EXPORT bool	operator>(
				 char const* s1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(s1,
				   (s1 ? strlen(s1) : 0),
				   s2.string(),s2.length()) > 0;
				}

    friend OSE_EXPORT bool	operator<(
				 char c1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(&c1,1,
				   s2.string(),s2.length()) < 0;
				}

    friend OSE_EXPORT bool	operator<(
				 char const* s1,
				 OTC_String const& s2
				)
				{
				  return OTC_Rank::rank(s1,
				   (s1 ? strlen(s1) : 0),
				   s2.string(),s2.length()) < 0;
				}

  private:

			OTC_String(size_t theCapacity)
			  : data_(theCapacity,OTCLIB_UNBUFFERED)
			  	{ data_.reallocate(0); }
				// Creates an empty string with enough
				// underlying capacity to hold a string of
				// size <theCapacity>. The string will
				// initially have zero length but will not
				// register as being undefined. Made private
				// as no longer provided. Being private
				// should result in compiler error.

    // Privatise the following functions so that we do not get weird things
    // happening in code unexpectedly. Cannot block implicit use of char*
    // converison operator though.

    void		operator*() const {}

    void		operator~() const {}

    void		operator!() const {}

    // Implementation functions. What they do is easily derived by looking
    // at public equivalents.

    void		replace_(
			 long theStart,
			 size_t theLength,
			 char theChar,
			 size_t theNum=1
			);

    void		replace_(
			 long theStart,
			 size_t theLength,
			 char const* theString,
			 size_t theNum
			);

    void		replace_(
			 long theStart,
			 size_t theLength,
			 OTC_String const& theString,
			 size_t theNum
			);

    OTC_String		section_(long theStart, long theLength) const;

    OTC_String		except_(long theStart, size_t theLength) const;

    void		upper_(long theStart, size_t theLength);

    void		lower_(long theStart, size_t theLength);

    int			index_(
			 size_t theIndex,
			 char theChar,
			 size_t theNum=1
			) const;

    int			rindex_(
			 char theChar,
			 size_t theNum=1
			) const;

    int			index_(
			 size_t theIndex,
			 OTC_CBitSet const& theCharSet,
			 size_t theNum=1
			) const;

    int			rindex_(
			 OTC_CBitSet const& theCharSet,
			 size_t theNum=1
			) const;

    int			index_(
			 size_t theIndex,
			 char const* theString,
			 size_t theLength
			) const;

    int			rindex_(
			 char const* theString,
			 size_t theLength
			) const;

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static bool		globInitialised_;
				// True if the static string instances
				// have been initialised.

    static OTC_String const*	globNullString_;
				// String returned by <nullString()>.

    static OTC_String const*	globUndefinedString_;
				// String returned by <undefinedString()>.

    OTC_RString		data_;
				// The raw string data.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_RankActions<OTC_String>
{
  public:
    static int		rank(OTC_String const& s1, OTC_String const& s2)
				{ return s1.rank(s2); }
};

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_HashActions<OTC_String>
{
  public:
    static int		hash(OTC_String const& s)
				{ return s.hash(); }
};


/* ------------------------------------------------------------------------- */

OSE_EXPORT istream& operator>>(istream& ins, OTC_String& theString);
OSE_EXPORT ostream& operator<<(ostream& outs, OTC_String const& theString);

OSE_EXPORT OTC_TString operator+(OTC_String const& s1, OTC_String const& s2);
OSE_EXPORT OTC_TString operator+(OTC_String const& s1, char const* s2);
OSE_EXPORT OTC_TString operator+(char const* s1, OTC_String const& s2);
OSE_EXPORT OTC_TString operator+(OTC_SObject const& s1, OTC_String const& s2);
OSE_EXPORT OTC_TString operator+(OTC_SObject const& s1, char const* s2);
OSE_EXPORT OTC_TString operator+(OTC_String const& s1, OTC_SObject const& s2);
OSE_EXPORT OTC_TString operator+(OTC_String const& s1, char c1);
OSE_EXPORT OTC_TString operator+(char c1, OTC_String const& s1);

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_STRING_HH */
