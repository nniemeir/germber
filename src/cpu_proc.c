#include "cpu.h"
#include "emu.h"
#include "instructions.h"

#include <stdio.h>
#include <stdlib.h>

static void proc_none(void) {
  printf("INVALID INSTRUCTION\n");
  exit(-7);
}

static void proc_ld(void) {
  // TODO
}

void cpu_set_flags(char z, char n, char h, char c) {
  if (z != -1) {
    BIT_SET(cpu.regs.AF.F, 7, z);
  }
  if (n != -1) {
    BIT_SET(cpu.regs.AF.F, 6, n);
  }
  if (h != -1) {
    BIT_SET(cpu.regs.AF.F, 5, h);
  }
  if (c != -1) {
    BIT_SET(cpu.regs.AF.F, 4, c);
  }
}

static void proc_xor(void) {
  cpu.regs.AF.A ^= cpu.fetched_data & 0xFF;
  cpu_set_flags(cpu.regs.AF.A, 0, 0, 0);
}

static void proc_nop(void) {}

static bool check_cond(void) {
  bool z = CPU_FLAG_Z;
  bool c = CPU_FLAG_C;

  switch (cpu.current_instruction->cond) {
  case CT_NONE:
    return true;
  case CT_NZ:
    return !z;
  case CT_Z:
    return z;
  case CT_NC:
    return !c;
  case CT_C:
    return c;
  }
  return false;
}

static void proc_jp(void) {
  if (check_cond()) {
    cpu.regs.PC = cpu.fetched_data;
    emu_cycles(1);
  }
}

static void proc_di(void) { cpu.int_master_enabled = false; }

static IN_PROC processors[] = {
    [IN_NONE] = proc_none, [IN_NOP] = proc_nop, [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,     [IN_DI] = proc_di,   [IN_XOR] = proc_xor};

IN_PROC inst_get_processor(void) { return processors[cpu.current_instruction->type]; }
