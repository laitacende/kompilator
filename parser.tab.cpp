/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.ypp"

    #include <iostream>
    #include <string>
    #include <memory>
    #include <fstream>

    #include "./inc/IOOperations.hpp"
    #include "./inc/CodeGenerator.hpp"
    #include "./inc/Symbol.hpp"
    #include "./inc/Cond.hpp"

    extern int yylineno;
    extern int yylex();
    extern FILE *yyin;

    int yyerror(std::string s);

    std::shared_ptr<MemoryData> dataController = std::make_shared<MemoryData>();
    std::shared_ptr<CodeGenerator> codeGen = std::make_shared<CodeGenerator>(dataController);

    std::vector<std::string> newSet;
    bool error = false;
    long long int line = 0;
    long long int lineRepeat = 0;

#line 96 "parser.tab.cpp"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_TAB_HPP_INCLUDED
# define YY_YY_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BEGIN_ = 258,
    END = 259,
    VAR = 260,
    WRITE = 261,
    READ = 262,
    ASSIGN = 263,
    PLUS = 264,
    MINUS = 265,
    TIMES = 266,
    DIV = 267,
    MOD = 268,
    IF = 269,
    THEN = 270,
    ELSE = 271,
    ENDIF = 272,
    EQ = 273,
    NEQ = 274,
    GE = 275,
    LE = 276,
    LEQ = 277,
    GEQ = 278,
    WHILE = 279,
    DO = 280,
    ENDWHILE = 281,
    REPEAT = 282,
    UNTIL = 283,
    num = 284,
    pidentifier = 285,
    ERROR = 286
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
#line 29 "parser.ypp"
union types
{
#line 29 "parser.ypp"

    std::string* pidentifier;
    long long int num;
    Variable* var;
    Cond* cond;

#line 188 "parser.tab.cpp"

};
#line 29 "parser.ypp"
typedef union types YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
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


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   166

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  43
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  115

#define YYUNDEFTOK  2
#define YYMAXUTOK   286


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    32,    36,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    34,    37,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,    35,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    73,    73,    74,    76,    82,    88,    99,   110,   121,
     133,   144,   155,   166,   179,   180,   182,   189,   191,   196,
     196,   210,   220,   232,   232,   247,   248,   249,   250,   251,
     252,   254,   255,   256,   257,   258,   259,   261,   267,   268,
     270,   283,   302,   321
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BEGIN_", "END", "VAR", "WRITE", "READ",
  "ASSIGN", "PLUS", "MINUS", "TIMES", "DIV", "MOD", "IF", "THEN", "ELSE",
  "ENDIF", "EQ", "NEQ", "GE", "LE", "LEQ", "GEQ", "WHILE", "DO",
  "ENDWHILE", "REPEAT", "UNTIL", "num", "pidentifier", "ERROR", "','",
  "'['", "':'", "']'", "'-'", "';'", "$accept", "program", "declarations",
  "commands", "command", "$@1", "$@2", "expression", "condition", "value",
  "identifier", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,    44,    91,    58,    93,    45,    59
};
# endif

