//#include "BluetoothServiceDebug.h"
//#include "MicroBit.h"
//#include "Bytes.h"
//
//extern MicroBit uBit;
//
//static const uint8_t BluetoothServiceDebugUUID[] = {
//    0xff, 0x44, 0xdd, 0xee,
//    0x25, 0x1d,
//    0x47, 0x0a,
//    0xa0, 0x62,
//    0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
//};
//
//BluetoothServiceDebug::BluetoothServiceDebug(Interpreter& _interpreter)
//    : interpreter(_interpreter)
//    , ble(*uBit.ble)
//    , characteristic(
//          BluetoothServiceDebugUUID,
//          (uint8_t*)&characteristicsBuffer, 0, sizeof(characteristicsBuffer),
//          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ)
//    , characteristicsBuffer()
//    , selector()
//    , address()
//{
//    characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
//    characteristic.setReadAuthorizationCallback(this, &BluetoothServiceDebug::onDataRead);
//
//    GattCharacteristic* characteristics[] = {
//        &characteristic
//    };
//
//    GattService service(
//        BluetoothServiceDebugUUID,
//        characteristics,
//        sizeof(characteristics) / sizeof(GattCharacteristic*));
//
//    ble.addService(service);
//
//    characteristicsHandle = characteristic.getValueHandle();
//
//    ble.onDataWritten(this, &BluetoothServiceDebug::onDataWritten);
//}
//
//void BluetoothServiceDebug::onDataWritten(const GattWriteCallbackParams* params)
//{
//    if (params->handle == characteristicsHandle) {
//        const uint8_t* data = params->data;
//        const uint16_t len = params->len;
//
//        // set selector, hi(address), lo(address)
//        if (len == 3) {
//            selector = data[0];
//            address = BYTES_TO_UINT16(data[1], data[2]);
//        }
//    }
//}
//
//#define MIN(a, b) ((a) < (b) ? (a) : (b))
//
//void BluetoothServiceDebug::onDataRead(GattReadAuthCallbackParams* params)
//{
//    if (params->handle == characteristicsHandle) {
//
//        // get code at address
//        if (selector == 0x00) {
//
//            const uint8_t* buffer = &interpreter.code[0];
//
//            const uint16_t start = static_cast<const uint16_t> MIN(address, CODE_LEN);
//            const uint16_t stop = static_cast<const uint16_t> MIN(address + 16, CODE_LEN);
//            const uint16_t len = stop - start;
//
//            uint8_t data[1 + 2 + 16];
//            data[0] = 0x00;
//            data[1] = HI16(address);
//            data[2] = LO16(address);
//
//            if (len > 0) {
//                memcpy(&data[3], buffer + start, len);
//            }
//
//            ble.gattServer().write(
//                characteristicsHandle,
//                data,
//                static_cast<uint16_t>(3 + len));
//
//        } else
//
//            // get methods at address
//            if (selector == 0x01) {
//
//            const uint8_t* buffer = (uint8_t*)&interpreter.methods[0];
//
//            const uint16_t start = static_cast<const uint16_t> MIN(address, METHODS_COUNT * 2);
//            const uint16_t stop = static_cast<const uint16_t> MIN(address + 2, METHODS_COUNT * 2);
//            const uint16_t len = stop - start;
//
//            uint8_t data[1 + 2 + 2];
//            data[0] = 0x01;
//            data[1] = HI16(address);
//            data[2] = LO16(address);
//
//            if (len > 0) {
//                memcpy(&data[3], buffer + start, len);
//            }
//
//            ble.gattServer().write(
//                characteristicsHandle,
//                data,
//                static_cast<uint16_t>(3 + len));
//        }
//    }
//}