#include "cpu.h"
#include "bus.h"
#include "emu.h"
#include "stdio.h"
#include <stdlib.h>

struct cpu_ctx cpu = {0};

void cpu_init(void) {
  cpu.regs.PC = 0x100;
  cpu.regs.AF.A = 0x01;
}

static int fetch_instruction(void) {
  cpu.current_opcode = bus_read(cpu.regs.PC++);

  cpu.current_instruction = instruction_by_opcode(cpu.current_opcode);
  if (!cpu.current_instruction) {
    printf("Unhandled opcode: 0x%02X at PC: 0x%04X\n", cpu.current_opcode,
           cpu.regs.PC - 1);
    return 1;
  }

  return 0;
}

static void fetch_data(void) {
  cpu.mem_dest = 0;
  cpu.dest_is_mem = false;

  switch (cpu.current_instruction->mode) {
  case AM_IMP:
    return;

  case AM_R:
    cpu.fetched_data = cpu_read_reg(cpu.current_instruction->reg_1);
    return;

  case AM_R_D8:
    cpu.fetched_data = bus_read(cpu.regs.PC);
    emu_cycles(1);
    cpu.regs.PC++;
    return;

  case AM_D16: {
    uint16_t lo = bus_read(cpu.regs.PC);
    emu_cycles(1);
    uint16_t hi = bus_read(cpu.regs.PC + 1);
    emu_cycles(1);
    cpu.fetched_data = lo | (hi << 8);
    cpu.regs.PC += 2;
    return;
  }

  default:
    printf("Unknown addressing mode %d\n", cpu.current_instruction->mode);
    exit(-7);
  }
}

static void execute(void) { return; }

bool cpu_step() {
  uint16_t pc = cpu.regs.PC;
  if (cpu.halted) {
    return false;
  }

  if (fetch_instruction() == 1) {
    return false;
  }

  fetch_data();

  printf("Executing Instruction: %02X    PC: %04X\n", cpu.current_opcode, pc);

  execute();

  return true;
}
