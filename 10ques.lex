%{
#include <stdio.h>
%}

%%
\+?[0-9]+           { printf("Positive Integer: %s\n", yytext); }
-[0-9]+             { printf("Negative Integer: %s\n", yytext); }
\+?[0-9]+\.[0-9]+   { printf("Positive Real Number: %s\n", yytext); }
-[0-9]+\.[0-9]+     { printf("Negative Real Number: %s\n", yytext); }
[ \t\n]             ; /* Ignore whitespace */
.                   { printf("Unrecognized character: %s\n", yytext); }
%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter numbers to classify:\n");
    yylex();
    return 0;
}
