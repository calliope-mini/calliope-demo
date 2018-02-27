#include "Images.h"
#include <stdint.h>

static const uint8_t pixel_full[25] = {
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
};
static const uint8_t pixel_dot[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
static const uint8_t pixel_small[25] = {
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 0
};
static const uint8_t pixel_large[25] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
};
static const uint8_t pixel_arrow_left[25] = {
    0, 0, 1, 0, 0,
    0, 1, 0, 0, 0,
    1, 1, 1, 1, 1,
    0, 1, 0, 0, 0,
    0, 0, 1, 0, 0
};
static const uint8_t pixel_arrow_right[25] = {
    0, 0, 1, 0, 0,
    0, 0, 0, 1, 0,
    1, 1, 1, 1, 1,
    0, 0, 0, 1, 0,
    0, 0, 1, 0, 0
};
static const uint8_t pixel_arrow_leftright[25] = {
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0
};
static const uint8_t pixel_double_row[25] = {
    0, 1, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 1, 1, 0, 0
};
static const uint8_t pixel_tick[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 1, 0,
    1, 0, 1, 0, 0,
    0, 1, 0, 0, 0
};
static const uint8_t pixel_heart[25] = {
    0, 1, 0, 1, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0
};
static const uint8_t pixel_smiley[25] = {
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0
};
static const uint8_t pixel_sadly[25] = {
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1
};
static const uint8_t pixel_rock[25] = {
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 0
};
static const uint8_t pixel_scissors[25] = {
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1
};
static const uint8_t pixel_well[25] = {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0
};
static const uint8_t pixel_flash[25] = {
    0, 0, 1, 1, 0,
    0, 1, 1, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 1, 1, 0,
    0, 1, 1, 0, 0,
};
static const uint8_t pixel_wave[7 * 5] = {
    0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 0,
    1, 0, 0, 1, 0, 0, 1,
    0, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0
};

const MicroBitImage ImageSmiley(5, 5, pixel_smiley);
const MicroBitImage ImageSadly(5, 5, pixel_sadly);
const MicroBitImage ImageHeart(5, 5, pixel_heart);
const MicroBitImage ImageArrowLeft(5, 5, pixel_arrow_left);
const MicroBitImage ImageArrowRight(5, 5, pixel_arrow_right);
const MicroBitImage ImageArrowLeftRight(5, 5, pixel_arrow_leftright);
const MicroBitImage ImageFull(5, 5, pixel_full);
const MicroBitImage ImageDot(5, 5, pixel_dot);
const MicroBitImage ImageSmallRect(5, 5, pixel_small);
const MicroBitImage ImageLargeRect(5, 5, pixel_large);
const MicroBitImage ImageDoubleRow(5, 5, pixel_double_row);
const MicroBitImage ImageTick(5, 5, pixel_tick);
const MicroBitImage ImageRock(5, 5, pixel_rock);
const MicroBitImage ImageScissors(5, 5, pixel_scissors);
const MicroBitImage ImageWell(5, 5, pixel_well);
const MicroBitImage ImageFlash(5, 5, pixel_flash);
const MicroBitImage ImageWave(7, 5, pixel_wave);

const MicroBitImage *images[17] = {
    &ImageSmiley,
    &ImageSadly,
    &ImageHeart,
    &ImageArrowLeft,
    &ImageArrowRight,
    &ImageArrowLeftRight,
    &ImageFull,
    &ImageDot,
    &ImageSmallRect,
    &ImageLargeRect,
    &ImageDoubleRow,
    &ImageTick,
    &ImageRock,
    &ImageScissors,
    &ImageWell,
    &ImageFlash,
    &ImageWave,
};
