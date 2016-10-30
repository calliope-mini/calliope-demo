/**
 * Calliope Demo Code.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Matthias L. Jugel <leo@calliope.cc>
 * @author Stephan Noller <stephan@calliope.cc>
 * @author Franka Futterlieb <franka@urbn.de>
 * @author Niranjan Rao
 */
#include "MicroBit.h"
#include "CalliopeDemo.h"

const uint8_t full[25] = {1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1
};
const uint8_t dot[25] = {0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0,
                         0, 0, 1, 0, 0,
                         0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0
};
const uint8_t small[25] = {0, 0, 0, 0, 0,
                           0, 1, 1, 1, 0,
                           0, 1, 0, 1, 0,
                           0, 1, 1, 1, 0,
                           0, 0, 0, 0, 0
};
const uint8_t large[25] = {1, 1, 1, 1, 1,
                           1, 0, 0, 0, 1,
                           1, 0, 0, 0, 1,
                           1, 0, 0, 0, 1,
                           1, 1, 1, 1, 1,
};
const uint8_t arrow_left[25] = {0, 0, 1, 0, 0,
                                0, 1, 0, 0, 0,
                                1, 1, 1, 1, 1,
                                0, 1, 0, 0, 0,
                                0, 0, 1, 0, 0
};
const uint8_t arrow_right[25] = {0, 0, 1, 0, 0,
                                 0, 0, 0, 1, 0,
                                 1, 1, 1, 1, 1,
                                 0, 0, 0, 1, 0,
                                 0, 0, 1, 0, 0
};
const uint8_t arrow_leftright[25] = {0, 0, 1, 0, 0,
                                     0, 1, 0, 1, 0,
                                     1, 0, 1, 0, 1,
                                     0, 1, 0, 1, 0,
                                     0, 0, 1, 0, 0
};
const uint8_t double_row[25] = {0, 1, 1, 0, 0,
                                0, 1, 1, 0, 0,
                                0, 1, 1, 0, 0,
                                0, 1, 1, 0, 0,
                                0, 1, 1, 0, 0
};
const uint8_t tick[25] = {0, 0, 0, 0, 0,
                          0, 0, 0, 0, 1,
                          0, 0, 0, 1, 0,
                          1, 0, 1, 0, 0,
                          0, 1, 0, 0, 0
};
const uint8_t heart[25] = {0, 1, 0, 1, 0,
                           1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1,
                           0, 1, 1, 1, 0,
                           0, 0, 1, 0, 0
};
const uint8_t smiley[25] = {0, 1, 0, 1, 0,
                            0, 1, 0, 1, 0,
                            0, 0, 0, 0, 0,
                            1, 0, 0, 0, 1,
                            0, 1, 1, 1, 0
};
const uint8_t sadly[25] = {0, 1, 0, 1, 0,
                           0, 1, 0, 1, 0,
                           0, 0, 0, 0, 0,
                           0, 1, 1, 1, 0,
                           1, 0, 0, 0, 1
};
const uint8_t rock[25] = {0, 0, 0, 0, 0,
                          0, 1, 1, 1, 0,
                          0, 1, 1, 1, 0,
                          0, 1, 1, 1, 0,
                          0, 0, 0, 0, 0
};
const uint8_t scissors[25] = {1, 0, 0, 0, 1,
                              0, 1, 0, 1, 0,
                              0, 0, 1, 0, 0,
                              0, 1, 0, 1, 0,
                              1, 0, 0, 0, 1
};
const uint8_t well[25] = {0, 1, 1, 1, 0,
                          1, 0, 0, 0, 1,
                          1, 0, 0, 0, 1,
                          1, 0, 0, 0, 1,
                          0, 1, 1, 1, 0
};
const uint8_t flash[25] = {0, 0, 1, 1, 0,
                           0, 1, 1, 0, 0,
                           1, 1, 1, 1, 1,
                           0, 0, 1, 1, 0,
                           0, 1, 1, 0, 0,

};
const uint8_t wave[7 * 5] = {0, 0, 0, 0, 0, 0, 0,
                             0, 1, 1, 0, 0, 0, 0,
                             1, 0, 0, 1, 0, 0, 1,
                             0, 0, 0, 0, 1, 1, 0,
                             0, 0, 0, 0, 0, 0, 0
};

