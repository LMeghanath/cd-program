%{
#include <stdio.h>
#include <string.h>

void print_days(char *month) {
    if (strcasecmp(month, "january") == 0 || strcasecmp(month, "jan") == 0 ||
        strcasecmp(month, "march") == 0   || strcasecmp(month, "mar") == 0 ||
        strcasecmp(month, "may") == 0     ||
        strcasecmp(month, "july") == 0    || strcasecmp(month, "jul") == 0 ||
        strcasecmp(month, "august") == 0  || strcasecmp(month, "aug") == 0 ||
        strcasecmp(month, "october") == 0 || strcasecmp(month, "oct") == 0 ||
        strcasecmp(month, "december") == 0|| strcasecmp(month, "dec") == 0) {
        printf("%s has 31 days.\n", month);
    } else if (strcasecmp(month, "april") == 0     || strcasecmp(month, "apr") == 0 ||
               strcasecmp(month, "june") == 0      || strcasecmp(month, "jun") == 0 ||
               strcasecmp(month, "september") == 0 || strcasecmp(month, "sep") == 0 ||
               strcasecmp(month, "november") == 0  || strcasecmp(month, "nov") == 0) {
        printf("%s has 30 days.\n", month);
    } else if (strcasecmp(month, "february") == 0  || strcasecmp(month, "feb") == 0) {
        printf("%s has 28 or 29 days.\n", month);
    } else {
        printf("Unknown month: %s\n", month);
    }
}
%}

%%
[a-zA-Z]+   { print_days(yytext); }
[ \t\n]     ;
.           ;
%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter month name:\n");
    yylex();
    return 0;
}
