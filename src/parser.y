%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "codegen.h"

extern int yylex();
void yyerror(const char *s);

ASTNode *root = NULL;
%}

%code requires {
    #include "ast.h"
    #include "symbol_table.h"
}

%union {
    long double fval;
    char *str;
    ASTNode *ast;
    VarType vtype;
}

%token <vtype> NUMBER_TYPE STRING_TYPE
%token FETCH DISPLAY REPEAT WHEN OTHERWISE BEGONE
%token EQ NEQ LE GE LT GT AND OR
%token ASSIGN SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE
%token PLUS MINUS MUL DIV
%token <fval> NUMBER_LITERAL
%token <str> IDENTIFIER STRING_LITERAL

%type <ast> program statements statement expression

%left OR
%left AND
%nonassoc EQ NEQ
%nonassoc LT GT LE GE
%left PLUS MINUS
%left MUL DIV
%right ASSIGN

%%

program:
    statements { root = $1; }
;

statements:
    statements statement { $$ = append_statements($1, $2); }
  | statement { $$ = $1; }
;

statement:
    NUMBER_TYPE IDENTIFIER ASSIGN expression SEMICOLON {
        check_declaration($2, TYPE_NUMBER);
        $$ = make_declaration_assign_node($2, $4, TYPE_NUMBER);
    }
  | STRING_TYPE IDENTIFIER ASSIGN expression SEMICOLON {
        check_declaration($2, TYPE_STRING);
        $$ = make_declaration_assign_node($2, $4, TYPE_STRING);
    }
  | NUMBER_TYPE IDENTIFIER SEMICOLON {
        check_declaration($2, TYPE_NUMBER);
        $$ = make_declaration_node($2, TYPE_NUMBER);
    }
  | STRING_TYPE IDENTIFIER SEMICOLON {
        check_declaration($2, TYPE_STRING);
        $$ = make_declaration_node($2, TYPE_STRING);
    }
  | IDENTIFIER ASSIGN expression SEMICOLON {
        check_variable_declared($1);
        $$ = make_assign_node($1, $3);
    }
  | FETCH LPAREN IDENTIFIER RPAREN SEMICOLON {
        check_variable_declared($3);
        $$ = make_fetch_node($3);
    }
  | DISPLAY LPAREN IDENTIFIER RPAREN SEMICOLON {
        check_variable_declared($3);
        $$ = make_display_node($3);
    }
  | REPEAT LPAREN expression RPAREN LBRACE statements RBRACE {
        $$ = make_repeat_node($3, $6);
    }
  | WHEN LPAREN expression RPAREN LBRACE statements RBRACE {
        $$ = make_when_node($3, $6, NULL);
    }
  | WHEN LPAREN expression RPAREN LBRACE statements RBRACE OTHERWISE LBRACE statements RBRACE {
        $$ = make_when_node($3, $6, $10);
    }
  | BEGONE SEMICOLON {
        $$ = make_break_node();
    }
;

expression:
    IDENTIFIER {
        check_variable_declared($1);
        $$ = make_var_node($1);
    }
  | NUMBER_LITERAL {
        $$ = make_number_node($1);
    }
  | STRING_LITERAL {
        $$ = make_string_node($1);
    }
  | expression PLUS expression {
        if ($1->type == AST_STRING || $1->type == AST_STRING_CONCAT || $3->type == AST_STRING || $3->type == AST_STRING_CONCAT) {
            $$ = make_string_concat_node($1, $3);
        } else {
            $$ = make_binary_node(AST_ADD, $1, $3);
        }
    }
  | expression MINUS expression {
        $$ = make_binary_node(AST_SUB, $1, $3);
    }
  | expression MUL expression {
        $$ = make_binary_node(AST_MUL, $1, $3);
    }
  | expression DIV expression {
        $$ = make_binary_node(AST_DIV, $1, $3);
    }
  | expression EQ expression {
        $$ = make_binary_node(AST_EQ, $1, $3);
    }
  | expression NEQ expression {
        $$ = make_binary_node(AST_NEQ, $1, $3);
    }
  | expression LT expression {
        $$ = make_binary_node(AST_LT, $1, $3);
    }
  | expression GT expression {
        $$ = make_binary_node(AST_GT, $1, $3);
    }
  | expression LE expression {
        $$ = make_binary_node(AST_LE, $1, $3);
    }
  | expression GE expression {
        $$ = make_binary_node(AST_GE, $1, $3);
    }
  | expression AND expression {
        $$ = make_binary_node(AST_AND, $1, $3);
    }
  | expression OR expression {
        $$ = make_binary_node(AST_OR, $1, $3);
    }
  | LPAREN expression RPAREN {
        $$ = $2;
    }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
