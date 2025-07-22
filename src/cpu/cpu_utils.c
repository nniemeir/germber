#include <core/bus.h>
#include <cpu/cpu.h>

extern cpu_ctx cpu;

u16 reverse(u16 n) { return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8); }

u16 cpu_read_reg(reg_type rt) {
  switch (rt) {
  case RT_A:
    return cpu.regs.af.a;
  case RT_F:
    return cpu.regs.af.f;
  case RT_B:
    return cpu.regs.bc.b;
  case RT_C:
    return cpu.regs.bc.c;
  case RT_D:
    return cpu.regs.de.d;
  case RT_E:
    return cpu.regs.de.e;
  case RT_H:
    return cpu.regs.hl.h;
  case RT_L:
    return cpu.regs.hl.l;

  case RT_AF:
    return cpu.regs.af.af;
  case RT_BC:
    return cpu.regs.bc.bc;
  case RT_DE:
    return cpu.regs.de.de;
  case RT_HL:
    return cpu.regs.hl.hl;

  case RT_PC:
    return cpu.regs.pc;
  case RT_SP:
    return cpu.regs.sp;
  default:
    return 0;
  }
}

void cpu_set_reg(reg_type rt, u16 val) {
  switch (rt) {
  case RT_A:
    cpu.regs.af.a = val & 0xFF;
    break;
  case RT_F:
    cpu.regs.af.f = val & 0xFF;
    break;
  case RT_B:
    cpu.regs.bc.b = val & 0xFF;
    break;
  case RT_C: {
    cpu.regs.bc.c = val & 0xFF;
  } break;
  case RT_D:
    cpu.regs.de.d = val & 0xFF;
    break;
  case RT_E:
    cpu.regs.de.e = val & 0xFF;
    break;
  case RT_H:
    cpu.regs.hl.h = val & 0xFF;
    break;
  case RT_L:
    cpu.regs.hl.l = val & 0xFF;
    break;

  case RT_AF:
    cpu.regs.af.af = val;
    break;
  case RT_BC:
    cpu.regs.bc.bc = val;
    break;
  case RT_DE:
    cpu.regs.de.de = val;
    break;
  case RT_HL: {
    cpu.regs.hl.hl = val;
    break;
  }

  case RT_PC:
    cpu.regs.pc = val;
    break;
  case RT_SP:
    cpu.regs.sp = val;
    break;
  case RT_NONE:
    break;
  }
}

u8 cpu_read_reg8(reg_type rt) {
  switch (rt) {
  case RT_A:
    return cpu.regs.af.a;
  case RT_F:
    return cpu.regs.af.f;
  case RT_B:
    return cpu.regs.bc.b;
  case RT_C:
    return cpu.regs.bc.c;
  case RT_D:
    return cpu.regs.de.d;
  case RT_E:
    return cpu.regs.de.e;
  case RT_H:
    return cpu.regs.hl.h;
  case RT_L:
    return cpu.regs.hl.l;
  case RT_HL: {
    return bus_read(cpu_read_reg(RT_HL));
  }
  default:
    printf("**ERR INVALID REG8: %d\n", rt);
    NO_IMPL
  }
}

void cpu_set_reg8(reg_type rt, u8 val) {
  switch (rt) {
  case RT_A:
    cpu.regs.af.a = val & 0xFF;
    break;
  case RT_F:
    cpu.regs.af.f = val & 0xFF;
    break;
  case RT_B:
    cpu.regs.bc.b = val & 0xFF;
    break;
  case RT_C:
    cpu.regs.bc.c = val & 0xFF;
    break;
  case RT_D:
    cpu.regs.de.d = val & 0xFF;
    break;
  case RT_E:
    cpu.regs.de.e = val & 0xFF;
    break;
  case RT_H:
    cpu.regs.hl.h = val & 0xFF;
    break;
  case RT_L:
    cpu.regs.hl.l = val & 0xFF;
    break;
  case RT_HL:
    bus_write(cpu_read_reg(RT_HL), val);
    break;
  default:
    printf("**ERR INVALID REG8: %d\n", rt);
    NO_IMPL
  }
}

cpu_registers *cpu_get_regs(void) { return &cpu.regs; }
