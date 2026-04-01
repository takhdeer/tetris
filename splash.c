/**
 * splash.c - Splash screen implementation
 * Authors: Aaron, Takhdeer, Henry
 */

#include "splash.h"
#include "input.h"
#include <osbind.h>
#include "renderer.h"
#include "raster.h"

/**
 * Displays splash screen and waits for user input
 * Returns: 1 to start game, 0 to quit
 */
int show_splash_screen(UINT8 *base) {
    int done = 0;
    char key;
    int i;

    /* Clear screen */
    clear_screen((UINT32 *)base);

    /* (placeholder) Title */
    plot_string(base, 100, 288, "TETRONAS");

    /* Instructions */
    plot_string(base, 330, 228, "PRESS SPACEBAR TO START");
    plot_string(base, 350, 260, "PRESS Q TO QUIT");

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