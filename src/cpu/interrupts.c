#include <cpu/cpu.h>
#include <cpu/interrupts.h>
#include <core/stack.h>

void int_handle(cpu_context *cpu, u16 address) {
  stack_push16(cpu->regs.pc);
  cpu->regs.pc = address;
}

bool int_check(cpu_context *cpu, u16 address, interrupt_type it) {
  if (cpu->int_flags & it && cpu->ie_register & it) {
    int_handle(cpu, address);
    cpu->int_flags &= ~it;
    cpu->halted = false;
    cpu->int_master_enabled = false;

    return true;
  }

  return false;
}

void cpu_handle_interrupts(cpu_context *cpu) {
  if (int_check(cpu, 0x40, IT_VBLANK)) {

  } else if (int_check(cpu, 0x48, IT_LCD_STAT)) {

  } else if (int_check(cpu, 0x50, IT_TIMER)) {

  } else if (int_check(cpu, 0x58, IT_SERIAL)) {

  } else if (int_check(cpu, 0x60, IT_JOYPAD)) {
  }
}
