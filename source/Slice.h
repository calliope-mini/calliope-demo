#ifndef SLICE_H
#define SLICE_H

#include <stdint.h>

typedef struct {
    uint8_t *buffer;
    uint16_t start;
    uint16_t position;
    uint16_t stop;
} Slice;

Slice slice_create(uint8_t *buffer, uint16_t start, uint16_t stop);
uint16_t slice_available(Slice &slice);
uint8_t slice_read8(Slice &slice);
uint16_t slice_read16(Slice &slice);

#endif // SLICE_H
