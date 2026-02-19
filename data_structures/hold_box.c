// implementations related to hold_box.h header file
// author: Henry

#include "hold_box.h"
#include "tetromino.h"

unsigned int hold_box_contains(HoldBox *heldbox) {
    return heldbox->contains; // this gets automatically handled in the other 2 hold_box functions, can just return it
}

Tetromino hold_tetromino(HoldBox *heldbox, Tetromino *piece) {
    // Case 1: Check if HoldBox already contains a piece --> if yes SWAP!
    if (heldbox->contains == 1) {
        Tetromino to_release = release_tetromino(heldbox);
        heldbox->piece_held = *piece;
        heldbox->contains = 1;
        return to_release;
    }

    // else, Case 2: HoldBox is empty and first ever piece to be held!
    heldbox->piece_held = *piece;   // fill HoldBox with first ever piece to be held during the game
    heldbox->contains = 1;          // piece is now held, updated contains to true
    
     // since there is no previous piece to return, return a DUMMY value!
    Tetromino dummy;
    dummy.type = -1;
    return dummy;
}   

Tetromino release_tetromino(HoldBox *heldbox) {
    Tetromino to_release = heldbox->piece_held;
    heldbox->contains = 0;  // HoldBox is technically empty now since piece will be released out
    return to_release;
}

