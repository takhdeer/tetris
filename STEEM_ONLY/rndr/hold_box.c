/* implementations related to hold_box.h header file */
/* author: Henry */

#include "hold_box.h"
#include "tetrom.h"

void init_hold_box(HoldBox *heldbox) {
    heldbox->contains = 0; /* logically is empty at start of game */
}

unsigned int hold_box_contains(HoldBox *heldbox) {
    return heldbox->contains;
}

Tetromino hold_tetromino(HoldBox *heldbox, Tetromino *piece) {
    Tetromino to_release;
    Tetromino dummy;

    /* Case 1: HoldBox already contains a piece -> SWAP */
    if (heldbox->contains == 1) {
        to_release = release_tetromino(heldbox);
        heldbox->piece_held = *piece;
        heldbox->contains = 1;
        return to_release;
    }

    /* Case 2: HoldBox is empty, first piece to be held */
    heldbox->piece_held = *piece;
    heldbox->contains = 1;

    /* no previous piece to return, return a dummy value */
    dummy.type = -1;
    return dummy;
}

Tetromino release_tetromino(HoldBox *heldbox) {
    Tetromino to_release = heldbox->piece_held;
    heldbox->contains = 0; /* HoldBox is now empty */
    return to_release;
}