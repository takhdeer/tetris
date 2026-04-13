#include "vbl_inst.h"
#include "music.h"
#include "gravity.h"
#include <osbind.h>

#define VBL_VECTOR 0x70

static void *old_vbl_vector;
extern void vbl_isr();
extern volatile int render_request;

void install_vbl_isr() {
    long old_ssp = Super(0);
    old_vbl_vector = *(void **)VBL_VECTOR;
    *(void **)VBL_VECTOR = (void *)vbl_isr;
    Super(old_ssp);
}

void uninstall_vbl_isr() {
    long old_ssp = Super(0);
    *(void **)VBL_VECTOR = old_vbl_vector;
    Super(old_ssp);
}

void vbl_handler() {
    update_music(1);
    update_gravity();
    render_request = 1;
}