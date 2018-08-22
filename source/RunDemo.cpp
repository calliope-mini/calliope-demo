#include "RunDemo.h"
#include "Images.h"
#include "Localization.h"
#include "MicroBit.h"
#include "Utils.h"

extern MicroBit uBit;

static void beep()
{
    uBit.soundmotor.soundOn(784);
    uBit.sleep(125);
    uBit.soundmotor.soundOff();
}

static void startSound()
{
    uBit.soundmotor.soundOn(262);
    uBit.sleep(125);
    uBit.soundmotor.soundOff();
    uBit.sleep(63);
    uBit.soundmotor.soundOn(784);
    uBit.sleep(500);
    uBit.soundmotor.soundOff();
}

void demo_run()
{
    const uint16_t pause = 300;

    uBit.sleep(200);

    invoke(startSound);

    uBit.display.scroll(LS_DEMO_HELLO);

    // press A
    uBit.display.print(LS_DEMO_BUTTON_A);
    uBit.sleep(pause);
    blinkImageUntilEvent(
        MICROBIT_ID_BUTTON_A,
        MICROBIT_BUTTON_EVT_CLICK,
        *images(ImageArrowLeft));
    uBit.display.print(*images(ImageTick));
    uBit.sleep(pause);
    uBit.display.clear();

    // press B
    uBit.display.print(LS_DEMO_BUTTON_B);
    uBit.sleep(pause);
    blinkImageUntilEvent(
        MICROBIT_ID_BUTTON_B,
        MICROBIT_BUTTON_EVT_CLICK,
        *images(ImageArrowRight));
    uBit.display.print(*images(ImageTick));
    uBit.sleep(pause);
    uBit.display.clear();

    // press A+B
    uBit.display.scroll(LS_DEMO_BUTTON_AB);
    uBit.sleep(pause);
    blinkImageUntilEvent(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        *images(ImageArrowLeftRight));
    beep();
    uBit.display.print(*images(ImageTick));
    uBit.sleep(pause);
    uBit.display.clear();

    // shake
    //uBit.display.scroll(LS_DEMO_SHAKE);
    blinkImageUntilEvent(
        MICROBIT_ID_GESTURE,
        MICROBIT_ACCELEROMETER_EVT_SHAKE,
        *images(ImageDoubleRow),
        -1,
        2,
        100);
    uBit.display.print(*images(ImageTick));
    uBit.sleep(pause);
    uBit.display.clear();

    uBit.display.scroll(LS_DEMO_GREAT);

    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_A,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);
    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_B,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    for (uint8_t i = 0; i < 5 && !leave; i++) {

        int r = uBit.random(pause);
        if (r < 100) {
            uBit.rgb.setColour(0xFF, 0xA5, 0x00, 0x00);
        } else if (r < 200) {
            uBit.rgb.setColour(0x00, 0xFF, 0x00, 0x00);
        } else if (r < 300) {
            uBit.rgb.setColour(0x00, 0xA5, 0xFF, 0x00);
        }

        blinkImage(*images(ImageHeart), 0, 200);
    }

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_A,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);
    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_B,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    uBit.rgb.off();
    uBit.display.clear();

    uBit.display.print(*images(ImageSmiley));
    uBit.sleep(1000);
}