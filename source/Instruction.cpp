#include <tgmath.h>
#include "Instruction.h"
#include "MicroBit.h"
#include "Images.h"
#include "BluetoothServiceNotify.h"

#define UNUSED __attribute__((unused))

extern MicroBit uBit;
extern BluetoothServiceNotify *notify;

typedef struct {
    InterpreterStatus error;
    int32_t *value;
} Register;

static Register register_read(Slice &code, RunState &state)
{
    Register result = {};

    if (slice_available(code) < 1) {
        result.error = INTERPRETER_KO_INSTRUCTION_INVALID;
        return result;
    }

    const uint8_t reg = slice_read8(code);

    if (reg >= REGISTER_COUNT) {
        result.error = INTERPRETER_KO_INSTRUCTION_INVALID;
        return result;
    }

    result.error = INTERPRETER_OK;
    result.value = &state.reg[reg];

    return result;
}

static void compare(int32_t a, int32_t b, RunState &state)
{
    state.cs =
        (a == b) ? COMPARED_EQ :
        ((a < b) ? COMPARED_LT : COMPARED_GT);
}

static void ret(Slice &code, Interpreter &interpreter UNUSED, RunState &state, bool comparison)
{
    state.pc = comparison ? state.stack : code.position;
    state.stack = code.stop;
}

static void bra(Slice &code, Interpreter &interpreter, RunState &state, bool comparison)
{
    if (slice_available(code) < 1) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    const uint8_t value = slice_read8(code);
    const int8_t offset = value;

    uBit.sleep(5);

    state.pc = comparison ? code.position + offset : code.position;
}

void instruction_ret(Slice &code, Interpreter &interpreter, RunState &state)
{
    ret(code, interpreter, state,
        true);
}

void instruction_req(Slice &code, Interpreter &interpreter, RunState &state)
{
    ret(code, interpreter, state,
        state.cs == COMPARED_EQ);
}

void instruction_rne(Slice &code, Interpreter &interpreter, RunState &state)
{
    ret(code, interpreter, state,
        state.cs != COMPARED_EQ);
}

void instruction_rgt(Slice &code, Interpreter &interpreter, RunState &state)
{
    ret(code, interpreter, state,
        state.cs == COMPARED_GT);
}

void instruction_rlt(Slice &code, Interpreter &interpreter, RunState &state)
{
    ret(code, interpreter, state,
        state.cs == COMPARED_LT);
}

void instruction_rge(Slice &code, Interpreter &interpreter, RunState &state)
{
    ret(code, interpreter, state,
        state.cs == COMPARED_GT ||
        state.cs == COMPARED_EQ);
}

void instruction_rle(Slice &code, Interpreter &interpreter, RunState &state)
{
    ret(code, interpreter, state,
        state.cs == COMPARED_LT ||
        state.cs == COMPARED_EQ);
}

void instruction_jsr(Slice &code, Interpreter &interpreter, RunState &state)
{
    if (slice_available(code) < 1) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    const uint8_t value = slice_read8(code);
    const int8_t offset = value;
    const uint16_t address = code.position + offset;

    // this should be push
    state.stack = code.position;

    state.pc = address;
}


void instruction_bra(Slice &code, Interpreter &interpreter, RunState &state)
{
    bra(code, interpreter, state,
        true);
}

void instruction_beq(Slice &code, Interpreter &interpreter, RunState &state)
{
    bra(code, interpreter, state,
        state.cs == COMPARED_EQ);
}

void instruction_bne(Slice &code, Interpreter &interpreter, RunState &state)
{
    bra(code, interpreter, state,
        state.cs != COMPARED_EQ);
}

void instruction_bgt(Slice &code, Interpreter &interpreter, RunState &state)
{
    bra(code, interpreter, state,
        state.cs == COMPARED_GT);
}

void instruction_blt(Slice &code, Interpreter &interpreter, RunState &state)
{
    bra(code, interpreter, state,
        state.cs == COMPARED_LT);
}

void instruction_bge(Slice &code, Interpreter &interpreter, RunState &state)
{
    bra(code, interpreter, state,
        state.cs == COMPARED_GT ||
        state.cs == COMPARED_EQ);
}

void instruction_ble(Slice &code, Interpreter &interpreter, RunState &state)
{
    bra(code, interpreter, state,
        state.cs == COMPARED_LT ||
        state.cs == COMPARED_EQ);
}

void instruction_bra16(Slice &code, Interpreter &interpreter, RunState &state)
{
    if (slice_available(code) < 2) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    const int16_t value = slice_read16(code);

    state.pc = code.position + value;
}




