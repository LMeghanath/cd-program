#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 512

typedef struct Block {
    int size;               // Size of the block
    int is_free;            // 1 if free, 0 if allocated
    struct Block *next;     // Pointer to the next block
} Block;

static char memory_pool[HEAP_SIZE];
static Block *freeList = (Block *)memory_pool;

void init_heap() {
    freeList->size = HEAP_SIZE - sizeof(Block);
    freeList->is_free = 1;
    freeList->next = NULL;
}

// First-Fit Allocation Strategy
void *heap_alloc(int size) {
    Block *curr = freeList;

    while (curr != NULL) {
        if (curr->is_free && curr->size >= size) {
            // Split block if there is sufficient space
            if (curr->size >= size + (int)sizeof(Block) + 4) {
                Block *newBlock = (Block *)((char *)curr + sizeof(Block) + size);
                newBlock->size = curr->size - size - sizeof(Block);
                newBlock->is_free = 1;
                newBlock->next = curr->next;

                curr->size = size;
                curr->next = newBlock;
            }
            curr->is_free = 0;
            printf("Allocated %d bytes at address offset %ld\n",
                   size, (char *)curr + sizeof(Block) - memory_pool);
            return (void *)((char *)curr + sizeof(Block));
        }
        curr = curr->next;
    }

    printf("Error: Heap Out of Memory for %d bytes!\n", size);
    return NULL;
}

// Free memory block
void heap_free(void *ptr) {
    if (!ptr) return;

    Block *block = (Block *)((char *)ptr - sizeof(Block));
    block->is_free = 1;
    printf("Freed block at address offset %ld\n", (char *)ptr - memory_pool);

    // Coalesce adjacent free blocks
    Block *curr = freeList;
    while (curr && curr->next) {
        if (curr->is_free && curr->next->is_free) {
            curr->size += sizeof(Block) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

void display_heap_status() {
    Block *curr = freeList;
    int index = 0;
    printf("\n--- Current Heap Status ---\n");
    while (curr != NULL) {
        printf("Block %d: Size = %-4d bytes | Status = %s\n",
               index++,
               curr->size,
               curr->is_free ? "FREE" : "ALLOCATED");
        curr = curr->next;
    }
    printf("---------------------------\n");
}

int main() {
    init_heap();

    void *p1 = heap_alloc(64);
    void *p2 = heap_alloc(128);
    void *p3 = heap_alloc(32);

    display_heap_status();

    heap_free(p2); // Free middle chunk to demonstrate fragmentation / free list
    display_heap_status();

    void *p4 = heap_alloc(64); // Fits in the newly freed area
    display_heap_status();

    heap_free(p1);
    heap_free(p3);
    heap_free(p4);
    display_heap_status();

    return 0;
}
