/**
 * Calliope Demo Code.
 *
 * The code contains is the wrapper for the main functionality found after
 * unpacking the Calliope mini. It contains the basic test functionality for
 * in-production testing, as well as some small demo programs that can be
 * selected using A and B buttons.
 *
 * - Oracle (press a button and get a smiley or sad face)
 * - Rock, Paper, Scissors, Well
 * - Volume meter
 * - Multiplication
 * - Playground
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Matthias L. Jugel <leo@calliope.cc>
 * @author Stephan Noller <stephan@calliope.cc>
 * @author Franka Futterlieb <franka@urbn.de>
 * @author Niranjan Rao
 * @author Waldemar Gruenwald <waldemar.gruenwald@ubirch.com>
 */

#include <BMX055Accelerometer.h>
#include <nrf_soc.h>
#include "MicroBit.h"
#include "Storage.h"
#include "Images.h"
#include "RunTests.h"
#include "RunDemo.h"
#include "Menu.h"
#include "RunOracle.h"
#include "RunRockPaperScissors.h"
#include "RunMultiplication.h"
#include "RunVolumeMeter.h"
#include "nrf.h"
#include "PlaygroundFree.h"
#include "CalliopeServiceMaster.h"

MicroBit uBit;

CalliopeServiceMaster *masterService;

/*!
 * Show the Histogram, which represents the ID of the device
 * @param display The instance of the LED matrix display
 */
static void showNameHistogram(MicroBitDisplay &display)
{
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

/*!
 * This is the idle loop of the Calliope device.
 * Therein the Master status is checked, for eventually resetting the device,
 * if the corresponding flag was set.
 */
static inline void waitForever()
{
	while (true) {
		uBit.sleep(1000);
		uint32_t buffer;
		if (masterService->getStatus(&buffer)) {
			uBit.serial.printf("status changed:%08x\r\n", buffer);
			uBit.storage.put("MasterStatus", (uint8_t *) &buffer, 4);
			uBit.serial.printf("storing ok\r\n");
			if (buffer & CALLIOPE_SERVICE_FLAG_RESET) {
				uBit.serial.printf("resetting\r\n");
				//uBit.sleep(500);
				uBit.reset();
			}
		} elseLOG("status ok\r\n");
	}
}

/*!
 * Animation for entering a specific demo application.
 */
static void menuAnimateEnter()
{
	uBit.display.print(*images(ImageDot));
	uBit.sleep(200);
	uBit.display.print(*images(ImageSmallRect));
	uBit.sleep(200);
	uBit.display.print(*images(ImageLargeRect));
	uBit.sleep(200);
	uBit.display.clear();
}

/*!
 * Animation for leaving a specific demo application
 */
static void menuAnimateLeave()
{
	uBit.display.print(*images(ImageLargeRect));
	uBit.sleep(200);
	uBit.display.print(*images(ImageSmallRect));
	uBit.sleep(200);
	uBit.display.print(*images(ImageDot));
	uBit.sleep(200);
	uBit.display.clear();
}

/*!
 * Check if the Services have to be reset.
 * TODO maybe delete this functionality, because it might not be needed anymore?
 */
void checkServicesReset(void) {
	int value = uBit.io.P0.getAnalogValue(); // P0 is a value in the range of 0 - 1024

	if (value > 400) {
		uBit.serial.printf("RESET ALL SERVICES : %d\r\n", value);
		uint32_t buffer = 0;
		uBit.storage.put("MasterStatus", (uint8_t *) &buffer, 4);
	}
	uBit.io.P0.isTouched();
}

/*!
 * Get the reason for the reset condition, it might have come from the Master Service,
 * in which case the interpreter has to be restarted.
 */
void getResetReason() {
	KeyValuePair *status;
	status = uBit.storage.get("MasterStatus");
	uBit.serial.printf("status = 0x%08x, %s\r\n", *(uint32_t *) status->value, status->key);

	if (*(uint32_t *) status->value & CALLIOPE_SERVICE_FLAG_RESET) {
		setStorageKey(KEY_INTERPRETER);
		*(uint32_t *) status->value = *(uint32_t *) status->value & (~(CALLIOPE_SERVICE_FLAG_RESET));
		uBit.storage.put(reinterpret_cast<const char *>(status->key), status->value, sizeof(status->value));
	}
	free(status);
}


int main()
{
	uBit.init();

	getResetReason();

	// check the accelerometer
	if (BMX055Accelerometer::isDetected(uBit.i2c)) {
		uBit.accelerometer.updateSample();
	} else {
		uBit.serial.send("Accelerometer not available\r\n");
	}

    uBit.serial.send("Calliope Demo v2.5\r\n");

	if (hasStorageKey(KEY_INTERPRETER)) {
		removeStorageKey(KEY_INTERPRETER);

		PlaygroundFreeInit(uBit);
		checkServicesReset();

		//minimize serial buffer
		uBit.serial.setTxBufferSize(0);

		showNameHistogram(uBit.display); //uBit.bleManager.pairingMode(uBit.display, uBit.buttonA);//

		// not required - just to make it obvious this does not return
		waitForever();
	}

	if (!hasStorageKey(KEY_TEST)) {
		setStorageKey(KEY_TEST);

		tests_run();

		// not required - just to make it obvious this does not return
		waitForever();
	}

	if (!hasStorageKey(KEY_DEMO)) {
		setStorageKey(KEY_DEMO);

		demo_run();
	}

    // start state
    menustate_t state = MenuStateOracle;

    while (true) {
        state = menuWaitForChoice(state);
        switch (state) {
	        // 1
	        case MenuStateOracle:
		        menuAnimateEnter();
                oracle_run();
                menuAnimateLeave();
		        break;
		        // 2
	        case MenuStateRockPaperScissors:
		        menuAnimateEnter();
                rockpaperscissors_run();
                menuAnimateLeave();
		        break;
		        // 3
	        case MenuStateMultiplication:
		        menuAnimateEnter();
                multiplication_run();
                menuAnimateLeave();
		        break;
		        // 4
	        case MenuStateVolumeMeter:
		        menuAnimateEnter();
                volumemeter_run();
                menuAnimateLeave();
		        break;
		        // 5
	        case MenuStateInterpreter:
		        setStorageKey(KEY_INTERPRETER);
		        uBit.reset();
		        break;
        }
    }
}