#include "Slice.h"

Slice slice_create(uint8_t *buffer, uint16_t start, uint16_t stop)
{
    Slice slice = {};
    slice.buffer = buffer;
    slice.start = start;
    slice.position = start;
    slice.stop = stop;
    return slice;
}

uint16_t slice_available(Slice &slice)
{
    if (slice.position < slice.start) {
        return 0;
    }
    if (slice.position >= slice.stop) {
        return 0;
    }
    return slice.stop - slice.position;
}

uint8_t slice_read8(Slice &slice)
{
    if (slice.position < slice.start) {
        // assert
        return 0;
    }
    if (slice.position >= slice.stop) {
        // assert
        return 0;
    }
    return slice.buffer[slice.position++];
}

uint16_t slice_read16(Slice &slice)
{
    uint8_t hi = slice_read8(slice);
    uint8_t lo = slice_read8(slice);
    return hi << 8 | lo;
}
