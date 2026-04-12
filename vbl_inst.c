#include "vbl_inst.h"

#define VBL_VECTOR 0x70

static void *old_vbl_vector;

extern void _vbl_isr();

void install_vbl_isr() {
    old_vbl_vector = *(void **)VBL_VECTOR;
    *(void **)VBL_VECTOR = (void *)_vbl_isr;
}

void uninstall_vbl_isr() {
    *(void **)VBL_VECTOR = old_vbl_vector;
}