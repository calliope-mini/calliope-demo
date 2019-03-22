//TODO add header
//
// Created by wowa on 21.03.19.
//

#ifndef CALLIOPE_DEMO_CALLIOPESERVICEINTERPRETER_H
#define CALLIOPE_DEMO_CALLIOPESERVICEINTERPRETER_H

#include <ble/BLE.h>
#include "Interpreter.h"

extern const uint8_t CalliopeServiceInterpreterUUID[];
extern const uint8_t CalliopeServiceInterpreterNotifyUUID[];
extern const uint8_t CalliopeServiceInterpreterProgramUUID[];


class CalliopeServiceInterpreter {
public:

    /*!
     * Constructor.
     * Create a representation of CalliopeServiceNotify
     *
     * @param _ble Reference to a BLE device instance
     * @param _interpreter Reference to an Interpreter instance
     */
    CalliopeServiceInterpreter(BLEDevice &_ble, Interpreter &_interpreter);

    //###########NOTIFY
    /*!
     * Send data via BLE.
     * @param address
     * @param value
     */
    void send(uint16_t address, uint16_t value);

    //##########PROGRAM
    /*!
     * Callback. Invoked when any of our attributes are written via BLE.
     */
    void onDataWritten(const GattWriteCallbackParams *params);

    /*!
     * Callback. Invoked when any of our attributes are read via BLE.
     */
    void onDataRead(GattReadAuthCallbackParams *params);


private:
    Interpreter &interpreter;
    BLEDevice &ble;

    //###########NOTIFY
    GattAttribute::Handle_t notifyCharacteristicsHandle;
    uint8_t notifyCharacteristicsBuffer[4];
    GattCharacteristic notifyCharacteristic;

    //############PROGRAM
    GattAttribute::Handle_t programCharacteristicsHandle;
    uint8_t programCharacteristicsBuffer[CHARACTERISTICS_BUFFER_LEN];
    GattCharacteristic programCharacteristic;


};

#endif //CALLIOPE_DEMO_CALLIOPESERVICEINTERPRETER_H
