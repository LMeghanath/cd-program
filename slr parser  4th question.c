
#include <stdio.h>
#include <string.h>

#define MAX 200

#define ERR  (-1)
#define ACC  200
#define S(n) (n)          /* shift to state n   */
#define R(n) (100 + (n))  /* reduce by prod n   */

const char TERMS[]    = "ab$";  
const char NONTERMS[] = "SAB";  


const char  prod_lhs[] = { ' ', 'S',     'A',    'A',     'B',    'B'    };
const int   prod_len[] = {  0,   2,       1,      0,       1,      0     };
const char *prod_str[] = { "", "S->AB", "A->a", "A->e", "B->b", "B->e" };

/*                a      b      $   */
int action[6][3] = {
/* 0 */ { S(3),  R(3),  R(3) },
/* 1 */ { ERR,   ERR,   ACC  },
/* 2 */ { ERR,   S(5),  R(5) },
/* 3 */ { ERR,   R(2),  R(2) },
/* 4 */ { ERR,   ERR,   R(1) },
/* 5 */ { ERR,   ERR,   R(4) }
};

/*               S    A    B   */
int go_to[6][3] = {
/* 0 */ {  1,   2,  -1 },
/* 1 */ { -1,  -1,  -1 },
/* 2 */ { -1,  -1,   4 },
/* 3 */ { -1,  -1,  -1 },
/* 4 */ { -1,  -1,  -1 },
/* 5 */ { -1,  -1,  -1 }
};

int t_index(char c) {
    const char *p = strchr(TERMS, c);
    return (p && c) ? (int)(p - TERMS) : -1;
}
int nt_index(char c) {
    const char *p = strchr(NONTERMS, c);
    return (p && c) ? (int)(p - NONTERMS) : -1;
}

void print_stack(const int *st, const char *sym, int top) {
    char buf[MAX * 4]; int k = 0;
    for (int i = 0; i <= top; i++) {
        if (i > 0) k += sprintf(buf + k, "%c ", sym[i]);
        k += sprintf(buf + k, "%d ", st[i]);
    }
    buf[k] = '\0';
    printf("%-20s", buf);
}

int parse(const char *input) {
    int  states[MAX];
    char syms[MAX];
    int  top = 0, ip = 0;

    states[0] = 0;
    syms[0]   = '-';

    printf("\n%-20s%-12s%s\n", "STACK", "INPUT", "ACTION");
    printf("-----------------------------------------------------\n");

    for (;;) {
        int s = states[top];
        char a = input[ip];
        int c = t_index(a);

        print_stack(states, syms, top);
        printf("%-12s", input + ip);

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
        } else if (act < 100) {                       
            printf("Shift %d\n", act);
            syms[++top] = a;
            states[top] = act;
            ip++;
        } else {                                      
            int p = act - 100;
            printf("Reduce by %s\n", prod_str[p]);
            top -= prod_len[p];                       
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

    printf("Grammar: S->AB   A->a|e   B->b|e\n");
    printf("Enter string (empty line = epsilon): ");
    if (!fgets(line, sizeof line, stdin)) line[0] = '\0';

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
