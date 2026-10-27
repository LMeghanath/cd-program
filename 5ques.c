/*
 * Recursive Descent Parser  (Question 5)
 *
 * Grammar (E' = Ep, T' = Tp, id is written as 'i'):
 *   E  -> T E'
 *   E' -> + T E' | epsilon
 *   T  -> F T'
 *   T' -> * F T' | epsilon
 *   F  -> ( E ) | id
 *
 * One function per nonterminal. Each returns 1 on success, 0 on error.
 * Epsilon is chosen only when the lookahead is in FOLLOW:
 *   FOLLOW(E)  = FOLLOW(E') = { ), $ }
 *   FOLLOW(T)  = FOLLOW(T') = { +, ), $ }
 *
 * Compile: gcc rdp_expr.c -o rdp_expr
 * Run:     ./rdp_expr        (then type e.g.  i+i*i )
 */
#include <stdio.h>

#define MAX 200

const char *ip;                 /* input pointer (lookahead = *ip) */

int E(void);
int Ep(void);
int T(void);
int Tp(void);
int F(void);

int error(const char *nt) {
    if (*ip) printf("Error in %s: unexpected '%c'\n", nt, *ip);
    else     printf("Error in %s: unexpected end of input\n", nt);
    return 0;
}

int E(void) {
    printf("E  -> T E'\n");
    return T() && Ep();
}

int Ep(void) {
    if (*ip == '+') {
        printf("E' -> + T E'\n");
        ip++;
        return T() && Ep();
    }
    if (*ip == ')' || *ip == '\0') {
        printf("E' -> epsilon\n");
        return 1;
    }
    return error("E'");
}

int T(void) {
    printf("T  -> F T'\n");
    return F() && Tp();
}

int Tp(void) {
    if (*ip == '*') {
        printf("T' -> * F T'\n");
        ip++;
        return F() && Tp();
    }
    if (*ip == '+' || *ip == ')' || *ip == '\0') {
        printf("T' -> epsilon\n");
        return 1;
    }
    return error("T'");
}

int F(void) {
    if (*ip == '(') {
        printf("F  -> ( E )\n");
        ip++;
        if (!E()) return 0;
        if (*ip != ')') return error("F (expected ')')");
        ip++;
        return 1;
    }
    if (*ip == 'i') {
        printf("F  -> id\n");
        ip++;
        return 1;
    }
    return error("F");
}

int main(void) {
    char line[MAX], input[MAX];

    printf("Grammar: E->TE'  E'->+TE'|e  T->FT'  T'->*FT'|e  F->(E)|id  (i = id)\n");
    printf("Enter string (use 'i' for id, e.g. i+i*i): ");
    if (!fgets(line, sizeof line, stdin)) return 1;

    int n = 0;
    for (int k = 0; line[k] && n < MAX - 1; k++)
        if (line[k] != ' ' && line[k] != '\n' && line[k] != '\r')
            input[n++] = line[k];
    input[n] = '\0';

    ip = input;
    printf("\nParsing steps:\n");
    if (E() && *ip == '\0')
        printf("\nResult: String ACCEPTED\n");
    else {
        if (*ip) printf("Stopped at '%c'\n", *ip);
        printf("\nResult: String REJECTED\n");
    }
    return 0;
}
