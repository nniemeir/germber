#ifndef CPU_H
#define CPU_H

#include <common.h>
#include <cpu/instruction_types.h>
#include <cpu/reg_types.h>

#define CPU_DEBUG 0
#define CPU_FLAG_Z BIT(cpu_get_ctx()->regs.f, 7)
#define CPU_FLAG_N BIT(cpu_get_ctx()->regs.f, 6)
#define CPU_FLAG_H BIT(cpu_get_ctx()->regs.f, 5)
#define CPU_FLAG_C BIT(cpu_get_ctx()->regs.f, 4)

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

} cpu_ctx;

cpu_ctx *cpu_get_ctx(void);

// Core Logic
void cpu_init(void);
bool cpu_step(void);

// Fetch
void fetch_data(void);

// Register Access Utilities
u16 cpu_read_reg(reg_type rt);
void cpu_set_reg(reg_type rt, u16 val);

u8 cpu_read_reg8(reg_type rt);
void cpu_set_reg8(reg_type rt, u8 val);

cpu_registers *cpu_get_regs(void);

#endif
