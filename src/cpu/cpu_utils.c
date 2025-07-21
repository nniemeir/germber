#include <core/bus.h>
#include <cpu/cpu.h>

extern cpu_context cpu;

u16 reverse(u16 n) { return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8); }

u16 cpu_read_reg(reg_type rt) {
  switch (rt) {
  case RT_A:
    return cpu.regs.a;
  case RT_F:
    return cpu.regs.f;
  case RT_B:
    return cpu.regs.b;
  case RT_C:
    return cpu.regs.c;
  case RT_D:
    return cpu.regs.d;
  case RT_E:
    return cpu.regs.e;
  case RT_H:
    return cpu.regs.h;
  case RT_L:
    return cpu.regs.l;

  case RT_AF:
    return reverse(*((u16 *)&cpu.regs.a));
  case RT_BC:
    return reverse(*((u16 *)&cpu.regs.b));
  case RT_DE:
    return reverse(*((u16 *)&cpu.regs.d));
  case RT_HL:
    return reverse(*((u16 *)&cpu.regs.h));

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
    cpu.regs.a = val & 0xFF;
    break;
  case RT_F:
    cpu.regs.f = val & 0xFF;
    break;
  case RT_B:
    cpu.regs.b = val & 0xFF;
    break;
  case RT_C: {
    cpu.regs.c = val & 0xFF;
  } break;
  case RT_D:
    cpu.regs.d = val & 0xFF;
    break;
  case RT_E:
    cpu.regs.e = val & 0xFF;
    break;
  case RT_H:
    cpu.regs.h = val & 0xFF;
    break;
  case RT_L:
    cpu.regs.l = val & 0xFF;
    break;

  case RT_AF:
    *((u16 *)&cpu.regs.a) = reverse(val);
    break;
  case RT_BC:
    *((u16 *)&cpu.regs.b) = reverse(val);
    break;
  case RT_DE:
    *((u16 *)&cpu.regs.d) = reverse(val);
    break;
  case RT_HL: {
    *((u16 *)&cpu.regs.h) = reverse(val);
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
    return cpu.regs.a;
  case RT_F:
    return cpu.regs.f;
  case RT_B:
    return cpu.regs.b;
  case RT_C:
    return cpu.regs.c;
  case RT_D:
    return cpu.regs.d;
  case RT_E:
    return cpu.regs.e;
  case RT_H:
    return cpu.regs.h;
  case RT_L:
    return cpu.regs.l;
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
    cpu.regs.a = val & 0xFF;
    break;
  case RT_F:
    cpu.regs.f = val & 0xFF;
    break;
  case RT_B:
    cpu.regs.b = val & 0xFF;
    break;
  case RT_C:
    cpu.regs.c = val & 0xFF;
    break;
  case RT_D:
    cpu.regs.d = val & 0xFF;
    break;
  case RT_E:
    cpu.regs.e = val & 0xFF;
    break;
  case RT_H:
    cpu.regs.h = val & 0xFF;
    break;
  case RT_L:
    cpu.regs.l = val & 0xFF;
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

u8 cpu_get_int_flags(void) { return cpu.int_flags; }

void cpu_set_int_flags(u8 value) { cpu.int_flags = value; }
