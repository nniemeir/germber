#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "instructions.h"

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)

union a_f{
  struct {
    uint8_t A;
    uint8_t F;
  };
  uint16_t AF;
};

union b_c{
  struct {
    uint8_t B;
    uint8_t C;
  };
  uint16_t BC;
};

union d_e{
  struct {
    uint8_t D;
    uint8_t E;
  };
  uint16_t DE;
};

union h_l{
  struct {
    uint8_t H;
    uint8_t L;
  };
  uint16_t HL;
};

struct registers {
    union a_f AF;
    union b_c BC;
    union d_e DE;
    union h_l HL;
    uint16_t SP;
    uint16_t PC;
};

struct cpu_ctx {
  struct registers regs;
  struct instruction *current_instruction;
  uint16_t fetched_data;
  uint16_t mem_dest;
  bool dest_is_mem;
  uint8_t current_opcode;
  bool halted;
  bool stepping;
  bool int_master_enabled;
};

extern struct cpu_ctx cpu;

uint16_t cpu_read_reg(reg_type rt);
bool cpu_step(void);
void cpu_init(void);

typedef void (*IN_PROC)(void);

IN_PROC inst_get_processor(void);

#define CPU_FLAG_Z BIT(cpu.regs.AF.F, 7)
#define CPU_FLAG_C BIT(cpu.regs.AF.F, 4)
#define BIT_SET(a, n, on) { if (on) a |= (1 << n); else a &= ~(1 << n);}


#endif
