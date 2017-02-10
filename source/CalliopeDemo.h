#ifndef CALLOPE_DEMO_H
#define CALLOPE_DEMO_H


// MENU HANDLING
typedef volatile enum State {
    Intro = 0,
    Menu,
    Oracle,
    RockPaperScissors,
    LoveMeter,
    Snake,
    SpecialAttachment = 55,
    TakeOver = 99,
} state_t;

extern const MicroBitImage Full;
extern const MicroBitImage Tick;

void testBoard();
void snake();

//#define COMPILE_FIRMWARE_MASTER 0

#endif
