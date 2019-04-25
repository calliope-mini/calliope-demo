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

static inline void waitForever()
{
    while (true) {
        uBit.sleep(1000);
        uint8_t buffer[4];
        if(masterService->getStatus(buffer)){
            LOG("status changed\r\n");
            uBit.storage.put("MasterStatus",buffer, 4);
            LOG("storing ok\r\n");
            if((uint32_t)(buffer[3] << 24) & CALLIOPE_SERVICE_FLAG_RESET){
                LOG("resetting\r\n");
	            //uBit.sleep(500);
                uBit.reset();
            }
        } else
            LOG("status ok\r\n");
    }
}

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

static void onReset(void /*MicroBitEvent event*/) {
	uint32_t buffer = 0;
	uBit.storage.put("MasterStatus", (uint8_t *) &buffer, 4);
}

void checkReset(void) {
	int value = uBit.io.P0.getAnalogValue(); // P0 is a value in the range of 0 - 1024

	if (value > 400) {
		uBit.serial.printf("RESET ALL SERVICES : %d\r\n", value);
		onReset();
	}
	uBit.io.P0.isTouched();
}

int main()
{
    uBit.init();
	// check the accelerometer
	if (BMX055Accelerometer::isDetected(uBit.i2c)) {
		uBit.accelerometer.updateSample();
	} else {
		uBit.serial.send("Accelerometer not available\r\n");
	}

    uBit.serial.send("Calliope Demo v2.5\r\n");

	if (hasStorageKey(KEY_INTERPRETER)) {
		removeStorageKey(KEY_INTERPRETER);

		//minimize serial buffer
		uBit.serial.setTxBufferSize(0);

		showNameHistogram(uBit.display); //uBit.bleManager.pairingMode(uBit.display, uBit.buttonA);//


		checkReset();


		PlaygroundFreeInit(uBit);
//        interpreter_start();

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