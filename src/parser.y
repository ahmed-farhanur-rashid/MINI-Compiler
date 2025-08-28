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
    double fval;
    char *str;
    ASTNode *ast;
    VarType vtype;
}

%token <vtype> NUMBER_TYPE STRING_TYPE
%token LIST_TYPE
%token FETCH DISPLAY PRINT REPEAT WHEN OTHERWISE BEGONE
%token EQ NEQ LE GE LT GT AND OR
%token ASSIGN SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET DOT
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
  | LIST_TYPE LT NUMBER_TYPE GT IDENTIFIER LPAREN expression RPAREN SEMICOLON {
        check_declaration($5, TYPE_LIST_NUMBER);
        $$ = make_list_decl_node($5, LIST_TYPE_NUMBER, $7);
    }
  | LIST_TYPE LT STRING_TYPE GT IDENTIFIER LPAREN expression RPAREN SEMICOLON {
        check_declaration($5, TYPE_LIST_STRING);
        $$ = make_list_decl_node($5, LIST_TYPE_STRING, $7);
    }
  | IDENTIFIER ASSIGN expression SEMICOLON {
        check_variable_declared($1);
        $$ = make_assign_node($1, $3);
    }
  | IDENTIFIER LBRACKET expression RBRACKET ASSIGN expression SEMICOLON {
        check_variable_declared($1);
        ASTNode *access = make_list_access_node($1, $3);
        $$ = make_assign_node(NULL, $6);  // Special assignment handling needed in codegen
        $$->left = access;
    }
  | IDENTIFIER DOT IDENTIFIER LPAREN expression RPAREN SEMICOLON {
        check_variable_declared($1);
        $$ = make_list_func_node($1, $3, $5);
    }
  | IDENTIFIER DOT IDENTIFIER LPAREN RPAREN SEMICOLON {
        check_variable_declared($1);
        $$ = make_list_func_node($1, $3, NULL);
    }
  | FETCH LPAREN IDENTIFIER RPAREN SEMICOLON {
        check_variable_declared($3);
        $$ = make_fetch_node($3);
    }
  | DISPLAY LPAREN expression RPAREN SEMICOLON {
        $$ = make_display_expression_node($3);
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
  | IDENTIFIER LPAREN expression RPAREN {
        if (strcmp($1, "toString") == 0) {
            $$ = make_tostring_node($3);
        } else if (strcmp($1, "toNumber") == 0) {
            $$ = make_tonumber_node($3);
        } else {
            yyerror("Unknown function");
            YYABORT;
        }
    }
  | IDENTIFIER LBRACKET expression RBRACKET {
        check_variable_declared($1);
        $$ = make_list_access_node($1, $3);
    }
  | IDENTIFIER DOT IDENTIFIER LPAREN expression RPAREN {
        check_variable_declared($1);
        $$ = make_list_func_node($1, $3, $5);
    }
  | IDENTIFIER DOT IDENTIFIER LPAREN RPAREN {
        check_variable_declared($1);
        $$ = make_list_func_node($1, $3, NULL);
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
  /*| MINUS expression %prec UMINUS {
        $$ = make_unary_node(AST_NEGATE, $2);
    }*/
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