const MicroBitImage Full(5, 5, full);
const MicroBitImage Dot(5, 5, dot);
const MicroBitImage SmallRect(5, 5, small);
const MicroBitImage LargeRect(5, 5, large);
const MicroBitImage ArrowLeft(5, 5, arrow_left);
const MicroBitImage ArrowRight(5, 5, arrow_right);
const MicroBitImage ArrowLeftRight(5, 5, arrow_leftright);
const MicroBitImage DoubleRow(5, 5, double_row);
const MicroBitImage Tick(5, 5, tick);
const MicroBitImage Heart(5, 5, heart);
const MicroBitImage Smiley(5, 5, smiley);
const MicroBitImage Sadly(5, 5, sadly);
const MicroBitImage Rock(5, 5, rock);
const MicroBitImage Scissors(5, 5, scissors);
const MicroBitImage Well(5, 5, well);
const MicroBitImage Flash(5, 5, flash);
const MicroBitImage Wave(7, 5, wave);

MicroBit uBit;

// DEMO 0 - 3 
static const int MAX_DEMOS = 3;
static const int DEFAULT_PAUSE = 300;

volatile bool eventOK = false;
volatile static bool introEventSkip = false;

void simpleEventHandler(MicroBitEvent event) { eventOK = true; }

void introSkipEventHandler(MicroBitEvent event) {
    introEventSkip = true;
    eventOK = true;
}

static bool on = true;
void blinkImage(const MicroBitImage &image, int interval, int rate) {
    if (rate % interval == 0) {
        if (on) uBit.display.printAsync(image, 0, 0, 0, 100);
        else uBit.display.clear();
        on = !on;
    }
    uBit.sleep(100);
}

void introBlinkImage(const MicroBitImage &image, int interval) {
    int rate = 0;
    do blinkImage(image, interval, ++rate); while (!eventOK);
    uBit.display.clear();
}

void introAnimateImage(const MicroBitImage &image, int interval, int pos1, int pos2) {
    int rate = 0;
    bool on = true;
    do {
        if (++rate % interval == 0) {
            uBit.display.clear();
            if (on) uBit.display.print(image, pos1, 0, 0, 200);
            else uBit.display.print(image, pos2, 0, 0, 200);
            on = !on;
        }
        uBit.sleep(100);
    } while (!eventOK);
    uBit.display.clear();
}

void dadadada() {
    for (int i = 0; i < 3; i++) {
        uBit.soundmotor.soundOn(392);
        fiber_sleep(125);
        uBit.soundmotor.soundOff();
        fiber_sleep(63);
    }
    uBit.soundmotor.soundOn(311);
    fiber_sleep(1500);
    uBit.soundmotor.soundOff();
    fiber_sleep(63);
    for (int i = 0; i < 3; i++) {
        uBit.soundmotor.soundOn(349);
        fiber_sleep(125);
        uBit.soundmotor.soundOff();
        fiber_sleep(63);
    }
    uBit.soundmotor.soundOn(294);
    fiber_sleep(1500);
    uBit.soundmotor.soundOff();
}

void showIntro() {
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_LONG_CLICK, introSkipEventHandler);

    invoke(dadadada);

    uBit.display.scroll("Hallo!");
    // press A
    uBit.display.print("A");
    uBit.sleep(DEFAULT_PAUSE);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, simpleEventHandler);
    introBlinkImage(ArrowLeft, 4);
    if (introEventSkip) return;
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, simpleEventHandler);
    eventOK = false;
    uBit.display.print(Tick);
    uBit.sleep(DEFAULT_PAUSE);
    uBit.display.clear();

    // press B
    uBit.display.print("B");
    uBit.sleep(DEFAULT_PAUSE);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, simpleEventHandler);
    introBlinkImage(ArrowRight, 4);
    if (introEventSkip) return;
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, simpleEventHandler);
    eventOK = false;
    uBit.display.print(Tick);
    uBit.sleep(DEFAULT_PAUSE);
    uBit.display.clear();

    // press A+B
    uBit.display.scroll("A+B");
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, simpleEventHandler);
    introBlinkImage(ArrowLeftRight, 4);
    if (introEventSkip) return;
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, simpleEventHandler);
    eventOK = false;
    uBit.display.print(Tick);
    uBit.sleep(DEFAULT_PAUSE);
    uBit.display.clear();

    // shake
    uBit.display.scroll("SCHUETTELN");
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, simpleEventHandler);
    introAnimateImage(DoubleRow, 2, -1, 2);
    if (introEventSkip) return;
    uBit.messageBus.ignore(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, simpleEventHandler);
    eventOK = false;
    uBit.display.print(Tick);
    uBit.sleep(DEFAULT_PAUSE);
    uBit.display.clear();
    uBit.display.scroll("SUPER!");

    // show heart and change RGB led colors randomly

    on = true;
    for (int rate = 0; rate < 25; rate++) {
        if (introEventSkip) return;
        blinkImage(Heart, 2, rate);
        uBit.seedRandom();
        if (rate % 2 == 0) {
            int r = uBit.random(DEFAULT_PAUSE);
            if (r < 100) uBit.rgb.setColour(0xFF, 0xA5, 0x00, 0x00);
            else if (r < 200) uBit.rgb.setColour(0x00, 0xFF, 0x00, 0x00);
            else if (r < 300) uBit.rgb.setColour(0xFF, 0xA5, 0x00, 0x000);
        }
        uBit.sleep(100);
    }
    uBit.rgb.off();
    uBit.display.clear();

    if (introEventSkip) return;
    uBit.display.print(Smiley);
    uBit.sleep(1000);
}


