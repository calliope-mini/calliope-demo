#include "Interpreter.h"
#include "MicroBit.h"
#include "BluetoothServiceDebug.h"
#include "BluetoothServiceProgram.h"
#include "BluetoothServiceNotify.h"
#include "Instruction.h"

extern MicroBit uBit;

static Interpreter interpreter;
BluetoothServiceNotify *notify;

static uint16_t find_stop(InterpreterMethod method)
{
    for(uint8_t i = method + 1; i < METHODS_COUNT; i++) {
      uint16_t stop = interpreter.methods[i];
      if (stop != METHOD_UNUSED) {
          return stop;
      }
    }
    return CODE_LEN;
}

#ifdef LOG
static uint8_t running = 0;
#endif
static void interpreter_run(InterpreterMethod method, uint16_t r0 = 0, uint16_t r1 = 0, uint16_t r2 = 0)
{
    uint16_t start = interpreter.methods[method];

    if (start == METHOD_UNUSED) {
        return;
    }

    uint16_t stop = find_stop(method);

    #ifdef LOG
    running += 1;
    uint8_t id = running;
    #endif

    // initialize interpreter state
    RunState state;
    state.pc = start;
    state.cs = COMPARED_EQ;
    state.stack = stop;
    memset(state.reg, 0, REGISTER_COUNT * sizeof(state.reg[0]));
    state.reg[0] = r0;
    state.reg[1] = r1;
    state.reg[2] = r2;

    Slice code = slice_create(interpreter.code, start, stop);

    #ifdef LOG
    uBit.serial.printf("%d: run 0x%x-0x%x start\n\r", id, start, stop);
    #endif

    while(interpreter.status == INTERPRETER_OK) {

        uint8_t instruction = slice_read8(code);

        // #ifdef LOG
        // uBit.serial.printf("%d: @0x%x ins 0x%x\n\r", id, code.position-1, instruction);
        // #endif

        switch(instruction) {
            case INS_RET:
                instruction_ret(code, interpreter, state);
                break;
            case INS_REQ:
                instruction_req(code, interpreter, state);
                break;
            case INS_RNE:
                instruction_rne(code, interpreter, state);
                break;
            case INS_RGT:
                instruction_rgt(code, interpreter, state);
                break;
            case INS_RLT:
                instruction_rlt(code, interpreter, state);
                break;
            case INS_RGE:
                instruction_rge(code, interpreter, state);
                break;
            case INS_RLE:
                instruction_rle(code, interpreter, state);
                break;

            case INS_JSR:
                instruction_jsr(code, interpreter, state);
                break;

            case INS_BRA:
                instruction_bra(code, interpreter, state);
                break;
            case INS_BEQ:
                instruction_beq(code, interpreter, state);
                break;
            case INS_BNE:
                instruction_bne(code, interpreter, state);
                break;
            case INS_BGT:
                instruction_bgt(code, interpreter, state);
                break;
            case INS_BLT:
                instruction_blt(code, interpreter, state);
                break;
            case INS_BGE:
                instruction_bge(code, interpreter, state);
                break;
            case INS_BLE:
                instruction_ble(code, interpreter, state);
                break;
            case INS_BRA16:
                instruction_bra16(code, interpreter, state);
                break;


            case INS_CMP:
                instruction_cmp(code, interpreter, state);
                break;
            case INS_CMPI:
                instruction_cmpi(code, interpreter, state);
                break;
            case INS_MOV:
                instruction_mov(code, interpreter, state);
                break;
            case INS_MOVI:
                instruction_movi(code, interpreter, state);
                break;

            case INS_ADD:
                instruction_add(code, interpreter, state);
                break;
            case INS_SUB:
                instruction_sub(code, interpreter, state);
                break;
            case INS_MUL:
                instruction_mul(code, interpreter, state);
                break;
            case INS_DIV:
                instruction_div(code, interpreter, state);
                break;

            case INS_SLEEP:
                instruction_sleep(code, interpreter, state);
                break;
            case INS_RANDOM:
                instruction_random(code, interpreter, state);
                break;
            case INS_TIME:
                instruction_time(code, interpreter, state);
                break;

            case INS_TEMPERATURE:
                instruction_temperature(code, interpreter, state);
                break;
            case INS_NOISE:
                instruction_noise(code, interpreter, state);
                break;
            case INS_BRIGHTNESS:
                instruction_brightness(code, interpreter, state);
                break;

            case INS_BUTTON:
                instruction_button(code, interpreter, state);
                break;
            case INS_PIN:
                instruction_pin(code, interpreter, state);
                break;

            case INS_DISPLAY_CLEAR:
                instruction_display_clear(code, interpreter, state);
                break;
            case INS_DISPLAY_SHOW_NUMBER:
                instruction_display_show_number(code, interpreter, state);
                break;
            case INS_DISPLAY_SHOW_IMAGE:
                instruction_display_show_image(code, interpreter, state);
                break;
            case INS_DISPLAY_SHOW_GRID:
                instruction_display_show_grid(code, interpreter, state);
                break;
            case INS_DISPLAY_SHOW_TEXT:
                instruction_display_show_text(code, interpreter, state);
                break;

            case INS_RGB_OFF:
                instruction_rgb_off(code, interpreter, state);
                break;
            case INS_RGB_ON:
                instruction_rgb_on(code, interpreter, state);
                break;

            case INS_SOUND_OFF:
                instruction_sound_off(code, interpreter, state);
                break;
            case INS_SOUND_ON:
                instruction_sound_on(code, interpreter, state);
                break;

            case INS_ACC_GESTURE:
                instruction_gesture(code, interpreter, state);
                break;
            case INS_ACC_PITCH:
                instruction_pitch(code, interpreter, state);
                break;
            case INS_ACC_ROLL:
                instruction_roll(code, interpreter, state);
                break;
            case INS_ACC_POSITION:
                instruction_position(code, interpreter, state);
                break;

            case INS_NOTIFY:
                instruction_notify(code, interpreter, state);
                break;
            case INS_DEBUG:
                instruction_debug(code, interpreter, state);
                break;

            default:
                interpreter.status = INTERPRETER_KO_INSTRUCTION_UNKNOWN;
        }

        code.position = state.pc;

        if (slice_available(code) < 1) {
            #ifdef LOG
            uBit.serial.printf("%d: run 0x%x-0x%x stop\n\r", id, start, stop);
            #endif
            return;
        }
    }
}

