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
  * Class definition for the custom MicroBit LED Service.
  * Provides a BLE service to remotely read and write the state of the LED display.
  */
#include "MicroBitConfig.h"
#include "ble/UUID.h"

#include "CalliopeServiceRGB.h"

/**
  * Constructor.
  * Create a representation of the LEDService
  * @param _ble The instance of a BLE device that we're running on.
  * @param _display An instance of MicroBitDisplay to interface with.
  */
CalliopeRGBService::CalliopeRGBService(BLEDevice &_ble, MicroBitDisplay &_display, CalliopeRGB &_rgb) :
        ble(_ble), display(_display), rgb(_rgb),
        matrixCharacteristic(CalliopeRGBServiceMatrixUUID, (uint8_t *)&matrixCharacteristicBuffer, 0, sizeof(matrixCharacteristicBuffer),
                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ)
{
    // Create the data structures that represent each of our characteristics in Soft Device.
    GattCharacteristic  textCharacteristic(CalliopeRGBServiceTextUUID, (uint8_t *)textCharacteristicBuffer, 0, MICROBIT_BLE_MAXIMUM_SCROLLTEXT,
                                           GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    GattCharacteristic  scrollingSpeedCharacteristic(CalliopeRGBServiceScrollingSpeedUUID, (uint8_t *)&scrollingSpeedCharacteristicBuffer, 0,
                                                     sizeof(scrollingSpeedCharacteristicBuffer), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

    // Initialise our characteristic values.
    memclr(matrixCharacteristicBuffer, sizeof(matrixCharacteristicBuffer));
    textCharacteristicBuffer[0] = 0;
    scrollingSpeedCharacteristicBuffer = MICROBIT_DEFAULT_SCROLL_SPEED;

    matrixCharacteristic.setReadAuthorizationCallback(this, &CalliopeRGBService::onDataRead);

    // Set default security requirements
    matrixCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    textCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    scrollingSpeedCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *characteristics[] = {&matrixCharacteristic, &textCharacteristic, &scrollingSpeedCharacteristic};
    GattService         service(CalliopeRGBServiceUUID, characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    matrixCharacteristicHandle = matrixCharacteristic.getValueHandle();
    textCharacteristicHandle = textCharacteristic.getValueHandle();
    scrollingSpeedCharacteristicHandle = scrollingSpeedCharacteristic.getValueHandle();

    ble.gattServer().write(scrollingSpeedCharacteristicHandle, (const uint8_t *)&scrollingSpeedCharacteristicBuffer, sizeof(scrollingSpeedCharacteristicBuffer));
    ble.gattServer().write(matrixCharacteristicHandle, (const uint8_t *)&matrixCharacteristicBuffer, sizeof(matrixCharacteristicBuffer));

    ble.onDataWritten(this, &CalliopeRGBService::onDataWritten);
}


/**
  * Callback. Invoked when any of our attributes are written via BLE.
  */
void CalliopeRGBService::onDataWritten(const GattWriteCallbackParams *params)
{
    uint8_t *data = (uint8_t *)params->data;

    if (params->handle == matrixCharacteristicHandle && params->len > 0 && params->len < 6)
    {
        // interrupt any animation that might be currently going on
        display.stopAnimation();
        for (int y=0; y<params->len; y++)
            for (int x=0; x<5; x++)
                display.image.setPixelValue(x, y, (data[y] & (0x01 << (4-x))) ? 255 : 0);
    }

    else if (params->handle == textCharacteristicHandle)
    {
        // Create a ManagedString representation from the UTF8 data.
        // We do this explicitly to control the length (in case the string is not NULL terminated!)
        ManagedString s((char *)params->data, params->len);

        // interrupt any animation that might be currently going on
        display.stopAnimation();

        // Start the string scrolling and we're done.
        display.scrollAsync(s, (int) scrollingSpeedCharacteristicBuffer);

        colorBuffer[0] = params->data[0];
        colorBuffer[1] = params->data[1];
        colorBuffer[2] = params->data[2];
        colorBuffer[3] = params->data[3];
        rgb.setColour(colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3]);
    }

    else if (params->handle == scrollingSpeedCharacteristicHandle && params->len >= sizeof(scrollingSpeedCharacteristicBuffer))
    {
        // Read the speed requested, and store it locally.
        // We use this as the speed for all scroll operations subsquently initiated from BLE.
        scrollingSpeedCharacteristicBuffer = *((uint16_t *)params->data);

        // if the rg.off is called to frequently it begins to flicker
        if (rgb.isOn()) {
            rgb.off();
        }
    }
}

/**
  * Callback. Invoked when any of our attributes are read via BLE.
  */
void CalliopeRGBService::onDataRead(GattReadAuthCallbackParams *params)
{
    if (params->handle == matrixCharacteristicHandle)
    {
        for (int y=0; y<5; y++)
        {
            matrixCharacteristicBuffer[y] = 0;

            for (int x=0; x<5; x++)
            {
                if (display.image.getPixelValue(x, y))
                    matrixCharacteristicBuffer[y] |= 0x01 << (4-x);
            }
        }

        ble.gattServer().write(matrixCharacteristicHandle, (const uint8_t *)&matrixCharacteristicBuffer, sizeof(matrixCharacteristicBuffer));
    }
}


const uint8_t  CalliopeRGBServiceUUID[] = {
        0xca,0x11,0x01,0x02,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};

const uint8_t  CalliopeRGBServiceMatrixUUID[] = {
        0xca,0x11,0x01,0x03,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};

const uint8_t  CalliopeRGBServiceTextUUID[] = {
        0xca,0x11,0x01,0x04,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};

const uint8_t  CalliopeRGBServiceScrollingSpeedUUID[] = {
        0xca,0x11,0x01,0x05,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};
