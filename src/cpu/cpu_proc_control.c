#include <core/bus.h>
#include <cpu/cpu.h>
#include <core/emu.h>
#include <core/stack.h>

void proc_halt(void) { get_cpu_ctx()->halted = true; }

void proc_jp(void) { goto_addr(get_cpu_ctx()->fetched_data, false); }

void proc_jr(void) {
  int8_t rel = (int8_t)(get_cpu_ctx()->fetched_data & 0xFF);
  u16 addr = get_cpu_ctx()->regs.pc + rel;
  goto_addr(addr, false);
}

void proc_call(void) { goto_addr(get_cpu_ctx()->fetched_data, true); }

void proc_rst(void) { goto_addr(get_cpu_ctx()->cur_inst->param, true); }

void proc_ret(void) {
  if (get_cpu_ctx()->cur_inst->cond != CT_NONE) {
    emu_cycles(1);
  }

  if (check_cond()) {
    u16 lo = stack_pop();
    emu_cycles(1);
    u16 hi = stack_pop();
    emu_cycles(1);

    u16 n = (hi << 8) | lo;
    get_cpu_ctx()->regs.pc = n;

    emu_cycles(1);
  }
}

void proc_reti(void) {
  get_cpu_ctx()->int_master_enabled = true;
  proc_ret();
}