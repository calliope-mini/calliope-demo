/*!
 * @file BluetoothServiceProgram.h
 *
 * Programming Service, neccessary for the Playground Application
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt <https://github.com/tcurdt>
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */

#ifndef BLUETOOTH_SERVICE_PROGRAM_H
#define BLUETOOTH_SERVICE_PROGRAM_H

#include "ble/BLE.h"
#include "Interpreter.h"

extern const uint8_t BluetoothServiceProgramUUID[];


/*!
 * @class BluetoothServiceProgram
 */
class BluetoothServiceProgram
{
public:

    /*!
     * Constructor.
     * Create a representation of BluetoothServiceProgram
     * @param interpreter
     */
    BluetoothServiceProgram(Interpreter &interpreter);

    /*!
     * Callback. Invoked when any of our attributes are written via BLE.
     */
    void onDataWritten(const GattWriteCallbackParams *params);

    /*!
     * Callback. Invoked when any of our attributes are read via BLE.
     */
    void onDataRead(GattReadAuthCallbackParams *params);

private:

    Interpreter &interpreter;
    BLEDevice &ble;

    GattAttribute::Handle_t characteristicsHandle;
    uint8_t characteristicsBuffer[CHARACTERISTICS_BUFFER_LEN];
    GattCharacteristic characteristic;
};

#endif // BLUETOOTH_SERVICE_PROGRAM_H