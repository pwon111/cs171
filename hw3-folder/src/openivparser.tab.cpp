/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         openivparse
#define yylex           openivlex
#define yyerror         openiverror
#define yydebug         openivdebug
#define yynerrs         openivnerrs

#define yylval          openivlval
#define yychar          openivchar

/* Copy the first part of user declarations.  */
#line 13 "openivparser.ypp" /* yacc.c:339  */

#include <FlexLexer.h>
#include <iostream>
#include <vector>
#include "ivdata.h"

using namespace std;

/* Notifies the compiler of the extern functions from flex. We do not need
 * to define these functions. */
extern int openivlex();
extern void openivlex_destroy();

/* Error function for parse errors. We define this later. */
int openiverror(const char *s);

static Camera camera; /* For the camera block. */

/* This vector is for the Separator blocks. We will push an Object struct
 * into the vector for each Separator block in the file in the order they
 * appear. The last Object in the vector always corresponds to the most
 * recently read Separator block in the file. */
static vector<Object> objects;

/* This vector is for the PointLight blocks. We will push a Point_Light struct
 * into the vector for each PointLight block in the file in the order they
 * appear. The last Point_Light in the vector always corresponds to the most
 * recently read PointLight block in the file. */
static vector<Point_Light> lights;

#line 105 "openivparser.tab.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "openivparser.tab.hpp".  */
#ifndef YY_OPENIV_OPENIVPARSER_TAB_HPP_INCLUDED
# define YY_OPENIV_OPENIVPARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int openivdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    TERMINATOR = 259,
    FLOAT = 260,
    OPEN_BRACE = 261,
    CLOSE_BRACE = 262,
    OPEN_BRACKET = 263,
    CLOSE_BRACKET = 264,
    COMMA = 265,
    CAMERA = 266,
    CAM_POSITION = 267,
    CAM_ORIENTATION = 268,
    CAM_NEAR = 269,
    CAM_FAR = 270,
    CAM_LEFT = 271,
    CAM_RIGHT = 272,
    CAM_TOP = 273,
    CAM_BOTTOM = 274,
    LIGHT = 275,
    LIGHT_POSITION = 276,
    LIGHT_COLOR = 277,
    SEPARATOR = 278,
    TRANSFORM = 279,
    TRANSLATE = 280,
    ROTATE = 281,
    SCALE = 282,
    MATERIAL = 283,
    AMBIENT = 284,
    DIFFUSE = 285,
    SPECULAR = 286,
    SHININESS = 287,
    COORDINATE = 288,
    POINT = 289,
    NORMAL = 290,
    VECTOR = 291,
    FACESET = 292,
    COORDINATE_INDEX = 293,
    NORMAL_INDEX = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 54 "openivparser.ypp" /* yacc.c:355  */

    float fval;
    int ival;

#line 190 "openivparser.tab.cpp" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE openivlval;

int openivparse (void);

