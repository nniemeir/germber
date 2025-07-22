#include <core/bus.h>
#include <core/emu.h>
#include <core/stack.h>
#include <cpu/cpu.h>
#include <cpu/instructions.h>

// processes CPU instructions...

void cpu_set_flags(int8_t z, int8_t n, int8_t h, int8_t c) {
  if (z != -1) {
    BIT_SET(cpu_get_ctx()->regs.f, 7, z);
  }

  if (n != -1) {
    BIT_SET(cpu_get_ctx()->regs.f, 6, n);
  }

  if (h != -1) {
    BIT_SET(cpu_get_ctx()->regs.f, 5, h);
  }

  if (c != -1) {
    BIT_SET(cpu_get_ctx()->regs.f, 4, c);
  }
}

reg_type rt_lookup[] = {RT_B, RT_C, RT_D, RT_E, RT_H, RT_L, RT_HL, RT_A};

reg_type decode_reg(u8 reg) {
  if (reg > 0b111) {
    return RT_NONE;
  }

  return rt_lookup[reg];
}

bool is_16_bit(reg_type rt) { return rt >= RT_AF; }

bool check_cond(void) {
  bool z = CPU_FLAG_Z;
  bool c = CPU_FLAG_C;

  switch (cpu_get_ctx()->cur_inst->cond) {
  case CT_NONE:
    return true;
  case CT_C:
    return c;
  case CT_NC:
    return !c;
  case CT_Z:
    return z;
  case CT_NZ:
    return !z;
  }

  return false;
}

void goto_addr(u16 addr, bool pushpc) {
  if (check_cond()) {
    if (pushpc) {
      emu_cycles(2);
      stack_push16(cpu_get_ctx()->regs.pc);
    }

    cpu_get_ctx()->regs.pc = addr;
    emu_cycles(1);
  }
}

static IN_PROC processors[] = {
    [IN_NONE] = proc_none, [IN_NOP] = proc_nop,   [IN_LD] = proc_ld,
    [IN_LDH] = proc_ldh,   [IN_JP] = proc_jp,     [IN_DI] = proc_di,
    [IN_POP] = proc_pop,   [IN_PUSH] = proc_push, [IN_JR] = proc_jr,
    [IN_CALL] = proc_call, [IN_RET] = proc_ret,   [IN_RST] = proc_rst,
    [IN_DEC] = proc_dec,   [IN_INC] = proc_inc,   [IN_ADD] = proc_add,
    [IN_ADC] = proc_adc,   [IN_SUB] = proc_sub,   [IN_SBC] = proc_sbc,
    [IN_AND] = proc_and,   [IN_XOR] = proc_xor,   [IN_OR] = proc_or,
    [IN_CP] = proc_cp,     [IN_CB] = proc_cb,     [IN_RRCA] = proc_rrca,
    [IN_RLCA] = proc_rlca, [IN_RRA] = proc_rra,   [IN_RLA] = proc_rla,
    [IN_STOP] = proc_stop, [IN_HALT] = proc_halt, [IN_DAA] = proc_daa,
    [IN_CPL] = proc_cpl,   [IN_SCF] = proc_scf,   [IN_CCF] = proc_ccf,
    [IN_EI] = proc_ei,     [IN_RETI] = proc_reti};

IN_PROC inst_get_processor(in_type type) { return processors[type]; }
