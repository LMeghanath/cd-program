%{
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_random_number() {
    int num = (rand() % 1000) + 1; // Generates a random number from 1 to 1000
    printf("Random Number: %d\n", num);
}
%}

%%
generate|random { print_random_number(); }
exit            { return 0; }
[ \t\n]+        ; /* Ignore whitespace */
.               { printf("Type 'generate' to get a random number, or 'exit' to quit.\n"); }
%%

int yywrap() {
    return 1;
}

int main() {
    srand(time(NULL));
    printf("Type 'generate' to print a random number between 1 and 1000:\n");
    yylex();
    return 0;
}
