// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file bison.tab.hh
 ** Define the ts::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_BISON_TAB_HH_INCLUDED
# define YY_YY_BISON_TAB_HH_INCLUDED
// "%code requires" blocks.
#line 11 "bison.y"


   namespace ts {
      class tsCompiler;
      class tsScanner;
   }

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif


#line 65 "bison.tab.hh"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"
#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 7 "bison.y"
namespace ts {
#line 200 "bison.tab.hh"




  /// A Bison parser.
  class tsParser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator= (const self_type&);
    semantic_type (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // expression
      // value
      char dummy1[sizeof (size_t)];

      // tstTRUE
      // tstFALSE
      // tstCONST_INT
      // tstCONST_FLOAT
      // tstCONST_STRING
      // tstIDENTIFIER
      char dummy2[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        tstEND = 258,
        tstTRUE = 259,
        tstFALSE = 260,
        tstCONST_INT = 261,
        tstCONST_FLOAT = 262,
        tstCONST_STRING = 263,
        tstDEF_BOOL = 264,
        tstDEF_INT = 265,
        tstDEF_FLOAT = 266,
        tstDEF_STRING = 267,
        tstOPEN_BRACKET = 268,
        tstCLOSE_BRACKET = 269,
        tstOPEN_CBRACKET = 270,
        tstCLOSE_CBRACKET = 271,
        tstOPEN_PAREN = 272,
        tstCLOSE_PAREN = 273,
        tstIDENTIFIER = 274,
        tstIF = 275,
        tstWHILE = 276,
        tstFOR = 277,
        tstADD = 278,
        tstSUB = 279,
        tstMUL = 280,
        tstDIV = 281,
        tstLESS = 282,
        tstMORE = 283,
        tstEQUAL = 284,
        tstLESS_EQUAL = 285,
        tstMORE_EQUAL = 286,
        tstAND = 287,
        tstOR = 288,
        tstNOT = 289,
        tstEXP_END = 290,
        tstGLOBAL_REF = 291,
        tstGLOBAL_IN = 292
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef signed char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that);
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, size_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const size_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        // User destructor.
        symbol_number_type yytype = this->type_get ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yytype)
        {
       default:
          break;
        }

        // Type destructor.
switch (yytype)
    {
      case 44: // expression
      case 45: // value
        value.template destroy< size_t > ();
        break;

      case 4: // tstTRUE
      case 5: // tstFALSE
      case 6: // tstCONST_INT
      case 7: // tstCONST_FLOAT
      case 8: // tstCONST_STRING
      case 19: // tstIDENTIFIER
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_type (by_type&& that);
#endif

      /// Copy constructor.
      by_type (const by_type& that);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_type>
    {
      /// Superclass.
      typedef basic_symbol<by_type> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
      {
        YY_ASSERT (tok == 0 || tok == token::tstEND || tok == token::tstDEF_BOOL || tok == token::tstDEF_INT || tok == token::tstDEF_FLOAT || tok == token::tstDEF_STRING || tok == token::tstOPEN_BRACKET || tok == token::tstCLOSE_BRACKET || tok == token::tstOPEN_CBRACKET || tok == token::tstCLOSE_CBRACKET || tok == token::tstOPEN_PAREN || tok == token::tstCLOSE_PAREN || tok == token::tstIF || tok == token::tstWHILE || tok == token::tstFOR || tok == token::tstADD || tok == token::tstSUB || tok == token::tstMUL || tok == token::tstDIV || tok == token::tstLESS || tok == token::tstMORE || tok == token::tstEQUAL || tok == token::tstLESS_EQUAL || tok == token::tstMORE_EQUAL || tok == token::tstAND || tok == token::tstOR || tok == token::tstNOT || tok == token::tstEXP_END || tok == token::tstGLOBAL_REF || tok == token::tstGLOBAL_IN);
      }
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
      {
        YY_ASSERT (tok == 0 || tok == token::tstEND || tok == token::tstDEF_BOOL || tok == token::tstDEF_INT || tok == token::tstDEF_FLOAT || tok == token::tstDEF_STRING || tok == token::tstOPEN_BRACKET || tok == token::tstCLOSE_BRACKET || tok == token::tstOPEN_CBRACKET || tok == token::tstCLOSE_CBRACKET || tok == token::tstOPEN_PAREN || tok == token::tstCLOSE_PAREN || tok == token::tstIF || tok == token::tstWHILE || tok == token::tstFOR || tok == token::tstADD || tok == token::tstSUB || tok == token::tstMUL || tok == token::tstDIV || tok == token::tstLESS || tok == token::tstMORE || tok == token::tstEQUAL || tok == token::tstLESS_EQUAL || tok == token::tstMORE_EQUAL || tok == token::tstAND || tok == token::tstOR || tok == token::tstNOT || tok == token::tstEXP_END || tok == token::tstGLOBAL_REF || tok == token::tstGLOBAL_IN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::tstTRUE || tok == token::tstFALSE || tok == token::tstCONST_INT || tok == token::tstCONST_FLOAT || tok == token::tstCONST_STRING || tok == token::tstIDENTIFIER);
      }
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::tstTRUE || tok == token::tstFALSE || tok == token::tstCONST_INT || tok == token::tstCONST_FLOAT || tok == token::tstCONST_STRING || tok == token::tstIDENTIFIER);
      }
#endif
    };

    /// Build a parser object.
    tsParser (tsScanner  &scanner_yyarg, tsCompiler  &compiler_yyarg);
    virtual ~tsParser ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstEND (location_type l)
      {
        return symbol_type (token::tstEND, std::move (l));
      }
