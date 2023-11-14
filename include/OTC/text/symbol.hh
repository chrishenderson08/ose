#ifndef OTC_TEXT_SYMBOL_HH
#define OTC_TEXT_SYMBOL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/symbol.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/hashactn.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/symbol.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Symbol : public OTC_SObject
    // = TITLE
    //	   Symbol class for a string identifier.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   The <OTC_Symbol> class is an alternative to using the <OTC_String>
    //	   class as an identifier. The benefits of this class are that it
    //	   doesn't actually keep a copy of the string in the class. Instead,
    //	   all strings are kept in a central symbol table with this class
    //	   holding a pointer to the central string. This allows equality tests
    //	   to be very quick as it requires a pointer comparison only. Also,
    //	   as there is only one copy of the string in a central symbol table
    //	   excess memory will not be consumed due to duplicate copies of the
    //	   string.
    //	   
    // = SEE ALSO
    //	   <OTC_String>
{
  public:

			~OTC_Symbol();

    // = CONSTRUCTION

			OTC_Symbol()
			  : data_("",0)
				{}
				// Creates an instance of a symbol class
				// to represent the null string.

			OTC_Symbol(OTC_Symbol const& theSymbol)
			  : OTC_SObject(), data_(theSymbol.data_) {}
				// Creates an instance of the symbol
				// class which is the same as <theSymbol>.

			OTC_Symbol(char const* theString)
			  :  data_(theString,(theString?::strlen(theString):0))
				{}
				// Creates an instance of the symbol
				// class to represent <theString>.

			OTC_Symbol(char const* theString, size_t theLength)
			  : data_(theString,theLength)
				{}
				// Creates an instance of the symbol
				// class to represent the first <theLength>
				// characters of <theString>.

			OTC_Symbol(char theChar, size_t theNum=1);
				// Creates an instance of the symbol
				// class to represent a string containing
				// <theNum> instances of <theString>.

			OTC_Symbol(OTC_String const& theString);
				// Creates an instance of the symbol class to
				// represent <theString>. If <theString>
				// represents a symbol, ie., it was created
				// through a conversion of a symbol to a
				// string, the conversion back to a symbol is
				// trivial provided that <theString> hadn't
				// been used in such a way that the delayed
				// copy linkage was broken.

			OTC_Symbol(
			 OTC_String const& theString,
			 size_t theLength
			);
				// Creates an instance of the symbol
				// class to represent the first <theLength>
				// characters of <theString>.

			OTC_Symbol(OTC_TString const& theString);
				// Creates an instance of the symbol class to
				// represent <theString>.

    OTC_Symbol&		operator=(OTC_Symbol const& theSymbol)
				{ data_ = theSymbol.data_; return *this; }
				// Sets this symbol to be the same as
				// <theSymbol>.

    // = QUERY

    bool		isEmpty() const
				{ return data_.length() == 0; }
				// Returns <true> if this symbol is that of
				// an empty string.

    char		operator[](size_t theIndex) const;
				// Returns the character at the location in
				// the string for this symbol, given by
				// <theIndex>. If <theIndex> is greater than
				// or equal to the length of the string for
				// this symbol, an exception is raised.

    char const*		string() const
				{ return data_.string(); }
				// Returns a pointer to the character
				// string for the symbol.

    size_t		length() const
				{ return data_.length(); }
				// Returns the length of the character
				// string for the symbol.

    static bool		exists(char const* theString)
				{
				  return exists(theString,
				   (theString ? ::strlen(theString) : 0));
				}
				// Returns <true> if <theString> already
				// exists as a symbol. The null symbol always
				// exists.

    static bool		exists(char const* theString, size_t theLength);
				// Returns <true> if the symbol given by the
				// first <theLength> characters of
				// <theString> already exists as a symbol.
				// The null symbol always exists.

    // = INEQUALITY

    bool		operator==(OTC_Symbol const& theSymbol) const
				{
				  return data_.string() ==
				   theSymbol.data_.string();
				}
				// Returns <true> if this symbol is the same
				// as <theSymbol>.

    bool		operator!=(OTC_Symbol const& theSymbol) const
				{
				  return data_.string() !=
				   theSymbol.data_.string();
				}
				// Returns <true> if this symbol is not the
				// same as <theSymbol>.

    // Following versions are provided so that comparisons against
    // instances of <OTC_String> and <char const*> will not result in
    // an entry being added to the symbol database.

    bool		operator==(OTC_String const& theString) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),theString.string(),
				   theString.length()) == 0;
				}
				// Returns <true> if this symbol is the same
				// as <theString>.

    bool		operator!=(OTC_String const& theString) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),theString.string(),
				   theString.length()) != 0;
				}
				// Returns <true> if this symbol is not the
				// same as <theString>.

