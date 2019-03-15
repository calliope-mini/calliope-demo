//
// Created by wowa on 06.03.19.
//

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
        ble(_ble), rgb(_rgb)
{
    // Create the data structures that represent each of our characteristics in Soft Device.
    GattCharacteristic  rgbColorCharacteristic(CalliopeRGBServiceColorCharacteristicUUID,
                                               (uint8_t *)rgbColorCharacteristicBuffer,
                                               0,
                                               sizeof(rgbColorCharacteristicBuffer),
                                               GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    GattCharacteristic  rgbOffCharacteristic(CalliopeRGBServiceOffCharacteristicUUID,
                                             (uint8_t *)&rgbOffCharacteristicBuffer,
                                             0,
                                             sizeof(rgbOffCharacteristicBuffer),
                                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    // Initialise our characteristic values.
    rgbColorCharacteristicBuffer[0] = 0;
    rgbOffCharacteristicBuffer = 0;


    // Set default security requirements
    rgbColorCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    rgbOffCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *characteristics[] = {&rgbColorCharacteristic, &rgbOffCharacteristic};
    GattService         service(CalliopeRGBServiceUUID,
                                characteristics,
                                sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.gattServer().addService(service);

    rgbColorCharacteristicHandle = rgbColorCharacteristic.getValueHandle();
    rgbOffCharacteristicHandle = rgbOffCharacteristic.getValueHandle();

    ble.onDataWritten(this, &CalliopeRGBService::onDataWritten);
}


/**
  * Callback. Invoked when any of our attributes are written via BLE.
  */
void CalliopeRGBService::onDataWritten(const GattWriteCallbackParams *params) {
    if (params->handle == rgbColorCharacteristicHandle && params->len == sizeof(rgbColorCharacteristicBuffer))
    {
        const uint8_t *colorBuffer = (uint8_t *)params->data;
        rgb.setColour(colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3]);
    }

    else if (params->handle == rgbOffCharacteristicHandle && params->len >= sizeof(rgbOffCharacteristicBuffer))
    {
        rgbOffCharacteristicBuffer = *((uint8_t *)params->data);

        // if the rg.off is called to frequently it begins to flicker
        if (rgb.isOn()) {
            rgb.off();
        }
    }
}

const uint8_t   CalliopeRGBServiceUUID[] = {
        0xca,0x11,0x01,0x02,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};

const uint8_t   CalliopeRGBServiceColorCharacteristicUUID[] = {
        0xca,0x11,0x01,0x04,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};

const uint8_t   CalliopeRGBServiceOffCharacteristicUUID[] = {
        0xca,0x11,0x01,0x05,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};
