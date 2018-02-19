#ifndef BYTES_H
#define BYTES_H

#define HI16(i) (uint8_t)(((i)>>8)&0xff)
#define LO16(i) (uint8_t)((i)&0xff)
#define BYTES_TO_UINT16(hi,lo) (uint16_t)(((hi)<<8)|(lo))

#endif
