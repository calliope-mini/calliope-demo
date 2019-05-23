/*!
 * @file RunOracle.cpp
 *
 * Oracle for difficult decisions on Calliope-mini.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt		<https://github.com/tcurdt>
 * @author Waldemar Gruenwald 	<https://github.com/gruenwaldi>
 */

#include "RunOracle.h"
#include "MicroBit.h"
#include "Images.h"
#include "Utils.h"

extern MicroBit uBit;

void oracle_run()
{
	uBit.messageBus.listen(
			MICROBIT_ID_BUTTON_AB,
			MICROBIT_BUTTON_EVT_CLICK,
			leaveHandler);
	uBit.messageBus.listen(
			MICROBIT_ID_BUTTON_A,
			MICROBIT_BUTTON_EVT_CLICK,
			eventHandler);

	leave = false;
	while(!leave) {

		event = false;

		leave = false;
		while(!leave) {

			uBit.display.print(*images(ImageArrowLeft));

			for(uint32_t t=0, i=10; t<400 && !leave && !event; t+=i) {
				uBit.sleep(i);
			}

			if (leave || event) {
				break;
			}

			uBit.sleep(10);
			uBit.display.clear();

			for(uint32_t t=0, i=10; t<400 && !leave && !event; t+=i) {
				uBit.sleep(i);
			}
		}

		if (leave) {
			break;
		}

		for (int i = 0; i < 5; i++) {
			uBit.display.print(*images(ImageDot));
			uBit.sleep(200);
			uBit.display.clear();
			uBit.sleep(50);
		}

		int r = uBit.random(100);
		uBit.display.print((r < 50)
		                   ? *images(ImageSmiley)
		                   : *images(ImageSadly)
		);

		uBit.sleep(3000);
		uBit.display.clear();
	}

	uBit.messageBus.ignore(
			MICROBIT_ID_BUTTON_AB,
			MICROBIT_BUTTON_EVT_CLICK,
			leaveHandler);
	uBit.messageBus.ignore(
			MICROBIT_ID_BUTTON_A,
			MICROBIT_BUTTON_EVT_CLICK,
			eventHandler);
}