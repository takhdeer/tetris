#include "model.h"
#include "matrix.h"
#include "tetromino.h"
#include "../types.h"

/*
    Purpose: Determines if a tetromino collides with board boundaries or locked pieces
  
  Input:
     - board: Pointer to the game board matrix (contains locked pieces)
     - piece: Pointer to the tetromino being tested for collision
     - offset_x: The column position to test (proposed x-coordinate)
     - offset_y: The row position to test (proposed y-coordinate)
   
   Output:
     Returns 1 if collision detected (piece cannot move to offset position)
     Returns 0 if no collision (move is valid)
   
*/
UINT16 check_collision(const Matrix *board, const Tetromino *piece, int offset_x, int offset_y);

/*
    Will just call check_collisions
    Helper function for handle_tick() to reduce cllutering 
 */
UINT16 can_move_down(Model *model);

/*
    Purpose: Synchronous event handler for clock tick (gravity)
      
      Input:
        - model: Pointer to the complete game model (modified by this function)
      
      Output: None (modifies model state directly)
*/
void handle_tick (Model *model);
