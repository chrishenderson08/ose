#ifndef OTC_TEXT_RSTRING_HH
#define OTC_TEXT_RSTRING_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/rstring.hh
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

#include <OTC/memory/mpobject.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/atomic.hh>
#include <OTC/misc/bufflag.hh>

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/rstring.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_HashTable;

enum OTC_BufferingStrategy
    // = TITLE
    //     Indicates buffering strategy used in the string class.
{
  OTCLIB_BUFFERING_UNKNOWN,
  OTCLIB_BUFFERING_SCHEME0,
  OTCLIB_BUFFERING_SCHEME1,
  OTCLIB_BUFFERING_SCHEME2
};

class OSE_EXPORT OTC_StringData : public OTC_MPObject
    // = TITLE
    //     Holder of memory for a string.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     Holds the actual memory for a string. Also contains capacity,
    //     length and  reference count information. This data needs to
    //     be in a separate class so reference count can still be adjusted
    //     for a const string object.
{
  public:

			OTC_StringData() {}

			~OTC_StringData();

    size_t		capacity;
				// The capacity of the memory allocated.
				// This must always be at least one greater
				// that the length.

    size_t		length;
				// The amount of memory in use. This does
				// not count the null terminator.

    OTC_Atomic		count;
				// The number of references to this piece of
				// memory.

    short		spare;
				// This is not being used and is spare.

    short		symbol;
				// Whether the memory represents a symbol.

    char*		data;
				// The actual memory.
};

