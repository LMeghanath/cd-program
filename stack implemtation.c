#include <stdio.h>
#include <string.h>

#define MAX_FRAMES 10

typedef struct {
    char function_name[30];
    int return_address;
    int local_variable;
} ActivationRecord;

typedef struct {
    ActivationRecord records[MAX_FRAMES];
    int top;
} Stack;

void init_stack(Stack *s) {
    s->top = -1;
}

void push_frame(Stack *s, const char *func, int ret_addr, int local_var) {
    if (s->top >= MAX_FRAMES - 1) {
        printf("Stack Overflow!\n");
        return;
    }
    s->top++;
    strcpy(s->records[s->top].function_name, func);
    s->records[s->top].return_address = ret_addr;
    s->records[s->top].local_variable = local_var;
    printf("CALL: Pushed Activation Record for '%s' (Return Addr: %d, Local: %d)\n",
           func, ret_addr, local_var);
}

void pop_frame(Stack *s) {
    if (s->top < 0) {
        printf("Stack Underflow!\n");
        return;
    }
    printf("RETURN: Popped Activation Record for '%s'\n", s->records[s->top].function_name);
    s->top--;
}

void display_stack(Stack *s) {
    if (s->top == -1) {
        printf("\nRuntime Stack is empty.\n");
        return;
    }
    printf("\n--- Current Runtime Stack (Top to Bottom) ---\n");
    for (int i = s->top; i >= 0; i--) {
        printf("[Frame %d] Function: %-10s | RetAddr: %-4d | Local: %-4d\n",
               i,
               s->records[i].function_name,
               s->records[i].return_address,
               s->records[i].local_variable);
    }
    printf("---------------------------------------------\n");
}

int main() {
    Stack runtime_stack;
    init_stack(&runtime_stack);

    push_frame(&runtime_stack, "main", 0, 50);
    push_frame(&runtime_stack, "compute", 104, 10);
    push_frame(&runtime_stack, "helper", 212, 5);

    display_stack(&runtime_stack);

    pop_frame(&runtime_stack);
    display_stack(&runtime_stack);

    pop_frame(&runtime_stack);
    pop_frame(&runtime_stack);
    display_stack(&runtime_stack);

    return 0;
}