#endif /* !YY_OPENIV_OPENIVPARSER_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 205 "openivparser.tab.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   134

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  156

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   115,   115,   117,   117,   119,   119,   119,   121,   123,
     123,   125,   131,   138,   141,   144,   147,   150,   153,   157,
     159,   185,   185,   187,   193,   200,   202,   234,   234,   236,
     236,   238,   238,   238,   240,   242,   265,   265,   267,   273,
     280,   287,   289,   289,   291,   297,   303,   309,   312,   314,
     316,   324,   333,   335,   337,   345,   353,   358,   376,   378,
     381,   384,   394,   404,   406,   409,   412
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "TERMINATOR", "FLOAT",
  "OPEN_BRACE", "CLOSE_BRACE", "OPEN_BRACKET", "CLOSE_BRACKET", "COMMA",
  "CAMERA", "CAM_POSITION", "CAM_ORIENTATION", "CAM_NEAR", "CAM_FAR",
  "CAM_LEFT", "CAM_RIGHT", "CAM_TOP", "CAM_BOTTOM", "LIGHT",
  "LIGHT_POSITION", "LIGHT_COLOR", "SEPARATOR", "TRANSFORM", "TRANSLATE",
  "ROTATE", "SCALE", "MATERIAL", "AMBIENT", "DIFFUSE", "SPECULAR",
  "SHININESS", "COORDINATE", "POINT", "NORMAL", "VECTOR", "FACESET",
  "COORDINATE_INDEX", "NORMAL_INDEX", "$accept", "ivfile", "blocks",
  "block", "cam_block", "cam_data", "cam_line", "light_block",
  "light_init", "light_data", "light_line", "sep_block", "sep_init",
  "sep_data", "sep_line", "transform_block", "transform_init",
  "transform_data", "transform_line", "material_block", "material_data",
  "material_line", "coordinate_block", "coordinate_data", "points",
  "normal_block", "normal_data", "normals", "faceset_block",
  "faceset_init", "index_list", "index", "normal_set_init", "normal_list",
  "normal_index", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

#define YYPACT_NINF -25

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-25)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -8,     2,   -25,   -25,    10,   -25,    -8,   -25,   -25,     8,
     -25,    34,     4,   -25,   -25,     7,   -24,    36,    37,    38,
      39,    40,    41,    42,    43,    44,     4,    45,    47,    46,
       7,   -25,    48,    49,    50,    51,    52,   -24,   -25,    54,
     -25,   -25,   -25,   -25,    53,    56,   -25,   -25,   -25,   -25,
     -25,   -25,   -25,   -25,    57,    58,   -25,   -25,    -5,    15,
      28,    29,   -25,   -25,   -20,    60,    61,    63,    64,    65,
      66,    67,    68,    69,    -5,    70,    72,    73,    75,   -25,
      76,    78,    80,    81,    82,   -20,   -25,    83,   -25,   -25,
      85,    86,    87,   -25,   -25,   -25,    88,   -25,    89,   -25,
      71,    59,    90,    91,    92,   -25,   -25,   -25,    94,    95,
      96,    97,    21,    98,    23,   -25,    25,   -25,    79,    99,
     100,   101,   -25,   -25,   -25,   102,   -25,   103,   104,   -25,
     105,   -25,    -2,    74,   106,   -25,   107,   -25,   -25,   109,
     -25,   110,   -25,   -25,   -25,    27,   -25,   -25,   111,   112,
     -25,    35,   -25,   -25,   -25,   -25
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    20,    26,     0,     2,     3,     5,     6,     0,
       7,     0,     0,     1,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     9,     0,     0,     0,
      21,    35,     0,     0,     0,     0,     0,    27,    29,     0,
      30,    31,    32,    33,     0,     0,    13,    14,    15,    16,
      17,    18,     8,    10,     0,     0,    19,    22,     0,     0,
       0,     0,    25,    28,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,    36,    11,     0,    23,    24,
       0,     0,     0,    47,    41,    43,     0,    48,     0,    52,
       0,     0,     0,     0,     0,    34,    37,    12,     0,     0,
       0,     0,     0,     0,     0,    59,     0,    62,     0,     0,
       0,     0,    44,    45,    46,     0,    49,     0,     0,    53,
       0,    58,     0,     0,     0,    38,     0,    40,    50,     0,
      54,     0,    60,    61,    64,     0,    56,    39,     0,     0,
      63,     0,    51,    55,    65,    66
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -25,   -25,   113,   -25,   -25,   108,   -25,   -25,   -25,    93,
     -25,   -25,   -25,    84,   -25,   -25,   -25,   -10,   -25,   -25,
       6,   -25,   -25,   -25,   -25,   -25,   -25,   -25,   -25,   -25,
     -25,   -25,   -25,   -25,   -25
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    25,    26,     8,     9,    29,
      30,    10,    11,    36,    37,    38,    39,    84,    85,    40,
      73,    74,    41,    76,   112,    42,    78,   114,    43,    80,
     101,   116,   118,   134,   145
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      31,   142,   143,     1,    32,    81,    82,    83,    12,    33,
      13,    34,     2,    35,    15,     3,    17,    18,    19,    20,
      21,    22,    23,    24,    69,    70,    71,    72,    27,    28,
     126,   127,   129,   130,   131,   132,   150,   151,   154,   155,
      16,    44,    45,    46,    47,    48,    49,    50,    51,    75,
      54,    52,    55,    56,    58,    59,    60,    61,    65,    62,
      64,    66,    67,    68,    77,    86,    87,    79,    88,    89,
      90,    91,    92,    93,   115,   106,    94,   144,    96,    97,
      95,    98,    99,   102,   100,   103,   104,   133,   107,   105,
     108,   109,   110,   111,   113,   119,   120,   121,   117,   122,
     123,   124,   125,   128,   135,   136,   137,   138,   139,   140,
     141,     0,   147,   146,   148,   149,   152,   153,     0,    14,
       0,    63,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53
};

