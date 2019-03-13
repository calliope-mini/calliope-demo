//
// Created by wowa on 13.03.19.
//

#ifndef CALLIOPE_DEMO_CALLIOPESERVICEMASTER_H
#define CALLIOPE_DEMO_CALLIOPESERVICEMASTER_H


//#include "ManagedString.h"
#include "ble/BLE.h"
#include "Interpreter.h"

/*!
 * @class BluetoothServiceNotify
 */

class BluetoothServiceMaster
{
public:

    /*!
     * Constructor.
     * Create a representation of BluetoothServiceNotify
     * @param interpreter Reference to an Interpreter instance
     */
    BluetoothServiceMaster();

    /*!
     * Callback. Invoked when any of our attributes are read via BLE.
     */
    void onDataRead(GattReadAuthCallbackParams *params);

    /*!
     * Send data via BLE.
     * @param address
     * @param value
     */
    void send(uint16_t address, uint16_t value);

private:

    Interpreter &interpreter;
    BLEDevice &ble;

    GattAttribute::Handle_t characteristicsHandle;
    uint8_t characteristicsBuffer[4];
    GattCharacteristic characteristic;
};

#endif //CALLIOPE_DEMO_CALLIOPESERVICEMASTER_H
