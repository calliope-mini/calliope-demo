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
#include "CalliopeServiceSpeaker.h"

Interpreter *interpreter;

/**
  * Callback when a BLE connection is established.
  */
static void bleConnectionCallback(const Gap::ConnectionCallbackParams_t *) {
    LOG("M_CONNECTED\r\n");
    uBit.display.print(*images(ImageTick));
}


/**
  * Callback when a BLE connection is lost.
  */
static void bleDisconnectionCallback(const Gap::DisconnectionCallbackParams_t *) {
    LOG("M_DISCONNECTED\r\n");
    uBit.display.print(*images(ImageScissors));
}


CalliopeServiceMaster::CalliopeServiceMaster(BLEDevice &_ble) :
        ble(_ble),
        characteristicBitfield(),
        serviceStatus(),
        statusChanged()
{
    interpreter = NULL;


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
    ManagedString namePrefix("BBC micro:bit [");//"Calliope mini [");
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

    uint32_t tempStatus = 0;
    // check the stored status information
    KeyValuePair *status;
    status = uBit.storage.get("MasterStatus");
    LOG("M_status = 0x%08x, %s\r\n", *(uint32_t*)status->value, status->key);
    if (status != NULL){
        tempStatus = updateServices(*(uint32_t*)status->value);
        memcpy(serviceStatus,status->value, 4);
        free(status);
        LOG("M_tempStatus: 0x%08x\r\n", tempStatus);
    }

    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(200);
    ble.setAdvertisingTimeout(0);
    ble.startAdvertising();

    if(tempStatus == (CALLIOPE_SERVICE_FLAG_NOTIFY | CALLIOPE_SERVICE_FLAG_PROGRAM)){
        LOG("M_starting interpreter\r\n");
        interpreter_start();
    }
}


void CalliopeServiceMaster::send(const uint8_t *reply) {
    if (ble.getGapState().connected) {

        LOG("M_send: 0x%08x\r\n", reply);

        ble.gattServer().write(
                characteristicsHandle,
                (uint8_t *) reply,
                sizeof(reply));
    }
}

/**
  * Callback. Invoked when any of our attributes are written via BLE.
  */
void CalliopeServiceMaster::onDataWritten(const GattWriteCallbackParams *params) {
    LOG("master onDataWritten data: %08x, len:%d\r\n", *(uint32_t*)params->data, params->len);
    if (params->handle == characteristicsHandle && params->len == sizeof(characteristicBitfield)) {
        LOG("M_onDataWritten data: %08x, len:%d\r\n", *(uint32_t*)params->data, params->len);
        uBit.display.print(*images(ImageSmiley), 1000);
        if((uint32_t)*serviceStatus != *(uint32_t*)params->data) {
            send(params->data);
            setStatus(params->data);
        }
    }
}

