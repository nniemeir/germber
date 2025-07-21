#ifndef CPU_H
#define CPU_H

#include <common.h>
#include <cpu/instructions.h>

typedef struct {
  u8 a;
  u8 f;
  u8 b;
  u8 c;
  u8 d;
  u8 e;
  u8 h;
  u8 l;
  u16 pc;
  u16 sp;
} cpu_registers;

typedef struct {
  cpu_registers regs;

  // current fetch...
  u16 fetched_data;
  u16 mem_dest;
  bool dest_is_mem;
  u8 cur_opcode;
  instruction *cur_inst;

  bool halted;
  bool stepping;

  bool int_master_enabled;
  bool enabling_ime;
  u8 ie_register;
  u8 int_flags;

} cpu_context;

cpu_context *get_cpu_ctx(void);

cpu_registers *cpu_get_regs(void);

void cpu_init(void);
bool cpu_step(void);

typedef void (*IN_PROC)(void);

IN_PROC inst_get_processor(in_type type);

#define CPU_FLAG_Z BIT(get_cpu_ctx()->regs.f, 7)
#define CPU_FLAG_N BIT(get_cpu_ctx()->regs.f, 6)
#define CPU_FLAG_H BIT(get_cpu_ctx()->regs.f, 5)
#define CPU_FLAG_C BIT(get_cpu_ctx()->regs.f, 4)

void cpu_set_flags(int8_t z, int8_t n, int8_t h, int8_t c);

u16 cpu_read_reg(reg_type rt);
void cpu_set_reg(reg_type rt, u16 val);

u8 cpu_get_ie_register(void);
void cpu_set_ie_register(u8 n);

u8 cpu_read_reg8(reg_type rt);
void cpu_set_reg8(reg_type rt, u8 val);

u8 cpu_get_int_flags(void);
void cpu_set_int_flags(u8 value);

void inst_to_str(cpu_context *ctx, char *str);

void goto_addr(u16 addr, bool pushpc);
bool check_cond(void);
bool is_16_bit(reg_type rt);
reg_type decode_reg(u8 reg);

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
