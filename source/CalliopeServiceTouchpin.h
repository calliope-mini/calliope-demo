/*!
 * @file CalliopeServiceTouchpin.h
 *
 * BT service for the Touchpins on the Calliope board.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */

#ifndef CALLIOPE_DEMO_CALLIOPESERVICETOUCHPIN_H
#define CALLIOPE_DEMO_CALLIOPESERVICETOUCHPIN_H

#include "MicroBit.h"
#include "MicroBitConfig.h"
#include "ble/BLE.h"

// UUIDs for our service and characteristics
extern const uint8_t CalliopeTouchpinServiceUUID[];
extern const uint8_t CalliopeTouchpinServiceDataUUID[];

/**
  * Class definition for the custom Calliope Touchpin Service.
  * Provides a BLE service to remotely notify about a touh on the touchpins
  */
class CalliopeTouchpinService {
public:

	/**
	  * Constructor.
	  * Create a representation of the lightService
	  * @param _ble The instance of a BLE device that we're running on.
	  * @param _io The instance of the pins, connected to the touchpins.
	  */
	CalliopeTouchpinService(BLEDevice &_ble, MicroBitIO &_io);

private:

	/*!
	 * Touchpin update callback
	 * @param event
	 */
	void touchpinUpdate(MicroBitEvent event);


	// Bluetooth stack we're running on.
	BLEDevice &ble;
	MicroBitIO &io;

	// memory for our 8 bit light characteristic.
	uint16_t TouchpinDataCharacteristicBuffer;
	GattCharacteristic characteristic;

	// Handles to access each characteristic when they are held by Soft Device.
	GattAttribute::Handle_t characteristicHandle;
};


#endif //CALLIOPE_DEMO_CALLIOPESERVICETOUCHPIN_H