static void interpreter_on_button(MicroBitEvent event)
{
    #ifdef LOG
    uBit.serial.printf("button 0x%x 0x%x start\n\r", event.source, event.value);
    #endif

    interpreter_run(METHOD_ON_BUTTON, event.source, event.value);

    #ifdef LOG
    uBit.serial.printf("button 0x%x 0x%x stop\n\r", event.source, event.value);
    #endif
}

static void interpreter_on_pin(MicroBitEvent event)
{
    uint8_t source;
    switch(event.source) {
        case MICROBIT_ID_IO_P12: source = 0; break;
        case MICROBIT_ID_IO_P0:  source = 1; break;
        case MICROBIT_ID_IO_P1:  source = 2; break;
        case MICROBIT_ID_IO_P16: source = 3; break;
        default: return;
    }

    #ifdef LOG
    uBit.serial.printf("pin 0x%x 0x%x start\n\r", source, event.value);
    #endif

    interpreter_run(METHOD_ON_PIN, source, event.value);

    #ifdef LOG
    uBit.serial.printf("pin 0x%x 0x%x stop\n\r", source, event.value);
    #endif
}

static void interpreter_on_gesture(MicroBitEvent event)
{
    #ifdef LOG
    uBit.serial.printf("gesture 0x%x 0x%x start\n\r", event.source, event.value);
    #endif

    interpreter_run(METHOD_ON_GESTURE, event.source, event.value);

    #ifdef LOG
    uBit.serial.printf("gesture 0x%x 0x%x stop\n\r", event.source, event.value);
    #endif
 }

static void interpreter_startup_sound()
{
    uBit.soundmotor.soundOn(262);
    uBit.sleep(125);
    uBit.soundmotor.soundOff();

    uBit.sleep(63);

    uBit.soundmotor.soundOn(784);
    uBit.sleep(500);
    uBit.soundmotor.soundOff();
}

static void interpreter_reset_hardware()
{
    uBit.rgb.off();
    uBit.display.clear();
    uBit.soundmotor.soundOff();
}

static void interpreter_fiber()
{
    while(true) {
        interpreter_startup_sound();
        interpreter_reset_hardware();

        #ifdef LOG
        uBit.serial.send("start\n\r");
        #endif

        interpreter_run(METHOD_STARTUP);

        #ifdef LOG
        uBit.serial.send("forever\n\r");
        #endif

        while(interpreter.status == INTERPRETER_OK) {
            interpreter_run(METHOD_FOREVER);
            uBit.sleep(10);
        }

        #ifdef LOG
        uBit.serial.send("rx\n\r");
        #endif

        while(interpreter.status != INTERPRETER_RD) {
            uBit.sleep(10);
        }

        interpreter.status = INTERPRETER_OK;

        #ifdef LOG
        uBit.serial.send("reset\n\r");
        #endif
    }
}

