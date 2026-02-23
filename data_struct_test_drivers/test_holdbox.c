// individual test driver for HoldBox data structure
// directly testing hold_box.h and hold_box.c implementation
// author: Henry

#include <stdio.h>
#include "../data_structures/hold_box.h"

int main() {
    printf("=== Testing 'HoldBox' Data Strucure ===\n");

    HoldBox heldbox;
    init_hold_box(&heldbox);

    Tetromino piece_a, piece_b;
    init_tetromino(&piece_a, TETROMINO_I, 4);
    init_tetromino(&piece_b, TETROMINO_T, 4);

    /* --- Test 1: hold_box_contains() --- */
    printf(" --- Test 1a: hold_box_contains() is already empty ---\n");
    unsigned int result = hold_box_contains(&heldbox);
    printf("hold_box_contains empty box (expected 0): %s\n", result == 0 ? "PASSED" : "FAILED");

    printf("\n --- Test 1b: hold_box_contains() is actively holding a piece ---\n");
    init_hold_box(&heldbox); // reset per test
    hold_tetromino(&heldbox, &piece_a);
    result = hold_box_contains(&heldbox);
    printf("hold_box_contains occupied box (expected 1): %s\n", result == 1 ? "PASSED" : "FAILED");

    /* --- Test 2: hold_tetromino() --- */
    printf("\n --- Test 2a: hold_tetromino() test for first hold AKA dummy hold ---\n");
    init_hold_box(&heldbox);
    Tetromino dummy = hold_tetromino(&heldbox, &piece_a); // first hold of Tetromino piece b
    printf("hold_tetromino test for dummy value (expected -1): %s\n", dummy.type == -1 ? "PASSED" : "FAILED");
    
    printf("\n --- Test 2b: hold_tetromino() test for proper swap, Holdbox holds piece b + returns piece a to game ---\n");
    init_hold_box(&heldbox);
    hold_tetromino(&heldbox, &piece_a); // initialize for first hold, holding piece a
    Tetromino return_piece_a = hold_tetromino(&heldbox, &piece_b); // swap and hold piece b, return piece a to game
    printf("hold_tetromino test for swap, HoldBox holds piece b + returns piece a to game (expected .type = 0): %s\n", return_piece_a.type == TETROMINO_I ? "PASSED" : "FAILED"); 

    /* --- Test 3: release_tetromino() --- */
    printf("\n --- Test 3: release_tetromino() check if HoldBox piece is actually released ---\n");
    init_hold_box(&heldbox);
    hold_tetromino(&heldbox, &piece_b); // first hold of piece b
    Tetromino released = release_tetromino(&heldbox);
    printf("release_tetromino returns piece_b (expected .type = 2) %s\n", released.type == TETROMINO_T ? "PASSED" : "FAILED");
    printf("hold_box_contains is empty after release (expected 0): %s\n", hold_box_contains(&heldbox) == 0 ? "PASSED" : "FAILED");

    return 0;
}