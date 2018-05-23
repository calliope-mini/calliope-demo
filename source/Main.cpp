#include "MicroBit.h"
#include "Storage.h"
#include "Images.h"
#include "RunTests.h"
#include "RunDemo.h"
#include "Menu.h"
#include "RunOracle.h"
#include "RunRockPaperScissors.h"
#include "RunMultiplication.h"
#include "RunVolumeMeter.h"
#include "Interpreter.h"
#include "nrf.h"

MicroBit uBit;

static void showNameHistogram(MicroBitDisplay &display)
{
    NRF_FICR_Type *ficr = NRF_FICR;
    uint32_t n = ficr->DEVICEID[1];
    uint32_t ld = 1;
    uint32_t d = MICROBIT_DFU_HISTOGRAM_HEIGHT;
    uint32_t h;

    display.clear();
    for (uint32_t i = 0; i < MICROBIT_DFU_HISTOGRAM_WIDTH; i++)
    {
        h = (n % d) / ld;

        n -= h;
        d *= MICROBIT_DFU_HISTOGRAM_HEIGHT;
        ld *= MICROBIT_DFU_HISTOGRAM_HEIGHT;

        for (uint32_t j = 0; j < h + 1; j++)
            display.image.setPixelValue(
                static_cast<int16_t>(MICROBIT_DFU_HISTOGRAM_WIDTH - i - 1),
                static_cast<int16_t>(MICROBIT_DFU_HISTOGRAM_HEIGHT - j - 1),
                255);
    }
}

static inline void waitForever()
{
    while (true) {
        uBit.sleep(1000);
    }
}

static void menuAnimateEnter()
{
    uBit.display.print(ImageDot);
    uBit.sleep(200);
    uBit.display.print(ImageSmallRect);
    uBit.sleep(200);
    uBit.display.print(ImageLargeRect);
    uBit.sleep(200);
    uBit.display.clear();
}

static void menuAnimateLeave()
{
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

        // minimize serial buffer
        uBit.serial.setTxBufferSize(0);

        showNameHistogram(uBit.display);
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
        case MenuStateMultiplication:
            menuAnimateEnter();
                multiplication_run();
                menuAnimateLeave();
            break;
        // 4
        case MenuStateVolumeMeter:
            menuAnimateEnter();
                volumemeter_run();
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