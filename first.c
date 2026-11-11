#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 20

int n; // Number of productions
char production[MAX][MAX];
char first_set[MAX];
int found_count = 0;

void find_first(char c);
void add_to_result(char c);

int main() {
    int i;
    char choice;
    char c;

    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (e.g., E=TR, E=# for epsilon):\n");
    for (i = 0; i < n; i++) {
        scanf("%s", production[i]);
    }

    do {
        found_count = 0;
        printf("\nEnter the non-terminal to find FIRST: ");
        scanf(" %c", &c);

        find_first(c);

        printf("FIRST(%c) = { ", c);
        for (i = 0; i < found_count; i++) {
            printf("%c ", first_set[i]);
        }
        printf("}\n");

        printf("Do you want to continue (y/n)? ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    return 0;
}

/* Recursive function to calculate FIRST of a symbol */
void find_first(char c) {
    int i, j;
    int has_epsilon;

    // If c is a terminal or epsilon, FIRST(c) = {c}
    if (!isupper(c)) {
        add_to_result(c);
        return;
    }

    // If c is a non-terminal, look through productions where LHS == c
    for (i = 0; i < n; i++) {
        if (production[i][0] == c) {
            // Check RHS starting from index 2 (after 'X=')
            if (production[i][2] == '#') {
                add_to_result('#');
            } else {
                j = 2;
                while (production[i][j] != '\0') {
                    has_epsilon = 0;
                    int prev_count = found_count;

                    find_first(production[i][j]);

                    // Check if epsilon '#' was added by the recursive call
                    for (int k = prev_count; k < found_count; k++) {
                        if (first_set[k] == '#') {
                            has_epsilon = 1;
                            // Remove '#' unless it is the last symbol on RHS
                            if (production[i][j + 1] != '\0') {
                                first_set[k] = first_set[found_count - 1];
                                found_count--;
                                k--;
                            }
                        }
                    }

                    // If current RHS symbol cannot derive epsilon, stop checking next symbols
                    if (!has_epsilon) {
                        break;
                    }
                    j++;
                }
            }
        }
    }
}

/* Helper function to prevent duplicate entries in the result set */
void add_to_result(char c) {
    for (int i = 0; i < found_count; i++) {
        if (first_set[i] == c) {
            return;
        }
    }
    first_set[found_count++] = c;
}
