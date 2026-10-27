/*
 * SLR(1) Table-Driven (Shift-Reduce) Parser
 *
 * Grammar (id is written as 'i'):
 *   1: E -> E + T
 *   2: E -> T
 *   3: T -> T * F
 *   4: T -> F
 *   5: F -> ( E )
 *   6: F -> id
 *
 * To use another grammar, change: TERMS, NONTERMS, prod_lhs[], prod_len[],
 * prod_str[], action[][] and go_to[][].
 *
 * Compile: gcc slr_parser.c -o slr_parser
 * Run:     ./slr_parser        (then type e.g.  i+i*i )
 */
#include <stdio.h>
#include <string.h>

#define MAX 200

/* Action encoding in the table */
#define ERR  (-1)
#define ACC  200
#define S(n) (n)          /* shift to state n   (0..99)    */
#define R(n) (100 + (n))  /* reduce by prod n   (101..199) */

const char TERMS[]    = "i+*()$";   /* ACTION columns */
const char NONTERMS[] = "ETF";      /* GOTO columns   */

/* Productions (index 0 unused so numbering matches 1..6) */
const char  prod_lhs[] = { ' ', 'E',     'E',   'T',     'T',   'F',    'F'   };
const int   prod_len[] = {  0,   3,       1,     3,       1,     3,      1    };
const char *prod_str[] = { "", "E->E+T", "E->T", "T->T*F", "T->F", "F->(E)", "F->id" };

/*                  i      +      *      (      )      $   */
int action[12][6] = {
/* 0  */ { S(5),  ERR,   ERR,   S(4),  ERR,   ERR  },
/* 1  */ { ERR,   S(6),  ERR,   ERR,   ERR,   ACC  },
/* 2  */ { ERR,   R(2),  S(7),  ERR,   R(2),  R(2) },
/* 3  */ { ERR,   R(4),  R(4),  ERR,   R(4),  R(4) },
/* 4  */ { S(5),  ERR,   ERR,   S(4),  ERR,   ERR  },
/* 5  */ { ERR,   R(6),  R(6),  ERR,   R(6),  R(6) },
/* 6  */ { S(5),  ERR,   ERR,   S(4),  ERR,   ERR  },
/* 7  */ { S(5),  ERR,   ERR,   S(4),  ERR,   ERR  },
/* 8  */ { ERR,   S(6),  ERR,   ERR,   S(11), ERR  },
/* 9  */ { ERR,   R(1),  S(7),  ERR,   R(1),  R(1) },
/* 10 */ { ERR,   R(3),  R(3),  ERR,   R(3),  R(3) },
/* 11 */ { ERR,   R(5),  R(5),  ERR,   R(5),  R(5) }
};

/*                 E    T    F   */
int go_to[12][3] = {
/* 0  */ {  1,   2,   3 },
/* 1  */ { -1,  -1,  -1 },
/* 2  */ { -1,  -1,  -1 },
/* 3  */ { -1,  -1,  -1 },
/* 4  */ {  8,   2,   3 },
/* 5  */ { -1,  -1,  -1 },
/* 6  */ { -1,   9,   3 },
/* 7  */ { -1,  -1,  10 },
/* 8  */ { -1,  -1,  -1 },
/* 9  */ { -1,  -1,  -1 },
/* 10 */ { -1,  -1,  -1 },
/* 11 */ { -1,  -1,  -1 }
};

int t_index(char c) {
    const char *p = strchr(TERMS, c);
    return (p && c) ? (int)(p - TERMS) : -1;
}
int nt_index(char c) {
    const char *p = strchr(NONTERMS, c);
    return (p && c) ? (int)(p - NONTERMS) : -1;
}

/* Print stack as:  0 i 5 + 6 ... (states and symbols interleaved) */
void print_stack(const int *st, const char *sym, int top) {
    char buf[MAX * 4]; int k = 0;
    for (int i = 0; i <= top; i++) {
        if (i > 0) k += sprintf(buf + k, "%c ", sym[i]);
        k += sprintf(buf + k, "%d ", st[i]);
    }
    buf[k] = '\0';
    printf("%-28s", buf);
}

int parse(const char *input) {
    int  states[MAX];
    char syms[MAX];
    int  top = 0, ip = 0;

    states[0] = 0;
    syms[0]   = '-';

    printf("\n%-28s%-16s%s\n", "STACK", "INPUT", "ACTION");
    printf("---------------------------------------------------------------\n");

    for (;;) {
        int s = states[top];
        char a = input[ip];
        int c = t_index(a);

        print_stack(states, syms, top);
        printf("%-16s", input + ip);

        if (c == -1) {
            printf("Error: invalid symbol '%c'\n", a);
            return 0;
        }

        int act = action[s][c];

        if (act == ERR) {
            printf("Error: no action[%d,%c]\n", s, a);
            return 0;
        } else if (act == ACC) {
            printf("Accept\n");
            return 1;
        } else if (act < 100) {                       /* shift */
            printf("Shift %d\n", act);
            syms[++top]   = a;
            states[top]   = act;
            ip++;
        } else {                                      /* reduce */
            int p = act - 100;
            printf("Reduce by %s\n", prod_str[p]);
            top -= prod_len[p];                       /* pop |RHS| symbols */
            int t = states[top];
            int g = go_to[t][nt_index(prod_lhs[p])];
            if (g == -1) {
                printf("Error: no goto[%d,%c]\n", t, prod_lhs[p]);
                return 0;
            }
            syms[++top] = prod_lhs[p];
            states[top] = g;
        }
    }
}

int main(void) {
    char line[MAX], input[MAX];

    printf("Grammar: E->E+T|T   T->T*F|F   F->(E)|id   (i = id)\n");
    printf("Enter string (use 'i' for id, e.g. i+i*i): ");
    if (!fgets(line, sizeof line, stdin)) return 1;

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
