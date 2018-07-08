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


const uint8_t BluetoothServiceProgramUUID[] = {
        0xff, 0x66, 0xdd, 0xee,
        0x25, 0x1d,
        0x47, 0x0a,
        0xa0, 0x62,
        0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

#endif // BLUETOOTH_SERVICE_PROGRAM_H