// MENU handling
volatile state_t state = Intro;

static int selectedDemo = 0;

void menuDown(MicroBitEvent event) {
    selectedDemo--;
    if (selectedDemo < 0) selectedDemo = MAX_DEMOS;
    uBit.display.print(ManagedString(selectedDemo + 1));
}

void menuUp(MicroBitEvent event) {
    selectedDemo++;
    if (selectedDemo > MAX_DEMOS) selectedDemo = 0;
    uBit.display.print(ManagedString(selectedDemo + 1));
}

void menuAnimateEnter() {
    uBit.display.print(Dot, 0, 0, 0, 200);
    uBit.display.print(SmallRect, 0, 0, 0, 200);
    uBit.display.print(LargeRect, 0, 0, 0, 200);
    uBit.display.clear();
}

void menuAnimateLeave() {
    uBit.display.print(LargeRect, 0, 0, 0, 200);
    uBit.display.print(SmallRect, 0, 0, 0, 200);
    uBit.display.print(Dot, 0, 0, 0, 200);
    uBit.display.clear();
}

// Oracle Demo
void leaveOracle(MicroBitEvent event) {
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, simpleEventHandler);
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveOracle);
    state = Menu;
    eventOK = true;
}

void oracle() {
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveOracle);

    uBit.display.scroll("Orakel");
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, simpleEventHandler);
    do {
        eventOK = false;
        introBlinkImage(ArrowLeft, 4);
        if (state != Oracle) break;
        for (int i = 0; i < 5; i++) {
            uBit.display.print(Dot, 0, 0, 0, DEFAULT_PAUSE);
            uBit.display.clear();
            uBit.sleep(200);
        }
        int r = uBit.random(100);
        if (r < 50) uBit.display.print(Smiley);
        else uBit.display.print(Sadly);
        uBit.sleep(3000);
        uBit.display.clear();
    } while (state == Oracle);
}

// Rock Paper Scissors Well Demo
void leaveRockPaperScissors(MicroBitEvent event) {
    uBit.messageBus.ignore(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, simpleEventHandler);
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveRockPaperScissors);
    state = Menu;
    eventOK = true;
}

void rockPaperScissors() {
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveRockPaperScissors);

    uBit.display.print(Rock, 0, 0, 0, DEFAULT_PAUSE*2);
    uBit.display.print(Full, 0, 0, 0, DEFAULT_PAUSE*2);
    uBit.display.print(Scissors, 0, 0, 0, DEFAULT_PAUSE*2);
    uBit.display.print(Well, 0, 0, 0, DEFAULT_PAUSE*2);
    uBit.display.clear();
    uBit.sleep(DEFAULT_PAUSE);

    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, simpleEventHandler);
    do {
        eventOK = false;
        introAnimateImage(DoubleRow, 2, -1, 2);
        if (state != RockPaperScissors) break;
        int r = uBit.random(400);
        if (r < 100) uBit.display.print(Rock);
        else if (r < 200) uBit.display.print(Full);
        else if (r < 300) uBit.display.print(Scissors);
        else uBit.display.print(Well);
        uBit.sleep(3000);
        uBit.display.clear();
    } while (state == RockPaperScissors);
}

