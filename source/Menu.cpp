#include "Menu.h"
#include "MicroBit.h"
#include "nrf.h"

extern MicroBit uBit;

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


menustate_t menuWaitForChoice(menustate_t start)
{
    menustate_t state = start;
    while (true) {
        if (state == MenuStateInterpreter) {

            showNameHistogram(uBit.display);

            // static const uint8_t pixels[25] = {
            //     0, 0, 1, 1, 1,
            //     0, 0, 0, 1, 1,
            //     0, 0, 1, 0, 1,
            //     0, 1, 0, 0, 0,
            //     1, 0, 0, 0, 0
            // };
            // const MicroBitImage Image(5, 5, pixels);

            // uBit.display.print(Image);

        } else {

            uBit.display.print(ManagedString(state));
        }

        // event loop
        while (true) {
            // button A
            if (uBit.buttonA.isPressed()) {
                while (uBit.buttonA.isPressed()) {
                    uBit.sleep(10);
                }

                if (state > MenuStateMin) {
                    state = menustate_t(state - 1);
                    break;
                }
            }

            // button B
            if (uBit.buttonB.isPressed()) {
                while (uBit.buttonB.isPressed()) {
                    uBit.sleep(10);
                }

                if (state < MenuStateMax) {
                    state = menustate_t(state + 1);
                    break;
                }
            }

            // gesture
            if (uBit.accelerometer.getGesture() == MICROBIT_ACCELEROMETER_EVT_SHAKE) {
                while (uBit.accelerometer.getGesture() == MICROBIT_ACCELEROMETER_EVT_SHAKE) {
                    uBit.sleep(10);
                }
                return state;
            }

            uBit.sleep(10);
        }
    }
}