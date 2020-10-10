/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    LOWER_ELSE = 258,
    TYPE = 259,
    STRUCT = 260,
    IF = 261,
    ELSE = 262,
    WHILE = 263,
    RETURN = 264,
    INT = 265,
    FLOAT = 266,
    CHAR = 267,
    ID = 268,
    LC = 269,
    RC = 270,
    ASSIGN = 271,
    AND = 272,
    OR = 273,
    LT = 274,
    LE = 275,
    GT = 276,
    GE = 277,
    NE = 278,
    EQ = 279,
    PLUS = 280,
    MINUS = 281,
    MUL = 282,
    DIV = 283,
    NOT = 284,
    LP = 285,
    RP = 286,
    LB = 287,
    RB = 288,
    DOT = 289,
    SEMI = 290,
    COMMA = 291
  };
#endif
/* Tokens.  */
#define LOWER_ELSE 258
#define TYPE 259
#define STRUCT 260
#define IF 261
#define ELSE 262
#define WHILE 263
#define RETURN 264
#define INT 265
#define FLOAT 266
#define CHAR 267
#define ID 268
#define LC 269
#define RC 270
#define ASSIGN 271
#define AND 272
#define OR 273
#define LT 274
#define LE 275
#define GT 276
#define GE 277
#define NE 278
#define EQ 279
#define PLUS 280
#define MINUS 281
#define MUL 282
#define DIV 283
#define NOT 284
#define LP 285
#define RP 286
#define LB 287
#define RB 288
#define DOT 289
#define SEMI 290
#define COMMA 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "syntax.y" /* yacc.c:1909  */

    struct ast *ast;

#line 130 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