// Love Meter Demo
int touch_p1 = 0;
int touch_p2 = 0;

// a little co-routing that does the measuring while we are waiting for both pins touched
void loveMeterMeasuring() {
    touch_p1 = uBit.io.P1.getAnalogValue();
    touch_p2 = uBit.io.P2.getAnalogValue();
    do {
        fiber_sleep(300);
        int p1 = uBit.io.P1.getAnalogValue();
        int p2 = uBit.io.P2.getAnalogValue();
        eventOK = (abs(touch_p1 - p1) > 20 && abs(touch_p2 - p2) > 20);
    } while (state == LoveMeter);
}

void leaveLoveMeter(MicroBitEvent event) {
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveLoveMeter);
    uBit.serial.send("leave love meter...\r\n");
    state = Menu;
    eventOK = true;
}

void loveMeter() {
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveLoveMeter);

    uBit.display.print(Heart, 0, 0, 0, DEFAULT_PAUSE);
    invoke(loveMeterMeasuring);

    uBit.io.P1.isTouched();
    uBit.io.P2.isTouched();

    do {
        eventOK = false;
        introAnimateImage(Wave, 2, -1, -2);
        if (state != LoveMeter) break;
        int p1 = uBit.io.P1.getAnalogValue();
        int p2 = uBit.io.P2.getAnalogValue();
        int v1 = (int) (p1 * 9.0 / 1023.0);
        int v2 = (int) (p2 * 9.0 / 1023.0);
        int v3 = 9 - abs(v1 - v2);
        uBit.display.print(v3);
        uBit.sleep(1000);
        if (v3 > 6) {
            uBit.display.print(Heart);
            uBit.soundmotor.soundOn(1000);
            uBit.sleep(100);
            uBit.soundmotor.soundOn(2000);
            uBit.sleep(100);
            uBit.soundmotor.soundOn(3000);
            uBit.sleep(300);
            uBit.soundmotor.soundOff();
            uBit.sleep(4000);
        } else {
            uBit.display.print(Flash);
            uBit.soundmotor.soundOn(1000);
            uBit.sleep(100);
            uBit.soundmotor.soundOn(500);
            uBit.sleep(100);
            uBit.soundmotor.soundOn(100);
            uBit.sleep(300);
            uBit.soundmotor.soundOff();
            uBit.sleep(4000);
        }
        uBit.display.clear();
    } while (state == LoveMeter);
}

// Snake Demo
void leaveSnake(MicroBitEvent event) {
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveSnake);
    state = Menu;
    eventOK = true;
}

void runSnake() {
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, leaveLoveMeter);
    eventOK = false;
    snake();
}

// Menu Selection
void menuSelect(MicroBitEvent event);

void initializeMenu() {
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, menuDown);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, menuUp);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, menuSelect);
    uBit.display.print(ManagedString(selectedDemo + 1));
}

void menuSelect(MicroBitEvent event) {
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, menuDown);
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, menuUp);
    uBit.messageBus.ignore(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, menuSelect);

    menuAnimateEnter();
    switch (selectedDemo) {
        case 0:
            state = Oracle;
            oracle();
            break;
        case 1:
            state = RockPaperScissors;
            rockPaperScissors();
            break;
        case 2:
            state = LoveMeter;
            loveMeter();
            break;
        case 3:
            state = Snake;
            runSnake();
            break;
        default:
            state = Menu;
            uBit.display.scroll("Huch?");
            selectedDemo = 0;
            uBit.messageBus.send(MicroBitEvent(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK));
            break;
    }
    menuAnimateLeave();
    initializeMenu();
}

int main() {
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.serial.baud(115200);
    uBit.serial.send("Calliope Demo v1.0\r\n");
    // call the test board procedure, will return if done already
    testBoard();

    // initialize random
    uBit.seedRandom();

    showIntro();
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_LONG_CLICK, introSkipEventHandler);

    state = Menu;

    uBit.display.clear();
    on = true;
    for(int i = 0; i < 10; i++) {
        blinkImage(Dot, 2, 2);
        uBit.sleep(DEFAULT_PAUSE);
    }
    uBit.display.clear();
    initializeMenu();

    while (true) uBit.sleep(100);
}
