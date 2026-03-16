/* File: input.c
 * Author: Aaron
 * Team: Aaron, Henry, Takhdeer
 * 
 * Purpose: input.h implementations.
 * 
 */

#include "input.h"
#include <osbind.h>

static long last_raw = 0;   /*store the full raw value of last char*/

int has_input() {
    return Cconis();
}

char get_input() {
    last_raw = Cnecin();
    return (char)(last_raw & 0xFF);
}

char get_scan_code() {
    return (char)((last_raw >> 16) & 0xFF);
}