static const yytype_int8 yycheck[] =
{
      24,     3,     4,    11,    28,    25,    26,    27,     6,    33,
       0,    35,    20,    37,     6,    23,    12,    13,    14,    15,
      16,    17,    18,    19,    29,    30,    31,    32,    21,    22,
       9,    10,     9,    10,     9,    10,     9,    10,     3,     4,
       6,     5,     5,     5,     5,     5,     5,     5,     5,    34,
       5,     7,     5,     7,     6,     6,     6,     6,     5,     7,
       6,     5,     5,     5,    36,     5,     5,    38,     5,     5,
       5,     5,     5,     5,     3,    85,     7,     3,     8,     7,
      74,     8,     7,     5,     8,     5,     5,     8,     5,     7,
       5,     5,     5,     5,     5,     5,     5,     5,    39,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,    -1,     5,     7,     5,     5,     5,     5,    -1,     6,
      -1,    37,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    11,    20,    23,    41,    42,    43,    44,    47,    48,
      51,    52,     6,     0,    42,     6,     6,    12,    13,    14,
      15,    16,    17,    18,    19,    45,    46,    21,    22,    49,
      50,    24,    28,    33,    35,    37,    53,    54,    55,    56,
      59,    62,    65,    68,     5,     5,     5,     5,     5,     5,
       5,     5,     7,    45,     5,     5,     7,    49,     6,     6,
       6,     6,     7,    53,     6,     5,     5,     5,     5,    29,
      30,    31,    32,    60,    61,    34,    63,    36,    66,    38,
      69,    25,    26,    27,    57,    58,     5,     5,     5,     5,
       5,     5,     5,     5,     7,    60,     8,     7,     8,     7,
       8,    70,     5,     5,     5,     7,    57,     5,     5,     5,
       5,     5,    64,     5,    67,     3,    71,    39,    72,     5,
       5,     5,     5,     5,     5,     5,     9,    10,     5,     9,
      10,     9,    10,     8,    73,     5,     5,     5,     5,     5,
       5,     5,     3,     4,     3,    74,     7,     5,     5,     5,
       9,    10,     5,     5,     3,     4
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    43,    43,    44,    45,
      45,    46,    46,    46,    46,    46,    46,    46,    46,    47,
      48,    49,    49,    50,    50,    51,    52,    53,    53,    54,
      54,    54,    54,    54,    55,    56,    57,    57,    58,    58,
      58,    59,    60,    60,    61,    61,    61,    61,    62,    63,
      64,    64,    65,    66,    67,    67,    68,    69,    70,    71,
      71,    71,    72,    73,    74,    74,    74
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     4,     1,
       2,     4,     5,     2,     2,     2,     2,     2,     2,     4,
       1,     1,     2,     4,     4,     4,     1,     1,     2,     1,
       1,     1,     1,     1,     4,     1,     1,     2,     4,     5,
       4,     4,     1,     2,     4,     4,     4,     2,     4,     4,
       3,     5,     4,     4,     3,     5,     7,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 11:
#line 126 "openivparser.ypp" /* yacc.c:1646  */
    { camera.position[0] = (yyvsp[-2].fval);
             camera.position[1] = (yyvsp[-1].fval);
             camera.position[2] = (yyvsp[0].fval);
           }
#line 1389 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 132 "openivparser.ypp" /* yacc.c:1646  */
    { camera.orientation_axis[0] = (yyvsp[-3].fval);
             camera.orientation_axis[1] = (yyvsp[-2].fval);
             camera.orientation_axis[2] = (yyvsp[-1].fval);
             camera.orientation_angle = (yyvsp[0].fval);
           }
#line 1399 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 139 "openivparser.ypp" /* yacc.c:1646  */
    { camera.near = (yyvsp[0].fval); }
#line 1405 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 142 "openivparser.ypp" /* yacc.c:1646  */
    { camera.far = (yyvsp[0].fval); }
#line 1411 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 145 "openivparser.ypp" /* yacc.c:1646  */
    { camera.left = (yyvsp[0].fval); }
#line 1417 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 148 "openivparser.ypp" /* yacc.c:1646  */
    { camera.right = (yyvsp[0].fval); }
#line 1423 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 151 "openivparser.ypp" /* yacc.c:1646  */
    { camera.top = (yyvsp[0].fval); }
#line 1429 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 154 "openivparser.ypp" /* yacc.c:1646  */
    { camera.bottom = (yyvsp[0].fval); }
#line 1435 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 160 "openivparser.ypp" /* yacc.c:1646  */
    { /* We've hit the start of a new PointLight block, so we should
                * push a new Point_Light to represent the block into our vector.
                * We can access this Point_Light easily via lights.back() since
                * it will be the last element of the vector until we hit the next
                * PointLight block in the file (at which point, we would be done
                * reading this current PointLight block and have no more need to
                * edit the corresponding Point_Light struct.) */
               Point_Light light;
               
               /* Set default values for position and color in case specific
                * values are not explicitly specified in the file. */
               
               light.position[0] = 0;
               light.position[1] = 0;
               light.position[2] = 1;
               light.position[3] = 1;
               
               light.color[0] = 1;
               light.color[1] = 1;
               light.color[2] = 1;
               
               lights.push_back(light);
             }
#line 1463 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 188 "openivparser.ypp" /* yacc.c:1646  */
    { lights.back().position[0] = (yyvsp[-2].fval);
               lights.back().position[1] = (yyvsp[-1].fval);
               lights.back().position[2] = (yyvsp[0].fval);
             }
#line 1472 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 194 "openivparser.ypp" /* yacc.c:1646  */
    { lights.back().color[0] = (yyvsp[-2].fval);
               lights.back().color[1] = (yyvsp[-1].fval);
               lights.back().color[2] = (yyvsp[0].fval);
             }
#line 1481 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 203 "openivparser.ypp" /* yacc.c:1646  */
    { /* We've hit the start of a new Separator block, so we should
              * push a new Object to represent the block into our vector. We
              * can access this Object easily via objects.back() since it
              * will be the last element of the vector until we hit the next
              * Separator block in the file (at which point, we would be done
              * with this current Separator block and have no more need to edit
              * the corresponding Object struct.) */
             Object object;
             
             /* Set default values for the reflectances and Phong exponent
              * in case specific values are not explicitly specified in
              * the file. */
              
             object.ambient_reflect[0] = 0.2;
             object.ambient_reflect[1] = 0.2;
             object.ambient_reflect[2] = 0.2;
             
             object.diffuse_reflect[0] = 0.8;
             object.diffuse_reflect[1] = 0.8;
             object.diffuse_reflect[2] = 0.8;
             
             object.specular_reflect[0] = 0;
             object.specular_reflect[1] = 0;
             object.specular_reflect[2] = 0;
             
             object.shininess = 0.2;
             
             objects.push_back(object);
           }
#line 1515 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 243 "openivparser.ypp" /* yacc.c:1646  */
    { Transforms transforms;
                   
                   /* Set default transformations to their "identity versions"
                    * in case any of them are unspecified. These transformations
                    * produce no effect when applied. */
                    
                   transforms.translation[0] = 0;
                   transforms.translation[1] = 0;
                   transforms.translation[2] = 0;
                   
                   transforms.rotation[0] = 0;
                   transforms.rotation[1] = 0;
                   transforms.rotation[2] = 0;
                   transforms.rotation_angle = 0;
                   
                   transforms.scaling[0] = 1;
                   transforms.scaling[1] = 1;
                   transforms.scaling[2] = 1;
                   
                   objects.back().transform_sets.push_back(transforms);
                 }
#line 1541 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 268 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().transform_sets.back().translation[0] = (yyvsp[-2].fval);
                   objects.back().transform_sets.back().translation[1] = (yyvsp[-1].fval);
                   objects.back().transform_sets.back().translation[2] = (yyvsp[0].fval);
                 }
