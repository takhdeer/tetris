// File: test_next_box.c
// Author: Aaron

#include <stdio.h>
#include "../data_structures/next_box.h"
#include "../data_structures/tetromino.h"

int main() {
    NextBox box;

    printf("=== Testing Next Box ===\n");

    // Initializing test:
    init_next_box(&box, TETROMINO_T);
    int type = get_next_type(&box);

    printf("Initialize Next Box Test:\n");
    if (type == TETROMINO_T) 
        printf("PASS\n");
    else {
        printf("FAIL\n");
    }

    // Update next box test:
    set_next_type(&box, TETROMINO_S);
    int updated_type = get_next_type(&box);

    printf("Updated Next Box Test:\n");
    if (updated_type == TETROMINO_S) 
        printf("PASS\n");
    else {
        printf("FAIL\n");
    }

}