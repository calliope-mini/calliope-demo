#ifndef IMAGES_H
#define IMAGES_H

#include "MicroBitImage.h"


enum Image_t{
    ImageSmiley,
    ImageSadly,
    ImageHeart,
    ImageArrowLeft,
    ImageArrowRight,
    ImageArrowLeftRight,
    ImageFull,
    ImageDot,
    ImageSmallRect,
    ImageLargeRect,
    ImageDoubleRow,
    ImageTick,
    ImageRock,
    ImageScissors,
    ImageWell,
    ImageFlash,
    ImageWave,
    ImageMultiplier,
    ImageCOUNTER
};

MicroBitImage *images(Image_t index);

#endif // IMAGES_H