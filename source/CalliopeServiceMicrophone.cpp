/*!
 * @file CalliopeServiceMicrophone.cpp
 *
 * BT service for the microphone on the Calliope board.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */


/**
  * Class definition for the custom Calliope Microphone Service.
  * Provides a BLE service to remotely read the microphone value on the Calliope board.
  */
#include "MicroBitConfig.h"
#include "ble/UUID.h"
#include "CalliopeServiceMicrophone.h"

/**
  * Constructor.
  * Create a representation of the CalliopeMicrophone Service
  * @param _ble The instance of a BLE device that we're running on.
  * @param _io The instance of the analog port, where the microphone is connected to (io.P21)
  */
CalliopeMicrophoneService::CalliopeMicrophoneService(BLEDevice &_ble, MicroBitIO &_io) :
        ble(_ble),
        io(_io),
        microphoneDataCharacteristicBuffer(),
        characteristic(
                CalliopeMicrophoneServiceDataUUID,
                (uint8_t *)&microphoneDataCharacteristicBuffer, 0, sizeof(microphoneDataCharacteristicBuffer),
                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        )
{
    characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    characteristic.setReadAuthorizationCallback(this, &CalliopeMicrophoneService::onDataRead);

    GattCharacteristic *characteristics[] = {
            &characteristic
    };

    GattService service(
            CalliopeMicrophoneServiceUUID,
            characteristics,
            sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    characteristicHandle = characteristic.getValueHandle();
}


void CalliopeMicrophoneService::onDataRead(GattReadAuthCallbackParams *params) {
    if (params->handle == characteristicHandle) {

	    // make 16 consecutive measurements and take the maximum value of it
	    uint16_t tempAnalogValue = 0;
	    microphoneDataCharacteristicBuffer = 0;
	    for (uint8_t i = 0; i < 16; ++i) {
		    tempAnalogValue = io.P21.getAnalogValue();
		    if (tempAnalogValue > microphoneDataCharacteristicBuffer) {
			    microphoneDataCharacteristicBuffer = tempAnalogValue;
		    }
	    }

//        if (microphoneDataCharacteristicBuffer > 512) {
//            // we do not support double and int32 should be enough
//            const int32_t gauge = static_cast<const int32_t>((log2(microphoneDataCharacteristicBuffer - 511) * 4) / 9);
//
//            microphoneDataCharacteristicBuffer = gauge;
//        } else {
//            microphoneDataCharacteristicBuffer = 0;
//        }

        ble.gattServer().write(
                characteristicHandle,
                (const uint8_t *)&microphoneDataCharacteristicBuffer,
                sizeof(microphoneDataCharacteristicBuffer));
    }
}

const uint8_t  CalliopeMicrophoneServiceUUID[] = {
        0xca, 0x11, 0x02, 0x01, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t  CalliopeMicrophoneServiceDataUUID[] = {
        0xca, 0x11, 0x02, 0x03, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

