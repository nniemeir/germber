#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu/cpu.h"
#include <common.h>
#include <cpu/instruction_types.h>
#include <cpu/reg_types.h>

#define MAX_INST_STR_LEN 64

typedef cpu_ctx cpu_ctx;

extern instruction instructions[0x100];

instruction *instruction_by_opcode(u8 opcode);

char *inst_name(in_type t);

// Instruction Formatting
void inst_to_str(cpu_ctx *ctx, char *str);

// Instruction Processing
void cpu_set_flags(int8_t z, int8_t n, int8_t h, int8_t c);
reg_type decode_reg(u8 reg);
bool is_16_bit(reg_type rt);
bool check_cond(void);
void goto_addr(u16 addr, bool pushpc);
IN_PROC inst_get_processor(in_type type);

// Instruction Implementations
void proc_none(void);
void proc_nop(void);
void proc_ld(void);
void proc_ldh(void);
void proc_jp(void);
void proc_di(void);
void proc_pop(void);
void proc_push(void);
void proc_jr(void);
void proc_call(void);
void proc_ret(void);
void proc_rst(void);
void proc_dec(void);
void proc_inc(void);
void proc_add(void);
void proc_adc(void);
void proc_sub(void);
void proc_sbc(void);
void proc_and(void);
void proc_xor(void);
void proc_or(void);
void proc_cp(void);
void proc_cb(void);
void proc_rrca(void);
void proc_rlca(void);
void proc_rra(void);
void proc_rla(void);
void proc_stop(void);
void proc_halt(void);
void proc_daa(void);
void proc_cpl(void);
void proc_scf(void);
void proc_ccf(void);
void proc_ei(void);
void proc_reti(void);

#endif