void instruction_cmp(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register ra = register_read(code, state);
    if (ra.error) {
        interpreter.status = ra.error;
        return;
    }

    Register rb = register_read(code, state);
    if (rb.error) {
        interpreter.status = rb.error;
        return;
    }

    compare(*ra.value, *rb.value, state);

    state.pc = code.position;
}

void instruction_cmpi(Slice &code, Interpreter &interpreter, RunState &state)
{
    if (slice_available(code) < 2) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    const int32_t a = slice_read16(code);

    Register rb = register_read(code, state);
    if (rb.error) {
        interpreter.status = rb.error;
        return;
    }

    compare(a, *rb.value, state);

    state.pc = code.position;
}

void instruction_mov(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register src = register_read(code, state);
    if (src.error) {
        interpreter.status = src.error;
        return;
    }

    Register dst = register_read(code, state);
    if (dst.error) {
        interpreter.status = dst.error;
        return;
    }

    const int32_t value = *src.value;

    *dst.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_movi(Slice &code, Interpreter &interpreter, RunState &state)
{
    if (slice_available(code) < 2) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    const int32_t value = slice_read16(code);

    Register dst = register_read(code, state);
    if (dst.error) {
        interpreter.status = dst.error;
        return;
    }

    *dst.value = value;

    compare(0, value, state);

    state.pc = code.position;
}


void instruction_add(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register src = register_read(code, state);
    if (src.error) {
        interpreter.status = src.error;
        return;
    }

    Register dst = register_read(code, state);
    if (dst.error) {
        interpreter.status = dst.error;
        return;
    }

    const int32_t value = *dst.value + *src.value;

    *dst.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_sub(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register src = register_read(code, state);
    if (src.error) {
        interpreter.status = src.error;
        return;
    }

    Register dst = register_read(code, state);
    if (dst.error) {
        interpreter.status = dst.error;
        return;
    }

    const int32_t value = *dst.value - *src.value;

    *dst.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_mul(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register src = register_read(code, state);
    if (src.error) {
        interpreter.status = src.error;
        return;
    }

    Register dst = register_read(code, state);
    if (dst.error) {
        interpreter.status = dst.error;
        return;
    }

    const int32_t value = *dst.value * *src.value;

    *dst.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_div(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register src = register_read(code, state);
    if (src.error) {
        interpreter.status = src.error;
        return;
    }

    Register dst = register_read(code, state);
    if (dst.error) {
        interpreter.status = dst.error;
        return;
    }

    if (*src.value == 0) {
        interpreter.status = INTERPRETER_KO_DIVISION_BY_ZERO;
        return;
    }

    const int32_t value = *dst.value / *src.value;

    *dst.value = value;

    compare(0, value, state);

    state.pc = code.position;
}


void instruction_sleep(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register time = register_read(code, state);
    if (time.error) {
        interpreter.status = time.error;
        return;
    }

    uBit.sleep(static_cast<uint32_t>(*time.value));

    state.pc = code.position;
}

void instruction_random(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const int32_t value = uBit.random(*r.value);

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_time(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const int32_t value = uBit.systemTime();

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}


void instruction_temperature(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const int32_t value = uBit.thermometer.getTemperature();

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_noise(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const int value = uBit.io.P21.getAnalogValue();

    if (value > 512) {
        // we do not support double and int32 should be enough
        const int32_t gauge = static_cast<const int32_t>((log2(value - 511) * 4) / 9);

        *r.value = gauge;
    } else {
        *r.value = 0;
    }

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_brightness(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const int32_t value = uBit.display.readLightLevel();

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}


void instruction_button(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    int32_t value;

    switch(*r.value) {
        case 0x01:
            value = uBit.buttonA.isPressed();
            break;
        case 0x02:
            value = uBit.buttonB.isPressed();
            break;
        case 0x03:
            value = uBit.buttonAB.isPressed();
            break;
        default:
            interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
            return;
    }

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

static uint8_t pins[] = { 12, 0, 1, 16 };

void instruction_pin(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const int32_t i = *r.value;
    if (i < 0 || static_cast<uint32_t>(i) >= sizeof(pins)) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    const int32_t value = uBit.io.pin[pins[i]].isTouched();

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}


void instruction_display_clear(Slice &code, Interpreter &interpreter UNUSED, RunState &state)
{
    uBit.display.clear();

    state.pc = code.position;
}

void instruction_display_show_number(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register number = register_read(code, state);
    if (number.error) {
        interpreter.status = number.error;
        return;
    }

    const int value = *number.value;

    uBit.display.print(ManagedString(value));

    state.pc = code.position;
}

void instruction_display_show_image(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register image = register_read(code, state);
    if (image.error) {
        interpreter.status = image.error;
        return;
    }

    const int32_t i = *image.value;
    if (static_cast<uint32_t>(i) > ImageCOUNTER) {

        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }
    if (i == ImageWave){
        uBit.display.scroll(*images((Image_t)(i)));
    }
    else
        uBit.display.print(*images((Image_t)(i)));

    state.pc = code.position;
}

void instruction_display_show_grid(Slice &code, Interpreter &interpreter, RunState &state)
{
    uint8_t pixels[25];

    if (slice_available(code) < sizeof(pixels)) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    for(unsigned int i=0; i<sizeof(pixels); i++) {
        pixels[i] = slice_read8(code);
    }
    const MicroBitImage Image(5, 5, pixels);

    uBit.display.print(Image);

    state.pc = code.position;
}

void instruction_display_show_text(Slice &code, Interpreter &interpreter, RunState &state)
{
    if (slice_available(code) < 2) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    const uint16_t len = slice_read16(code);

    if (len < 1) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    if (slice_available(code) < len) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    if ((code.position + len) > code.stop) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    // Important: The mind numbingly stupid implementation of ManagedString
    // requires a null terminated string (despite the given length).
    // So make sure to also send the 0 byte. For security reasons we force it.
    code.buffer[code.position+len-1] = 0;

    const ManagedString text = ManagedString((char*)&code.buffer[code.position]);

    uBit.display.scroll(ManagedString(text));

    state.pc = code.position + len;
}


void instruction_rgb_off(Slice &code, Interpreter &interpreter UNUSED, RunState &state)
{
    // if the rg.off is called to frequently it begins to flicker
    if (uBit.rgb.isOn()) {
        uBit.rgb.off();
    }

    state.pc = code.position;
}

void instruction_rgb_on(Slice &code, Interpreter &interpreter, RunState &state)
{
    if (slice_available(code) < 4) {
        interpreter.status = INTERPRETER_KO_INSTRUCTION_INVALID;
        return;
    }

    const uint8_t r = slice_read8(code);
    const uint8_t g = slice_read8(code);
    const uint8_t b = slice_read8(code);
    const uint8_t w = slice_read8(code);

    uBit.rgb.setColour(r, g, b, w);

    state.pc = code.position;
}


void instruction_sound_off(Slice &code, Interpreter &interpreter UNUSED, RunState &state)
{
    uBit.soundmotor.soundOff();

    state.pc = code.position;
}

void instruction_sound_on(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register freq = register_read(code, state);
    if (freq.error) {
        interpreter.status = freq.error;
        return;
    }

    uBit.soundmotor.soundOn(static_cast<uint16_t>(*freq.value));

    state.pc = code.position;
}


void instruction_gesture(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const uint16_t value = uBit.accelerometer.getGesture();

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_pitch(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const int32_t value = uBit.accelerometer.getPitch();

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_roll(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    const int32_t value = uBit.accelerometer.getRoll();

    *r.value = value;

    compare(0, value, state);

    state.pc = code.position;
}

void instruction_position(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register x = register_read(code, state);
    if (x.error) {
        interpreter.status = x.error;
        return;
    }
    Register y = register_read(code, state);
    if (y.error) {
        interpreter.status = y.error;
        return;
    }
    Register z = register_read(code, state);
    if (z.error) {
        interpreter.status = z.error;
        return;
    }

    *x.value = uBit.accelerometer.getX();
    *y.value = uBit.accelerometer.getY();
    *z.value = uBit.accelerometer.getZ();

    state.pc = code.position;
}


void instruction_notify(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register address = register_read(code, state);
    if (address.error) {
        interpreter.status = address.error;
        return;
    }

    Register value = register_read(code, state);
    if (value.error) {
        interpreter.status = value.error;
        return;
    }

    if (notify != nullptr) {
        // the protocol is allows only for the lower 16 bit
        notify->send(static_cast<uint16_t>(*address.value), static_cast<uint16_t>(*value.value));
    }

    state.pc = code.position;
}

void instruction_debug(Slice &code, Interpreter &interpreter, RunState &state)
{
    Register r = register_read(code, state);
    if (r.error) {
        interpreter.status = r.error;
        return;
    }

    uBit.serial.printf("debug: 0x%x\n\r", *r.value);

    state.pc = code.position;
}