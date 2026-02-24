// File: test_next_box_renderer.c
// Author: Aaron

#include "../renderer/aaron_renderer.h" // change to renderer.h
#include "../data_structures/next_box.h"
#include "../data_structures/tetromino.h"
#include "../raster_graphics_library/raster.h"
#include <osbind.h>

int main() {
    UINT8 *base = (UINT8 *)Physbase(); // Get actual frame buffer
    NextBox box;

    // Clear screen
    clear_screen((UINT32 *)base);

    // Test 1: Next box with I-piece
    init_next_box(&box, TETROMINO_I);
    render_next_box(base, &box);

    Crawcin(); // Press any key to continue

    // Test 3: Next box with T-piece
    clear_screen((UINT32 *)base);
    set_next_type(&box, TETROMINO_T);
    render_next_box(base, &box);

    Crawcin(); // Press any key to continue.

    return 0;
}