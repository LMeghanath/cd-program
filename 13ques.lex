%{
#include <stdio.h>
#include <stdlib.h>

char *units[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
char *teens[] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
char *tens[]  = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

void convert_to_words(int n) {
    if (n < 0 || n > 1000) {
        printf("Out of range (0-1000)\n");
        return;
    }
    if (n == 1000) {
        printf("one thousand\n");
        return;
    }
    if (n == 0) {
        printf("zero\n");
        return;
    }

    if (n >= 100) {
        printf("%s hundred", units[n / 100]);
        n %= 100;
        if (n > 0) printf(" and ");
        else { printf("\n"); return; }
    }

    if (n >= 20) {
        printf("%s", tens[n / 10]);
        if (n % 10 != 0) {
            printf(" %s", units[n % 10]);
        }
    } else if (n >= 10) {
        printf("%s", teens[n - 10]);
    } else if (n > 0) {
        printf("%s", units[n]);
    }
    printf("\n");
}
%}

%%
[0-9]+  { convert_to_words(atoi(yytext)); }
[ \t\n] ;
.       ;
%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter a number (0-1000):\n");
    yylex();
    return 0;
}
