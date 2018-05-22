#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Slice.h"
#include "Interpreter.h"


void instruction_ret(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_req(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_rne(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_rgt(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_rlt(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_rge(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_rle(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_jsr(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_bra(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_beq(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_bne(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_bgt(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_blt(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_bge(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_ble(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_bra16(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_cmp(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_cmpi(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_mov(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_movi(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_add(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_sub(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_mul(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_div(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_sleep(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_random(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_time(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_temperature(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_noise(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_brightness(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_button(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_pin(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_display_clear(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_display_show_number(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_display_show_image(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_display_show_grid(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_display_show_text(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_rgb_off(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_rgb_on(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_sound_off(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_sound_on(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_gesture(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_pitch(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_roll(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_position(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_notify(Slice &code, Interpreter &interpreter, RunState &state);
void instruction_debug(Slice &code, Interpreter &interpreter, RunState &state);

#endif // INSTRUCTION_H