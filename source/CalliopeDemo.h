#ifndef CALLOPE_DEMO_H
#define CALLOPE_DEMO_H

//#define COMPILE_FIRMWARE_MASTER 0
//#define USE_ENGLISH

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


// localization
#ifndef USE_ENGLISH
#define DISPLAY_HELLO "Hallo!"
#define DISPLAY_SHAKE "SCHUETTELN"
#define DISPLAY_ORACLE "Orakel"
#define DISPLAY_SUPER "SUPER!"
#define DISPLAY_THEEND "ENDE"
#define DISPLAY_ERROR "Huch?"
#else
#define DISPLAY_HELLO "Hello!"
#define DISPLAY_SHAKE "SHAKE"
#define DISPLAY_ORACLE "Oracle"
#define DISPLAY_SUPER "SUPER!"
#define DISPLAY_THEEND "THE END"
#define DISPLAY_ERROR "Oops?"
#endif

#endif
