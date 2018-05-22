#ifndef BLUETOOTH_SERVICE_PROGRAM_H
#define BLUETOOTH_SERVICE_PROGRAM_H

#include "ble/BLE.h"
#include "Interpreter.h"

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
    GattCharacteristic characteristic;
    uint8_t characteristicsBuffer[CHARACTERISTICS_BUFFER_LEN];
};

#endif // BLUETOOTH_SERVICE_PROGRAM_H