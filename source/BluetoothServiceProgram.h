#pragma once
#include "ble/BLE.h"
#include "Interpreter.h"

class BluetoothServiceProgram
{
    public:

        BluetoothServiceProgram(Interpreter &interpreter);

        void onDataWritten(const GattWriteCallbackParams *params);
        void onDataRead(GattReadAuthCallbackParams *params);

    private:

        Interpreter &interpreter;
        BLEDevice &ble;

        GattAttribute::Handle_t characteristicsHandle;
        GattCharacteristic characteristic;
        uint8_t characteristicsBuffer[CHARACTERISTICS_BUFFER_LEN];
};
