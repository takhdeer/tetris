#include "../renderer/takh_renderer.h"
#include "../model.h"
#include "../game_state.h"
#include "../data_structures/tetromino.h"
#include "raster.h"

#include <osbind.h>

int main() {
    UINT32 *base = (UINT32 *)Physbase();  // Get actual frame buffer 
    Model test_model;
    
    // Clear screen first 
    clear_screen((__UINT8__ *)base);
    
    // Initialize model
    init_model(&test_model);
    
    // Set up test scenario 
    init_tetromino(&test_model.piece, TETROMINO_I, 3);
    test_model.piece.row = 2;
    test_model.game_state.score = 1200;
    test_model.game_state.level = 3;
    test_model.game_state.lines_cleared = 15;
    
    // Render everything to screen 
    render(&test_model, base);
    
    // Wait for user to see the result 
    Crawcin();  
    
    // Clear and test different piece 
    clear_screen((__UINT8__ *)base);
    
    init_tetromino(&test_model.piece, TETROMINO_T, 5);
    test_model.piece.row = 8;
    test_model.game_state.score = 2400;
    test_model.game_state.level = 5;
    test_model.game_state.lines_cleared = 25;
    
    render(&test_model, base);
    
    Crawcin();  // Press any key to exit 
    
    return 0;

}