/*!
 * @file CalliopeServiceGesture.h
 *
 * Bluetooth Gesture Service, requires Accelerometer and events
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */

#ifndef CALLIOPE_DEMO_CALLIOPESERVICEMOVE_H
#define CALLIOPE_DEMO_CALLIOPESERVICEMOVE_H
/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

//#include <MicroBitDisplay.h>
//#include <MicroBitSerial.h>
#include "MicroBit.h"
#include "MicroBitConfig.h"
#include "ble/BLE.h"

// UUIDs for our service and characteristics
extern const uint8_t CalliopeGestureServiceUUID[];
extern const uint8_t CalliopeGestureServiceDataUUID[];

/**
  * Class definition for the custom MicroBit light Service.
  * Provides a BLE service to remotely read the silicon light of the nRF51822.
  */
class CalliopeGestureService {
public:

	/**
	  * Constructor.
	  * Create a representation of the lightService
	  * @param _ble The instance of a BLE device that we're running on.
	  * @param _display An instance of MicroBitDisplay to use as our light source.
	  */
	CalliopeGestureService(BLEDevice &_ble);

private:

	/*!
	 * Gesture update callback
	 * @param e
	 */
	void gestureUpdate(MicroBitEvent e);


	// Bluetooth stack we're running on.
	BLEDevice &ble;
//	MicroBitSerial &serial;

	// memory for our 8 bit light characteristic.
	uint8_t GestureDataCharacteristicBuffer;
	GattCharacteristic characteristic;

	// Handles to access each characteristic when they are held by Soft Device.
	GattAttribute::Handle_t characteristicHandle;
};


#endif //CALLIOPE_DEMO_CALLIOPESERVICEMOVE_H
