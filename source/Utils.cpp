#include "Utils.h"
#include "MicroBit.h"
#include "MicroBitImage.h"

extern MicroBit uBit;

bool event = false;
void eventHandler(MicroBitEvent)
{
    event = true;
}

bool leave = false;
void leaveHandler(MicroBitEvent)
{
    leave = true;
}

void blinkImage(
    const MicroBitImage& image,
    const int pos,
    const uint16_t delay)
{

    event = false;

    uBit.display.print(image, pos);

    for (uint32_t t = 0, i = 10; t < delay && !leave && !event; t += i) {
        uBit.sleep(i);
    }

    if (event) {
        return;
    }

    uBit.display.clear();

    for (uint32_t t = 0, i = 10; t < delay && !leave && !event; t += i) {
        uBit.sleep(i);
    }
}

void blinkImageUntilEvent(
    const uint16_t source,
    const uint16_t value,
    const MicroBitImage& image,
    const int pos1,
    const int pos2,
    const uint16_t delay)
{
    uBit.messageBus.listen(
        source,
        value,
        eventHandler);

    event = false;

    for (uint8_t i = 0; !leave && !event; i++) {
        blinkImage(image, (i % 2) == 0 ? pos1 : pos2, delay);
    }

    uBit.messageBus.ignore(
        source,
        value,
        eventHandler);
}