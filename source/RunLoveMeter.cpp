#include "RunLoveMeter.h"
#include "MicroBit.h"
#include "Images.h"
#include "Utils.h"

extern MicroBit uBit;

static int touch_p1 = 0;
static int touch_p2 = 0;

static void lovemeter_fiber()
{
    touch_p1 = uBit.io.P1.getAnalogValue();
    touch_p2 = uBit.io.P2.getAnalogValue();

    while(!leave) {
        uBit.sleep(300);

        int p1 = uBit.io.P1.getAnalogValue();
        int p2 = uBit.io.P2.getAnalogValue();

        if (abs(touch_p1 - p1) > 20 && abs(touch_p2 - p2) > 20) {
            event = true;
        }
    }
}

void lovemeter_run()
{
    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    uBit.io.P1.isTouched();
    uBit.io.P2.isTouched();

    uBit.display.print(*images(ImageHeart));

    invoke(lovemeter_fiber);

    leave = false;
    while (!leave) {

        event = false;
        for (uint8_t i = 0; !leave && !event; i++) {
            blinkImage(*images(ImageWave), (i % 2) == 0 ? -1 : -2, 200);
        }

        if (leave) {
            break;
        }

        int p1 = uBit.io.P1.getAnalogValue();
        int p2 = uBit.io.P2.getAnalogValue();
        int v1 = (int)(p1 * 9.0 / 1023.0);
        int v2 = (int)(p2 * 9.0 / 1023.0);
        int v3 = 9 - abs(v1 - v2);
        uBit.display.print(v3);

        uBit.sleep(1000);

        if (v3 > 6) {
            uBit.display.print(*images(ImageHeart));
            uBit.soundmotor.soundOn(1000);
            uBit.sleep(100);
            uBit.soundmotor.soundOn(2000);
            uBit.sleep(100);
            uBit.soundmotor.soundOn(3000);
            uBit.sleep(300);
            uBit.soundmotor.soundOff();
            uBit.sleep(4000);
        } else {
            uBit.display.print(*images(ImageFlash));
            uBit.soundmotor.soundOn(1000);
            uBit.sleep(100);
            uBit.soundmotor.soundOn(500);
            uBit.sleep(100);
            uBit.soundmotor.soundOn(100);
            uBit.sleep(300);
            uBit.soundmotor.soundOff();
            uBit.sleep(4000);
        }

        uBit.display.clear();
    }

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);
}