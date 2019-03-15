//
// Created by wowa on 11.03.19.
//

#include <MicroBit.h>
#include "PlaygroundFree.h"
#include "CalliopeServiceRGB.h"
#include "CalliopeServiceLightSensor.h"
#include "CalliopeServiceMicrophone.h"
#include "Interpreter.h"
#include "BluetoothServiceNotify.h"
#include "CalliopeServiceMaster.h"

//extern Interpreter interpreter;
//extern BluetoothServiceNotify *notify;
extern CalliopeServiceMaster *masterService;

void PlaygroundFreeInit(MicroBit &uBit){

    masterService = new CalliopeServiceMaster(*uBit.ble);
//    new CalliopeRGBService(*uBit.ble, uBit.rgb);
//    new CalliopeLightSensorService(*uBit.ble, uBit.display);
//    new CalliopeMicrophoneService(*uBit.ble, uBit.io);

//    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
//    new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
//    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);

//    notify = new BluetoothServiceNotify(interpreter);

}