void interpreter_reset()
{
    memset(interpreter.code, 0, CODE_LEN * sizeof(interpreter.code[0]));
    memset(interpreter.methods, METHOD_UNUSED, METHODS_COUNT * sizeof(interpreter.methods[0]));
    interpreter.status = INTERPRETER_OK;

/*
    const uint8_t program[] = {
0x23,
0x01,
0x00,
0x01,
0x23,
0x00,
0x03,
0x00,
0x71,
0x00,
0x40,
0x01,
0x23,
0x00,
0x02,
0x00,
0x71,
0x00,
0x40,
0x01,
0x23,
0x00,
0x01,
0x00,
0x71,
0x00,
0x40,
0x01,
0x74,
0x00,
0x04,
0x48,
0x69,
0x21,
0x00,
0x23,
0x10,
0x00,
0x00,
0x81,
0xff,
0x00,
0x00,
0x00,
0x40,
0x00,
0x80,
0x40,
0x00,
0x00,
0x23,
0x03,
0x00,
0x00,
0x23,
0x03,
0x00,
0x01,
0x91,
0x01,
0x40,
0x00,
0x90,
0x40,
0x00
    };
    memcpy(&interpreter.code[0], program, sizeof(program));

    const uint16_t methods[] = {
0x0000,
0x0023,
0x0032,
0xffff,
0xffff
    };
    memcpy(&interpreter.methods[0], methods, sizeof(methods));
*/

    // // test program
    // const uint8_t program[] = {
    //     0x21, 0x00, 0x01, 0x00,
    //     0x02,
    //     0x23, 0x00, 0x00, 0x00,
    //     0x72, 0x00,
    //     0x23, 0x00, 0x10, 0x00,
    //     0x40, 0x00,
    //     0x70,
    // };
    // memcpy(&interpreter.code[0], program, sizeof(program));

    // const uint16_t methods[] = {
    //     0xffff,
    //     0xffff,
    //     0x0000,
    //     0xffff,
    //     0xffff,
    // };
    // memcpy(&interpreter.methods[0], methods, sizeof(methods));

}

void interpreter_start()
{
    // uBit.messageBus.listen(MICROBIT_ID_ANY, MICROBIT_EVT_ANY, onEvent);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A,
        MICROBIT_BUTTON_EVT_CLICK,
        interpreter_on_button);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B,
        MICROBIT_BUTTON_EVT_CLICK,
        interpreter_on_button);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        interpreter_on_button);
        // MESSAGE_BUS_LISTENER_DROP_IF_BUSY);

    // MICROBIT_ACCELEROMETER_EVT_TILT_UP
    // MICROBIT_ACCELEROMETER_EVT_TILT_DOWN
    // MICROBIT_ACCELEROMETER_EVT_TILT_LEFT
    // MICROBIT_ACCELEROMETER_EVT_TILT_RIGHT
    // MICROBIT_ACCELEROMETER_EVT_FACE_UP
    // MICROBIT_ACCELEROMETER_EVT_FACE_DOWN
    // MICROBIT_ACCELEROMETER_EVT_FREEFALL
    // MICROBIT_ACCELEROMETER_EVT_3G
    // MICROBIT_ACCELEROMETER_EVT_6G
    // MICROBIT_ACCELEROMETER_EVT_8G
    // MICROBIT_ACCELEROMETER_EVT_SHAKE

    uBit.messageBus.listen(MICROBIT_ID_GESTURE,
        MICROBIT_ACCELEROMETER_EVT_SHAKE,
        interpreter_on_gesture);

    uBit.io.P12.isTouched();
    uBit.io.P0.isTouched();
    uBit.io.P1.isTouched();
    uBit.io.P16.isTouched();

    // uBit.messageBus.listen(MICROBIT_ID_IO_P12, MICROBIT_EVT_ANY, onButton0);
    // uBit.messageBus.listen(MICROBIT_ID_IO_P0, MICROBIT_EVT_ANY, onButton1);
    // uBit.messageBus.listen(MICROBIT_ID_IO_P1, MICROBIT_EVT_ANY, onButton2);
    // uBit.messageBus.listen(MICROBIT_ID_IO_P16, MICROBIT_EVT_ANY, onButton3);

    // Pin 0
    uBit.messageBus.listen(MICROBIT_ID_IO_P12,
        MICROBIT_EVT_ANY,
        interpreter_on_pin);

    // Pin 1
    uBit.messageBus.listen(MICROBIT_ID_IO_P0,
        MICROBIT_EVT_ANY,
        interpreter_on_pin);

    // Pin 2
    uBit.messageBus.listen(MICROBIT_ID_IO_P1,
        MICROBIT_EVT_ANY,
        interpreter_on_pin);

    // Pin 3
    uBit.messageBus.listen(MICROBIT_ID_IO_P16,
        MICROBIT_EVT_ANY,
        interpreter_on_pin);


    interpreter_reset();

    // new BluetoothServiceDebug(interpreter);
    new BluetoothServiceProgram(interpreter);
    notify = new BluetoothServiceNotify(interpreter);

    uBit.sleep(100);

    invoke(interpreter_fiber);
}

uint16_t interpreter_calculate_hash()
{
    return 0;
}
