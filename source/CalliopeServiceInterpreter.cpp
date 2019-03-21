//
// Created by wowa on 21.03.19.
//

#include "CalliopeServiceInterpreter.h"
#include "Bytes.h"
#include "CalliopeServiceMaster.h"

const uint8_t CalliopeServiceInterpreterUUID[] = {
        0xff, 0x44, 0xdd, 0xee, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t CalliopeServiceInterpreterNotifyUUID[] = {
        0xff, 0x55, 0xdd, 0xee, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t CalliopeServiceInterpreterProgramUUID[] = {
        0xff, 0x66, 0xdd, 0xee, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};


CalliopeServiceInterpreter::CalliopeServiceInterpreter(BLEDevice &_ble, Interpreter &_interpreter) :
        interpreter(_interpreter),
        ble(_ble),
        notifyCharacteristicsBuffer(),
        notifyCharacteristic(
                CalliopeServiceInterpreterNotifyUUID,
                (uint8_t *) &notifyCharacteristicsBuffer, 0, sizeof(notifyCharacteristicsBuffer),
                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ /* |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY*/
        ),
        programCharacteristicsBuffer(),
        programCharacteristic(
                CalliopeServiceInterpreterProgramUUID,
                (uint8_t *) &programCharacteristicsBuffer, 0, sizeof(programCharacteristicsBuffer),
                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        ) {
    notifyCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    programCharacteristic.requireSecurity(
            SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);//SECURITY_MODE_ENCRYPTION_OPEN_LINK);
    programCharacteristic.setReadAuthorizationCallback(this, &CalliopeServiceInterpreter::onDataRead);


    GattCharacteristic *characteristics[] = {
            &notifyCharacteristic,
            &programCharacteristic
    };

    GattService service(
            CalliopeServiceInterpreterUUID,
            characteristics,
            sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    notifyCharacteristicsHandle = notifyCharacteristic.getValueHandle();
    programCharacteristicsHandle = programCharacteristic.getValueHandle();

    ble.onDataWritten(this, &CalliopeServiceInterpreter::onDataWritten);

}


void CalliopeServiceInterpreter::send(uint16_t address, uint16_t value) {
    if (ble.getGapState().connected) {

//        LOG("send addr:%d, val:%d\r\n", address, value);

        //uint8_t buffer[4];
        notifyCharacteristicsBuffer[0] = HI16(address);
        notifyCharacteristicsBuffer[1] = LO16(address);
        notifyCharacteristicsBuffer[2] = HI16(value);
        notifyCharacteristicsBuffer[3] = LO16(value);

        ble.gattServer().write(
                notifyCharacteristicsHandle,
                (uint8_t *) notifyCharacteristicsBuffer, sizeof(notifyCharacteristicsBuffer));
    }
}

void CalliopeServiceInterpreter::onDataWritten(const GattWriteCallbackParams *params) {
    if (params->handle == programCharacteristicsHandle) {

        const uint8_t *data = params->data;
        const uint16_t len = params->len;

        if (len < 2 * 2) {
            interpreter.status = INTERPRETER_KO_LEN_TOO_SHORT;
            return;
        }

        const uint16_t length = BYTES_TO_UINT16(data[0], data[1]);

        if (length == 0) {
            // end of upload
            const uint16_t hash_sh = BYTES_TO_UINT16(data[2], data[3]);
            const uint16_t hash_is = interpreter_calculate_hash();
            // verify hash
            if (hash_is != hash_sh) {
                interpreter.status = INTERPRETER_KO_HASH_INVALID;
                return;
            }
            if (len != 2 * 2 + METHODS_COUNT * 2) {
                interpreter.status = INTERPRETER_KO_METHODS_TOO_SHORT;
                return;
            }
            // copy methods
            for (int i = 0, o = 4; i < METHODS_COUNT; i++, o += 2) {
                interpreter.methods[i] = BYTES_TO_UINT16(data[o], data[o + 1]);
            }
            interpreter.status = INTERPRETER_RD;
            return;
        }
        // reset the interpreter at the start of the transmission
        if (interpreter.status != INTERPRETER_RX) {
            interpreter_reset();
            interpreter.status = INTERPRETER_RX;
        }
        const uint16_t position = BYTES_TO_UINT16(data[2], data[3]);

        if (len != 2 * 2 + length) {
            interpreter.status = INTERPRETER_KO_CODE_TOO_SHORT;
            return;
        }
        if (position >= CODE_LEN) {
            interpreter.status = INTERPRETER_KO_CODE_START_OUT_OF_BOUNDS;
            return;
        }
        if (position + length > CODE_LEN) {
            interpreter.status = INTERPRETER_KO_CODE_STOP_OUT_OF_BOUNDS;
            return;
        }
        // copy length bytes to position
        memcpy((void *) &interpreter.code[position], (const void *) &data[4], length);
    }
}

void CalliopeServiceInterpreter::onDataRead(GattReadAuthCallbackParams *params) {
    if (params->handle == programCharacteristicsHandle) {

        const uint16_t hash_is = interpreter_calculate_hash();

        const uint8_t data[] = {
                VERSION_MAJOR,
                VERSION_MINOR,
                HI16(CODE_LEN),
                LO16(CODE_LEN),
                HI16(hash_is),
                LO16(hash_is),
                (uint8_t) (interpreter.status)
        };

        ble.gattServer().write(
                programCharacteristicsHandle,
                data, sizeof(data));

    }
}

