/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

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
#line 54 "openivparser.ypp" /* yacc.c:1909  */

    float fval;
    int ival;

#line 99 "openivparser.tab.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE openivlval;

int openivparse (void);

#endif /* !YY_OPENIV_OPENIVPARSER_TAB_HPP_INCLUDED  */