uint32_t CalliopeServiceMaster::updateServices(const uint32_t requestedStatus){
    LOG("M_updateServices: 0x%08x\r\n", requestedStatus);
    uint32_t tempStatus = 0;

    // CALLIOPE_SERVICE_FLAG_RGB    (uint32_t)0x00000001
    // RGB Service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_RGB){
        new CalliopeRGBService(*uBit.ble, uBit.rgb);
        tempStatus |= CALLIOPE_SERVICE_FLAG_RGB;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                CalliopeRGBServiceUUID,
                16);
        LOG("M_new CalliopeRGBService\r\n");
    }

    // CALLIOPE_SERVICE_FLAG_MICROPHONE    (uint32_t)0x00000002
    // Microphone Service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_MICROPHONE){
        new CalliopeMicrophoneService(*uBit.ble, uBit.io);
        tempStatus |= CALLIOPE_SERVICE_FLAG_MICROPHONE;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                CalliopeMicrophoneServiceUUID,
                16);
        LOG("M_new CalliopeMicrophoneService\r\n");
    }

    // CALLIOPE_SERVICE_FLAG_BRIGHTNESS         (uint32_t)0x00000004
    // Light Sensor Service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_BRIGHTNESS){
        new CalliopeLightSensorService(*uBit.ble, uBit.display);
        tempStatus |= CALLIOPE_SERVICE_FLAG_BRIGHTNESS;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                CalliopeLightSensorServiceUUID,
                16);
        LOG("M_new CalliopeLightSensorService\r\n");
    }
    // CALLIOPE_SERVICE_FLAG_SPEAKER       (uint32_t)0x00000008
    // Light Sensor Service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_SPEAKER){
        new CalliopeSpeakerService(*uBit.ble);
        tempStatus |= CALLIOPE_SERVICE_FLAG_SPEAKER;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                CalliopeSpeakerServiceUUID,
                16);
        LOG("M_new CalliopeSpeakerService\r\n");
    }
    // CALLIOPE_SERVICE_FLAG_LED           (uint32_t)0x00000010
    // LED service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_LED){
        new MicroBitLEDService(*uBit.ble, uBit.display);
        tempStatus |= CALLIOPE_SERVICE_FLAG_LED;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                MicroBitLEDServiceUUID,
                16);
        LOG("M_new MicroBitLEDService\r\n");
    }
    // CALLIOPE_SERVICE_FLAG_TEMPERATURE   (uint32_t)0x00000020
    // Temperature service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_TEMPERATURE){
        new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
        tempStatus |= CALLIOPE_SERVICE_FLAG_TEMPERATURE;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                MicroBitTemperatureServiceUUID,
                16);
        LOG("M_new MicroBitTemperatureService\r\n");
    }
    // CALLIOPE_SERVICE_FLAG_BUTTON        (uint32_t)0x00000040
    // Button service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_BUTTON){
        new MicroBitButtonService(*uBit.ble);
        tempStatus |= CALLIOPE_SERVICE_FLAG_BUTTON;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                MicroBitButtonServiceUUID,
                16);
        LOG("M_new MicroBitButtonService\r\n");
    }
    // CALLIOPE_SERVICE_FLAG_MAGNETOMETER  (uint32_t)0x00000080
    // Magnetometer service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_MAGNETOMETER){
        new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
        tempStatus |= CALLIOPE_SERVICE_FLAG_MAGNETOMETER;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                MicroBitMagnetometerServiceUUID,
                16);
        LOG("M_new MicroBitMagnetometerService\r\n");
    }
    // CALLIOPE_SERVICE_FLAG_ACCELEROMETER (uint32_t)0x00000100
    // Accelerometer Service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_ACCELEROMETER){
        new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
        tempStatus |= CALLIOPE_SERVICE_FLAG_ACCELEROMETER;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                MicroBitAccelerometerServiceUUID,
                16);
        LOG("M_new MicroBitAccelerometerService\r\n");
    }
    // CALLIOPE_SERVICE_FLAG_EVENT         (uint32_t)0x01000000
    // Event Service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_EVENT){
        new MicroBitEventService(*uBit.ble, uBit.messageBus);
        tempStatus |= CALLIOPE_SERVICE_FLAG_EVENT;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                MicroBitEventServiceUUID,
                16);
        LOG("M_new MicroBitEventService\r\n");
    }
    // CALLIOPE_SERVICE_FLAG_PROGRAM   (uint32_t)0x20000000
    // Interpreter Program Service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_PROGRAM){
        if(interpreter == NULL){
            interpreter = new Interpreter;
            LOG("M_new interpreter\r\n");
        }
        new BluetoothServiceProgram(*interpreter);
        tempStatus |= CALLIOPE_SERVICE_FLAG_PROGRAM;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                BluetoothServiceProgramUUID,
                16);
        LOG("M_new programService\r\n");
    }

    // CALLIOPE_SERVICE_FLAG_NOTIFY    (uint32_t)0x40000000
    // Interpreter Notify Service
    if (requestedStatus & CALLIOPE_SERVICE_FLAG_NOTIFY){
        if(interpreter == NULL){
            interpreter = new Interpreter;
            LOG("M_new interpreter\r\n");
        }
        new BluetoothServiceNotify(*interpreter);
        tempStatus |= CALLIOPE_SERVICE_FLAG_NOTIFY;    //>! set the corresponding flag
        ble.accumulateAdvertisingPayload(
                GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                BluetoothServiceNotifyUUID,
                16);
        LOG("M_new notifyService\r\n");
    }



    return tempStatus;
}


void CalliopeServiceMaster::setStatus(const uint8_t *status){
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

