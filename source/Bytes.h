#ifndef BYTES_H
#define BYTES_H

#include <stdint.h>

#define HI16(i) static_cast<uint8_t>(((i)>>8)&0xff)
#define LO16(i) static_cast<uint8_t>((i)&0xff)
#define BYTES_TO_UINT16(hi,lo) static_cast<uint16_t>(((hi)<<8)|(lo))

#endif // BYTES_H