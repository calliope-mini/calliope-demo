/*!
 * @file Utils.cpp.
 *
 * Utility functions for Calliope-demo
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt		<https://github.com/tcurdt>
 * @author Waldemar Gruenwald 	<https://github.com/gruenwaldi>
 */


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

/*!
 * Show the Histogram, which represents the ID of the device
 * @param display The instance of the LED matrix display
 */
void showNameHistogram(MicroBitDisplay &display) {
	NRF_FICR_Type *ficr = NRF_FICR;
	uint32_t n = ficr->DEVICEID[1];
	uint32_t ld = 1;
	uint32_t d = MICROBIT_DFU_HISTOGRAM_HEIGHT;
	uint32_t h;

	display.clear();
	for (uint32_t i = 0; i < MICROBIT_DFU_HISTOGRAM_WIDTH; i++) {
		h = (n % d) / ld;

		n -= h;
		d *= MICROBIT_DFU_HISTOGRAM_HEIGHT;
		ld *= MICROBIT_DFU_HISTOGRAM_HEIGHT;

		for (uint32_t j = 0; j < h + 1; j++)
			display.image.setPixelValue(
					static_cast<int16_t>(MICROBIT_DFU_HISTOGRAM_WIDTH - i - 1),
					static_cast<int16_t>(MICROBIT_DFU_HISTOGRAM_HEIGHT - j - 1),
					255);
	}
}