#line 1550 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 274 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().transform_sets.back().rotation[0] = (yyvsp[-3].fval);
                   objects.back().transform_sets.back().rotation[1] = (yyvsp[-2].fval);
                   objects.back().transform_sets.back().rotation[2] = (yyvsp[-1].fval);
                   objects.back().transform_sets.back().rotation_angle = (yyvsp[0].fval);
                 }
#line 1560 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 281 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().transform_sets.back().scaling[0] = (yyvsp[-2].fval);
                   objects.back().transform_sets.back().scaling[1] = (yyvsp[-1].fval);
                   objects.back().transform_sets.back().scaling[2] = (yyvsp[0].fval);
                 }
#line 1569 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 292 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().ambient_reflect[0] = (yyvsp[-2].fval);
                  objects.back().ambient_reflect[1] = (yyvsp[-1].fval);
                  objects.back().ambient_reflect[2] = (yyvsp[0].fval);
                }
#line 1578 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 298 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().diffuse_reflect[0] = (yyvsp[-2].fval);
                  objects.back().diffuse_reflect[1] = (yyvsp[-1].fval);
                  objects.back().diffuse_reflect[2] = (yyvsp[0].fval);
                }
#line 1587 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 304 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().specular_reflect[0] = (yyvsp[-2].fval);
                  objects.back().specular_reflect[1] = (yyvsp[-1].fval);
                  objects.back().specular_reflect[2] = (yyvsp[0].fval);
                }
