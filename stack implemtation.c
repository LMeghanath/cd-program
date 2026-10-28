#include <stdio.h>
#include <string.h>

#define MAX_VARS 5

typedef struct {
    char name[30];
    int size;       // Size in bytes
    int offset;     // Fixed memory offset
    int value;      // Stored data
} StaticSymbol;

StaticSymbol staticTable[MAX_VARS];
int varCount = 0;
int currentOffset = 0;

void allocate_static(const char *name, int size, int value) {
    if (varCount >= MAX_VARS) {
        printf("Error: Static data area full!\n");
        return;
    }
    strcpy(staticTable[varCount].name, name);
    staticTable[varCount].size = size;
    staticTable[varCount].offset = currentOffset;
    staticTable[varCount].value = value;

    currentOffset += size;
    varCount++;
}

void display_static_storage() {
    printf("\n--- Static Storage Allocation Map ---\n");
    printf("%-15s %-10s %-15s %-10s\n", "Variable", "Size(B)", "Offset Address", "Value");
    for (int i = 0; i < varCount; i++) {
        printf("%-15s %-10d 0x%04X          %-10d\n",
               staticTable[i].name,
               staticTable[i].size,
               staticTable[i].offset,
               staticTable[i].value);
    }
}

int main() {
    printf("Performing Static Allocation at Compile/Load Time...\n");

    // Allocation decisions are fixed before runtime execution
    allocate_static("global_counter", 4, 100);
    allocate_static("system_flag",    4, 1);
    allocate_static("config_code",    4, 404);

    display_static_storage();
    return 0;
}
