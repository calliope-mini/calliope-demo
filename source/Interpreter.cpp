/*!
 * @file Interpreter.cpp
 *
 * Interpreter functionality for Playground APP.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt		<https://github.com/tcurdt>
 * @author Waldemar Gruenwald 	<https://github.com/gruenwaldi>
 */

#include "Interpreter.h"
#include "MicroBit.h"
#include "BluetoothServiceProgram.h"
#include "BluetoothServiceNotify.h"
#include "Instruction.h"
#include "CalliopeServiceMaster.h"
#include "Utils.h"

extern MicroBit uBit;

extern Interpreter *interpreter;
extern CalliopeServiceMaster *masterService;

static uint16_t find_stop(InterpreterMethod method)
{
	for(uint8_t i = method + 1; i < METHODS_COUNT; i++) {
		uint16_t stop = interpreter->methods[i];
		if (stop != METHOD_UNUSED) {
			return stop;
		}
	}
	return CODE_LEN;
}

//extern void microbit_heap_print();
#ifdef DEBUG_MASTER
static uint8_t running = 0;
#endif
static void interpreter_run_method(InterpreterMethod method, int32_t r0 = 0, int32_t r1 = 0, int32_t r2 = 0)
{
	//microbit_heap_print();

	uint16_t start = interpreter->methods[method];

	if (start == METHOD_UNUSED) {
		return;
	}

	uint16_t stop = find_stop(method);

#ifdef DEBUG_MASTER
	running += 1;
	uint8_t id = running;
#endif

	// initialize interpreter state
	RunState state = {};
	state.pc = start;
	state.cs = COMPARED_EQ;
	state.stack = stop;
	memset(state.reg, 0, REGISTER_COUNT * sizeof(state.reg[0]));
	state.reg[0] = r0;
	state.reg[1] = r1;
	state.reg[2] = r2;

	Slice code = slice_create(interpreter->code, start, stop);

	// LOG("%d: run 0x%x-0x%x start\n\r", id, start, stop);

	while(interpreter->status == INTERPRETER_OK) {

		uint8_t instruction = slice_read8(code);

//         LOG("%d: @0x%x ins 0x%x\n\r", id, code.position-1, instruction);

		switch(instruction) {
			case INS_RET:
				instruction_ret(code, *interpreter, state);
				break;
			case INS_REQ:
				instruction_req(code, *interpreter, state);
				break;
			case INS_RNE:
				instruction_rne(code, *interpreter, state);
				break;
			case INS_RGT:
				instruction_rgt(code, *interpreter, state);
				break;
			case INS_RLT:
				instruction_rlt(code, *interpreter, state);
				break;
			case INS_RGE:
				instruction_rge(code, *interpreter, state);
				break;
			case INS_RLE:
				instruction_rle(code, *interpreter, state);
				break;

			case INS_JSR:
				instruction_jsr(code, *interpreter, state);
				break;

			case INS_BRA:
				instruction_bra(code, *interpreter, state);
				break;
			case INS_BEQ:
				instruction_beq(code, *interpreter, state);
				break;
			case INS_BNE:
				instruction_bne(code, *interpreter, state);
				break;
			case INS_BGT:
				instruction_bgt(code, *interpreter, state);
				break;
			case INS_BLT:
				instruction_blt(code, *interpreter, state);
				break;
			case INS_BGE:
				instruction_bge(code, *interpreter, state);
				break;
			case INS_BLE:
				instruction_ble(code, *interpreter, state);
				break;
			case INS_BRA16:
				instruction_bra16(code, *interpreter, state);
				break;


			case INS_CMP:
				instruction_cmp(code, *interpreter, state);
				break;
			case INS_CMPI:
				instruction_cmpi(code, *interpreter, state);
				break;
			case INS_MOV:
				instruction_mov(code, *interpreter, state);
				break;
			case INS_MOVI:
				instruction_movi(code, *interpreter, state);
				break;

			case INS_ADD:
				instruction_add(code, *interpreter, state);
				break;
			case INS_SUB:
				instruction_sub(code, *interpreter, state);
				break;
			case INS_MUL:
				instruction_mul(code, *interpreter, state);
				break;
			case INS_DIV:
				instruction_div(code, *interpreter, state);
				break;

			case INS_SLEEP:
				instruction_sleep(code, *interpreter, state);
				break;
			case INS_RANDOM:
				instruction_random(code, *interpreter, state);
				break;
			case INS_TIME:
				instruction_time(code, *interpreter, state);
				break;

			case INS_TEMPERATURE:
				instruction_temperature(code, *interpreter, state);
				break;
			case INS_NOISE:
				instruction_noise(code, *interpreter, state);
				break;
			case INS_BRIGHTNESS:
				instruction_brightness(code, *interpreter, state);
				break;

			case INS_BUTTON:
				instruction_button(code, *interpreter, state);
				break;
			case INS_PIN:
				instruction_pin(code, *interpreter, state);
				break;

			case INS_DISPLAY_CLEAR:
				instruction_display_clear(code, *interpreter, state);
				break;
			case INS_DISPLAY_SHOW_NUMBER:
				instruction_display_show_number(code, *interpreter, state);
				break;
			case INS_DISPLAY_SHOW_IMAGE:
				instruction_display_show_image(code, *interpreter, state);
				break;
			case INS_DISPLAY_SHOW_GRID:
				instruction_display_show_grid(code, *interpreter, state);
				break;
			case INS_DISPLAY_SHOW_TEXT:
				instruction_display_show_text(code, *interpreter, state);
				break;

			case INS_RGB_OFF:
				instruction_rgb_off(code, *interpreter, state);
				break;
			case INS_RGB_ON:
				instruction_rgb_on(code, *interpreter, state);
				break;

			case INS_SOUND_OFF:
				instruction_sound_off(code, *interpreter, state);
				break;
			case INS_SOUND_ON:
				instruction_sound_on(code, *interpreter, state);
				break;

			case INS_ACC_GESTURE:
				instruction_gesture(code, *interpreter, state);
				break;
			case INS_ACC_PITCH:
				instruction_pitch(code, *interpreter, state);
				break;
			case INS_ACC_ROLL:
				instruction_roll(code, *interpreter, state);
				break;
			case INS_ACC_POSITION:
				instruction_position(code, *interpreter, state);
				break;

			case INS_NOTIFY:
				instruction_notify(code, *interpreter, state);
				break;
			case INS_DEBUG:
				instruction_debug(code, *interpreter, state);
				break;

			default:
				interpreter->status = INTERPRETER_KO_INSTRUCTION_UNKNOWN;
		}

		code.position = state.pc;

		if (slice_available(code) < 1) { LOG("%d: run 0x%x-0x%x stop\n\r", id, start, stop);
			return;
		}
	}
}

