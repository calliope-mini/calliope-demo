/*!
 * @file CalliopeServiceLightSensor.cpp
 *
 * BT service for the light sensor on  the Calliope board.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */


/*!
  * Class definition for the custom Calliope Light Sensor Service
  * Provides a BLE service to remotely read the light sensor from the LED matrix on the Calliope board..
  */
#include "MicroBitConfig.h"
#include "ble/UUID.h"
#include "CalliopeServiceLightSensor.h"

/*!
  * Constructor.
  * Create a representation of the CalliopeLightSensorService
  * @param _ble The instance of a BLE device that we're running on.
  * @param _display The instance of the LED matrix
  */
CalliopeLightSensorService::CalliopeLightSensorService(BLEDevice &_ble, MicroBitDisplay &_display) :
        ble(_ble),
        display(_display),
        lightDataCharacteristicBuffer(),
        characteristic(
                CalliopeLightSensorServiceDataUUID,
                (uint8_t *)&lightDataCharacteristicBuffer, 0, sizeof(lightDataCharacteristicBuffer),
                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        )
{
    characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    characteristic.setReadAuthorizationCallback(this, &CalliopeLightSensorService::onDataRead);

    GattCharacteristic *characteristics[] = {
            &characteristic
    };

    GattService service(
            CalliopeLightSensorServiceUUID,
            characteristics,
            sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    characteristicHandle = characteristic.getValueHandle();
}


void CalliopeLightSensorService::onDataRead(GattReadAuthCallbackParams *params) {
    if (params->handle == characteristicHandle) {

        lightDataCharacteristicBuffer = display.readLightLevel();

        ble.gattServer().write(
                characteristicHandle,
                (const uint8_t *)&lightDataCharacteristicBuffer,
                sizeof(uint8_t));
    }
}

const uint8_t  CalliopeLightSensorServiceUUID[] = {
        0xca, 0x11, 0x03, 0x01, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t  CalliopeLightSensorServiceDataUUID[] = {
        0xca, 0x11, 0x03, 0x03, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