#else
      static
      symbol_type
      make_tstEND (const location_type& l)
      {
        return symbol_type (token::tstEND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstTRUE (std::string v, location_type l)
      {
        return symbol_type (token::tstTRUE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_tstTRUE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::tstTRUE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstFALSE (std::string v, location_type l)
      {
        return symbol_type (token::tstFALSE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_tstFALSE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::tstFALSE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstCONST_INT (std::string v, location_type l)
      {
        return symbol_type (token::tstCONST_INT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_tstCONST_INT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::tstCONST_INT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstCONST_FLOAT (std::string v, location_type l)
      {
        return symbol_type (token::tstCONST_FLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_tstCONST_FLOAT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::tstCONST_FLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstCONST_STRING (std::string v, location_type l)
      {
        return symbol_type (token::tstCONST_STRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_tstCONST_STRING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::tstCONST_STRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstDEF_BOOL (location_type l)
      {
        return symbol_type (token::tstDEF_BOOL, std::move (l));
      }
#else
      static
      symbol_type
      make_tstDEF_BOOL (const location_type& l)
      {
        return symbol_type (token::tstDEF_BOOL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstDEF_INT (location_type l)
      {
        return symbol_type (token::tstDEF_INT, std::move (l));
      }
#else
      static
      symbol_type
      make_tstDEF_INT (const location_type& l)
      {
        return symbol_type (token::tstDEF_INT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstDEF_FLOAT (location_type l)
      {
        return symbol_type (token::tstDEF_FLOAT, std::move (l));
      }
#else
      static
      symbol_type
      make_tstDEF_FLOAT (const location_type& l)
      {
        return symbol_type (token::tstDEF_FLOAT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstDEF_STRING (location_type l)
      {
        return symbol_type (token::tstDEF_STRING, std::move (l));
      }
#else
      static
      symbol_type
      make_tstDEF_STRING (const location_type& l)
      {
        return symbol_type (token::tstDEF_STRING, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstOPEN_BRACKET (location_type l)
      {
        return symbol_type (token::tstOPEN_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_tstOPEN_BRACKET (const location_type& l)
      {
        return symbol_type (token::tstOPEN_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstCLOSE_BRACKET (location_type l)
      {
        return symbol_type (token::tstCLOSE_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_tstCLOSE_BRACKET (const location_type& l)
      {
        return symbol_type (token::tstCLOSE_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstOPEN_CBRACKET (location_type l)
      {
        return symbol_type (token::tstOPEN_CBRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_tstOPEN_CBRACKET (const location_type& l)
      {
        return symbol_type (token::tstOPEN_CBRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstCLOSE_CBRACKET (location_type l)
      {
        return symbol_type (token::tstCLOSE_CBRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_tstCLOSE_CBRACKET (const location_type& l)
      {
        return symbol_type (token::tstCLOSE_CBRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstOPEN_PAREN (location_type l)
      {
        return symbol_type (token::tstOPEN_PAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_tstOPEN_PAREN (const location_type& l)
      {
        return symbol_type (token::tstOPEN_PAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstCLOSE_PAREN (location_type l)
      {
        return symbol_type (token::tstCLOSE_PAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_tstCLOSE_PAREN (const location_type& l)
      {
        return symbol_type (token::tstCLOSE_PAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstIDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::tstIDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_tstIDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::tstIDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstIF (location_type l)
      {
        return symbol_type (token::tstIF, std::move (l));
      }
#else
      static
      symbol_type
      make_tstIF (const location_type& l)
      {
        return symbol_type (token::tstIF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstWHILE (location_type l)
      {
        return symbol_type (token::tstWHILE, std::move (l));
      }
#else
      static
      symbol_type
      make_tstWHILE (const location_type& l)
      {
        return symbol_type (token::tstWHILE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstFOR (location_type l)
      {
        return symbol_type (token::tstFOR, std::move (l));
      }
#else
      static
      symbol_type
      make_tstFOR (const location_type& l)
      {
        return symbol_type (token::tstFOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstADD (location_type l)
      {
        return symbol_type (token::tstADD, std::move (l));
      }
#else
      static
      symbol_type
      make_tstADD (const location_type& l)
      {
        return symbol_type (token::tstADD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstSUB (location_type l)
      {
        return symbol_type (token::tstSUB, std::move (l));
      }
#else
      static
      symbol_type
      make_tstSUB (const location_type& l)
      {
        return symbol_type (token::tstSUB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstMUL (location_type l)
      {
        return symbol_type (token::tstMUL, std::move (l));
      }
#else
      static
      symbol_type
      make_tstMUL (const location_type& l)
      {
        return symbol_type (token::tstMUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstDIV (location_type l)
      {
        return symbol_type (token::tstDIV, std::move (l));
      }
#else
      static
      symbol_type
      make_tstDIV (const location_type& l)
      {
        return symbol_type (token::tstDIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstLESS (location_type l)
      {
        return symbol_type (token::tstLESS, std::move (l));
      }
#else
      static
      symbol_type
      make_tstLESS (const location_type& l)
      {
        return symbol_type (token::tstLESS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstMORE (location_type l)
      {
        return symbol_type (token::tstMORE, std::move (l));
      }
#else
      static
      symbol_type
      make_tstMORE (const location_type& l)
      {
        return symbol_type (token::tstMORE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstEQUAL (location_type l)
      {
        return symbol_type (token::tstEQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_tstEQUAL (const location_type& l)
      {
        return symbol_type (token::tstEQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstLESS_EQUAL (location_type l)
      {
        return symbol_type (token::tstLESS_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_tstLESS_EQUAL (const location_type& l)
      {
        return symbol_type (token::tstLESS_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstMORE_EQUAL (location_type l)
      {
        return symbol_type (token::tstMORE_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_tstMORE_EQUAL (const location_type& l)
      {
        return symbol_type (token::tstMORE_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstAND (location_type l)
      {
        return symbol_type (token::tstAND, std::move (l));
      }
#else
      static
      symbol_type
      make_tstAND (const location_type& l)
      {
        return symbol_type (token::tstAND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstOR (location_type l)
      {
        return symbol_type (token::tstOR, std::move (l));
      }
#else
      static
      symbol_type
      make_tstOR (const location_type& l)
      {
        return symbol_type (token::tstOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstNOT (location_type l)
      {
        return symbol_type (token::tstNOT, std::move (l));
      }
#else
      static
      symbol_type
      make_tstNOT (const location_type& l)
      {
        return symbol_type (token::tstNOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstEXP_END (location_type l)
      {
        return symbol_type (token::tstEXP_END, std::move (l));
      }
#else
      static
      symbol_type
      make_tstEXP_END (const location_type& l)
      {
        return symbol_type (token::tstEXP_END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstGLOBAL_REF (location_type l)
      {
        return symbol_type (token::tstGLOBAL_REF, std::move (l));
      }
#else
      static
      symbol_type
      make_tstGLOBAL_REF (const location_type& l)
      {
        return symbol_type (token::tstGLOBAL_REF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_tstGLOBAL_IN (location_type l)
      {
        return symbol_type (token::tstGLOBAL_IN, std::move (l));
      }
#else
      static
      symbol_type
      make_tstGLOBAL_IN (const location_type& l)
      {
        return symbol_type (token::tstGLOBAL_IN, l);
      }
#endif


  private:
    /// This class is not copyable.
    tsParser (const tsParser&);
    tsParser& operator= (const tsParser&);

    /// Stored state numbers (used for stacks).
    typedef signed char state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    /// In theory \a t should be a token_type, but character literals
    /// are valid, yet not members of the token_type enum.
    static token_number_type yytranslate_ (int t);

    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const signed char yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const signed char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const signed char yytable_[];

    static const signed char yycheck_[];

    // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
    // symbol of state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
    static const signed char yyr1_[];

    // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
    static const signed char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const unsigned char yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::reverse_iterator iterator;
      typedef typename S::const_reverse_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      std::ptrdiff_t
      ssize () const YY_NOEXCEPT
      {
        return std::ptrdiff_t (size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.rbegin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.rend ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
      stack (const stack&);
      stack& operator= (const stack&);
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Some specific tokens.
    static const token_number_type yy_error_token_ = 1;
    static const token_number_type yy_undef_token_ = 2;

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 98,     ///< Last index in yytable_.
      yynnts_ = 10,  ///< Number of nonterminal symbols.
      yyfinal_ = 38, ///< Termination state number.
      yyntokens_ = 38  ///< Number of tokens.
    };


    // User arguments.
    tsScanner  &scanner;
    tsCompiler  &compiler;
  };


#line 7 "bison.y"
} // ts
#line 1594 "bison.tab.hh"





#endif // !YY_YY_BISON_TAB_HH_INCLUDED
