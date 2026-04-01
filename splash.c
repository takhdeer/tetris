/**
 * splash.c - Splash screen implementation
 * Authors: Aaron, Takhdeer, Henry
 */

#include "splash.h"
#include "input.h"
#include <osbind.h>

/**
 * Displays splash screen and waits for user input
 * Returns: 1 to start game, 0 to quit
 */
int show_splash_screen(UINT8 *base) {
    int done = 0;
    char key;

    /* Clear screen (TO DO) */

    while (!done) {
        if (has_input()) {
            key = get_input();

            if (key == ' ') {
                return 1;
            }
            else if (key == 'q') {
                return 0; /* Quit */
            }
        }

    }
    return 0;

}