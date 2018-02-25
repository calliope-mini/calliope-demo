#include "BluetoothServiceNotify.h"
#include "MicroBit.h"
//#include "ble/UUID.h"
#include "Bytes.h"

extern MicroBit uBit;

static const uint8_t  BluetoothServiceNotifyUUID[] = {
    0xff,0x55,0xdd,0xee,
    0x25,0x1d,
    0x47,0x0a,
    0xa0,0x62,
    0xfa,0x19,0x22,0xdf,0xa9,0xa8
};


BluetoothServiceNotify::BluetoothServiceNotify(Interpreter &_interpreter) :
    interpreter(_interpreter),
    ble(*uBit.ble),
    characteristic(
        BluetoothServiceNotifyUUID,
        (uint8_t *)&characteristicsBuffer, 0, sizeof(characteristicsBuffer),
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    characteristicsBuffer()
{
    characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    characteristic.setReadAuthorizationCallback(this, &BluetoothServiceNotify::onDataRead);

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

void BluetoothServiceNotify::onDataRead(GattReadAuthCallbackParams *params)
{
    if (params->handle == characteristicsHandle) {
    }
}

void BluetoothServiceNotify::send(uint16_t address, uint16_t value)
{
    if (ble.getGapState().connected) {

        uint8_t buffer[4];
        buffer[0] = HI16(address);
        buffer[1] = LO16(address);
        buffer[2] = HI16(value);
        buffer[3] = LO16(value);

        ble.gattServer().notify(
            characteristicsHandle,
            (uint8_t *)buffer, sizeof(buffer));
    }
}