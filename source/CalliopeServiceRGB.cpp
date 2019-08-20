/*!
 * @file CalliopeServiceRGB.cpp
 *
 * BT service for the RGB LED on the Calliope board.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */


/**
  * Class definition for the custom Calliope RGB LED Service.
  * Provides a BLE service to remotely set the color or turn of the RGB LED.
  */
#include "MicroBitConfig.h"
#include "ble/UUID.h"

#include "CalliopeServiceRGB.h"

/**
  * Constructor.
  * Create a representation of the RGBService
  * @param _ble The instance of a BLE device that we're running on.
  * @param _rgb An instance of CalliopeRGB to interface with.
  */
CalliopeRGBService::CalliopeRGBService(BLEDevice &_ble, CalliopeRGB &_rgb) :
        ble(_ble), rgb(_rgb) {
    // Create the data structures that represent each of our characteristics in Soft Device.
	GattCharacteristic rgbColorCharacteristic(
			CalliopeRGBServiceColorCharacteristicUUID,
			(uint8_t *) rgbColorCharacteristicBuffer, 0, sizeof(rgbColorCharacteristicBuffer),
			GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE
	);

    // Initialise our characteristic values.
    rgbColorCharacteristicBuffer[0] = 0;

    // Set default security requirements
    rgbColorCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *characteristics[] = {&rgbColorCharacteristic};
    GattService service(CalliopeRGBServiceUUID,
                        characteristics,
                        sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.gattServer().addService(service);

    rgbColorCharacteristicHandle = rgbColorCharacteristic.getValueHandle();

    ble.onDataWritten(this, &CalliopeRGBService::onDataWritten);
}


/**
  * Callback. Invoked when any of our attributes are written via BLE.
  */
void CalliopeRGBService::onDataWritten(const GattWriteCallbackParams *params) {
    if (params->handle == rgbColorCharacteristicHandle && params->len == sizeof(rgbColorCharacteristicBuffer)) {
        const uint8_t *colorBuffer = (uint8_t *) params->data;
        if ((*(uint32_t *) colorBuffer == 0) && rgb.isOn()) {  //>! turn the LED off
            rgb.off();
        }
        rgb.setColour(colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3]);
    }
}

const uint8_t CalliopeRGBServiceUUID[] = {
        0xca, 0x11, 0x01, 0x01, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t CalliopeRGBServiceColorCharacteristicUUID[] = {
        0xca, 0x11, 0x01, 0x02, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};