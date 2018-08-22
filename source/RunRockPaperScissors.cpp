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

    uBit.display.print(*images(ImageRock), 0, 0, 0, pause * 2);
    uBit.display.print(*images(ImageFull), 0, 0, 0, pause * 2);
    uBit.display.print(*images(ImageScissors), 0, 0, 0, pause * 2);
    uBit.display.print(*images(ImageWell), 0, 0, 0, pause * 2);
    uBit.display.clear();
    uBit.sleep(pause);

    leave = false;
    while (!leave) {

        blinkImageUntilEvent(
            MICROBIT_ID_GESTURE,
            MICROBIT_ACCELEROMETER_EVT_SHAKE,
            *images(ImageDoubleRow),
            -1,
            2,
            100);

        if (leave) {
            break;
        }

        int r = uBit.random(400);
        if (r < 100) {
            uBit.display.print(*images(ImageRock));
        } else if (r < 200) {
            uBit.display.print(*images(ImageFull));
        } else if (r < 300) {
            uBit.display.print(*images(ImageScissors));
        } else {
            uBit.display.print(*images(ImageWell));
        }

        uBit.sleep(3000);
        uBit.display.clear();
    }

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);
}