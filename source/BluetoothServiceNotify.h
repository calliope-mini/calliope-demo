#ifndef INTERPRETER_BLUETOOTH_NOTIFY_H
#define INTERPRETER_BLUETOOTH_NOTIFY_H

#include "ble/BLE.h"
#include "Interpreter.h"

class BluetoothServiceNotify
{
    public:

        BluetoothServiceNotify(Interpreter &interpreter);

        void onDataRead(GattReadAuthCallbackParams *params);
        void send(uint16_t address, uint16_t value);

    private:

        Interpreter &interpreter;
        BLEDevice &ble;

        GattAttribute::Handle_t characteristicsHandle;
        GattCharacteristic characteristic;
        uint8_t characteristicsBuffer[CHARACTERISTICS_BUFFER_LEN];
};

#endif
