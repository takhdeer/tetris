/* File: tst_a.c */
/* Author: Aaron */

#include "renderer.h" 
#include "next_box.h"
#include "tetrom.h"
#include "raster.h"
#include <osbind.h>

int main() {
    UINT32 *base = (UINT32 *)Physbase(); /* Get actual frame buffer */
    NextBox box;

    /* Clear screen */
    clear_screen((UINT32 *)base);

    /* Test 1: Next box with S-piece */
    init_next_box(&box, TETROMINO_S);
    render_next_box((UINT32 *)base, &box);

    Crawcin(); /* Press any key to continue */

    /* Test 2: Next box with T-piece */
    clear_screen((UINT32 *)base);
    set_next_type(&box, TETROMINO_T);
    render_next_box((UINT32 *)base, &box);

    Crawcin(); /* Press any key to continue. */

    return 0;
}