
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 15 "parser.y"

    #include "ast.h"
    #include "symbol_table.h"



/* Line 1676 of yacc.c  */
#line 47 "parser.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER_TYPE = 258,
     STRING_TYPE = 259,
     LIST_TYPE = 260,
     FETCH = 261,
     DISPLAY = 262,
     PRINT = 263,
     REPEAT = 264,
     WHEN = 265,
     OTHERWISE = 266,
     BEGONE = 267,
     EQ = 268,
     NEQ = 269,
     LE = 270,
     GE = 271,
     LT = 272,
     GT = 273,
     AND = 274,
     OR = 275,
     ASSIGN = 276,
     SEMICOLON = 277,
     COMMA = 278,
     LPAREN = 279,
     RPAREN = 280,
     LBRACE = 281,
     RBRACE = 282,
     LBRACKET = 283,
     RBRACKET = 284,
     DOT = 285,
     PLUS = 286,
     MINUS = 287,
     MUL = 288,
     DIV = 289,
     NUMBER_LITERAL = 290,
     IDENTIFIER = 291,
     STRING_LITERAL = 292
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 20 "parser.y"

    double fval;
    char *str;
    ASTNode *ast;
    VarType vtype;



/* Line 1676 of yacc.c  */
#line 110 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


