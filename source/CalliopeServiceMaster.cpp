//
// Created by wowa on 13.03.19.
//

#include "CalliopeServiceMaster.h"

#include "BluetoothServiceNotify.h"
#include "MicroBit.h"
#include "Bytes.h"
#include "BluetoothServiceProgram.h"
#include "Images.h"
#include "CalliopeServiceRGB.h"
#include "CalliopeServiceLightSensor.h"
#include "CalliopeServiceMicrophone.h"
#include "Storage.h"


//extern MicroBit uBit;

/**
  * Callback when a BLE connection is established.
  */
static void bleConnectionCallback(const Gap::ConnectionCallbackParams_t *) {
    LOG("CONNECTED\r\n");
    uBit.display.print(*images(ImageTick));
}


/**
  * Callback when a BLE connection is lost.
  */
static void bleDisconnectionCallback(const Gap::DisconnectionCallbackParams_t *) {
    LOG("DISCONNECTED\r\n");
    uBit.display.print(*images(ImageScissors));
}


CalliopeServiceMaster::CalliopeServiceMaster(BLEDevice &_ble) :
        interpreter(),
        ble(_ble),
//        rgbService(NULL),
        characteristicBitfield(),
        serviceStatus(),
        statusChanged()
{

    // Create the data structures that represent each of our characteristics in Soft Device.
    GattCharacteristic characteristic(
            CalliopeMasterServiceCharacteristicUUID,
            (uint8_t *) characteristicBitfield, 0, sizeof(characteristicBitfield),
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
            );


    characteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *characteristics[] = {
            &characteristic
    };


    GattService service(
            CalliopeMasterServiceUUID,
            characteristics,
            sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.gap().onConnection(bleConnectionCallback);
    ble.gap().onDisconnection(bleDisconnectionCallback);

    ble.addService(service);

    characteristicsHandle = characteristic.getValueHandle();

    ble.onDataWritten(this, &CalliopeServiceMaster::onDataWritten);



    // TODO make this configuration dependent
#ifdef TARGET_NRF51_CALLIOPE
    ManagedString namePrefix("micro:bit [");//"Calliope mini [");
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
                                     CalliopeMasterServiceUUID,
                                     16);

    // check the stored status information
    KeyValuePair *status;
    status = uBit.storage.get("MasterStatus");
    LOG("status = %d, %s\r\n", *(uint32_t*)status->value, status->key);
    if (status != NULL){
        updateServices(*(uint32_t*)status->value);
        memcpy(serviceStatus,status->value, 4);
        free(status);
    }

    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(200);
    ble.setAdvertisingTimeout(0);
    ble.startAdvertising();
}


void CalliopeServiceMaster::send(uint16_t address, uint16_t value) {
    if (ble.getGapState().connected) {

        LOG("send addr:%d, val:%d\r\n", address, value);

        ble.gattServer().write(
                characteristicsHandle,
                (uint8_t *) &characteristicBitfield,
                sizeof(characteristicBitfield));
    }
}

/**
  * Callback. Invoked when any of our attributes are written via BLE.
  */
void CalliopeServiceMaster::onDataWritten(const GattWriteCallbackParams *params) {
    LOG("onDataWritten data: %08x, len:%d\r\n", *(uint32_t*)params->data, params->len);
    if (params->handle == characteristicsHandle && params->len == sizeof(characteristicBitfield)) {
        uBit.display.print(*images(ImageSmiley), 500);
        uint8_t tempStatus[4];
        memcpy(tempStatus,params->data,4);
        if((uint32_t)*serviceStatus != (uint32_t)*tempStatus) {
            send(0,0);
            setStatus(tempStatus);
        }
    }
    else if (params->handle == characteristicsHandle && params->len >= sizeof(characteristicBitfield)) {
    }
}

