#include <core/bus.h>
#include <core/emu.h>
#include <core/stack.h>
#include <cpu/cpu.h>
#include <cpu/instructions.h>

void proc_halt(void) { cpu_get_ctx()->halted = true; }

void proc_jp(void) { goto_addr(cpu_get_ctx()->fetched_data, false); }

void proc_jr(void) {
  cpu_ctx *cpu = cpu_get_ctx();
  int8_t rel = (int8_t)(cpu->fetched_data & 0xFF);
  u16 addr = cpu->regs.pc + rel;
  goto_addr(addr, false);
}

void proc_call(void) { goto_addr(cpu_get_ctx()->fetched_data, true); }

void proc_rst(void) { goto_addr(cpu_get_ctx()->cur_inst->param, true); }

void proc_ret(void) {
  cpu_ctx *cpu = cpu_get_ctx();
  if (cpu->cur_inst->cond != CT_NONE) {
    emu_cycles(1);
  }

  if (check_cond()) {
    u16 lo = stack_pop();
    emu_cycles(1);
    u16 hi = stack_pop();
    emu_cycles(1);

    u16 n = (hi << 8) | lo;
    cpu->regs.pc = n;

    emu_cycles(1);
  }
}

void proc_reti(void) {
  cpu_get_ctx()->int_master_enabled = true;
  proc_ret();
}
