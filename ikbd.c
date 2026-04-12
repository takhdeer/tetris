/* C file for installing/removing IKBD ISR and related logic */
/* Author: Henry */

#include "ikbd.h"
#include <osbind.h>

/* ====== HARDWARE ADDRESSES ====== */
#define IKBD_VECTOR     0x118       /* IKBD ISR vector address              */
#define MIDI_ACIA_CTRL  0xFFFFFC04  /* MIDI 6850 control register           */
#define MFP_IMRB        0xFFFA13    /* MFP interrupt mask register B        */
#define MFP_IERB        0xFFFA09    /* MFP interrupt enable register B      */
#define IKBD_IRQ_BIT    6           /* bit 6 = IKBD/ACIA in MFP registers   */

/* GLOBALS (declared in ikbd.h, defined here) */
volatile ScanQueue  scan_queue;
volatile UINT8      key_state[128];
volatile long        mouse_x;
volatile long        mouse_y;
volatile UINT8      mouse_buttons;

/* SAVED VECTORS */
static void (*old_ikbd_isr)(void);  /* saved TOS IKBD vector */

/* 
    install_ikbd_isr
    Saves old vector, installs ours.
    Must be called from supervisor mode via Super().
*/
void install_ikbd_isr(void) {
    long old_ssp;
    int i;

    /* zero out all globals before installing */
    scan_queue.head  = 0;
    scan_queue.tail  = 0;
    scan_queue.count = 0;
    for (i = 0; i < 128; i++) key_state[i] = 0;
    mouse_x       = 320;    /* start mouse at screen centre */
    mouse_y       = 200;
    mouse_buttons = 0;

    old_ssp = Super(0);

    /* save old vector */
    old_ikbd_isr = (void (*)(void))(*(long *)IKBD_VECTOR);

    /* install ours */
    *(long *)IKBD_VECTOR = (long)ikbd_isr;

    Super(old_ssp);
}

/* 
    remove_ikbd_isr
    Restores the original TOS IKBD vector.
*/
void remove_ikbd_isr(void) {
    long old_ssp = Super(0);
    *(long *)IKBD_VECTOR = (long)old_ikbd_isr;
    Super(old_ssp);
}

/* 
    disable_midi
    Disables MIDI 6850 interrupts so it doesn't
    fire on our IKBD vector.
    Call once at program start.
 
*/
void disable_midi(void) {
    long old_ssp = Super(0);
    /* writing $03 to MIDI ACIA control = reset, no interrupts */
    *((volatile UINT8 *)MIDI_ACIA_CTRL) = 0x03;
    Super(old_ssp);
}

/* 
    enable_midi
    Re-enables MIDI 6850 interrupts on exit.
*/
void enable_midi(void) {
    long old_ssp = Super(0);
    /* writing $96 restores default TOS MIDI ACIA config */
    *((volatile UINT8 *)MIDI_ACIA_CTRL) = 0x96;
    Super(old_ssp);
}

/* 
    mask_ikbd
    Masks IKBD interrupt at MFP level (bit 6 of IMRB).
    Call at start of critical section in main code.
*/
void mask_ikbd(void) {
    long old_ssp = Super(0);
    *((volatile UINT8 *)MFP_IMRB) &= ~(1 << IKBD_IRQ_BIT);
    Super(old_ssp);
}

/* 
    unmask_ikbd
    Unmasks IKBD interrupt at MFP level.
    Call at end of critical section in main code.
*/
void unmask_ikbd(void) {
    long old_ssp = Super(0);
    *((volatile UINT8 *)MFP_IMRB) |= (1 << IKBD_IRQ_BIT);
    Super(old_ssp);
}