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

#ifdef YOTTA_CFG_COMPILE_MASTER
#define COMPILE_FIRMWARE_MASTER 0
#endif

// localization
#ifdef YOTTA_CFG_USE_ENGLISH
#define DISPLAY_HELLO "Hello!"
#define DISPLAY_SHAKE "SHAKE"
#define DISPLAY_ORACLE "Oracle"
#define DISPLAY_SUPER "SUPER!"
#define DISPLAY_THEEND "THE END"
#define DISPLAY_ERROR "Oops?"
#else
#define DISPLAY_HELLO "Hallo!"
#define DISPLAY_SHAKE "SCHUETTELN"
#define DISPLAY_ORACLE "Orakel"
#define DISPLAY_SUPER "SUPER!"
#define DISPLAY_THEEND "ENDE"
#define DISPLAY_ERROR "Huch?"
#endif

#endif
