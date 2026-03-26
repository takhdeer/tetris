/**
 * tst_psg.c - Test driver for PSG module
 * Authors: Aaron, Takhdeer, Henry
 * 
 * Tests all PSG functions
 */

#include <osbind.h>
#include <stdio.h>
#include "psg.h"

void wait_for_key(void);
UINT32 get_time(void);

int main() 
{
    UINT8 value;
    int i;

    printf("===== PSG Module Test Driver=====\n\n");

    /* TEST 1: write_psg() and read_psg() */
    printf("===== TEST 1: write_psg() and read_psg() =====\n");

    printf("Writing volume 7 to register 8.\n");
    /* Setting volume 7 to channel 8 */
    write_psg(8,7);
    value = read_psg(8);
    
    if (value == 7){
        printf("PASS\n");
    }

    else {
        printf("FAIL\n");
    }

    printf("\n");
    wait_for_key();

    /* TEST 2: set_tone() */
    printf("===== TEST 2: set_tone() =====\n");

    enable_channel(CHANNEL_A, 1, 0);
    set_volume(CHANNEL_A, 11);

    printf("Playing E\n");
    set_tone(CHANNEL_A, E_NOTE);
    {
        UINT32 start = get_time();
        while (get_time() - start < 35) {}
    }

    printf("Playing B\n");
    set_tone(CHANNEL_A, B_NOTE);
    {
        UINT32 start = get_time();
        while (get_time() - start < 35) {}
    }
    
    printf("Playing C\n");
    set_tone(CHANNEL_A, C_NOTE);
    {
        UINT32 start = get_time();
        while (get_time() - start < 35) {}
    }

    printf("Playing D\n");
    set_tone(CHANNEL_A, D_NOTE);
    {
        UINT32 start = get_time();
        while (get_time() - start < 35) {}
    }

    set_volume(CHANNEL_A, 0);
    printf("NOTES TESTED.\n");
    wait_for_key();

    /* TEST 3: set_volume */
    printf("===== TEST 3: set_volume() =====\n");

    set_tone(CHANNEL_A, E_NOTE);
    enable_channel(CHANNEL_A, 1, 0);

    for (i = 0 ; i <= 15 ; i++) {
        set_volume(CHANNEL_A, i);
        printf("Volume: %d\n", i);
        {
            UINT32 start = get_time();
            while (get_time() - start < 10) { }
        }
    }
    
    set_volume(CHANNEL_A, 0);
    printf("TESTED VOLUME INCREASE.\n");
    wait_for_key();

    /* TEST 4: enable_channel() */
    printf("===== TEST 4: enable_channel =====\n");

    set_tone(CHANNEL_A, E_NOTE);
    set_volume(CHANNEL_A, 11);

    printf("Tone ON\n");
    enable_channel(CHANNEL_A, 1, 0);
    {
        UINT32 start = get_time();
        while (get_time() - start < 35) { }
    }

    printf("Tone OFF (silence)\n");
    enable_channel(CHANNEL_A, 0, 0);
    {
        UINT32 start = get_time();
        while (get_time() - start < 35) { }
    }

    printf("Tone ON (again)\n");
    enable_channel(CHANNEL_A, 1, 0);
    {
        UINT32 start = get_time();
        while (get_time() - start < 35) { }
    }

    set_volume(CHANNEL_A, 0);
    printf("TESTED ENABLE (ON/OFF/ON)\n");
    wait_for_key();

    /* TEST 5: stop_sound */
    printf("===== TEST 5: stop_sound() =====\n");

    /* sound on all 3 channels ('A' chord triad) */
    set_tone(CHANNEL_A, A_NOTE);
    enable_channel(CHANNEL_A, 1, 0);
    set_volume(CHANNEL_A, 11);

    set_tone(CHANNEL_B, C_NOTE);
    enable_channel(CHANNEL_B, 1, 0);
    set_volume(CHANNEL_B, 11);

    set_tone(CHANNEL_C, E_NOTE);
    enable_channel(CHANNEL_C, 1, 0);
    set_volume(CHANNEL_C, 11);

    printf(" Playing A chord triad (all 3 channels)\n");
    {
        UINT32 start = get_time();  
        while (get_time() - start < 140) { }
    }
    printf("Calling stop_sound()\n");
    stop_sound();

    printf("stop_sound TEST DONE.\n");
    wait_for_key();

    printf(" ===== TEST DRIVER FINISHED. ===== \n");

    return 0;
}

/* Helper functions */
void wait_for_key(void) {
    printf("(Press any key to continue)\n\n");
    while (!Cconis()) { }
    Cnecin();
}

UINT32 get_time(void) {
    long *timer = (long *)0x462;
    UINT32 timeNow;
    long old_ssp;

    old_ssp = Super(0);
    timeNow = *timer;
    Super(old_ssp);

    return timeNow;
}