#define YYPACT_NINF (-25)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      34,   136,   -23,    16,   -19,    -8,   -19,   -19,   -25,    10,
      74,   -25,    19,    14,     3,   -25,   -25,    20,    -9,   -25,
      11,    17,    51,    31,   136,   -15,   -25,   -25,   -19,   -24,
     136,    21,   -25,   -25,   -25,   136,   -19,   -19,   -19,   -19,
     -19,   -19,   136,   100,    22,    23,    32,    38,    73,    42,
      48,    96,    27,   105,   -25,   -25,   -25,   -25,   -25,   -25,
     127,   -19,   -25,   -25,    44,   -25,   -19,   -19,   -19,   -19,
     -19,   -11,    53,   -25,   -10,   -25,    83,   -25,    68,   -25,
     -25,   -25,   -25,   -25,   -25,    55,    79,    -5,    75,    84,
     136,   -25,   -25,    80,    81,    88,     4,    87,   131,   -25,
     -25,    90,   101,    89,     9,   -25,   -25,   -25,   104,   109,
     102,   -25,   -25,   111,   -25
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,    23,    40,
       0,    15,     0,     5,     0,     1,    38,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     3,    14,     0,     0,
       0,     0,    39,    16,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     4,    19,    32,    31,    34,    33,    35,    36,
       0,     0,    41,    43,     0,    17,     0,     0,     0,     0,
       0,     0,     0,     2,     0,    21,     0,    22,     0,    42,
      26,    27,    28,    29,    30,     0,     0,     0,     0,     0,
       0,    24,     6,     0,     0,     0,     0,     0,     0,     8,
       7,     0,     0,     0,     0,    20,     9,    12,     0,     0,
       0,    11,    10,     0,    13
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -25,   -25,   -25,   -22,    -7,   -25,   -25,   -25,    -6,    26,
      -1
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,    14,    10,    11,    76,    24,    47,    21,    22,
      19
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      12,    23,    43,    27,    20,    49,    30,    13,    51,    12,
      16,     9,    50,    53,    44,    45,    15,    17,    85,    88,
      60,    46,     9,    12,    94,    86,    89,    28,    33,    12,
      18,    95,    35,   102,    12,    31,    27,     1,   109,     2,
     103,    12,    12,    25,    27,   110,    27,    29,    34,    32,
      12,    52,    12,    27,    48,    78,    42,    62,    63,    12,
      74,    64,    54,    55,    56,    57,    58,    59,    98,    36,
      37,    38,    39,    40,    41,    65,    71,    72,    26,    79,
       4,     5,    66,    67,    68,    69,    70,    87,     6,    12,
      92,    27,    80,    81,    82,    83,    84,    12,     7,    90,
      73,     8,     4,     5,     9,    91,     4,     5,    93,    96,
       6,     4,     5,    97,     6,    99,   100,   101,   108,     6,
       7,   104,    75,     8,     7,   106,     9,     8,    61,     7,
       9,   113,     8,     4,     5,     9,   107,     4,     5,   111,
       0,     6,     4,     5,   112,     6,   114,     0,   105,     0,
       6,     7,     0,    77,     8,     7,     0,     9,     8,     0,
       7,     9,     0,     8,     0,     0,     9
};

