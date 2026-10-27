/*
 * LL(1) Table-Driven Predictive Parser
 *
 * Grammar (E' = A, T' = B, id = i):
 *   0: E -> T A
 *   1: A -> + T A
 *   2: A -> epsilon
 *   3: T -> F B
 *   4: B -> * F B
 *   5: B -> epsilon
 *   6: F -> ( E )
 *   7: F -> i
 *
 * To use another grammar, change: NONTERMS, TERMS, prod[], and table[][].
 *
 * Compile: gcc ll1_parser.c -o ll1_parser
 * Run:     ./ll1_parser            (then type e.g.  i+i*i )
 */
#include <stdio.h>
#include <string.h>

#define MAX 100
#define NO_RULE (-1)

/* Nonterminals (rows) and terminals (columns) of the parse table */
const char NONTERMS[] = "EATBF";
const char TERMS[]    = "i+*()$";

/* Productions: right-hand sides ("" means epsilon) */
const char  prod_lhs[] = { 'E', 'A',  'A', 'T', 'B',  'B', 'F',  'F' };
const char *prod_rhs[] = { "TA", "+TA", "",  "FB", "*FB", "",  "(E)", "i" };

/* LL(1) parse table: table[nonterminal][terminal] = production index */
/*                     i   +   *   (   )   $  */
int table[5][6] = {
    /* E */ {  0, -1, -1,  0, -1, -1 },
    /* A */ { -1,  1, -1, -1,  2,  2 },
    /* T */ {  3, -1, -1,  3, -1, -1 },
    /* B */ { -1,  5,  4, -1,  5,  5 },
    /* F */ {  7, -1, -1,  6, -1, -1 }
};

int nt_index(char c) {
    const char *p = strchr(NONTERMS, c);
    return (p && c != '\0') ? (int)(p - NONTERMS) : -1;
}

int t_index(char c) {
    const char *p = strchr(TERMS, c);
    return (p && c != '\0') ? (int)(p - TERMS) : -1;
}

int is_nonterminal(char c) { return nt_index(c) != -1; }

int parse(const char *input) {
    char stack[MAX];
    int top = -1;
    int ip = 0;

    stack[++top] = '$';
    stack[++top] = NONTERMS[0];          /* start symbol */

    printf("\n%-20s %-20s %s\n", "STACK (top left)", "INPUT", "ACTION");
    printf("---------------------------------------------------------------\n");

    while (top >= 0) {
        char X = stack[top];
        char a = input[ip];

        /* print current configuration */
        char st[MAX]; int k = 0;
        for (int i = top; i >= 0; i--) st[k++] = stack[i];
        st[k] = '\0';
        printf("%-20s %-20s ", st, input + ip);

        if (X == '$' && a == '$') {
            printf("Accept\n");
            return 1;
        }

        if (!is_nonterminal(X)) {                 /* terminal (or $) on top */
            if (X == a) {
                printf("Match '%c'\n", a);
                top--; ip++;
            } else {
                printf("Error: expected '%c', found '%c'\n", X, a);
                return 0;
            }
        } else {                                  /* nonterminal on top */
            int r = nt_index(X), c = t_index(a);
            if (c == -1) {
                printf("Error: invalid symbol '%c'\n", a);
                return 0;
            }
            int p = table[r][c];
            if (p == NO_RULE) {
                printf("Error: no entry M[%c,%c]\n", X, a);
                return 0;
            }
            printf("%c -> %s\n", prod_lhs[p], prod_rhs[p][0] ? prod_rhs[p] : "epsilon");
            top--;                                /* pop X */
            int len = (int)strlen(prod_rhs[p]);
            for (int i = len - 1; i >= 0; i--)    /* push RHS reversed */
                stack[++top] = prod_rhs[p][i];
        }
    }
    return 0;
}

int main(void) {
    char line[MAX], input[MAX];

    printf("Grammar:  E->TA  A->+TA|e  T->FB  B->*FB|e  F->(E)|i   (i = id)\n");
    printf("Enter string (use 'i' for id, e.g. i+i*i): ");
    if (!fgets(line, sizeof line, stdin)) return 1;

    /* strip spaces/newline, append end marker */
    int n = 0;
    for (int i = 0; line[i] && n < MAX - 2; i++)
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r')
            input[n++] = line[i];
    input[n++] = '$';
    input[n] = '\0';

    if (parse(input))
        printf("\nResult: String ACCEPTED\n");
    else
        printf("\nResult: String REJECTED\n");
    return 0;
}
