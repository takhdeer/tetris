/* File: input.c
 * Author: Aaron
 * Team: Aaron, Henry, Takhdeer
 * 
 * Purpose: input.h implementations.
 * 
 */

#include "input.h"
#include <osbind.h>

int has_input() {
    return Cconis();
}

char get_input() {
    return (char)Cnecin();
}
