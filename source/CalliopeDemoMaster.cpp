/**
 * Callipe Demo Master
 *
 * This is a special master program that can be flashed onto
 * an extra board and used to activate the hidden games in the
 * official firmware.
 *
 * Press A to activate Conways game of life in all minis found.
 * Press B to acticate Priximity Hearts in all minis found.
 * Press A+B to stop the games.
 *
 * The minis will proxy the start and stop codes.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 *
 * @author Matthias L. Jugel <leo@calliope.cc>
 */

#include "MicroBit.h"
#include "CalliopeDemo.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"


#ifdef COMPILE_FIRMWARE_MASTER

MicroBit uBit;


void buttonA(MicroBitEvent event) {
    uBit.serial.send("START\r\n");
    uBit.radio.datagram.send("S");
}

void buttonB(MicroBitEvent event) {
    uBit.serial.send("START 1\r\n");
    uBit.radio.datagram.send("H1");
}

void buttonB1(MicroBitEvent event) {
    uBit.serial.send("RANDOM SEED\r\n");
    uBit.radio.datagram.send(uBit.random(100) > 20 ? "1" : "0");
}

void buttonAB(MicroBitEvent event) {
    uBit.serial.send("END\r\n");
    uBit.radio.datagram.send("E");
}


void onData(MicroBitEvent event) {
    PacketBuffer packet = uBit.radio.datagram.recv();
    uBit.serial.send("RECEIVED\r\n");
    uBit.serial.send(packet);
    uBit.display.print(packet);
}

int main() {
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, buttonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, buttonB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, buttonB1);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, buttonAB);

    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);

    if(uBit.radio.enable() == MICROBIT_OK) uBit.display.scroll("RDY");
    uBit.radio.setGroup(227);

    while(true) uBit.sleep(100);
}

#endif
