// File: next_box.h
// Header File for next box.
// Author: Aaron

#ifndef NEXT_BOX_H
#define NEXT_BOX_H

// NextBox Structure
typedef struct {
    int next_type;
} NextBox;

// FUNCTIONS 

/*----- Function: init_next_box -----
 PURPOSE: Initializes the next box with the next tetromino type.

 INPUT: box: pointer to the NextBox to initialize.
        type: the tetromino type to display.

 OUTPUT: None
*/
void init_next_box(NextBox *box, int type);

/*----- Function: get_next_type -----
 PURPOSE: To check the tetromino type inside of the next box.

 INPUT: box: pointer to the NextBox to check.

 OUTPUT: Returns the tetromino type in the next box.
*/
int get_next_type(const NextBox *box);

/*----- Function: set_next_type -----
 PURPOSE: Updates the next box with a new tetromino type.

 INPUT: box: pointer to the NextBox to update.
        type: the new tetromino type to display.

 OUTPUT: None
*/
void set_next_type(NextBox *box, int type);

#endif