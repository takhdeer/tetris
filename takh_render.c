#include "takh_renderer.h"

#define SCORE_X 200   // score x poisition
#define SCORE_Y 10    // score y position
#define LEVEL_X 200  
#define LEVEL_Y 30    
#define LINES_X 200  
#define LINES_Y 50    

void render_peice(const Tetromino *piece, UINT32 *base) {
    int piece_row;
    int piece_col; 
    int screen_x;
    int screen_y;
    int line; 

    for (piece_row = 0; piece_row < 4; piece_row++) {
        for (piece_col = 0; piece_col < 4; piece_col) {

            //If cell is part of the piece 
            if(get_cell(piece,piece_row,piece_col) != 0) {

                screen_x = (piece->col + piece_col) * BLOCK_SIZE;
                screen_y = (piece->row + piece_row) * BLOCK_SIZE;

                for (line = 0; line < BLOCK_SIZE; line++) {
                    plot_horizontal_line(base,screen_y + line, screen_x, BLOCK_SIZE);
                }
            }
        }
    }
}

void render_score(const GameState *game, UINT8 *base) { 
    // creating arrays
    char score_text[20];

    // converting nums into strings
    sprintf(score_text, "SCORE: %u", game->score);

    // rendering each line of text
    plot_string(base, 10, 200, score_text);
}

void render_level(const GameState *game, UINT8 base) {
    char level_text[20];
    sprintf(level_text, "LEVEL: %U", game->level);
    plot_string(base, 30, 200, level_text);

}
