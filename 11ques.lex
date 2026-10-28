%{
#include <stdio.h>
#include <stdlib.h>
%}

%%
[0-9]+\.[0-9]+ {
    int int_val = (int)atof(yytext);
    printf("Real number '%s' converted to integer: %d\n", yytext, int_val);
}
[0-9]+ {
    printf("Integer: %s\n", yytext);
}
[ \t\n] ;
. { ECHO; }
%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter text with real numbers:\n");
    yylex();
    return 0;
}
