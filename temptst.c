/* input_tst.c - Test driver for input module */
/* Tests: basic keypresses, arrow keys, quit, and timer interaction */
/* Authors: Takhdeer */

#include "input.h"
#include "types.h"
#include <stdio.h>
#include <osbind.h>

const char LEFT_ARROW  = 0x4B;
const char RIGHT_ARROW = 0x4D;
const char UP_ARROW    = 0x48;
const char DOWN_ARROW  = 0x50;

UINT32 get_time() {
    long *timer = (long *)0x462;
    UINT32 timeNow;
    long old_ssp;

    old_ssp = Super(0);
    timeNow = *timer;
    Super(old_ssp);

    return timeNow;
}

/* ====== TEST 1: Basic input and quit ====== */
void test_basic_input() {
    printf("TEST 1: Basic Input\n");
    printf("Press keys to see them printed. Press q to move to next test.\n");

    while (1) {
        if (has_input()) {
            char key = get_input();
            printf("Key pressed: %c (raw: %d)\n", key, (int)key);
            if (key == 'q') {
                printf("TEST 1 PASSED: q quit the loop\n\n");
                break;
            }
        }
    }
}

/* ====== TEST 2: Arrow keys ====== */
void test_arrow_keys() {
    printf("TEST 2: Arrow Keys\n");
    printf("Press arrow keys to test. Press q to move to next test.\n");

    while (1) {
        if (has_input()) {
            char key = get_input();

            if (key == 'q') {
                printf("TEST 2 DONE\n\n");
                break;
            }

            if (key == 0) {
                key = get_input();
                if (key == LEFT_ARROW)       printf("LEFT ARROW detected\n");
                else if (key == RIGHT_ARROW) printf("RIGHT ARROW detected\n");
                else if (key == UP_ARROW)    printf("UP ARROW detected\n");
                else if (key == DOWN_ARROW)  printf("DOWN ARROW detected\n");
                else                         printf("Unknown special key: %d\n", (int)key);
            }
        }
    }
}

/* ====== TEST 3: Timer only (no input) ====== */
void test_timer() {
    UINT32 timeThen, timeNow, timeElapsed;
    int tick_count = 0;

    printf("TEST 3: Timer\n");
    printf("Counting 70 ticks (approx 1 second)...\n");

    timeThen = get_time();

    while (tick_count < 70) {
        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        if (timeElapsed > 0) {
            tick_count++;
            timeThen = timeNow;
        }
    }

    printf("TEST 3 PASSED: 70 ticks counted\n\n");
}

/* ====== TEST 4: Timer + input together ====== */
void test_timer_and_input() {
    UINT32 timeThen, timeNow, timeElapsed;

    printf("TEST 4: Timer + Input Together\n");
    printf("Press keys while timer runs. Press q to quit.\n");

    timeThen = get_time();

    while (1) {
        if (has_input()) {
            char key = get_input();
            printf("Key pressed: %c (raw: %d)\n", key, (int)key);
            if (key == 'q') {
                printf("TEST 4 PASSED: quit while timer running\n\n");
                break;
            }
        }

        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        if (timeElapsed > 0) {
            printf("Tick!\n");
            timeThen = timeNow;
        }
    }
}

int main() {
    printf("=== INPUT MODULE TEST DRIVER ===\n\n");

    test_basic_input();
    test_arrow_keys();
    test_timer();
    test_timer_and_input();

    printf("=== ALL TESTS COMPLETE ===\n");
    return 0;
}