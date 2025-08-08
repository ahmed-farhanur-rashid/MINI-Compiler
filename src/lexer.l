
%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
int yylineno;
%}

%x COMMENT

%%
"---"              { BEGIN(COMMENT); }
<COMMENT>"---"      { BEGIN(INITIAL); }
<COMMENT>.         ; /* skip any character inside comment */
<COMMENT>\n        ; /* skip newlines inside comment */

"number"           { return NUMBER_TYPE; }
"string"           { return STRING_TYPE; }
"fetch"            { return FETCH; }
"display"          { return DISPLAY; }
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
"+"                { return PLUS; }
"-"                { return MINUS; }
"*"                { return MUL; }
"/"                { return DIV; }

\"([^"\n]*)\"      {
    yylval.str = strdup(yytext + 1);
    yylval.str[strlen(yylval.str) - 1] = '\0'; /* remove trailing " */
    return STRING_LITERAL;
}

[0-9]+(\.[0-9]+)?  {
    yylval.fval = atof(yytext);
    return NUMBER_LITERAL;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

[ \t\r\n]+          ; /* skip whitespace */

.                    { fprintf(stderr, "Unknown char: %s at line %d\n", yytext, yylineno); exit(1); }

%%

int yywrap(void) {
    return 1;
}
