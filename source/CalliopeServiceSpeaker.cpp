/*!
 * @file CalliopeServiceSpeaker.cpp
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


/**
  * Class definition for the custom Calliope Speaker Service.
  * Provides a BLE service to remotely play a tone over the speaker on the Calliope board..
  */
#include "MicroBitConfig.h"
#include "ble/UUID.h"

#include "CalliopeServiceSpeaker.h"

extern MicroBit uBit;

/*!
 * Play a tone, which is asked for by the CalliopeServiceSpeaker
 *
 * @note: this function is called inderictly, via invoke(...)
 *
 * @param buffer (uint16_t frequency[Hz], uint16_t length[ms])
 *
 * @code:
 * invoke(playSound, (void *) params->data);
 */
static void  playSound(void *buffer) {
	uint16_t *speakerBuffer = (uint16_t *) buffer;
    //todo make this independent of uBit
	if ((speakerBuffer[1] == 0) ||
	    (speakerBuffer[0] == 0)) {    //!< if a 0 is transmitted, just turn the sound off.
		uBit.soundmotor.soundOff();
	} else { //!< if the length of the tone is bigger then 0
		uBit.soundmotor.soundOn(speakerBuffer[0]);
		// TODO the next two lines create a memory overflow
//		fiber_sleep((uint16_t) (speakerBuffer[2] + (speakerBuffer[3] << 8)));
//		uBit.soundmotor.soundOff();
	}
}


/**
  * Constructor.
  * Create a representation of the RGBService
  * @param _ble The instance of a BLE device that we're running on.
  * @param _rgb An instance of CalliopeSpeaker to interface with.
  */
CalliopeSpeakerService::CalliopeSpeakerService(BLEDevice &_ble, CalliopeSoundMotor &_soundmotor) :
		ble(_ble),
		soundmotor(_soundmotor) {

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
	    uint16_t *speakerBuffer = (uint16_t *) params->data;
	    if ((speakerBuffer[1] == 0) ||
	        (speakerBuffer[0] == 0)) {    //!< if a 0 is transmitted, just turn the sound off.
		    soundmotor.soundOff();
	    } else { //!< if the length of the tone is bigger then 0
		    soundmotor.soundOn(speakerBuffer[0]);
	    }
    }
}


const uint8_t CalliopeSpeakerServiceUUID[] = {
        0xca, 0x11, 0x04, 0x01, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t CalliopeSpeakerCharacteristicUUID[] = {
        0xca, 0x11, 0x04, 0x02, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

