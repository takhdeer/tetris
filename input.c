/* File: input.c
 * Author: Aaron
 * Team: Aaron, Henry, Takhdeer
 * 
 * Purpose: input.h implementations.
 * 
 */

#include "input.h"
#include <osbind.h>
#include "ikbd.h"
#include "types.h"

/* static long last_raw = 0; */   /*store the full raw value of last char*/

static UINT8 last_scan = 0;
#define SCREEN_W 640
#define SCREEN_H 400

int has_input() {
    return (scan_queue.count > 0);
}

char get_input() {
    UINT8 scan;
    scan = scan_queue.buf[scan_queue.head];
    scan_queue.head = (scan_queue.head + 1) & 7;
    scan_queue.count--;
    last_scan = scan;

    if (scan == 0x4B) return 0;
    if (scan == 0x4D) return 0;
    if (scan == 0x48) return 0;
    if (scan == 0x50) return 0;

    switch (scan) {
        case 0x2E: return 'c';
        case 0x39: return ' ';
        case 0x10: return 'q';
        default:   return 0;
    }
}

char get_scan_code() {
    return (char)last_scan;
}

int get_mouse_x() {
    return mouse_x;
}

int get_mouse_y() {
    return mouse_y;
}

int get_mouse_buttons() {
    return mouse_buttons;
}

void update_mouse(int dx, int dy) {
    mouse_x += dx;
    mouse_y += dy;

    if (mouse_x < 0) {
        mouse_x = 0;
    }
    if (mouse_x >= SCREEN_W){
        mouse_x = SCREEN_W - 1;
    }

    if (mouse_y < 0) {
        mouse_y = 0;
    }
    if (mouse_y >= SCREEN_H){
        mouse_y = SCREEN_H - 1;
    }
}

void set_mouse_buttons(int buttons) {
    mouse_buttons = buttons;
}