#pragma once
#include "Interpreter.h"
#include "ble/BLE.h"

class BluetoothServiceDebug {
public:
    BluetoothServiceDebug(Interpreter& interpreter);

    void onDataWritten(const GattWriteCallbackParams* params);
    void onDataRead(GattReadAuthCallbackParams* params);

private:
    Interpreter& interpreter;
    BLEDevice& ble;

    GattAttribute::Handle_t characteristicsHandle;
    GattCharacteristic characteristic;
    uint8_t characteristicsBuffer[CHARACTERISTICS_BUFFER_LEN];

    uint8_t selector;
    uint16_t address;
};
