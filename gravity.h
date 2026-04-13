#ifndef GRAVITY_H
#define GRAVITY_H

/*
 * Increments the gravity counter by one VBL tick and sets
 * drop_requested when the counter reaches the threshold for
 * the current level. Called from the VBL ISR -- must be fast.
 * Assumptions: game_model is initialized before ISR is installed.
 */
void update_gravity();

#endif