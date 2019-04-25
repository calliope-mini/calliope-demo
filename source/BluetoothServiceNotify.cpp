/*!
 * @file BluetoothServiceNotify.cpp
 *
 * Notification Service, neccessary for the Playground Application
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt <https://github.com/tcurdt>
 * @author Waldemar Gruenwald <https://github.com/gruenwaldi>
 */

#include <inc/MicroBit.h>
#include "Bytes.h"
#include "CalliopeServiceMaster.h"
#include "Interpreter.h"
#include "BluetoothServiceNotify.h"

extern MicroBit uBit;

const uint8_t  BluetoothServiceNotifyUUID[] = {
    0xff,0x55,0xdd,0xee,
    0x25,0x1d,
    0x47,0x0a,
    0xa0,0x62,
    0xfa,0x19,0x22,0xdf,0xa9,0xa8
};


BluetoothServiceNotify::BluetoothServiceNotify(Interpreter &_interpreter) :
    interpreter(_interpreter),
    ble(*uBit.ble),
    characteristicsBuffer(),
    characteristic(
        BluetoothServiceNotifyUUID,
        (uint8_t *)&characteristicsBuffer, 0, sizeof(characteristicsBuffer),
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ /* |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY*/
    )
{
    characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *characteristics[] = {
        &characteristic
    };

    GattService service(
        BluetoothServiceNotifyUUID,
        characteristics,
        sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    characteristicsHandle = characteristic.getValueHandle();
}


void BluetoothServiceNotify::send(uint16_t address, uint16_t value)
{
    if (ble.getGapState().connected) {

        LOG("send addr:%d, val:%d\r\n", address, value);

        //uint8_t buffer[4];
        characteristicsBuffer[0] = HI16(address);
        characteristicsBuffer[1] = LO16(address);
        characteristicsBuffer[2] = HI16(value);
        characteristicsBuffer[3] = LO16(value);

        ble.gattServer().write(
            characteristicsHandle,
            (uint8_t *)characteristicsBuffer, sizeof(characteristicsBuffer));
    }
}

