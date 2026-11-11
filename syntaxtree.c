#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Node {
    char op;                // Operator or '\0' for operands
    char operand[10];       // Variable or constant identifier
    struct Node *left;
    struct Node *right;
} Node;

char expr[100];
int pos = 0;
int tempCount = 1;

// Helper to create a leaf node (operand)
Node* createOperandNode(char val) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->op = '\0';
    node->operand[0] = val;
    node->operand[1] = '\0';
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Helper to create an operator node
Node* createOpNode(char op, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->op = op;
    node->operand[0] = '\0';
    node->left = left;
    node->right = right;
    return node;
}

// Forward declarations for recursive descent parsing
Node* parseExpression();
Node* parseTerm();
Node* parseFactor();

Node* parseFactor() {
    if (isalnum(expr[pos])) {
        return createOperandNode(expr[pos++]);
    } else if (expr[pos] == '(') {
        pos++; // Skip '('
        Node* node = parseExpression();
        if (expr[pos] == ')') {
            pos++; // Skip ')'
        }
        return node;
    }
    return NULL;
}

Node* parseTerm() {
    Node* left = parseFactor();

    while (expr[pos] == '*' || expr[pos] == '/') {
        char op = expr[pos++];
        Node* right = parseFactor();
        left = createOpNode(op, left, right);
    }
    return left;
}

Node* parseExpression() {
    Node* left = parseTerm();

    while (expr[pos] == '+' || expr[pos] == '-') {
        char op = expr[pos++];
        Node* right = parseTerm();
        left = createOpNode(op, left, right);
    }
    return left;
}

// Generate Three-Address Code via post-order traversal
// Out parameter 'res' stores the identifier (e.g., "a" or "t1")
void generate3AC(Node* root, char* res) {
    if (root == NULL) return;

    // Leaf node: return operand directly
    if (root->op == '\0') {
        strcpy(res, root->operand);
        return;
    }

    char leftRes[10];
    char rightRes[10];

    // Traverse left and right subtrees first
    generate3AC(root->left, leftRes);
    generate3AC(root->right, rightRes);

    // Generate intermediate variable for this operation
    sprintf(res, "t%d", tempCount++);
    printf("%s = %s %c %s\n", res, leftRes, root->op, rightRes);
}

// Free tree memory
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    printf("Enter expression (e.g., a+b*c): ");
    if (scanf("%99s", expr) != 1) return 1;

    Node* root = parseExpression();

    printf("\n--- Generated Three-Address Code ---\n");
    char finalTarget[10];
    generate3AC(root, finalTarget);

    printf("Result stored in %s\n", finalTarget);

    freeTree(root);
    return 0;
}