#line 1596 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 310 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().shininess = (yyvsp[0].fval); }
#line 1602 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 317 "openivparser.ypp" /* yacc.c:1646  */
    { Triple point;
           point.x = (yyvsp[-2].fval);
           point.y = (yyvsp[-1].fval);
           point.z = (yyvsp[0].fval);
           objects.back().points.push_back(point);
         }
#line 1613 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 325 "openivparser.ypp" /* yacc.c:1646  */
    { Triple point;
           point.x = (yyvsp[-2].fval);
           point.y = (yyvsp[-1].fval);
           point.z = (yyvsp[0].fval);
           objects.back().points.push_back(point);
         }
#line 1624 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 338 "openivparser.ypp" /* yacc.c:1646  */
    { Triple normal;
           normal.x = (yyvsp[-2].fval);
           normal.y = (yyvsp[-1].fval);
           normal.z = (yyvsp[0].fval);
           objects.back().normals.push_back(normal);
         }
#line 1635 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 346 "openivparser.ypp" /* yacc.c:1646  */
    { Triple normal;
           normal.x = (yyvsp[-2].fval);
           normal.y = (yyvsp[-1].fval);
           normal.z = (yyvsp[0].fval);
           objects.back().normals.push_back(normal);
         }
#line 1646 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 359 "openivparser.ypp" /* yacc.c:1646  */
    { /* A COORDINATE_INDEX token indicates that we are about to
                 * read "facesets" from the file. In order to store the first
                 * faceset in our Object struct, we need to initialize our
                 * 'facesets' vector by pushing into it an empty 'faceset'
                 * vector. We will then push the indices that we read next
                 * from the file into this faceset vector until we hit a -1,
                 * which indicates the end of a faceset. At that point, we push
                 * on a new 'faceset' vector into 'facesets' and push the
                 * indices for the next faceset into this new 'faceset' vector.
                 * The most current faceset will always be represented by
                 * the last vector in 'facesets', letting us easily access it
                 * via facesets.back(). */
                vector<int> faceset;
                objects.back().facesets.push_back(faceset);
              }
