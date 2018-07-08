#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdint.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 0

#define METHODS_COUNT 5
#define CODE_LEN 256
#define REGISTER_COUNT 5
#define CHARACTERISTICS_BUFFER_LEN 40

//#define DEBUG

#ifdef DEBUG
#define LOG(format, ...) uBit.serial.printf(format, ##__VA_ARGS__)
#else
#define LOG(format, ...) ;
#endif

typedef enum {
    INTERPRETER_OK = 0x00,
    INTERPRETER_RX = 0x01,
    INTERPRETER_RD = 0x02,

    INTERPRETER_KO_LEN_TOO_SHORT = 0x03,
    INTERPRETER_KO_HASH_INVALID = 0x04,
    INTERPRETER_KO_METHODS_TOO_SHORT = 0x05,
    INTERPRETER_KO_METHODS_START_OUT_OF_BOUNDS = 0x06,
    INTERPRETER_KO_METHODS_STOP_OUT_OF_BOUNDS = 0x07,
    INTERPRETER_KO_CODE_TOO_SHORT = 0x08,
    INTERPRETER_KO_CODE_START_OUT_OF_BOUNDS = 0x09,
    INTERPRETER_KO_CODE_STOP_OUT_OF_BOUNDS = 0x0a,
    INTERPRETER_KO_INSTRUCTION_UNKNOWN = 0x0b,
    INTERPRETER_KO_INSTRUCTION_INVALID = 0x0c,
    INTERPRETER_KO_DIVISION_BY_ZERO = 0x0d

} InterpreterStatus;

typedef struct {
    InterpreterStatus status;
    uint8_t code[CODE_LEN];
    uint16_t methods[METHODS_COUNT];
} Interpreter;

typedef enum {
    COMPARED_EQ = 0,
    COMPARED_GT = 1,
    COMPARED_LT = 2
} CompareState;

typedef struct {
    uint16_t pc;
    uint16_t stack; // this should be a stack, for now one address is enough
    int32_t reg[REGISTER_COUNT];
    CompareState cs;
} RunState;

typedef enum {
    METHOD_STARTUP = 0,
    METHOD_FOREVER = 1,
    METHOD_ON_BUTTON = 2,
    METHOD_ON_PIN = 3,
    METHOD_ON_GESTURE = 4
} InterpreterMethod;

#define METHOD_UNUSED 0xffff

typedef enum {
    INS_RET = 0x00,
    INS_REQ = 0x01,
    INS_RNE = 0x02,
    INS_RGT = 0x03,
    INS_RLT = 0x04,
    INS_RGE = 0x05,
    INS_RLE = 0x06,

    INS_JSR = 0x07,

    INS_BRA = 0x10,
    INS_BEQ = 0x11,
    INS_BNE = 0x12,
    INS_BGT = 0x13,
    INS_BLT = 0x14,
    INS_BGE = 0x15,
    INS_BLE = 0x16,
    INS_BRA16 = 0x17,

    INS_CMP = 0x20,
    INS_CMPI = 0x21,
    INS_MOV = 0x22,
    INS_MOVI = 0x23,

    INS_ADD = 0x30,
    INS_SUB = 0x31,
    INS_MUL = 0x32,
    INS_DIV = 0x33,

    INS_SLEEP = 0x40,
    INS_RANDOM = 0x41,
    INS_TIME = 0x42,

    INS_TEMPERATURE = 0x50,
    INS_NOISE = 0x51,
    INS_BRIGHTNESS = 0x52,

    INS_BUTTON = 0x60,
    INS_PIN = 0x61,

    INS_DISPLAY_CLEAR = 0x70,
    INS_DISPLAY_SHOW_NUMBER = 0x71,
    INS_DISPLAY_SHOW_IMAGE = 0x72,
    INS_DISPLAY_SHOW_GRID = 0x73,
    INS_DISPLAY_SHOW_TEXT = 0x74,

    INS_RGB_OFF = 0x80,
    INS_RGB_ON = 0x81,

    INS_SOUND_OFF = 0x90,
    INS_SOUND_ON = 0x91,

    INS_ACC_GESTURE = 0xa0,
    INS_ACC_PITCH = 0xa1,
    INS_ACC_ROLL = 0xa2,
    INS_ACC_POSITION = 0xa3,

    INS_NOTIFY = 0xf0,
    INS_DEBUG = 0xf1

} InterpreterInstruction;

void interpreter_run();
void interpreter_start();
void interpreter_reset();
uint16_t interpreter_calculate_hash();

#endif // INTERPRETER_H