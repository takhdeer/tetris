#include "game_state.h"
#include "data_structures/tetromino.h"
#include "data_structures/synch.h"
#include <stdio.h>

/*
    Spawns a new peice and checks for collisions. 
    Sets is_game_over to 1 if there are collisions
 */
int spawn_piece(Model *model);