static const yytype_int8 yycheck[] =
{
       1,     7,    24,    10,     5,    29,     3,    30,    30,    10,
      29,    30,    36,    35,    29,    30,     0,    36,    29,    29,
      42,    36,    30,    24,    29,    36,    36,     8,    37,    30,
       4,    36,    15,    29,    35,    32,    43,     3,    29,     5,
      36,    42,    43,    33,    51,    36,    53,    33,    37,    29,
      51,    30,    53,    60,    28,    61,    25,    35,    35,    60,
      33,    29,    36,    37,    38,    39,    40,    41,    90,    18,
      19,    20,    21,    22,    23,    37,    34,    29,     4,    35,
       6,     7,     9,    10,    11,    12,    13,    34,    14,    90,
      35,    98,    66,    67,    68,    69,    70,    98,    24,    16,
       4,    27,     6,     7,    30,    37,     6,     7,    29,    34,
      14,     6,     7,    29,    14,    35,    35,    29,    29,    14,
      24,    34,    17,    27,    24,    35,    30,    27,    28,    24,
      30,    29,    27,     6,     7,    30,    35,     6,     7,    35,
      -1,    14,     6,     7,    35,    14,    35,    -1,    17,    -1,
      14,    24,    -1,    26,    27,    24,    -1,    30,    27,    -1,
      24,    30,    -1,    27,    -1,    -1,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,    39,     6,     7,    14,    24,    27,    30,
      41,    42,    48,    30,    40,     0,    29,    36,    47,    48,
      48,    46,    47,    46,    44,    33,     4,    42,     8,    33,
       3,    32,    29,    37,    37,    15,    18,    19,    20,    21,
      22,    23,    25,    41,    29,    30,    36,    45,    47,    29,
      36,    41,    30,    41,    47,    47,    47,    47,    47,    47,
      41,    28,    35,    35,    29,    37,     9,    10,    11,    12,
      13,    34,    29,     4,    33,    17,    43,    26,    46,    35,
      47,    47,    47,    47,    47,    29,    36,    34,    29,    36,
      16,    37,    35,    29,    29,    36,    34,    29,    41,    35,
      35,    29,    29,    36,    34,    17,    35,    35,    29,    29,
      36,    35,    35,    29,    35
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    39,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    41,    41,    42,    42,    42,    43,
      42,    42,    42,    44,    42,    45,    45,    45,    45,    45,
      45,    46,    46,    46,    46,    46,    46,    47,    47,    47,
      48,    48,    48,    48
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     3,     3,     1,     6,     7,     7,     8,
       9,     9,     8,    10,     2,     1,     3,     4,     3,     0,
       8,     5,     5,     0,     6,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     2,
       1,     4,     5,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4:
#line 76 "parser.ypp"
                                          { Variable* v = codeGen->allocateVariable(*(yyvsp[0].pidentifier));
                                             if (nullptr == v) {
                                                error = true;
                                                yyerror("Redefinition of variable " + *(yyvsp[0].pidentifier));
                                             }
                                            }
#line 1446 "parser.tab.cpp"
    break;

  case 5:
#line 82 "parser.ypp"
                                { Variable* v = codeGen->allocateVariable(*(yyvsp[0].pidentifier));
                                    if (nullptr == v && !error) {
                                        error = true;
                                        yyerror("Redeclaration of variable " + *(yyvsp[0].pidentifier));
                                    }
                                }
#line 1457 "parser.tab.cpp"
    break;

  case 6:
#line 88 "parser.ypp"
                                             { Variable* v = codeGen->allocateArray(*(yyvsp[-5].pidentifier), (yyvsp[-3].num), (yyvsp[-1].num));
                                                if (nullptr == v && !error) {
                                                    error = true;
                                                    yyerror("Redeclaration of variable " + *(yyvsp[-5].pidentifier));
                                                }

                                                if (v->name == "error") {
                                                    error = true;
                                                    yyerror("Invalid range of array " + *(yyvsp[-5].pidentifier));
                                                }
                                             }
#line 1473 "parser.tab.cpp"
    break;

  case 7:
#line 99 "parser.ypp"
                                                { Variable* v = codeGen->allocateArray(*(yyvsp[-6].pidentifier), -(yyvsp[-3].num), (yyvsp[-1].num));
                                                               if (nullptr == v && !error) {
                                                                   error = true;
                                                                   yyerror("Redeclaration of variable " + *(yyvsp[-6].pidentifier));
                                                               }

                                                               if (v->name == "error") {
                                                                   error = true;
                                                                   yyerror("Invalid range of array " + *(yyvsp[-6].pidentifier));
                                                               }
                                                            }
#line 1489 "parser.tab.cpp"
    break;

  case 8:
#line 110 "parser.ypp"
                                                { Variable* v = codeGen->allocateArray(*(yyvsp[-6].pidentifier), (yyvsp[-4].num), -(yyvsp[-1].num));
                                                    if (nullptr == v && !error) {
                                                        error = true;
                                                        yyerror("Redeclaration of variable " + *(yyvsp[-6].pidentifier));
                                                    }

                                                    if (v->name == "error") {
                                                        error = true;
                                                        yyerror("Invalid range of array " + *(yyvsp[-6].pidentifier));
                                                    }
                                                 }
#line 1505 "parser.tab.cpp"
    break;

  case 9:
#line 121 "parser.ypp"
                                                   { Variable* v = codeGen->allocateArray(*(yyvsp[-7].pidentifier), -(yyvsp[-4].num), -(yyvsp[-1].num));
                                                 if (nullptr == v && !error) {
                                                     error = true;
                                                     yyerror("Redeclaration of variable " + *(yyvsp[-7].pidentifier));
                                                 }

                                                 if (v->name == "error") {
                                                     error = true;
                                                     yyerror("Invalid range of array " + *(yyvsp[-7].pidentifier));
                                                 }
                                              }
#line 1521 "parser.tab.cpp"
    break;

  case 10:
#line 133 "parser.ypp"
                                                              { Variable* v = codeGen->allocateArray(*(yyvsp[-6].pidentifier), -(yyvsp[-3].num), (yyvsp[-1].num));
                                                               if (nullptr == v && !error) {
                                                                   error = true;
                                                                   yyerror("Redeclaration of variable " + *(yyvsp[-6].pidentifier));
                                                               }

                                                               if (v->name == "error") {
                                                                   error = true;
                                                                   yyerror("Invalid range of array " + *(yyvsp[-6].pidentifier));
                                                               }
                                                            }
#line 1537 "parser.tab.cpp"
    break;

  case 11:
#line 144 "parser.ypp"
                                                              { Variable* v = codeGen->allocateArray(*(yyvsp[-6].pidentifier), (yyvsp[-4].num), -(yyvsp[-1].num));
                                                                if (nullptr == v && !error) {
                                                                    error = true;
                                                                    yyerror("Redeclaration of variable " + *(yyvsp[-6].pidentifier));
                                                                }

                                                                if (v->name == "error") {
                                                                    error = true;
                                                                    yyerror("Invalid range of array " + *(yyvsp[-6].pidentifier));
                                                                }
                                                             }
#line 1553 "parser.tab.cpp"
    break;

  case 12:
#line 155 "parser.ypp"
                                                           { Variable* v = codeGen->allocateArray(*(yyvsp[-5].pidentifier), (yyvsp[-3].num), (yyvsp[-1].num));
                                                                             if (nullptr == v && !error) {
                                                                                 error = true;
                                                                                 yyerror("Redeclaration of variable " + *(yyvsp[-5].pidentifier));
                                                                             }

                                                                             if (v->name == "error") {
                                                                                 error = true;
                                                                                 yyerror("Invalid range of array " + *(yyvsp[-5].pidentifier));
                                                                             }
                                                                          }
#line 1569 "parser.tab.cpp"
    break;

  case 13:
#line 166 "parser.ypp"
                                                                  { Variable* v = codeGen->allocateArray(*(yyvsp[-7].pidentifier), -(yyvsp[-4].num), -(yyvsp[-1].num));
                                                                  if (nullptr == v && !error) {
                                                                      error = true;
                                                                      yyerror("Redeclaration of variable " + *(yyvsp[-7].pidentifier));
                                                                  }

                                                                  if (v->name == "error") {
                                                                      error = true;
                                                                      yyerror("Invalid range of array " + *(yyvsp[-7].pidentifier));
                                                                  }
                                                               }
#line 1585 "parser.tab.cpp"
    break;

  case 16:
#line 182 "parser.ypp"
                           { if (!codeGen->write((yyvsp[-1].var)) && !error) {
                                error = true;
                                if ((yyvsp[-1].var) != nullptr)
                                yyerror("Variable " + (yyvsp[-1].var)->name +" is not initialized");
                              }
                            }
#line 1596 "parser.tab.cpp"
    break;

  case 17:
#line 189 "parser.ypp"
                                              {  codeGen->assignToVariable((yyvsp[-3].var), (yyvsp[-1].var)); }
#line 1602 "parser.tab.cpp"
    break;

  case 18:
#line 191 "parser.ypp"
                                    { if (!codeGen->read((yyvsp[-1].var)) && !error) {
                                        error = true;
                                      }
                                    }
#line 1611 "parser.tab.cpp"
    break;

  case 19:
#line 196 "parser.ypp"
                                         { line = codeGen->addInstruction("JUMP "); // omit else, edit later
                                            std::string instr = codeGen->getInstruction((yyvsp[-2].cond)->index);
                                            instr = instr + std::to_string(codeGen->offset - (yyvsp[-2].cond)->index);
                                            codeGen->changeInstruction((yyvsp[-2].cond)->index, instr); // go to else
                                            if ((yyvsp[-2].cond)->type == "EQ" || (yyvsp[-2].cond)->type == "LE" || (yyvsp[-2].cond)->type == "GE") {
                                                instr = codeGen->getInstruction((yyvsp[-2].cond)->index - 1);
                                                instr = instr + std::to_string(codeGen->offset - (yyvsp[-2].cond)->index + 1);
                                                codeGen->changeInstruction((yyvsp[-2].cond)->index - 1, instr);
                                            }
                                          }
#line 1626 "parser.tab.cpp"
    break;

  case 20:
#line 206 "parser.ypp"
                                         { std::string instr = "JUMP " + std::to_string(codeGen->offset - line);
                                            codeGen->changeInstruction(line, instr); // edit instr from first block, jump adter else
                                         }
#line 1634 "parser.tab.cpp"
    break;

  case 21:
#line 210 "parser.ypp"
                                               {    std::string instr = codeGen->getInstruction((yyvsp[-3].cond)->index); /* if EQ there are two jumps */
                                                    instr = instr + std::to_string(codeGen->offset - (yyvsp[-3].cond)->index);
                                                    codeGen->changeInstruction((yyvsp[-3].cond)->index, instr);
                                                    if ((yyvsp[-3].cond)->type == "EQ" || (yyvsp[-3].cond)->type == "LE" || (yyvsp[-3].cond)->type == "GE") {
                                                        instr = codeGen->getInstruction((yyvsp[-3].cond)->index - 1);
                                                        instr = instr + std::to_string(codeGen->offset - (yyvsp[-3].cond)->index + 1);
                                                        codeGen->changeInstruction((yyvsp[-3].cond)->index - 1, instr);
                                                    }
                                                }
#line 1648 "parser.tab.cpp"
    break;

  case 22:
#line 220 "parser.ypp"
                                                    { std::string instr = codeGen->getInstruction((yyvsp[-3].cond)->index); /* if EQ there are two jumps */
                                                      instr = instr + std::to_string((codeGen->offset - (yyvsp[-3].cond)->index + 1));
                                                      codeGen->changeInstruction((yyvsp[-3].cond)->index, instr);
                                                      if ((yyvsp[-3].cond)->type == "EQ" || (yyvsp[-3].cond)->type == "LE" || (yyvsp[-3].cond)->type == "GE") {
                                                          instr = codeGen->getInstruction((yyvsp[-3].cond)->index - 1);
                                                          instr = instr + std::to_string((codeGen->offset - (yyvsp[-3].cond)->index + 2));
                                                          codeGen->changeInstruction((yyvsp[-3].cond)->index - 1, instr);
                                                      }
                                                      //std::cout << codeGen->offset << std::endl;
                                                      codeGen->addInstruction("JUMP -" + std::to_string((codeGen->offset - (yyvsp[-3].cond)->firstIndex + 1)));
                                                     }
#line 1664 "parser.tab.cpp"
    break;

  case 23:
#line 232 "parser.ypp"
                     { lineRepeat = codeGen->offset; }
#line 1670 "parser.tab.cpp"
    break;

  case 24:
#line 233 "parser.ypp"
                                          {   std::string instr = codeGen->getInstruction((yyvsp[-1].cond)->index); /* if EQ there are two jumps */
                                                instr = instr + std::to_string((-(codeGen->offset - lineRepeat) + 1));
                                                codeGen->changeInstruction((yyvsp[-1].cond)->index, instr);
                                                if ((yyvsp[-1].cond)->type == "EQ" || (yyvsp[-1].cond)->type == "LE" || (yyvsp[-1].cond)->type == "GE") {
                                                    instr = codeGen->getInstruction((yyvsp[-1].cond)->index - 1);
                                                    instr = instr + std::to_string((-(codeGen->offset - lineRepeat) + 2));
                                                    codeGen->changeInstruction((yyvsp[-1].cond)->index - 1, instr);
                                                }
                                                //std::cout << codeGen->offset << std::endl;

                                               // codeGen->addInstruction("JUMP -" + std::to_string((codeGen->offset - lineRepeat)));
                                           }
#line 1687 "parser.tab.cpp"
    break;

  case 26:
#line 248 "parser.ypp"
                               { codeGen->add((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1693 "parser.tab.cpp"
    break;

  case 27:
#line 249 "parser.ypp"
                                { codeGen->subtract((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1699 "parser.tab.cpp"
    break;

  case 28:
#line 250 "parser.ypp"
                                { codeGen->multiply((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1705 "parser.tab.cpp"
    break;

  case 29:
#line 251 "parser.ypp"
                                { codeGen->divide((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1711 "parser.tab.cpp"
    break;

  case 30:
#line 252 "parser.ypp"
                                { codeGen->modulo((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1717 "parser.tab.cpp"
    break;

  case 31:
#line 254 "parser.ypp"
                            { (yyval.cond) = codeGen->evalNotEqual((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1723 "parser.tab.cpp"
    break;

  case 32:
#line 255 "parser.ypp"
                             { (yyval.cond) = codeGen->evalEqual((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1729 "parser.tab.cpp"
    break;

  case 33:
#line 256 "parser.ypp"
                             { (yyval.cond) = codeGen->evalLess((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1735 "parser.tab.cpp"
    break;

  case 34:
#line 257 "parser.ypp"
                             { (yyval.cond) = codeGen->evalGreater((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1741 "parser.tab.cpp"
    break;

  case 35:
#line 258 "parser.ypp"
                              { (yyval.cond) = codeGen->evalLessEqual((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1747 "parser.tab.cpp"
    break;

  case 36:
#line 259 "parser.ypp"
                              { (yyval.cond) = codeGen->evalGreaterEqual((yyvsp[-2].var), (yyvsp[0].var)); }
#line 1753 "parser.tab.cpp"
    break;

  case 37:
#line 261 "parser.ypp"
                       { (yyval.var) = (yyvsp[0].var);
                         if ((yyvsp[0].var) != nullptr && (yyvsp[0].var)->isVariable && !(yyvsp[0].var)->isInit && !error) {
                            error = true;
                            yyerror("Variable " + (yyvsp[0].var)->name + "is not initialized");
                         }
                        }
#line 1764 "parser.tab.cpp"
    break;

  case 38:
#line 267 "parser.ypp"
                  { (yyval.var) = codeGen->allocateConstant((yyvsp[0].num));  }
#line 1770 "parser.tab.cpp"
    break;

  case 39:
#line 268 "parser.ypp"
                      { (yyval.var) = codeGen->allocateConstantNegate((yyvsp[0].num)); }
#line 1776 "parser.tab.cpp"
    break;

  case 40:
#line 270 "parser.ypp"
                            { Variable* var = codeGen->getVar(*(yyvsp[0].pidentifier));
                                if (var == nullptr && !error) {
                                    error = true;
                                    yyerror("Variable " + *(yyvsp[0].pidentifier) + " not defined");
                                   // $$ = nullptr;
                                 } else if (!var->isVariable) {
                                    error = true;
                                    yyerror("Improper use of array variable " + *(yyvsp[0].pidentifier));
                                    //$$ = nullptr;
                                 } else {
                                    (yyval.var) = var;
                                 }
                            }
#line 1794 "parser.tab.cpp"
    break;

  case 41:
#line 283 "parser.ypp"
                                       {  Variable* var = codeGen->getVar(*(yyvsp[-3].pidentifier)); // first index of table
                                          if (var == nullptr && !error) {
                                                yyerror("Variable " + *(yyvsp[-3].pidentifier) + " not defined");
                                                (yyval.var) = nullptr;
                                          } else if (!var->isArray) {
                                                error = true;
                                                yyerror("Improper use of variable " + *(yyvsp[-3].pidentifier));
                                                //$$ = nullptr;
                                          } else {
                                            // get this variable with index
                                            Variable* var1 = codeGen->getVarArrayNum(var, (yyvsp[-1].num));
                                            if (var1 == nullptr && !error) {
                                                error = true;
                                                yyerror("Index out of bounds for array " + *(yyvsp[-3].pidentifier));
                                            } else {
                                                (yyval.var) = var1;
                                            }
                                          }
                                        }
#line 1818 "parser.tab.cpp"
    break;

  case 42:
#line 302 "parser.ypp"
                                          {  Variable* var = codeGen->getVar(*(yyvsp[-4].pidentifier)); // first index of table
                                            if (var == nullptr && !error) {
                                                  yyerror("Variable " + *(yyvsp[-4].pidentifier) + " not defined");
                                                  (yyval.var) = nullptr;
                                            } else if (!var->isArray) {
                                                  error = true;
                                                  yyerror("Improper use of variable " + *(yyvsp[-4].pidentifier));
                                                  //$$ = nullptr;
                                            } else {
                                              // get this variable with index
                                              Variable* var1 = codeGen->getVarArrayNum(var, -(yyvsp[-1].num));
                                              if (var1 == nullptr && !error) {
                                                  error = true;
                                                  yyerror("Index out of bounds for array " + *(yyvsp[-4].pidentifier));
                                              } else {
                                                  (yyval.var) = var1;
                                              }
                                            }
                                          }
#line 1842 "parser.tab.cpp"
    break;

  case 43:
#line 321 "parser.ypp"
                                                { // make new object variable with address of array and addres of variable
                                                     Variable* arr = codeGen->getVar(*(yyvsp[-3].pidentifier)); // first index of table
                                                     Variable* var = codeGen->getVar(*(yyvsp[-1].pidentifier)); // variable which is index of array now
                                                     if (arr == nullptr && !error) {
                                                           error = true;
                                                           yyerror("Variable " + *(yyvsp[-3].pidentifier) + " not defined");
                                                           //$$ = nullptr;
                                                     } else if (var == nullptr && !error) {
                                                            error = true;
                                                            yyerror("Variable " + *(yyvsp[-1].pidentifier) + " not defined");
                                                     } else if (!var->isVariable) {
                                                           error = true;
                                                           yyerror("Improper use of variable " + *(yyvsp[-1].pidentifier));
                                                     } else if (!var->isInit) {
                                                        error = true;
                                                        yyerror("Variable " + var->name + "is not initialized");
                                                     } else if (!arr->isArray) {
                                                        error = true;
                                                        yyerror("Improper use of variable " + *(yyvsp[-3].pidentifier));
                                                     } else { // add to arr offset which is address of var
                                                        arr->offset = var->address;
                                                        arr->isArrayWithVar = true;
                                                        (yyval.var) = arr;
                                                     }
                                                }
#line 1872 "parser.tab.cpp"
    break;


#line 1876 "parser.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 347 "parser.ypp"


int yyerror (std::string s) {
    error = true;
    std::cerr << "Error: " << s << " in line " << yylineno << std::endl;
    return 0;
}

int main(int argc, char** argv) {
    // variable array

    std::shared_ptr<IOOperations> io = std::make_shared<IOOperations>(argc, argv);
    // read input file
    yyin = io->readCode();

    // check if file was read successfully
    if (yyin == nullptr) {
        return -1;
    }

    std::cout << "-------- ...Compiling... --------" << std::endl;

    yyparse();

    // error handling
    // write to file if no errors occurred
    codeGen->addInstruction("HALT");
    if (!error) {
        io->writeCode(codeGen->getCode());
        std::cout << "-------- Compilation successful --------" << std::endl;
    } else {
        std::cout << "-------- Compilation failed --------" << std::endl;
    }
    return 0;
}
