/*!
 * @file CalliopeServiceMicrophone.h
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


#ifndef CALLIOPE_DEMO_CALLIOPESERVICEMICROPHONE_H
#define CALLIOPE_DEMO_CALLIOPESERVICEMICROPHONE_H

#include "MicroBit.h"
#include "MicroBitConfig.h"
#include "ble/BLE.h"

// UUIDs for our service and characteristics
extern const uint8_t  CalliopeMicrophoneServiceUUID[];
extern const uint8_t  CalliopeMicrophoneServiceDataUUID[];

/**
  * Class definition for the custom MicroBit light Service.
  * Provides a BLE service to remotely read the silicon light of the nRF51822.
  */
class CalliopeMicrophoneService
{
public:

    /**
      * Constructor.
      * Create a representation of the lightService
      * @param _ble The instance of a BLE device that we're running on.
      * @param _display An instance of MicroBitDisplay to use as our light source.
      */
    CalliopeMicrophoneService(BLEDevice &_ble, MicroBitIO &_io);

    /*!
     * Callback. Invoked when any of our attributes are read via BLE.
     */
    void onDataRead(GattReadAuthCallbackParams *params);

private:

    // Bluetooth stack we're running on.
    BLEDevice           	&ble;
    MicroBitIO              &io;

	// memory for our 32 bit microphone characteristic.
    int32_t             microphoneDataCharacteristicBuffer;
    GattCharacteristic  characteristic;

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t characteristicHandle;
};

#endif //CALLIOPE_DEMO_CALLIOPESERVICEMICROPHONE_H
