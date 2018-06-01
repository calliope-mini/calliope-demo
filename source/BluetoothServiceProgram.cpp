#include "BluetoothServiceProgram.h"
#include "MicroBit.h"
//#include "ble/UUID.h"
#include "Bytes.h"

extern MicroBit uBit;

BluetoothServiceProgram::BluetoothServiceProgram(Interpreter &_interpreter) :
    interpreter(_interpreter),
    ble(*uBit.ble),
    characteristic(
        BluetoothServiceProgramUUID,
        (uint8_t *)&characteristicsBuffer, 0, sizeof(characteristicsBuffer),
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
    ),
    characteristicsBuffer()
{
    characteristic.requireSecurity(SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK);//MICROBIT_BLE_SECURITY_LEVEL);
    characteristic.setReadAuthorizationCallback(this, &BluetoothServiceProgram::onDataRead);

    GattCharacteristic *characteristics[] = {
        &characteristic
    };

    GattService service(
        BluetoothServiceProgramUUID,
        characteristics,
        sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

//    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS, BluetoothServiceProgramUUID, 16);
//    ble.gap().updateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS, BluetoothServiceProgramUUID,16);


    characteristicsHandle = characteristic.getValueHandle();

    ble.onDataWritten(this, &BluetoothServiceProgram::onDataWritten);
}

void BluetoothServiceProgram::onDataWritten(const GattWriteCallbackParams *params)
{
    if (params->handle == characteristicsHandle) {

        const uint8_t *data = params->data;
        const uint16_t len = params->len;

        if (len < 2*2) {
            interpreter.status = INTERPRETER_KO_LEN_TOO_SHORT;
            return;
        }

        const uint16_t length = BYTES_TO_UINT16(data[0],data[1]);

        if (length == 0) {
            // end of upload

            const uint16_t hash_sh = BYTES_TO_UINT16(data[2],data[3]);
            const uint16_t hash_is = interpreter_calculate_hash();

            // verify hash
            if (hash_is != hash_sh) {
                interpreter.status = INTERPRETER_KO_HASH_INVALID;
                return;
            }

            if (len != 2*2 + METHODS_COUNT*2) {
                interpreter.status = INTERPRETER_KO_METHODS_TOO_SHORT;
                return;
            }

            // copy methods
            for(int i=0, o=4; i<METHODS_COUNT;i++,o+=2) {
                interpreter.methods[i] = BYTES_TO_UINT16(data[o],data[o+1]);
            }

            interpreter.status = INTERPRETER_RD;
            return;
        }

        // reset the interpreter at the start of the transmission
        if (interpreter.status != INTERPRETER_RX) {
            interpreter_reset();
            interpreter.status = INTERPRETER_RX;
        }

        const uint16_t position = BYTES_TO_UINT16(data[2],data[3]);

        if (len != 2*2+length) {
            interpreter.status = INTERPRETER_KO_CODE_TOO_SHORT;
            return;
        }

        if (position >= CODE_LEN) {
            interpreter.status = INTERPRETER_KO_CODE_START_OUT_OF_BOUNDS;
            return;
        }

        if (position+length > CODE_LEN) {
            interpreter.status = INTERPRETER_KO_CODE_STOP_OUT_OF_BOUNDS;
            return;
        }

        // copy length bytes to position
        memcpy((void*)&interpreter.code[position], (const void*)&data[4], length);
    }
}

void BluetoothServiceProgram::onDataRead(GattReadAuthCallbackParams *params)
{
    if (params->handle == characteristicsHandle) {

        const uint16_t hash_is = interpreter_calculate_hash();

        const uint8_t data[] = {
            VERSION_MAJOR,
            VERSION_MINOR,
            HI16(CODE_LEN),
            LO16(CODE_LEN),
            HI16(hash_is),
            LO16(hash_is),
            interpreter.status
        };

        ble.gattServer().write(
            characteristicsHandle,
            data, sizeof(data));

    }
}