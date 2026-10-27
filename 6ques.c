/*
 * Recursive Descent Parser  (Question 6)
 *
 * Grammar (L' = Lp):
 *   S  -> ( L ) | a
 *   L  -> S L'
 *   L' -> , S L' | epsilon
 *
 * One function per nonterminal. Each returns 1 on success, 0 on error.
 * Epsilon for L' is chosen only when the lookahead is in FOLLOW(L') = { ) }.
 *
 * Compile: gcc rdp_list.c -o rdp_list
 * Run:     ./rdp_list        (then type e.g.  (a,(a,a),a) )
 */
#include <stdio.h>

#define MAX 200

const char *ip;                 /* input pointer (lookahead = *ip) */

int S(void);
int L(void);
int Lp(void);

int error(const char *nt) {
    if (*ip) printf("Error in %s: unexpected '%c'\n", nt, *ip);
    else     printf("Error in %s: unexpected end of input\n", nt);
    return 0;
}

int S(void) {
    if (*ip == '(') {
        printf("S  -> ( L )\n");
        ip++;
        if (!L()) return 0;
        if (*ip != ')') return error("S (expected ')')");
        ip++;
        return 1;
    }
    if (*ip == 'a') {
        printf("S  -> a\n");
        ip++;
        return 1;
    }
    return error("S");
}

int L(void) {
    printf("L  -> S L'\n");
    return S() && Lp();
}

int Lp(void) {
    if (*ip == ',') {
        printf("L' -> , S L'\n");
        ip++;
        return S() && Lp();
    }
    if (*ip == ')') {
        printf("L' -> epsilon\n");
        return 1;
    }
    return error("L'");
}

int main(void) {
    char line[MAX], input[MAX];

    printf("Grammar: S->(L)|a   L->SL'   L'->,SL'|e\n");
    printf("Enter string (e.g. (a,(a,a),a)): ");
    if (!fgets(line, sizeof line, stdin)) return 1;

    int n = 0;
    for (int k = 0; line[k] && n < MAX - 1; k++)
        if (line[k] != ' ' && line[k] != '\n' && line[k] != '\r')
            input[n++] = line[k];
    input[n] = '\0';

    ip = input;
    printf("\nParsing steps:\n");
    if (S() && *ip == '\0')
        printf("\nResult: String ACCEPTED\n");
    else {
        if (*ip) printf("Stopped at '%c'\n", *ip);
        printf("\nResult: String REJECTED\n");
    }
    return 0;
}
