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
