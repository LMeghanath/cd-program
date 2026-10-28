%{
#include <stdio.h>
%}

%%
\/\/[^\n]*                { printf("Single-line Comment: %s\n", yytext); }
\/\*([^*]|\*+[^/*])*\*+\/ { printf("Multi-line Comment:\n%s\n", yytext); }
[ \t\n]                   ;
.                         ; /* Ignore non-comment text */
%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter code to extract comments:\n");
    yylex();
    return 0;
}
