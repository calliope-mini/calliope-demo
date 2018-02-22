#include "Menu.h"
#include "MicroBit.h"
#include "Images.h"
#include "Storage.h"
#include "RunTests.h"
#include "RunDemo.h"
#include "RunOracle.h"
#include "RunRockPaperScissors.h"
#include "RunLoveMeter.h"
#include "RunSnake.h"
#include "Interpreter.h"

MicroBit uBit;

static inline void waitForever()
{
    while (true) {
        uBit.sleep(1000);
    }
}

static void menuAnimateEnter() {
    uBit.display.print(ImageDot);
    uBit.sleep(200);
    uBit.display.print(ImageSmallRect);
    uBit.sleep(200);
    uBit.display.print(ImageLargeRect);
    uBit.sleep(200);
    uBit.display.clear();
}

static void menuAnimateLeave() {
    uBit.display.print(ImageLargeRect);
    uBit.sleep(200);
    uBit.display.print(ImageSmallRect);
    uBit.sleep(200);
    uBit.display.print(ImageDot);
    uBit.sleep(200);
    uBit.display.clear();
}

int main()
{
    uBit.init();
    uBit.accelerometer.updateSample();

    uBit.serial.send("Calliope Demo v1.1\r\n");

    if (hasStorageKey(KEY_INTERPRETER)) {
        removeStorageKey(KEY_INTERPRETER);

        // mimimize serial buffer
        uBit.serial.setTxBufferSize(0);

        interpreter_run();

        // not required - just to make it obvious this does not return
        waitForever();
    }

    if (!hasStorageKey(KEY_TEST)) {
        setStorageKey(KEY_TEST);

        tests_run();

        // not required - just to make it obvious this does not return
        waitForever();
    }

    if (!hasStorageKey(KEY_DEMO)) {
        setStorageKey(KEY_DEMO);

        demo_run();
    }

    // start state
    menustate_t state = MenuStateOracle;

    while (true) {
        state = menuWaitForChoice(state);
        switch (state) {
        // 1
        case MenuStateOracle:
            menuAnimateEnter();
            oracle_run();
            menuAnimateLeave();
            break;
        // 2
        case MenuStateRockPaperScissors:
            menuAnimateEnter();
            rockpaperscissors_run();
            menuAnimateLeave();
            break;
        // 3
        case MenuStateLoveMeter:
            menuAnimateEnter();
            lovemeter_run();
            menuAnimateLeave();
            break;
        // 4
        case MenuStateSnake:
            menuAnimateEnter();
            snake_run();
            menuAnimateLeave();
            break;
        // 5
        case MenuStateInterpreter:
            setStorageKey(KEY_INTERPRETER);
            uBit.reset();
            break;
        }
    }
}