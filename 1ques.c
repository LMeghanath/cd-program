#include <stdio.h>   /* only for printf and scanf (I/O) */

/* Flow control keywords of C */
char *keywords[] = {
    "if", "else", "switch", "case", "default",
    "for", "while", "do", "break", "continue",
    "goto", "return"
};
int nKeywords = 12;

/* own string compare: returns 1 if equal, 0 otherwise */
int isEqual(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i])
            return 0;
        i++;
    }
    return a[i] == b[i];   /* both must end together */
}

int isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

/* Returns 1 if str is a flow control keyword */
int isKeyword(char *str) {
    int i;
    for (i = 0; i < nKeywords; i++)
        if (isEqual(str, keywords[i]))
            return 1;
    return 0;
}

/* Returns 1 if str is a valid C identifier */
int isIdentifier(char *str) {
    int i;
    if (!(isLetter(str[0]) || str[0] == '_'))
        return 0;
    for (i = 1; str[i] != '\0'; i++)
        if (!(isLetter(str[i]) || isDigit(str[i]) || str[i] == '_'))
            return 0;
    return 1;
}

int main() {
    char str[100];
    char choice;

    do {
        printf("\nEnter a string: ");
        scanf("%99s", str);

        if (isKeyword(str))
            printf("\"%s\" is a KEYWORD (flow control statement)\n", str);
        else if (isIdentifier(str))
            printf("\"%s\" is a valid IDENTIFIER\n", str);
        else
            printf("\"%s\" is neither a flow control keyword nor a valid identifier\n", str);

        printf("Continue? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    return 0;
}
