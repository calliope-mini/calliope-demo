/*!
 * @file CalliopeServiceGesture.cpp
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




/**
  * Class definition for the custom Calliope Gesture Service.
  * Provides a BLE service to remotely read the Gestures of the Calliope mini accelerometer.
  */
#include "MicroBitConfig.h"
#include "CalliopeServiceMaster.h"
#include "ble/UUID.h"
#include "CalliopeServiceGesture.h"

/**
  * Constructor.
  * Create a representation of the Gesture Service
  * @param _ble The instance of a BLE device that we're running on.
  */
CalliopeGestureService::CalliopeGestureService(BLEDevice &_ble) :
		ble(_ble),
		GestureDataCharacteristicBuffer(),
		characteristic(
				CalliopeGestureServiceDataUUID,
				(uint8_t *) &GestureDataCharacteristicBuffer, 0, sizeof(GestureDataCharacteristicBuffer),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
		) {
	characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

	GattCharacteristic *characteristics[] = {
			&characteristic
	};

	GattService service(
			CalliopeGestureServiceUUID,
			characteristics,
			sizeof(characteristics) / sizeof(GattCharacteristic *)
	);

	ble.addService(service);

	characteristicHandle = characteristic.getValueHandle();

	ble.gattServer().write(
			characteristicHandle,
			(uint8_t *) &GestureDataCharacteristicBuffer,
			sizeof(GestureDataCharacteristicBuffer)
	);


	if (EventModel::defaultEventBus) {
		EventModel::defaultEventBus->listen(
				MICROBIT_ID_GESTURE,
				MICROBIT_EVT_ANY,
				this,
				&CalliopeGestureService::gestureUpdate
		);
	}
}

/**
  * Gesture update callback
  */
void CalliopeGestureService::gestureUpdate(MicroBitEvent e) {
	if (ble.getGapState().connected) {
		GestureDataCharacteristicBuffer = e.value;
		ble.gattServer().notify(
				characteristicHandle,
				(const uint8_t *) &GestureDataCharacteristicBuffer,
				sizeof(GestureDataCharacteristicBuffer));
	}
}


const uint8_t CalliopeGestureServiceUUID[] = {
		0xca, 0x11, 0x06, 0x01, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t CalliopeGestureServiceDataUUID[] = {
		0xca, 0x11, 0x06, 0x03, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};


