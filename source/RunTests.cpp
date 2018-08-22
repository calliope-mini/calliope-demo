#include <cmath>
#include "RunTests.h"
#include "MicroBit.h"
#include "MicroBitSerial.h"
#include "Images.h"
#include "Utils.h"

extern MicroBit uBit;

void onButtonA(MicroBitEvent)
{
    uBit.display.print("A");
    uBit.rgb.off();
    uBit.rgb.setColour(0, 0, 255, 0);
}

void onButtonB(MicroBitEvent)
{
    uBit.display.print("B");
    uBit.rgb.off();
    uBit.rgb.setColour(255, 0, 0, 0);
}

void onButtonAB(MicroBitEvent)
{
    uBit.display.print("#");
    uBit.rgb.off();
    uBit.rgb.setColour(0, 255, 0, 0);
}

void onButton0(MicroBitEvent)
{
    uBit.display.print("0");
}

void onButton1(MicroBitEvent)
{
    uBit.display.print("1");
}

void onButton2(MicroBitEvent)
{
    uBit.display.print("2");
}

void onButton3(MicroBitEvent)
{
    uBit.display.print("3");
}

void onShake(MicroBitEvent)
{
    uBit.display.print("S");
}

void onFaceUp(MicroBitEvent)
{
    uBit.display.print("+");
}

void onFaceDown(MicroBitEvent)
{
    uBit.display.print("-");
}

void onTiltUp(MicroBitEvent)
{
    uBit.display.print("U");
}

void onTiltDown(MicroBitEvent)
{
    uBit.display.print("D");
}

void onTiltLeft(MicroBitEvent)
{
    uBit.display.print("L");
}

void onTiltRight(MicroBitEvent)
{
    uBit.display.print("R");
}

void tests_run()
{
    uBit.serial.setTxBufferSize(MICROBIT_SERIAL_DEFAULT_BUFFER_SIZE);

    uBit.sleep(200);

    uBit.serial.send("sound\r\n");
    uBit.soundmotor.setSoundSilentMode(true);
    uBit.soundmotor.soundOn(500);
    uBit.sleep(100);
    uBit.soundmotor.soundOn(2000);
    uBit.sleep(100);
    uBit.soundmotor.soundOn(3000);
    uBit.sleep(100);
    uBit.soundmotor.soundOff();
    uBit.sleep(500);

    uBit.serial.send("display\r\n");
    uBit.display.clear();
    uBit.display.print(*images(ImageFull));
    for (int i = 255; i > 0; i -= 2) {
        uBit.display.setBrightness(i);
        uBit.sleep(3);
    }
    uBit.sleep(3);
    for (int i = 0; i < 255; i += 2) {
        uBit.display.setBrightness(i);
        uBit.sleep(3);
    }

    uBit.serial.send("RGB led\r\n");
    uBit.rgb.off();
    uBit.rgb.setColour(255, 0, 0, 0);
    uBit.sleep(200);
    uBit.rgb.off();
    uBit.rgb.setColour(0, 255, 0, 0);
    uBit.sleep(200);
    uBit.rgb.off();
    uBit.rgb.setColour(0, 0, 255, 0);
    uBit.sleep(200);
    uBit.rgb.off();

    uBit.serial.send("accelerometer\r\n");
    for (int i = 0; i < 10; i++) {
        uBit.accelerometer.getX();
        uBit.accelerometer.getY();
        uBit.accelerometer.getZ();
    }
    uBit.display.print(*images(ImageTick));

    uBit.io.P12.isTouched();
    uBit.io.P0.isTouched();
    uBit.io.P1.isTouched();
    uBit.io.P16.isTouched();

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);
    uBit.messageBus.listen(MICROBIT_ID_IO_P12, MICROBIT_EVT_ANY, onButton0);
    uBit.messageBus.listen(MICROBIT_ID_IO_P0, MICROBIT_EVT_ANY, onButton1);
    uBit.messageBus.listen(MICROBIT_ID_IO_P1, MICROBIT_EVT_ANY, onButton2);
    uBit.messageBus.listen(MICROBIT_ID_IO_P16, MICROBIT_EVT_ANY, onButton3);

    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, onShake);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_FACE_UP, onFaceUp);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_FACE_DOWN, onFaceDown);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_TILT_UP, onTiltUp);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_TILT_DOWN, onTiltDown);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_TILT_LEFT, onTiltLeft);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_TILT_RIGHT, onTiltRight);

    uBit.sleep(500);
    uBit.serial.send("microphone\r\n");
    uBit.display.clear();

    while (true) {
        int mic = uBit.io.P21.getAnalogValue();
        // ((value - fromLow) * (toHigh - toLow)) / (fromHigh - fromLow) + toLow
        if (mic > 512) {
        const int gauge = static_cast<const int>((log2(mic - 511) * 4) / 9);

            for (uint8_t i = 0; i <= 4; i++) {
                const int16_t x = static_cast<uint16_t>(4);
                const int16_t y = static_cast<uint16_t>(4 - i);
                const uint8_t t = static_cast<uint8_t>(i > gauge ? 0 : 255);
                uBit.display.image.setPixelValue(x, y, t);
            }
            uBit.sleep(10);

            uBit.serial.printf("mic: %d -> %d\r\n", mic, gauge);
        }
    }
}