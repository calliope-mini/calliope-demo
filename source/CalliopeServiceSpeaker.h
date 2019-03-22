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

#ifndef CALLIOPE_DEMO_CALLIOPESERVICESPEAKER_H
#define CALLIOPE_DEMO_CALLIOPESERVICESPEAKER_H

#include "MicroBit.h"
#include "MicroBitConfig.h"
#include "ble/BLE.h"

// UUIDs for our service and characteristics
extern const uint8_t CalliopeSpeakerServiceUUID[];
extern const uint8_t CalliopeSpeakerCharacteristicUUID[];

//TODO documentation

/**
  * Class definition for the custom MicroBit LED Service.
  * Provides a BLE service to remotely read and write the state of the LED display.
  */
class CalliopeSpeakerService {
public:

    /**
      * Constructor.
      * Create a representation of the LEDService
      * @param _ble The instance of a BLE device that we're running on.
      * @param _rgb An instance of CalliopeSpeaker to interface with.
      */
    CalliopeSpeakerService(BLEDevice &_ble);

    /**
      * Callback. Invoked when any of our attributes are written via BLE.
      */
    void onDataWritten(const GattWriteCallbackParams *params);

private:

    // Bluetooth stack we're running on.
    BLEDevice &ble;

    // memory for our 8 bit control characteristics.
    uint8_t speakerCharacteristicBuffer[4];

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t speakerCharacteristicHandle;
};


#endif //CALLIOPE_DEMO_CALLIOPESERVICESPEAKER_H
