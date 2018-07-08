#include "BluetoothServiceNotify.h"
#include "MicroBit.h"
#include "Bytes.h"
#include "BluetoothServiceProgram.h"


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
    characteristic.requireSecurity(SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK);
    characteristic.setReadAuthorizationCallback(this, &BluetoothServiceNotify::onDataRead);

    GattCharacteristic *characteristics[] = {
        &characteristic
    };

    GattService service(
        BluetoothServiceNotifyUUID,
        characteristics,
        sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    // TODO make this configuration dependent
#ifdef TARGET_NRF51_CALLIOPE
    ManagedString namePrefix("Calliope mini [");
#else
    ManagedString namePrefix("BBC micro:bit [");
#endif
    ManagedString namePostfix("]");
    ManagedString BLEName = namePrefix + microbit_friendly_name() + namePostfix;


    // Update the advertised name of this micro:bit to include the device name
    ble.clearAdvertisingPayload();

    ble.accumulateAdvertisingPayload(
            GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *) BLEName.toCharArray(),
                                     BLEName.length());
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                     BluetoothServiceNotifyUUID,
                                     16);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                     BluetoothServiceProgramUUID,
                                     16);
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(200);
    ble.setAdvertisingTimeout(0);
    ble.startAdvertising();

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