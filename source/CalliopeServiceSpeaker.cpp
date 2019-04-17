//TODO add header
//
// Created by wowa on 19.03.19.
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

#include "CalliopeServiceSpeaker.h"

extern MicroBit uBit;

/*!
 * Play a tone, which is asked for by the CalliopeServiceSpeaker
 *
 * @param buffer (uint16_t frequency[Hz], uint16_t length[ms])
 */
static void  playSound(void *buffer) {
    uint8_t *speakerBuffer = (uint8_t*)buffer;
    //todo make this independent of uBit
	if ((uint16_t) (speakerBuffer[2] + (speakerBuffer[3] << 8)) ==
	    0) {    //!< if a 0 is transmitted, just turn the sound off.
		uBit.soundmotor.soundOff();
	} else { //!< if the length of the tone is bigger then 0
		uBit.soundmotor.soundOn((uint16_t) (speakerBuffer[0] + (speakerBuffer[1] << 8)));
		fiber_sleep((uint16_t) (speakerBuffer[2] + (speakerBuffer[3] << 8)));
		uBit.soundmotor.soundOff();
	}
}


/**
  * Constructor.
  * Create a representation of the RGBService
  * @param _ble The instance of a BLE device that we're running on.
  * @param _rgb An instance of CalliopeSpeaker to interface with.
  */
CalliopeSpeakerService::CalliopeSpeakerService(BLEDevice &_ble) :
        ble(_ble) {

    // Create the data structures that represent each of our characteristics in Soft Device.
    GattCharacteristic speakerCharacteristic(CalliopeSpeakerCharacteristicUUID,
                                             (uint8_t *) speakerCharacteristicBuffer,
                                             0,
                                             sizeof(speakerCharacteristicBuffer),
                                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    // Initialise our characteristic values.
    speakerCharacteristicBuffer[0] = 0;

    // Set default security requirements
    speakerCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *characteristics[] = {&speakerCharacteristic};
    GattService service(CalliopeSpeakerServiceUUID,
                        characteristics,
                        sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.gattServer().addService(service);

    speakerCharacteristicHandle = speakerCharacteristic.getValueHandle();

    ble.onDataWritten(this, &CalliopeSpeakerService::onDataWritten);
}

/**
  * Callback. Invoked when any of our attributes are written via BLE.
  */
void CalliopeSpeakerService::onDataWritten(const GattWriteCallbackParams *params) {
    if (params->handle == speakerCharacteristicHandle && params->len == sizeof(speakerCharacteristicBuffer)) {
        invoke(playSound, (void *) params->data);
    }
}


const uint8_t CalliopeSpeakerServiceUUID[] = {
        0xca, 0x11, 0x04, 0x01, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t CalliopeSpeakerCharacteristicUUID[] = {
        0xca, 0x11, 0x04, 0x02, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