uint32_t CalliopeServiceMaster::updateServices(const uint32_t requestedStatus){
    LOG("updateServices: %d\r\n", requestedStatus);
    uint32_t tempStatus = 0;

    // check for RGB Service
    if (requestedStatus && CALLIOPE_SERVICE_FLAG_RGB){
        new CalliopeRGBService(*uBit.ble, uBit.rgb);
        tempStatus |= CALLIOPE_SERVICE_FLAG_RGB;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                CalliopeRGBServiceUUID,
                16);
        LOG("new rgbService\r\n");
    }
    else if (!requestedStatus && CALLIOPE_SERVICE_FLAG_RGB){
//        if (rgbService){
//            delete rgbService;
//            rgbService = NULL;
            LOG("deleted rgbService\r\n");
//        }
        tempStatus &= ~CALLIOPE_SERVICE_FLAG_RGB;    //>! reset the corresponding flag
    }
//#define CALLIOPE_SERVICE_FLAG_PROGRAM   (uint32_t)0x20000000
    if (requestedStatus && CALLIOPE_SERVICE_FLAG_PROGRAM){
        new BluetoothServiceProgram(interpreter);
        tempStatus |= CALLIOPE_SERVICE_FLAG_PROGRAM;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                BluetoothServiceProgramUUID,
                16);
        LOG("new programService\r\n");
    }
    else if (!requestedStatus && CALLIOPE_SERVICE_FLAG_PROGRAM){
//        if (program){
//            delete program;
//            program = NULL;
            LOG("deleted programService\r\n");
//        }
        tempStatus &= ~CALLIOPE_SERVICE_FLAG_PROGRAM;    //>! reset the corresponding flag
    }

//#define CALLIOPE_SERVICE_FLAG_NOTIFY    (uint32_t)0x40000000
    if (requestedStatus && CALLIOPE_SERVICE_FLAG_NOTIFY){
        new BluetoothServiceNotify(interpreter);
        tempStatus |= CALLIOPE_SERVICE_FLAG_NOTIFY;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                BluetoothServiceNotifyUUID,
                16);
        LOG("new notifyService\r\n");
    }
    else if (!requestedStatus && CALLIOPE_SERVICE_FLAG_NOTIFY){
//        if (notify){
//            delete notify;
//            notify = NULL;
            LOG("deleted notifyService\r\n");
//        }
        tempStatus &= ~CALLIOPE_SERVICE_FLAG_NOTIFY;    //>! reset the corresponding flag
    }

//#define CALLIOPE_SERVICE_FLAG_RESET     (uint32_t)0x80000000

    return tempStatus;
}

//uint32_t CalliopeServiceMaster::updateAdvertizing(uint8_t *requestedStatus) {
//    LOG("updateAdvertizing: 0x%08x\r\n", *(uint32_t*)requestedStatus);
////    serviceStatus[0] = requestedStatus[0];
////    serviceStatus[1] = requestedStatus[1];
////    serviceStatus[2] = requestedStatus[2];
////    serviceStatus[3] = requestedStatus[3];
//    memcpy(serviceStatus,requestedStatus, sizeof(serviceStatus));
//    LOG("%04x,%04x,%04x,%04x\r\n", serviceStatus[0],serviceStatus[1],serviceStatus[2],serviceStatus[3]);
////    uBit.storage.put("MasterStatus", characteristicBitfield, 4);
////    LOG("STORED stATUS\r\n");
//    uBit.storage.put("MasterStatus", serviceStatus, 4);
//    LOG("STORED staTUS\r\n");
//    return 0;
//}

void CalliopeServiceMaster::setStatus(uint8_t *status){
    memcpy(serviceStatus,status, 4);
    statusChanged = 1;
}

uint8_t CalliopeServiceMaster::getStatus(uint8_t *status){
    if(statusChanged) {
        memcpy(status, serviceStatus, 4);
        statusChanged = 0;
        return 1;
    }
    return 0;
}


const uint8_t CalliopeMasterServiceUUID[] = {
        0xca, 0x11, 0xff, 0x01, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

const uint8_t CalliopeMasterServiceCharacteristicUUID[] = {
        0xca, 0x11, 0xff, 0x02, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8
};

