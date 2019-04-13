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
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
		) {
	characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
	characteristic.setReadAuthorizationCallback(this, &CalliopeTouchpinService::onDataRead);

	GattCharacteristic *characteristics[] = {
			&characteristic
	};

	GattService service(
			CalliopeTouchpinServiceUUID,
			characteristics,
			sizeof(characteristics) / sizeof(GattCharacteristic *));

	ble.addService(service);

	characteristicHandle = characteristic.getValueHandle();
}


void CalliopeTouchpinService::onDataRead(GattReadAuthCallbackParams *params) {
	if (params->handle == characteristicHandle) {

		TouchpinDataCharacteristicBuffer = io.P21.getAnalogValue();

//        if (TouchpinDataCharacteristicBuffer > 512) {
//            // we do not support double and int32 should be enough
//            const int32_t gauge = static_cast<const int32_t>((log2(TouchpinDataCharacteristicBuffer - 511) * 4) / 9);
//
//            TouchpinDataCharacteristicBuffer = gauge;
//        } else {
//            TouchpinDataCharacteristicBuffer = 0;
//        }

		ble.gattServer().write(
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
