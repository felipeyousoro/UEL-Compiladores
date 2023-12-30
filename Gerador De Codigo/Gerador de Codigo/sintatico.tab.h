/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SINTATICO_TAB_H_INCLUDED
# define YY_YY_SINTATICO_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    START = 258,
    FUNCTION = 259,
    RETURN_TYPE = 260,
    PARAMETER = 261,
    RETURN = 262,
    END_FUNCTION = 263,
    PRINTF = 264,
    SCANF = 265,
    CONSTANT = 266,
    GLOBAL_VARIABLE = 267,
    VALUE = 268,
    VARIABLE = 269,
    TYPE = 270,
    IF = 271,
    ID = 272,
    INT = 273,
    CHAR = 274,
    VOID = 275,
    BITWISE_AND = 276,
    BITWISE_OR = 277,
    BITWISE_XOR = 278,
    CHARACTER = 279,
    STRING = 280,
    NUM_INTEGER = 281,
    PLUS = 282,
    MINUS = 283,
    MULTIPLY = 284,
    DIV = 285,
    REMAINDER = 286,
    LESS_THAN = 287,
    GREATER_THAN = 288,
    LESS_EQUAL = 289,
    GREATER_EQUAL = 290,
    SEMICOLON = 291,
    COMMA = 292,
    L_PAREN = 293,
    R_PAREN = 294,
    L_SQUARE_BRACKET = 295,
    R_SQUARE_BRACKET = 296,
    POINTER = 297,
    NUMBER_SIGN = 298,
    L_CURLY_BRACKET = 299,
    R_CURLY_BRACKET = 300,
    COLON = 301,
    TERNARY_CONDITIONAL = 302,
    PLUS_ASSIGN = 303,
    MINUS_ASSIGN = 304,
    ASSIGN = 305,
    R_SHIFT = 306,
    L_SHIFT = 307,
    EQUAL = 308,
    NOT_EQUAL = 309,
    LOGICAL_OR = 310,
    LOGICAL_AND = 311,
    LOGICAL_NOT = 312,
    BITWISE_NOT = 313,
    DEC = 314,
    INC = 315,
    NEWLINE = 316,
    WHILE = 317,
    DO_WHILE = 318,
    FOR = 319,
    EXIT = 320
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 83 "sintatico.y"

    int integer;
    char character;
    char* string;
    void* ast_node;

#line 130 "sintatico.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SINTATICO_TAB_H_INCLUDED  */
