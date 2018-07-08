#include "RunRockPaperScissors.h"
#include "Images.h"
#include "MicroBit.h"
#include "Utils.h"

extern MicroBit uBit;

void rockpaperscissors_run()
{
    const uint16_t pause = 200;

    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    uBit.display.print(ImageRock, 0, 0, 0, pause * 2);
    uBit.display.print(ImageFull, 0, 0, 0, pause * 2);
    uBit.display.print(ImageScissors, 0, 0, 0, pause * 2);
    uBit.display.print(ImageWell, 0, 0, 0, pause * 2);
    uBit.display.clear();
    uBit.sleep(pause);

    leave = false;
    while (!leave) {

        blinkImageUntilEvent(
            MICROBIT_ID_GESTURE,
            MICROBIT_ACCELEROMETER_EVT_SHAKE,
            ImageDoubleRow,
            -1,
            2,
            100);

        if (leave) {
            break;
        }

        int r = uBit.random(400);
        if (r < 100) {
            uBit.display.print(ImageRock);
        } else if (r < 200) {
            uBit.display.print(ImageFull);
        } else if (r < 300) {
            uBit.display.print(ImageScissors);
        } else {
            uBit.display.print(ImageWell);
        }

        uBit.sleep(3000);
        uBit.display.clear();
    }

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);
}