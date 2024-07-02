/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PUBLIC_CLASS = 258,
     CLASS_NAME = 259,
     LEFT_BRACE = 260,
     RIGHT_BRACE = 261,
     QMARK = 262,
     PUBLIC = 263,
     PRIVATE = 264,
     VAR = 265,
     INT = 266,
     DOUBLE = 267,
     CHAR = 268,
     BOOLEAN = 269,
     STRING = 270,
     M_PUBLIC = 271,
     M_PRIVATE = 272,
     VOID = 273,
     INT_NUM = 274,
     DOUBLE_NUM = 275,
     CHAR_VAR = 276,
     STRING_VAR = 277,
     TRUE = 278,
     FALSE = 279,
     DOT = 280,
     LEFT_BRACKET = 281,
     RIGHT_BRACKET = 282,
     COMMA = 283,
     MUL = 284,
     DIV = 285,
     PLUS = 286,
     MINUS = 287,
     EQUAL = 288,
     NEW = 289,
     RETURN = 290,
     DO = 291,
     WHILE = 292,
     AND = 293,
     OR = 294,
     CHECK_EQUAL = 295,
     LESS = 296,
     GREATER = 297,
     NOT_EQUAL = 298,
     FOR = 299,
     SWITCH = 300,
     CASE = 301,
     DOUBLE_DOT = 302,
     DEFAULT = 303,
     BREAK = 304,
     PRINT = 305,
     IF = 306,
     ELSE = 307,
     ELSE_IF = 308,
     SINGLE_COMMENT = 309,
     MULTILINE_COMMENT = 310
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 345 "parser.y"

 char *str;


/* Line 2058 of yacc.c  */
#line 117 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
