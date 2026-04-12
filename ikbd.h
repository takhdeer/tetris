/* header file for IKBD ISR*/
/* Author: Henry */

#ifndef IKBD_H
#define IKBD_H

#include "types.h"

/* FOR KEYBOARD CIRCULAR QUEUE */
#define QUEUE_SIZE 8

typedef struct {
    UINT8 buf[QUEUE_SIZE];
    UINT8 head;     /* dequeue from here */
    UINT8 tail;      /* enqueue to here */
    UINT8 count;    /* number of items*/
} ScanQueue;

/* SHARED GLOBALS */

/* keyboard queue - to MAKE CODES only */
extern volatile ScanQueue scan_queue;

/* key state table: key_state[scan_code] = 1 if held, 0 if released */
extern volatile UINT8 key_state[128];

/* GLOBALS for mouse */
extern volatile long mouse_x;
extern volatile long mouse_y;
extern volatile UINT8 mouse_buttons;    /* bit1 = left mouse click | bit0 = right mouse click*/

/* IKBD functions */
void install_ikbd_isr(void);
void remove_ikbd_isr(void);
void disable_midi(void);
void enable_midi(void);
void mask_ikbd(void);
void unmask_ikbd(void);

/* ISR entry point */
extern void ikbd_isr(void);

#endif