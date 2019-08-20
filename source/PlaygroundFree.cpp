/*!
 * @file PlaygroundFree.cpp
 *
 * Free programmable bluetooth application to use with Playground on Calliope-mini.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Waldemar Gruenwald 	<https://github.com/gruenwaldi>
 */

#include <MicroBit.h>
#include "PlaygroundFree.h"
#include "CalliopeServiceRGB.h"
#include "CalliopeServiceLightSensor.h"
#include "CalliopeServiceMicrophone.h"
#include "Interpreter.h"
#include "BluetoothServiceNotify.h"
#include "CalliopeServiceMaster.h"

extern CalliopeServiceMaster *masterService;

void PlaygroundFreeInit(MicroBit &uBit){

    masterService = new CalliopeServiceMaster(*uBit.ble);
}
