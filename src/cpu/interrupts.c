#include <core/stack.h>
#include <cpu/cpu.h>
#include <cpu/interrupts.h>

u8 cpu_get_ie_register(void) { return cpu_get_ctx()->ie_register; }

void cpu_set_ie_register(u8 n) { cpu_get_ctx()->ie_register = n; }

u8 cpu_get_int_flags(void) { return cpu_get_ctx()->int_flags; }

void cpu_set_int_flags(u8 value) { cpu_get_ctx()->int_flags = value; }

void int_handle(cpu_ctx *cpu, u16 address) {
  stack_push16(cpu->regs.pc);
  cpu->regs.pc = address;
}

bool int_check(cpu_ctx *cpu, u16 address, interrupt_type it) {
  if (cpu->int_flags & it && cpu->ie_register & it) {
    int_handle(cpu, address);
    cpu->int_flags &= ~it;
    cpu->halted = false;
    cpu->int_master_enabled = false;

    return true;
  }

  return false;
}

void cpu_handle_interrupts(cpu_ctx *cpu) {
  if (int_check(cpu, 0x40, IT_VBLANK)) {

  } else if (int_check(cpu, 0x48, IT_LCD_STAT)) {

  } else if (int_check(cpu, 0x50, IT_TIMER)) {

  } else if (int_check(cpu, 0x58, IT_SERIAL)) {

  } else if (int_check(cpu, 0x60, IT_JOYPAD)) {
  }
}

void cpu_request_interrupt(interrupt_type t) { cpu_get_ctx()->int_flags |= t; }
