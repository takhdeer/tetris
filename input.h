/* File: input.h
 * Author: Aaron
 * Team: Aaron, Henry, Takhdeer
 * 
 * Purpose: Hardware-independent input module
 *          Provides a middle layer between game code and OS-specific input
 */

#ifndef INPUT_H
#define INPUT_H

/*----- Function: has_input -----
 PURPOSE: Check if keyboard input is pending

 INPUT: None

 OUTPUT: Returns 1 (true) if pending input is to be processed, 0 (false) otherwise
*/
int has_input();

/*----- Function: get_input -----
 PURPOSE: Read the next character from keyboard input

 INPUT: None

 OUTPUT: Returns the next character to be pressed.
*/
char get_input();

char get_scan_code();

#endif