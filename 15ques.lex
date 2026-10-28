%{
#include <stdio.h>
%}

FLOW_KEYWORDS  if|else|switch|case|default|while|do|for|break|continue|goto|return
IDENTIFIER     [a-zA-Z_][a-zA-Z0-9_]*

%%
{FLOW_KEYWORDS}   { printf("Flow Control Keyword: %s\n", yytext); }
{IDENTIFIER}      { printf("Identifier: %s\n", yytext); }
[ \t\n]           ;
.                 ; /* Ignore operators, symbols, etc. */
%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter C code snippet:\n");
    yylex();
    return 0;
}
