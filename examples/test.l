%{
    #include <stdio.h>
%}

%%
 { 
        printf("Valid Email: %s\n", yytext); 
}
%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}