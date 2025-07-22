#include "cpu/instructions.h"
#include <core/bus.h>
#include <core/dbg.h>
#include <core/emu.h>
#include <cpu/cpu.h>
#include <cpu/interrupts.h>
#include <io/timer.h>

cpu_ctx cpu = {0};

cpu_ctx *cpu_get_ctx(void) { return &cpu; }

void cpu_init(void) {
  cpu.regs.pc = 0x100;
  cpu.regs.sp = 0xFFFE;
  *((short *)&cpu.regs.a) = 0xB001;
  *((short *)&cpu.regs.b) = 0x1300;
  *((short *)&cpu.regs.d) = 0xD800;
  *((short *)&cpu.regs.h) = 0x4D01;
  cpu.ie_register = 0;
  cpu.int_flags = 0;
  cpu.int_master_enabled = false;
  cpu.enabling_ime = false;

  timer_get_ctx()->div = 0xABCC;
}

static void fetch_instruction(void) {
  cpu.cur_opcode = bus_read(cpu.regs.pc++);
  cpu.cur_inst = instruction_by_opcode(cpu.cur_opcode);
}

static void execute(void) {
  IN_PROC proc = inst_get_processor(cpu.cur_inst->type);

  if (!proc) {
    NO_IMPL
  }

  proc();
}

bool cpu_step(void) {
  if (!cpu.halted) {
    u16 pc = cpu.regs.pc;

    fetch_instruction();
    emu_cycles(1);
    fetch_data();

    if (emu_get_ctx()->debug_mode) {
      char flags[16];
      snprintf(flags, 16, "%c%c%c%c", cpu.regs.f & (1 << 7) ? 'Z' : '-',
               cpu.regs.f & (1 << 6) ? 'N' : '-',
               cpu.regs.f & (1 << 5) ? 'H' : '-',
               cpu.regs.f & (1 << 4) ? 'C' : '-');

      char inst[16];
      inst_to_str(&cpu, inst);

      printf("%08lX - %04X: %-12s (%02X %02X %02X) A: %02X F: %s BC: %02X%02X "
             "DE: %02X%02X HL: %02X%02X\n",
             emu_get_ctx()->ticks, pc, inst, cpu.cur_opcode, bus_read(pc + 1),
             bus_read(pc + 2), cpu.regs.a, flags, cpu.regs.b, cpu.regs.c,
             cpu.regs.d, cpu.regs.e, cpu.regs.h, cpu.regs.l);
    }

    if (cpu.cur_inst == NULL) {
      printf("Unknown Instruction! %02X\n", cpu.cur_opcode);
      exit(EXIT_FAILURE);
    }

    dbg_update();
    dbg_print();

    execute();
  } else {
    emu_cycles(1);

    if (cpu.int_flags) {
      cpu.halted = false;
    }
  }

  if (cpu.int_master_enabled) {
    cpu_handle_interrupts(&cpu);
    cpu.enabling_ime = false;
  }

  if (cpu.enabling_ime) {
    cpu.int_master_enabled = true;
  }

  return true;
}
