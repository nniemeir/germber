#include <core/bus.h>
#include <core/emu.h>
#include <core/stack.h>
#include <cpu/cpu.h>
#include <cpu/instructions.h>

void proc_none(void) {
  printf("INVALID INSTRUCTION!\n");
  cleanup();
  exit(EXIT_FAILURE);
}

void proc_nop(void) {}

void proc_cpl(void) {
  cpu_ctx *cpu = cpu_get_ctx();
  cpu->regs.af.a = ~cpu->regs.af.a;
  cpu_set_flags(-1, 1, 1, -1);
}

void proc_scf(void) { cpu_set_flags(-1, 0, 0, 1); }

void proc_ccf(void) { cpu_set_flags(-1, 0, 0, CPU_FLAG_C ^ 1); }