static void interpreter_reset_hardware()
{
	uBit.rgb.off();
	uBit.display.clear();
	uBit.soundmotor.soundOff();
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

static void interpreter_on_event(MicroBitEvent event)
{
	uint16_t source;
	uint16_t value;
	InterpreterMethod method;
	switch(event.source) {
		// check the BUTTONs
		case MICROBIT_ID_BUTTON_A: // = 1
			source = 1;
			if (event.value == MICROBIT_BUTTON_EVT_DOWN) // = 3
				value = event.value;
			else
				return;
			method = METHOD_ON_BUTTON; // = 2
			LOG("button 0x%x 0x%x start\n\r", source, event.value);
			break;

		case MICROBIT_ID_BUTTON_B: // = 2
			source = 2;
			if (event.value == MICROBIT_BUTTON_EVT_DOWN) // = 3
				value = event.value;
			else
				return;
			method = METHOD_ON_BUTTON; // = 2
			LOG("button 0x%x 0x%x start\n\r", source, event.value);
			break;

		case MICROBIT_ID_BUTTON_AB: // = 26
			source = 3;
			if (event.value == MICROBIT_BUTTON_EVT_DOWN) // = 3
				value = event.value;
			else
				return;
			method = METHOD_ON_BUTTON; // = 2
			LOG("button 0x%x 0x%x start\n\r", source, event.value);
			break;

			// check the TOUCH-PINs
		case MICROBIT_ID_IO_P12: // = 19
			source = 0;
			value =event.value;
			method = METHOD_ON_PIN; // = 3
			LOG("pin 0x%x 0x%x start\n\r", source, event.value);
			break;

		case MICROBIT_ID_IO_P0: // = 7
			source = 1;
			value =event.value;
			method = METHOD_ON_PIN; // = 3
			LOG("pin 0x%x 0x%x start\n\r", source, event.value);
			break;

		case MICROBIT_ID_IO_P1: // = 8
			source = 2;
			value =event.value;
			method = METHOD_ON_PIN; // = 3
			LOG("pin 0x%x 0x%x start\n\r", source, event.value);
			break;

		case MICROBIT_ID_IO_P16: // = 23
			source = 3;
			value =event.value;
			method = METHOD_ON_PIN; // = 3
			LOG("pin 0x%x 0x%x start\n\r", source, event.value);
			break;

		case MICROBIT_ID_GESTURE: // = 27
			source = event.source;LOG("gesture 0x%x 0x%x start\n\r", event.source, event.value);
			if (event.value == MICROBIT_ACCELEROMETER_EVT_SHAKE) // = 11
				value = event.value;
			else
				return;
			method = METHOD_ON_GESTURE; // = 4
			break;


		default: return;
	}

	uBit.sleep(10);
	interpreter_run_method(method, source, value);
	uBit.sleep(10);

	LOG("int 0x%x 0x%x stop\n\r", source, event.value);
}

static void interpreter_on_pin(MicroBitEvent event) {
	uint8_t source;
	switch (event.source) {
		case MICROBIT_ID_IO_P12:
			source = 0;
			break;
		case MICROBIT_ID_IO_P0:
			source = 1;
			break;
		case MICROBIT_ID_IO_P1:
			source = 2;
			break;
		case MICROBIT_ID_IO_P16:
			source = 3;
			break;
		default:
			return;
	}

	LOG("PPpin 0x%x 0x%x start\n\r", source, event.value);

	interpreter_run_method(METHOD_ON_PIN, source, event.value);

	LOG("PPpin 0x%x 0x%x stop\n\r", source, event.value);
}

static bool nothingToRun()
{
	for(uint8_t i = 0; i < METHODS_COUNT; i++) {
		uint16_t method = interpreter->methods[i];
		if (method != METHOD_UNUSED) {
			return false;
		}
	}
	return true;
}

static void interpreterCheckMaster(int i) {
	LOG("\r\ninterpreter checking master %d\r\n\r\n", i);
	uint32_t buffer;
	masterService->getStatus(&buffer);
	if (!(buffer & (CALLIOPE_SERVICE_FLAG_NOTIFY | CALLIOPE_SERVICE_FLAG_PROGRAM))) {
		buffer |= CALLIOPE_SERVICE_FLAG_RESET;LOG("status changed:%08x\r\n", buffer);
		uBit.storage.put("MasterStatus", (uint8_t *) &buffer, 4);LOG("storing ok\r\n");
		uBit.reset();
	}
}

static void interpreter_fiber()
{
	LOG("interpreter\n\r");

	while (nothingToRun()) {
		uBit.sleep(100);  // hier ist das problem
		if (interpreter->status == INTERPRETER_MASTER_RX) {
			interpreterCheckMaster(1);
		}
	}

	while (true) {
		interpreter_reset_hardware();


		LOG("start\n\r");

		interpreter_run_method(METHOD_STARTUP);

		LOG("forever\n\r");

		while(interpreter->status == INTERPRETER_OK) {
			interpreter_run_method(METHOD_FOREVER);
			uBit.sleep(10);
			if (interpreter->status == INTERPRETER_MASTER_RX) {
				interpreterCheckMaster(2);
			}
		}

		if (interpreter->status == INTERPRETER_MASTER_RX) {
			interpreterCheckMaster(3);
		}


		LOG("rx\n\r");

		while(interpreter->status != INTERPRETER_RD) {
			uBit.sleep(10);
		}

		interpreter->status = INTERPRETER_OK;

		LOG("reset\n\r");

		interpreter_startup_sound();
	}
}

void interpreter_reset()
{
	memset(interpreter->code, 0, CODE_LEN * sizeof(interpreter->code[0]));
	memset(interpreter->methods, METHOD_UNUSED, METHODS_COUNT * sizeof(interpreter->methods[0]));
	interpreter->status = INTERPRETER_OK;
}


static void interpreter_init()
{
	uBit.io.P12.isTouched();
	uBit.io.P0.isTouched();
	uBit.io.P1.isTouched();
	uBit.io.P16.isTouched();

	uBit.messageBus.listen(MICROBIT_ID_ANY, MICROBIT_EVT_ANY, interpreter_on_event);

	showNameHistogram(uBit.display);

	interpreter_reset();

	uBit.sleep(200);
}

void interpreter_run()
{
	interpreter_init();
	interpreter_fiber();
}

void interpreter_start()
{
	interpreter_init();
	invoke(interpreter_fiber);
}

uint16_t interpreter_calculate_hash()
{
	return 0;
}
