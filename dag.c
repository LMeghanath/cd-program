#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_NODES 100

typedef struct DAGNode {
    int id;                 // Unique identifier in the DAG table
    char op;                // Operator, or '\0' for leaf nodes
    char val[10];           // Variable or constant identifier
    struct DAGNode *left;
    struct DAGNode *right;
    char tempVar[10];       // Stores generated 3AC name (e.g., "t1" or variable name)
    int visited;            // Guard against emitting duplicate 3AC for shared subgraphs
} DAGNode;

DAGNode* nodeTable[MAX_NODES];
int nodeCount = 0;
int tempCounter = 1;

char expr[100];
int pos = 0;

// Search for or create a leaf node (operand)
DAGNode* getOrCreateLeaf(char val) {
    char str[2] = {val, '\0'};
    for (int i = 0; i < nodeCount; i++) {
        if (nodeTable[i]->op == '\0' && strcmp(nodeTable[i]->val, str) == 0) {
            return nodeTable[i]; // Reuse identical leaf
        }
    }

    DAGNode* node = (DAGNode*)malloc(sizeof(DAGNode));
    node->id = nodeCount;
    node->op = '\0';
    strcpy(node->val, str);
    node->left = NULL;
    node->right = NULL;
    strcpy(node->tempVar, str); // Operands act as their own value representation
    node->visited = 0;

    nodeTable[nodeCount++] = node;
    return node;
}

// Search for or create an interior node (operator)
DAGNode* getOrCreateOp(char op, DAGNode* left, DAGNode* right) {
    // Check if an identical operation already exists with the same children
    for (int i = 0; i < nodeCount; i++) {
        if (nodeTable[i]->op == op && 
            nodeTable[i]->left == left && 
            nodeTable[i]->right == right) {
            return nodeTable[i]; // Reuse common subexpression
        }
    }

    DAGNode* node = (DAGNode*)malloc(sizeof(DAGNode));
    node->id = nodeCount;
    node->op = op;
    node->val[0] = '\0';
    node->left = left;
    node->right = right;
    node->tempVar[0] = '\0';
    node->visited = 0;

    nodeTable[nodeCount++] = node;
    return node;
}

// Recursive descent parsing
DAGNode* parseExpression();
DAGNode* parseTerm();
DAGNode* parseFactor();

DAGNode* parseFactor() {
    if (isalnum(expr[pos])) {
        return getOrCreateLeaf(expr[pos++]);
    } else if (expr[pos] == '(') {
        pos++;
        DAGNode* node = parseExpression();
        if (expr[pos] == ')') pos++;
        return node;
    }
    return NULL;
}

DAGNode* parseTerm() {
    DAGNode* left = parseFactor();

    while (expr[pos] == '*' || expr[pos] == '/') {
        char op = expr[pos++];
        DAGNode* right = parseFactor();
        left = getOrCreateOp(op, left, right);
    }
    return left;
}

DAGNode* parseExpression() {
    DAGNode* left = parseTerm();

    while (expr[pos] == '+' || expr[pos] == '-') {
        char op = expr[pos++];
        DAGNode* right = parseTerm();
        left = getOrCreateOp(op, left, right);
    }
    return left;
}

// Generate 3AC from DAG using post-order traversal with a visited flag
void generate3ACFromDAG(DAGNode* root) {
    if (root == NULL || root->visited) return;

    // Process sub-graphs first
    generate3ACFromDAG(root->left);
    generate3ACFromDAG(root->right);

    root->visited = 1;

    // Operator nodes emit a new temporary variable
    if (root->op != '\0') {
        sprintf(root->tempVar, "t%d", tempCounter++);
        printf("%s = %s %c %s\n", root->tempVar, root->left->tempVar, root->op, root->right->tempVar);
    }
}

int main() {
    printf("Enter expression (e.g., (a+b)*(a+b)): ");
    if (scanf("%99s", expr) != 1) return 1;

    DAGNode* root = parseExpression();

    printf("\n--- Generated Three-Address Code (DAG Optimized) ---\n");
    generate3ACFromDAG(root);

    printf("Result stored in %s\n", root->tempVar);

    // Free allocated table entries
    for (int i = 0; i < nodeCount; i++) {
        free(nodeTable[i]);
    }

    return 0;
}
