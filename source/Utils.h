#ifndef UTILS_H
#define UTILS_H

#include "MicroBit.h"

extern bool event;
void leaveHandler(MicroBitEvent);

extern bool leave;
void eventHandler(MicroBitEvent);

void blinkImage(
    const MicroBitImage& image,
    const int pos = 0,
    const uint16_t delay = 200);

void blinkImageUntilEvent(
    const uint16_t source,
    const uint16_t value,
    const MicroBitImage& image,
    const int pos1 = 0,
    const int pos2 = 0,
    const uint16_t delay = 200);

#endif // UTILS_H