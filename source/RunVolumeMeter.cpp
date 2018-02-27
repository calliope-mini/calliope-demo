#include <cmath>
#include "RunVolumeMeter.h"
#include "MicroBit.h"
#include "Utils.h"

extern MicroBit uBit;

const int threshold = 519;

static int read()
{
    int mic = 0;
    const int n = 30;
    for (uint8_t i = 0; i < n; i++) {
        const int v = uBit.io.P21.getAnalogValue();
        if (v > mic) mic = v;
    }

    if (mic < threshold) {
        return 0;
    }

    // mic 512...900 ~> 0...5
    const int gauge = static_cast<const int>((log2(mic - threshold + 1) * 5) / 8);

    // uBit.serial.printf("m:%d -> g:%d\n", mic, gauge);

    if (gauge < 0) {
        return 0;
    }

    if (gauge > 5) {
        return 5;
    }

    return gauge;
}

void volumemeter_run()
{
    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    int gauges[5] = {};

    leave = false;
    while (!leave) {

        // shift values
        for (uint8_t i = 0; i < (5-1); i++) {
            gauges[i] = gauges[i+1];
        }

        gauges[4] = read();

        for (uint16_t x = 0; x < 5; x++) {
            const int gauge = gauges[x];
            for (uint16_t y = 0; y < 5; y++) {
                const uint8_t t = static_cast<uint8_t>(gauge > y ? 255 : 0);
                uBit.display.image.setPixelValue(x, 4-y, t);
            }
        }

        int sum = 0;
        for (uint8_t i = 0; i < 5; i++) {
            sum += gauges[i];
        }

        if (sum > 5*2.5) {
            uBit.rgb.setColour(0xFF, 0x00, 0x00, 0x00);
        } else {
            uBit.rgb.setColour(0x00, 0xFF, 0x00, 0x00);
        }

        uBit.sleep(100);
    }

    uBit.rgb.off();

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);
}