#line 1666 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 379 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().facesets.back().push_back((yyvsp[0].ival)); }
#line 1672 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 382 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().facesets.back().push_back((yyvsp[0].ival)); }
#line 1678 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 385 "openivparser.ypp" /* yacc.c:1646  */
    { /* We've hit a -1, so we push in a new 'faceset' vector for the next
           * faceset. If there is no next faceset, then we will end up pushing
           * in a vector that will remain empty. We will need to get rid of this
           * empty vector for convenience later in the 'parse_iv' function. */
          vector<int> faceset;
          objects.back().facesets.push_back(faceset);
        }
#line 1690 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 395 "openivparser.ypp" /* yacc.c:1646  */
    { /* This follows the same idea as faceset_init above. The
                    * "normal_sets" parallel the "facesets", hence the procedure
                    * for parsing them and representing them as data structures
                    * is the same procedure that we use for facesets. */
                   vector<int> normal_set;
                   objects.back().normal_sets.push_back(normal_set);
                 }
#line 1702 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 407 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().normal_sets.back().push_back((yyvsp[0].ival)); }
#line 1708 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 410 "openivparser.ypp" /* yacc.c:1646  */
    { objects.back().normal_sets.back().push_back((yyvsp[0].ival)); }
#line 1714 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 413 "openivparser.ypp" /* yacc.c:1646  */
    { /* Same idea as the one above for the faceset indices. */
                 vector<int> normal_set;
                 objects.back().normal_sets.push_back(normal_set);
               }
#line 1723 "openivparser.tab.cpp" /* yacc.c:1646  */
    break;


#line 1727 "openivparser.tab.cpp" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
                  yystos[*yyssp], yyvsp);
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
#line 419 "openivparser.ypp" /* yacc.c:1906  */


/* Straight-forward error printing function for parse errors. */
int openiverror(const char *s)
{
    cerr << "\nParser error: " << s << endl;
    openivlex_destroy();
    exit(1);
}

/* The parse function that will be called in the .cpp file. It will parse
 * the Open Inventor file that gets piped into the compiled program and
 * format the data in the file appropriately in an IVData struct. The
 * IVData struct is returned by this function. */
IVData parse_iv(istream &datafile)
{
    if(openivparse()) /* If our parsing fails. */
    {
        cerr << "\nParse failed!" << endl;
        openivlex_destroy();
        exit(1);
    }
    
    openivlex_destroy();
    
    int num_objects = objects.size();
    
    /* We need to delete the last element of each 'facesets' and 'normal_sets'
     * vector since we have extraneous, empty vectors at the end of each one
     * due to the way we parsed the -1 terminators. */
    for(int i = 0; i < num_objects; ++i)
    {
        objects[i].facesets.pop_back();
        objects[i].normal_sets.pop_back();
    }
    
    IVData data;
    data.camera = camera;
    data.objects = objects;
    data.lights = lights;
    
    return data;
}
