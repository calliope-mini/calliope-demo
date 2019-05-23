/*!
 * @file RunTests.h
 *
 * Run functionality tests on Calliope-mini.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt		<https://github.com/tcurdt>
 * @author Waldemar Gruenwald 	<https://github.com/gruenwaldi>
 */

#include <cmath>
#include "RunTests.h"
#include "MicroBit.h"
#include "MicroBitSerial.h"
#include "Images.h"
#include "Utils.h"

extern MicroBit uBit;

void onButtonA(MicroBitEvent)
{
	uBit.display.print("A");
	uBit.rgb.off();
	uBit.rgb.setColour(0, 0, 255, 0);
}

void onButtonB(MicroBitEvent)
{
	uBit.display.print("B");
	uBit.rgb.off();
	uBit.rgb.setColour(255, 0, 0, 0);
}

void onButtonAB(MicroBitEvent)
{
	uBit.display.print("#");
	uBit.rgb.off();
	uBit.rgb.setColour(0, 255, 0, 0);
}

void onTouch(MicroBitEvent e) {
	uBit.serial.printf("EVENT: s= %d, e=%d\r\n", e.source, e.value);
	if ((e.value == MICROBIT_BUTTON_EVT_DOWN) || (e.value == MICROBIT_BUTTON_EVT_HOLD)) {
		switch(e.source) {
			case MICROBIT_ID_IO_P12:
				uBit.display.print("0");
				return;
			case MICROBIT_ID_IO_P0:
				uBit.display.print("1");
				return;
			case MICROBIT_ID_IO_P1:
				uBit.display.print("2");
				return;
			case MICROBIT_ID_IO_P16:
				uBit.display.print("3");
				return;
			default:
				return;
		}
	}
}

void onGesture(MicroBitEvent e)
{
	switch(e.value) {
		case MICROBIT_ACCELEROMETER_EVT_SHAKE:
			uBit.display.print("S");
			break;
		case MICROBIT_ACCELEROMETER_EVT_FACE_UP:
			uBit.display.print("+");
			break;
		case MICROBIT_ACCELEROMETER_EVT_FACE_DOWN:
			uBit.display.print("-");
			break;
		case MICROBIT_ACCELEROMETER_EVT_TILT_UP:
			uBit.display.print("U");
			break;
		case MICROBIT_ACCELEROMETER_EVT_TILT_DOWN:
			uBit.display.print("D");
			break;
		case MICROBIT_ACCELEROMETER_EVT_TILT_LEFT:
			uBit.display.print("L");
			break;
		case MICROBIT_ACCELEROMETER_EVT_TILT_RIGHT:
			uBit.display.print("R");
			break;
		default:
			return;
	}
}

void tests_run()
{
	uBit.soundmotor.setSoundSilentMode(true);
	uBit.soundmotor.soundOn(500);
	uBit.sleep(100);
	uBit.soundmotor.soundOn(2000);
	uBit.sleep(100);
	uBit.soundmotor.soundOn(3000);
	uBit.sleep(100);
	uBit.soundmotor.soundOff();
	uBit.sleep(500);

	uBit.display.clear();
	uBit.display.print(*images(ImageFull));
	for (int i = 255; i > 0; i -= 2) {
		uBit.display.setBrightness(i);
		uBit.sleep(3);
	}
	uBit.sleep(3);
	for (int i = 0; i < 255; i += 2) {
		uBit.display.setBrightness(i);
		uBit.sleep(3);
	}

	uBit.rgb.off();
	uBit.rgb.setColour(255, 0, 0, 0);
	uBit.sleep(200);
	uBit.rgb.off();
	uBit.rgb.setColour(0, 255, 0, 0);
	uBit.sleep(200);
	uBit.rgb.off();
	uBit.rgb.setColour(0, 0, 255, 0);
	uBit.sleep(200);
	uBit.rgb.off();

	for (int i = 0; i < 10; i++) {
		uBit.accelerometer.getX();
		uBit.accelerometer.getY();
		uBit.accelerometer.getZ();
	}
	uBit.display.print(*images(ImageTick));
	uBit.sleep(500);

	uBit.io.P12.isTouched();
	uBit.io.P0.isTouched();
	uBit.io.P1.isTouched();
	uBit.io.P16.isTouched();

	uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
	uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
	uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);
////    uBit.messageBus.listen(MICROBIT_ID_ANY, MICROBIT_PIN_EVENT_ON_TOUCH, onTouch);
	uBit.messageBus.listen(MICROBIT_ID_IO_P12, MICROBIT_EVT_ANY, onTouch);
	uBit.messageBus.listen(MICROBIT_ID_IO_P0, MICROBIT_EVT_ANY, onTouch);
	uBit.messageBus.listen(MICROBIT_ID_IO_P1, MICROBIT_EVT_ANY, onTouch);
	uBit.messageBus.listen(MICROBIT_ID_IO_P16, MICROBIT_EVT_ANY, onTouch);

	uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_EVT_ANY, onGesture);

	uBit.display.clear();

	while (true) {
		int mic = uBit.io.P21.getAnalogValue();
		// ((value - fromLow) * (toHigh - toLow)) / (fromHigh - fromLow) + toLow
		if (mic > 512) {
			const int gauge = static_cast<const int>((log2(mic - 511) * 4) / 9);

			for (uint8_t i = 0; i <= 4; i++) {
				const int16_t x = static_cast<uint16_t>(4);
				const int16_t y = static_cast<uint16_t>(4 - i);
				const uint8_t t = static_cast<uint8_t>(i > gauge ? 0 : 255);
				uBit.display.image.setPixelValue(x, y, t);
			}
			uBit.sleep(100);

			uBit.serial.printf("mic: %d -> %d\r", mic, gauge, ASYNC);
		}
	}
}