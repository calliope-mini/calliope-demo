#include "RunMultiplication.h"
#include "MicroBit.h"
#include "Utils.h"
#include "Images.h"

extern MicroBit uBit;

static int factor1 = 0;
static int factor2 = 0;
static bool ready = false;

static void factor1Handler(MicroBitEvent)
{
    factor1 = uBit.random(10);
    uBit.display.print(factor1);

    ready = false;
}

static void factor2Handler(MicroBitEvent)
{
    uBit.display.print(*images(ImageMultiplier));
    uBit.sleep(300);

    factor2 = uBit.random(10);
    uBit.display.print(factor2);

    ready = true;
}

static void helpHandler(MicroBitEvent)
{
    if (!ready) {
        return;
    }

    int result = factor1 * factor2;
    uBit.display.scroll(result);
    uBit.sleep(300);

    uBit.display.print(*images(ImageArrowLeft));
}

void multiplication_run() {

    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_A,
        MICROBIT_BUTTON_EVT_CLICK,
        factor1Handler);

    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_B,
        MICROBIT_BUTTON_EVT_CLICK,
        factor2Handler);

    uBit.messageBus.listen(
        MICROBIT_ID_GESTURE,
        MICROBIT_ACCELEROMETER_EVT_SHAKE,
        helpHandler);

    uBit.display.print(*images(ImageArrowLeft));

    leave = false;
    while(!leave) {
        uBit.sleep(100);
    }

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_A,
        MICROBIT_BUTTON_EVT_CLICK,
        factor1Handler);

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_B,
        MICROBIT_BUTTON_EVT_CLICK,
        factor2Handler);

    uBit.messageBus.ignore(
        MICROBIT_ID_GESTURE,
        MICROBIT_ACCELEROMETER_EVT_SHAKE,
        helpHandler);
}