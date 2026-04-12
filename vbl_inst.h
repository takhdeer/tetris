#ifndef VBL_ISST_H
#define VBL_ISST_H

/*
    Saves the current TOS VBL vector and installs custom ISR
    Must be called in supervisor mode
*/
void install_vbl_isr();

/*
    Restores the original TOS VBL vector
    Must be called in supervisor mode & before program exists
*/
void uninstall_vbl_isr();

#endif