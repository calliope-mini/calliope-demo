/*!
 * @file Images.h
 *
 * Images to display on Calliope led matrix.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt		<https://github.com/tcurdt>
 * @author Waldemar Gruenwald 	<https://github.com/gruenwaldi>
 */

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