#include "gravity.h"
#include "model.h"

#define GRAVITY_BASE      50
#define GRAVITY_MIN        5
#define GRAVITY_PER_LEVEL  5

extern Model        game_model;
extern volatile int drop_requested;

void update_gravity() {
    int threshold;

    game_model.gravity_counter++;

    threshold = GRAVITY_BASE -
                (game_model.game_state.level * GRAVITY_PER_LEVEL);

    if (threshold < GRAVITY_MIN) {
        threshold = GRAVITY_MIN;
    }

    if (game_model.gravity_counter >= threshold) {
        game_model.gravity_counter = 0;
        drop_requested = 1;
    }
}