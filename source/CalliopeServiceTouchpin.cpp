/*!
 * @file CalliopeServiceTouchpin.cpp
 *
 * BT service for the Touchpins on the Calliope board.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */


/**
  * Class definition for the custom Calliope Touchpin Service.
  * Provides a BLE service to remotely notify about the touch on the touchpins..
  */
#include "MicroBitConfig.h"
#include "CalliopeServiceMaster.h"
#include "ble/UUID.h"
#include "CalliopeServiceTouchpin.h"

/**
  * Constructor.
  * Create a representation of the CalliopeTouchpinService.
  * @param _ble The instance of a BLE device that we're running on.
  * @param _io The instance of the pins connected to the touchpins.
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
		// check the TOUCH-PINs
		switch (event.source) {
			case MICROBIT_ID_IO_P12: // = 19
				source = 0;
				break;
			case MICROBIT_ID_IO_P0: // = 7
				source = 1;
				break;
			case MICROBIT_ID_IO_P1: // = 8
				source = 2;
				break;
			case MICROBIT_ID_IO_P16: // = 23
				source = 3;
				break;
			default:
				source = 5;
				break;

		}
		// write the value to the buffer and notify the service
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

