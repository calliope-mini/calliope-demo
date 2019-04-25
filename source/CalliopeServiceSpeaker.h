/*!
 * @file CalliopeServiceSpeaker.h
 *
 * BT service for the Speaker on the Calliope board.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */

#ifndef CALLIOPE_DEMO_CALLIOPESERVICESPEAKER_H
#define CALLIOPE_DEMO_CALLIOPESERVICESPEAKER_H

#include "MicroBit.h"
#include "MicroBitConfig.h"
#include "ble/BLE.h"

// UUIDs for our service and characteristics
extern const uint8_t CalliopeSpeakerServiceUUID[];
extern const uint8_t CalliopeSpeakerCharacteristicUUID[];


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
	  * @param _soundmotor The instance of the soundmotor, which is the soundgenerator
	  */
	CalliopeSpeakerService(BLEDevice &_ble, CalliopeSoundMotor &_soundmotor);

    /**
      * Callback. Invoked when any of our attributes are written via BLE.
      */
    void onDataWritten(const GattWriteCallbackParams *params);

private:

    // Bluetooth stack we're running on.
    BLEDevice &ble;
	CalliopeSoundMotor &soundmotor;

    // memory for our 8 bit control characteristics.
    uint8_t speakerCharacteristicBuffer[4];

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t speakerCharacteristicHandle;
};


#endif //CALLIOPE_DEMO_CALLIOPESERVICESPEAKER_H
