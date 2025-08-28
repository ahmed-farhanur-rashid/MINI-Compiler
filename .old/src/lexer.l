
%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
int yylineno;
%}

%x COMMENT

%%
"---"              { BEGIN(COMMENT); }
<COMMENT>"---"     { BEGIN(INITIAL); }
<COMMENT>.         ; /* skip any character inside comment */
<COMMENT>\n        ; /* skip newlines inside comment */

"number"           { yylval.vtype = TYPE_NUMBER; return NUMBER_TYPE; }
"string"           { yylval.vtype = TYPE_STRING; return STRING_TYPE; }
"list"             { return LIST_TYPE; }
"fetch"            { return FETCH; }
"display"          { return DISPLAY; }
"print"            { return PRINT; }
"repeat"           { return REPEAT; }
"when"             { return WHEN; }
"otherwise"        { return OTHERWISE; }
"begone"           { return BEGONE; }

"=="               { return EQ; }
"!="               { return NEQ; }
"<="               { return LE; }
">="               { return GE; }
"<"                { return LT; }
">"                { return GT; }
"and"              { return AND; }
"or"               { return OR; }

"="                { return ASSIGN; }
";"                { return SEMICOLON; }
","                { return COMMA; }
"("                { return LPAREN; }
")"                { return RPAREN; }
"{"                { return LBRACE; }
"}"                { return RBRACE; }
"["                { return LBRACKET; }
"]"                { return RBRACKET; }
"<"                { return LT; }
">"                { return GT; }
"."                { return DOT; }
"+"                { return PLUS; }
"-"                { return MINUS; }
"*"                { return MUL; }
"/"                { return DIV; }

\"([^"\\]*(\\.[^"\\]*)*)\"      {
    yylval.str = strdup(yytext + 1);
    if (!yylval.str) {
        fprintf(stderr, "Memory allocation failed for string literal\n");
        exit(1);
    }
    yylval.str[strlen(yylval.str) - 1] = '\0'; /* remove trailing " */
    
    /* Process escape sequences */
    char *src = yylval.str, *dst = yylval.str;
    while (*src) {
        if (*src == '\\' && *(src + 1)) {
            switch (*(src + 1)) {
                case 'n': *dst++ = '\n'; src += 2; break;
                case 't': *dst++ = '\t'; src += 2; break;
                case 'r': *dst++ = '\r'; src += 2; break;
                case '\\': *dst++ = '\\'; src += 2; break;
                case '"': *dst++ = '"'; src += 2; break;
                default: *dst++ = *src++; break;
            }
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    return STRING_LITERAL;
}

-?[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?  {
    yylval.fval = atof(yytext);
    return NUMBER_LITERAL;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    if (!yylval.str) {
        fprintf(stderr, "Memory allocation failed for identifier\n");
        exit(1);
    }
    return IDENTIFIER;
}

[ \t\r\n]+          ; /* skip whitespace */

.                    { fprintf(stderr, "Unknown char: %s at line %d\n", yytext, yylineno); exit(1); }

%%

int yywrap(void) {
    return 1;
}
