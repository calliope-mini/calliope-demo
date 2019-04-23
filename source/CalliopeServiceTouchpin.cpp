//
// Created by wowa on 13.04.19.
//

#include "CalliopeServiceTouchpin.h"

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

/**
  * Class definition for the custom MicroBit light Service.
  * Provides a BLE service to remotely read the silicon light of the nRF51822.
  */
#include "MicroBitConfig.h"
#include "CalliopeServiceMaster.h"
#include "ble/UUID.h"

/**
  * Constructor.
  * Create a representation of the lightService
  * @param _ble The instance of a BLE device that we're running on.
  * @param _thermometer An instance of MicroBitThermometer to use as our light source.
  */
CalliopeTouchpinService::CalliopeTouchpinService(BLEDevice &_ble, MicroBitIO &_io) :
		ble(_ble),
		io(_io),
		TouchpinDataCharacteristicBuffer(),
		characteristic(
				CalliopeTouchpinServiceDataUUID,
				(uint8_t *) &TouchpinDataCharacteristicBuffer, 0, sizeof(TouchpinDataCharacteristicBuffer),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
		) {
	characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

	GattCharacteristic *characteristics[] = {
			&characteristic
	};

	GattService service(
			CalliopeTouchpinServiceUUID,
			characteristics,
			sizeof(characteristics) / sizeof(GattCharacteristic *)
	);

	ble.addService(service);

	characteristicHandle = characteristic.getValueHandle();

	ble.gattServer().write(
			characteristicHandle,
			(uint8_t *) &TouchpinDataCharacteristicBuffer,
			sizeof(TouchpinDataCharacteristicBuffer)
	);

	// initialize the touchpin functionality
	io.P12.isTouched();
	io.P0.isTouched();
	io.P1.isTouched();
//	MicroBitPin P16(MICROBIT_ID_IO_P16, MICROBIT_PIN_P16, PIN_CAPABILITY_DIGITAL_IN);
//	P16.isTouched();

	io.P16.isTouched();


	if (EventModel::defaultEventBus) {
		EventModel::defaultEventBus->listen(
				MICROBIT_ID_IO_P12,
				MICROBIT_EVT_ANY,
				this,
				&CalliopeTouchpinService::touchpinUpdate,
				MESSAGE_BUS_LISTENER_IMMEDIATE);
		EventModel::defaultEventBus->listen(
				MICROBIT_ID_IO_P0,
				MICROBIT_EVT_ANY,
				this,
				&CalliopeTouchpinService::touchpinUpdate,
				MESSAGE_BUS_LISTENER_IMMEDIATE);
		EventModel::defaultEventBus->listen(
				MICROBIT_ID_IO_P1,
				MICROBIT_EVT_ANY,
				this,
				&CalliopeTouchpinService::touchpinUpdate,
				MESSAGE_BUS_LISTENER_IMMEDIATE);
		EventModel::defaultEventBus->listen(
				MICROBIT_ID_IO_P16,
				MICROBIT_EVT_ANY,
				this,
				&CalliopeTouchpinService::touchpinUpdate,
				MESSAGE_BUS_LISTENER_IMMEDIATE);
	}
}


/**
  * Touchpin update callback
  */
void CalliopeTouchpinService::touchpinUpdate(MicroBitEvent event) {
	if (ble.getGapState().connected) {
		uint16_t source;
		uint16_t value;
		// check the event
		switch (event.value) {
			case MICROBIT_BUTTON_EVT_CLICK:            //!< release after short touch
			case MICROBIT_BUTTON_EVT_LONG_CLICK:    //!< release after long touch
			case MICROBIT_BUTTON_EVT_UP:            //!< release
				value = 0;
				break;
			case MICROBIT_BUTTON_EVT_DOWN:          //!< short
				value = 1;
				break;
			case MICROBIT_BUTTON_EVT_HOLD:          //!< long touch
				value = 2;
				break;
			default:
				value = event.value;
		}
		switch (event.source) {
			// check the TOUCH-PINs
			case MICROBIT_ID_IO_P12:
				source = 0;
//				value = event.value;
//				serial.printf("pin 0x%x 0x%x start\n\r", source, event.value);
				break;

			case MICROBIT_ID_IO_P0:
				source = 1;
//				value = event.value;
//				serial.printf("pin 0x%x 0x%x start\n\r", source, event.value);
				break;

			case MICROBIT_ID_IO_P1: // = 8
				source = 2;
//				value = event.value;
//				serial.printf("pin 0x%x 0x%x start\n\r", source, event.value);
				break;

			case MICROBIT_ID_IO_P16: // = 23
				source = 3;
//				value = event.value;
//				serial.printf("pin 0x%x 0x%x start\n\r", source, event.value);
				break;
			default:
				source = 5;
//				value = 2;
//				serial.printf("default pin 0x%x 0x%x start\n\r", event.source,  event.value);
				break;

		}
		TouchpinDataCharacteristicBuffer = (source << 8) + value;
		ble.gattServer().notify(
				characteristicHandle,
				(const uint8_t *) &TouchpinDataCharacteristicBuffer,
				sizeof(TouchpinDataCharacteristicBuffer));
	}
}


const uint8_t CalliopeTouchpinServiceUUID[] = {
		0xca, 0x11, 0x05, 0x01, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t CalliopeTouchpinServiceDataUUID[] = {
		0xca, 0x11, 0x05, 0x03, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