#if 0

    friend OSE_EXPORT bool	operator==(
			 OTC_String const& theString,
			 OTC_Symbol const& theSymbol
			)
				{ return theSymbol == theString; }
				// Returns <true> if <theSymbol> is the same
				// as <theString>.

    friend OSE_EXPORT bool	operator!=(
			 OTC_String const& theString,
			 OTC_Symbol const& theSymbol
			)
				{ return theSymbol != theString; }
				// Returns <true> if <theSymbol> is not the
				// same as <theString>.

#endif

    bool		operator==(char const* theString) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),theString,
				   (theString?::strlen(theString):0)) == 0;
				}
				// Returns <true> if this symbol is the same
				// as <theString>.

    bool		operator!=(char const* theString) const
				{
				  return OTC_Rank::rank(data_.string(),
				   data_.length(),theString,
				   (theString?::strlen(theString):0)) != 0;
				}
				// Returns <true> if this symbol is not the
				// same as <theString>.

#if 0

    friend OSE_EXPORT bool	operator==(
			 char const* theString,
			 OTC_Symbol const& theSymbol
			)
				{ return theSymbol == theString; }
				// Returns <true> if <theSymbol> is the same
				// as <theString>.

    friend OSE_EXPORT bool	operator!=(
			 char const* theString,
			 OTC_Symbol const& theSymbol
			)
				{ return theSymbol != theString; }
				// Returns <true> if <theSymbol> is not the
				// same as <theString>.

#endif

    // = RANKING

    int			rank(OTC_Symbol const& theSymbol) const
				{
				  return long(data_.string()) -
				   long(theSymbol.data_.string());
				}
				// Ranks this symbol against <theSymbol>.

    // = HASHING

    int			hash() const
				{ return long(data_.string()); }
				// Returns a hash value for the symbol.

    // = NULL SYMBOL

    static OTC_Symbol const&	nullSymbol();
				// Returns a reference to the null symbol.

    // = DISPLAY

#if 0

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& outs,
				 OTC_Symbol const& theSymbol
				);
				// Prints <theSymbol> on the stream <outs>.

#endif

  public:

    // = CONVERSION

    OTC_RString		rawString() const;
				// Returns the string which the symbol
				// represents.

  private:

    OTC_RString		data_;
				// The raw string data.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static OTC_Symbol*	globNullSymbol_;
				// Symbol for null string.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_RankActions<OTC_Symbol>
{
  public:
    static int		rank(OTC_Symbol const& s1, OTC_Symbol const& s2)
				{ return s1.rank(s2); }
};

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_HashActions<OTC_Symbol>
{
  public:
    static int		hash(OTC_Symbol const& s)
				{ return s.hash(); }
};

/* ------------------------------------------------------------------------- */

inline bool operator==(
 OTC_String const& theString,
 OTC_Symbol const& theSymbol
)
{ return theSymbol == theString; }

inline bool operator!=(
 OTC_String const& theString,
 OTC_Symbol const& theSymbol
)
{ return theSymbol != theString; }

inline bool operator==(
 char const* theString,
 OTC_Symbol const& theSymbol
)
{ return theSymbol == theString; }

inline bool operator!=(
 char const* theString,
 OTC_Symbol const& theSymbol
)
{ return theSymbol != theString; }

OSE_EXPORT ostream& operator<<(
 ostream& outs,
 OTC_Symbol const& theSymbol
);

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_SYMBOL_HH */
