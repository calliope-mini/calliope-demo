#include "Images.h"

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
static const uint8_t pixel_multiplier[25] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 1, 0,
        0, 0, 0, 0, 0
};



MicroBitImage *images(Image_t index){
    static MicroBitImage returnImage;
    switch(index){
        case ImageSmiley: {
            returnImage = MicroBitImage(5, 5, pixel_smiley);
            break;
        }
        case ImageSadly: {
            returnImage = MicroBitImage(5, 5, pixel_sadly);
            break;
        }
        case ImageHeart: {
            returnImage = MicroBitImage(5, 5, pixel_heart);
            break;
        }
        case ImageArrowLeft: {
            returnImage = MicroBitImage(5, 5, pixel_arrow_left);
            break;
        }
        case ImageArrowRight: {
            returnImage = MicroBitImage(5, 5, pixel_arrow_right);
            break;
        }
        case ImageArrowLeftRight: {
            returnImage = MicroBitImage(5, 5, pixel_arrow_leftright);
            break;
        }
        case ImageFull: {
            returnImage = MicroBitImage(5, 5, pixel_full);
            break;
        }
        case ImageDot: {
            returnImage = MicroBitImage(5, 5, pixel_dot);
            break;
        }
        case ImageSmallRect: {
            returnImage = MicroBitImage (5, 5, pixel_small);
            break;
        }
        case ImageLargeRect: {
            returnImage = MicroBitImage (5, 5, pixel_large);
            break;
        }
        case ImageDoubleRow: {
            returnImage = MicroBitImage(5, 5, pixel_double_row);
            break;
        }
        case ImageTick: {
            returnImage = MicroBitImage(5, 5, pixel_tick);
            break;
        }
        case ImageRock: {
            returnImage = MicroBitImage(5, 5, pixel_rock);
            break;
        }
        case ImageScissors: {
            returnImage = MicroBitImage(5, 5, pixel_scissors);
            break;
        }
        case ImageWell: {

            returnImage = MicroBitImage(5, 5, pixel_well);
            break;
        }
        case ImageFlash: {
            returnImage = MicroBitImage(5, 5, pixel_flash);
            break;
        }
        case ImageWave: {
            returnImage = MicroBitImage(7, 5, pixel_wave);
            break;
        }
        case ImageMultiplier:
            returnImage = MicroBitImage(5, 5, pixel_multiplier);
            break;
        default:
            returnImage = MicroBitImage(5, 5, pixel_sadly);

            break;
    }
    return &returnImage;
}