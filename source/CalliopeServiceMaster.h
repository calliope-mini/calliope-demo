/*!
 * @file CalliopeServiceMaster.cpp
 *
 * BT Master Service, which allows to add new Services to the be used
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */

#ifndef CALLIOPE_DEMO_CALLIOPESERVICEMASTER_H
#define CALLIOPE_DEMO_CALLIOPESERVICEMASTER_H


#include <stdint-gcc.h>
#include <ble/BLE.h>

// Flags for new implemented Calliope Services
#define CALLIOPE_SERVICE_FLAG_RGB           (uint32_t)0x00000001
#define CALLIOPE_SERVICE_FLAG_MICROPHONE    (uint32_t)0x00000002
#define CALLIOPE_SERVICE_FLAG_BRIGHTNESS    (uint32_t)0x00000004
#define CALLIOPE_SERVICE_FLAG_SPEAKER       (uint32_t)0x00000008

// Flags for already implemented Microbit Services
#define CALLIOPE_SERVICE_FLAG_LED           (uint32_t)0x00000010
#define CALLIOPE_SERVICE_FLAG_TEMPERATURE   (uint32_t)0x00000020
#define CALLIOPE_SERVICE_FLAG_BUTTON        (uint32_t)0x00000040
#define CALLIOPE_SERVICE_FLAG_MAGNETOMETER  (uint32_t)0x00000080


#define CALLIOPE_SERVICE_FLAG_ACCELEROMETER (uint32_t)0x00000100
// Flags for new implemented Calliope Services
#define CALLIOPE_SERVICE_FLAG_TOUCHPIN      (uint32_t)0x00000200
#define CALLIOPE_SERVICE_FLAG_GESTURE       (uint32_t)0x00000400
#define CALLIOPE_SERVICE_FLAG_RES_12        (uint32_t)0x00000800


#define CALLIOPE_SERVICE_FLAG_RES_13        (uint32_t)0x00001000
#define CALLIOPE_SERVICE_FLAG_RES_14        (uint32_t)0x00002000
#define CALLIOPE_SERVICE_FLAG_RES_15        (uint32_t)0x00004000
#define CALLIOPE_SERVICE_FLAG_RES_16        (uint32_t)0x00008000


#define CALLIOPE_SERVICE_FLAG_RES_17        (uint32_t)0x00010000
#define CALLIOPE_SERVICE_FLAG_RES_18        (uint32_t)0x00020000
#define CALLIOPE_SERVICE_FLAG_RES_19        (uint32_t)0x00040000
#define CALLIOPE_SERVICE_FLAG_RES_20        (uint32_t)0x00080000


#define CALLIOPE_SERVICE_FLAG_RES_21        (uint32_t)0x00100000
#define CALLIOPE_SERVICE_FLAG_RES_22        (uint32_t)0x00200000
#define CALLIOPE_SERVICE_FLAG_RES_23        (uint32_t)0x00400000
#define CALLIOPE_SERVICE_FLAG_RES_24        (uint32_t)0x00800000

// Flags for already implemented Microbit Services
#define CALLIOPE_SERVICE_FLAG_EVENT         (uint32_t)0x01000000
#define CALLIOPE_SERVICE_FLAG_RES_26        (uint32_t)0x02000000
#define CALLIOPE_SERVICE_FLAG_RES_27        (uint32_t)0x04000000
#define CALLIOPE_SERVICE_FLAG_RES_28        (uint32_t)0x08000000

// Flags for new implemented Calliope Services
#define CALLIOPE_SERVICE_FLAG_INTERPRETER   (uint32_t)0x10000000
#define CALLIOPE_SERVICE_FLAG_PROGRAM       (uint32_t)0x20000000
#define CALLIOPE_SERVICE_FLAG_NOTIFY        (uint32_t)0x40000000
#define CALLIOPE_SERVICE_FLAG_RESET         (uint32_t)0x80000000

// LOG output on serial console
//#define DEBUG_MASTER
#ifdef DEBUG_MASTER
#define LOG(format, ...) uBit.serial.printf(format, ##__VA_ARGS__)
#else
#define LOG(format, ...) ;
#endif

// UUIDs
extern const uint8_t CalliopeMasterServiceUUID[];
extern const uint8_t CalliopeMasterServiceCharacteristicUUID[];

/*!
 * @class BluetoothServiceNotify
 */

class CalliopeServiceMaster
{
public:

    /*!
     * Constructor.
     * Create a representation of BluetoothServiceNotify
     * @param interpreter Reference to an Interpreter instance
     */
    CalliopeServiceMaster(BLEDevice &_ble);


    /*!
     * Callback. Invoked when any of our attributes are written via BLE.
     */
    void onDataWritten(const GattWriteCallbackParams *params);


	/*!
	 * Update the Services, which are requested
	 * @param requestedStatus Status of Services to be enabled (CALLIOPE_SERVICE_FLAG...)
	 * @return set status
	 */
    uint32_t updateServices(const uint32_t requestedStatus);


	/*!
	 * Set the current services status.
	 * @param status to be set
	 */
	void setStatus(const uint32_t *status);


	/*!
	 * Get the current services status.
	 * @param status will be
	 * @return
	 */
	uint8_t getStatus(uint32_t *status);


    /*!
     * Send data via BLE.
     * @param reply
     */
    void send(const uint8_t *reply);

private:

    BLEDevice &ble;

    GattAttribute::Handle_t characteristicsHandle;
    uint8_t characteristicBitfield[4];
	uint32_t serviceStatus;
    uint8_t statusChanged;
};

#endif //CALLIOPE_DEMO_CALLIOPESERVICEMASTER_H