class OSE_EXPORT OTC_RString
    // = TITLE
    //	   Envelope class for holder of string memory.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_RString> class is a wrapper for <OTC_StringData>. This
    //     class implements the functionality for maintaining the string
    //     memory and the delayed copy mechanism. The <OTC_StringData> class
    //     holds all the data required. This class ensures that the string is
    //     null terminated on initial allocation and reallocation. If you
    //     overwrite the null terminator it is your own problem.
    //
    // = NOTES
    //     Thread locking is performed around code related to the
    //     manipulation of the reference count used for the delayed copy
    //     mechanism. This is necessary as two threads could validly access
    //     the shared data before a delayed copy is broken. Lock is done by
    //     function not for each instance of the class. The functions where
    //     this locking occurs is <operator=()> and <reallocate()>. Because
    //     locking is done by function, this may or may not reduce possible
    //     concurrency when there is large amounts of string resizing and
    //     assignment.
{
  public:

			~OTC_RString();

    // = INITIALISATION

			OTC_RString(
			 size_t theLength,
			 OTC_BufferingFlag theType=OTCLIB_BUFFERED
			);
				// Allocates string memory for string of
				// length <theLength>. If <theType> is
				// <OTCLIB_UNBUFFERED>, only enough memory
				// to hold the string and a null terminator
				// will initially be allocated. If <theType>
				// is <OTCLIB_BUFFERED>, the default,
				// additional memory may be allocated to try
				// to reduce memory allocations on changes in
				// the length of the string.

			OTC_RString(char const* theString, size_t theLength);
				// This constructor is radically different
				// to other constructors in that it accepts
				// as argument the actual character string.
				// The constructor also behaves quite
				// differently so don't just assume it is
				// similar to the others. When this
				// constructor is used, the character string
				// defined by <theString> and <theLength> is
				// looked up in an internal symbol table. If
				// the string already exists, this class will
				// reference the existing string data. If the
				// string doesn't exist in the symbol table,
				// new memory will be allocated to hold the
				// string and the string added to the symbol
				// table. A reference to that data will then
				// be returned. The object created by this
				// constructor is distinguishable from
				// others due to <isSymbol()> returning
				// <true>. You should not modify the actual
				// string contents for the type of object
				// created by this class. If you do you will
				// corrupt the symbol table indexing
				// resulting in bogus entries in the symbol
				// table. This constructor is primarily
				// intended only to be used by the
				// <OTC_Symbol> class.

			OTC_RString(OTC_RString const& theString)
			  : data_(0)
			  	{ *this = theString; }
				// Results in this class referencing the same
				// memory as <theString>.

    // = QUERY

    size_t		length() const
				{ return data_->length; }
				// Returns the length of the string.
				// Does not include the null terminator.

    size_t		capacity() const
				{ return data_->capacity-1; }
				// Returns the capacity of the string.
				// Doesn't include the cell which is
				// reserved for the null terminator.

    bool		isShared() const
				{ return (data_->count != 1); }
				// Returns <true> if this class references a
				// chunk of memory which is also being
				// referenced by another instance of the
				// class.

    bool		isSymbol() const
				{ return data_->symbol != 0; }
				// Returns <true> if the string is indexed by
				// the symbol table.

    // = ACCESS
    //     Note that the following functions do nothing special if another
    //     instance of the class is referencing the same string. If
    //     modifications are going to be made to the memory, either
    //     <sync()> or <reallocate()> should be invoked to force a separate
    //     copy to be made.
    //
    //     If you overwrite the null terminator added by this class via
    //     these functions, it is your problem.

    char const*		string() const
      				{ return data_->data; }
				// Returns a pointer to the string.

    char*		string()
      				{ return data_->data; }
				// Returns a pointer to the string.

    char		operator[](size_t theIndex) const
				{ return data_->data[theIndex]; }
				// Returns a reference to the character at
				// position <theIndex> into the string. No
				// bounds checking is performed.

    char&		operator[](size_t theIndex)
				{ return data_->data[theIndex]; }
				// Returns a reference to the character at
				// position <theIndex> into the string. No
				// bounds checking is performed.

    // = ALLOCATION

    void		reallocate(
			 size_t theLength,
			 OTC_BufferingFlag theType=OTCLIB_BUFFERED
			);
				// Similar to invoking <realloc(3)>, this
				// function will result in a new string being
				// allocated with sufficient size to hold
				// <theLength> characters. If <theLength> is
				// less than or equal to the current capacity
				// of this string and the string is not also
				// being referenced by another instance of
				// this class, no reallocation will be
				// made, instead the same piece of memory
				// will be used, the length adjusted and a
				// new null terminator inserted. If a new
				// string is allocated, the contents of
				// the old string will be copied into the
				// new. In doing this, instances of this
				// class which previously shared the same
				// string as this class will no longer do so,
				// instead they will continue to reference
				// the old string.

    void		sync()
				{ reallocate(data_->length); }
				// If the string is shared, this forces a
				// copy to be made. It is equivalent to
				// invoking <reallocate(length())>.

    // = ASSIGNMENT

    OTC_RString const&	operator=(OTC_RString const& theString);
				// Results in the current string being
				// discarded and the one held by <theString>
				// to be referenced instead. The old string
				// will only be deleted if no other instances
				// of the class reference it.

    // = SYMBOL TABLE

    static bool		symbolExists(char const* theString, size_t theLength);
				// Returns <true> if the string described by
				// <theString> and <theLength> is currently
				// listed in the symbol database.

    // = STREAMS OUTPUT

#if 0

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& outs,
				 OTC_RString const& theString
				);
				// Dumps <theString> to the stream <outs>.
				// Width and justification specifications
                                // are honoured.

#endif

  private:

    size_t		capacity_(
			 size_t theLength,
			 OTC_BufferingFlag theType
			) const;
				// Calculates and returns the capacity
				// required to hold a string of <theLength>
				// when <theType> if buffering scheme is
				// in use. This is to facilitate performance
				// improvement through buffering. Doesn't
				// modify the current capacity, just works
				// out what it should be for a suggested
				// value of <theLength>.

    char*		allocate_(size_t theSize) const;
    				// Allocates a piece of memory of <theSize>.

    void		release_(char* theMemory, size_t theSize) const;
    				// Releases <theMemory> with <theSize>.

    static OTC_NRMutex	mutex_;
				// Lock for threads. This one is for global
				// data.

    static OTC_NRMutex	rcmutex_;
				// Lock for threads. This one is to protect
				// code manipulating the reference counts.

    static OTC_NRMutex	stmutex_;
				// Lock for threads. This one is to protect
				// code manipulating the symbol table.

    static bool		initialised_;
				// Indicates if buffering strategy has been
				// set.

    static OTC_BufferingStrategy	bufType_;
				// Strategy used for memory allocation.

    static OTC_HashTable*	index_;
				// Index for symbol table database.

    OTC_StringData*	data_;
				// Actual data.
};

/* ========================================================================= */

OSE_EXPORT ostream& operator<<(ostream& outs, OTC_RString const& theString);

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_RSTRING_HH */
