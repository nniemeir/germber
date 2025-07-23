#include <core/bus.h>
#include <core/emu.h>
#include <core/stack.h>
#include <cpu/cpu.h>
#include <cpu/instructions.h>

void proc_ld(void) {
  cpu_ctx *cpu = cpu_get_ctx();

  if (cpu->dest_is_mem) {
    // LD (BC), A for instance...

    if (is_16_bit(cpu->cur_inst->reg_2)) {
      // if 16 bit register...
      emu_cycles(1);
      bus_write16(cpu->mem_dest, cpu->fetched_data);
    } else {
      bus_write(cpu->mem_dest, cpu->fetched_data);
    }

    emu_cycles(1);

    return;
  }

  if (cpu->cur_inst->mode == AM_HL_SPR) {
    u8 hflag = (cpu_read_reg(cpu->cur_inst->reg_2) & 0xF) +
                   (cpu->fetched_data & 0xF) >=
               0x10;

    u8 cflag = (cpu_read_reg(cpu->cur_inst->reg_2) & 0xFF) +
                   (cpu->fetched_data & 0xFF) >=
               0x100;

    cpu_set_flags(0, 0, hflag, cflag);
    cpu_set_reg(cpu->cur_inst->reg_1,
                cpu_read_reg(cpu->cur_inst->reg_2) + (int8_t)cpu->fetched_data);

    return;
  }

  cpu_set_reg(cpu->cur_inst->reg_1, cpu->fetched_data);
}

void proc_ldh(void) {
  cpu_ctx *cpu = cpu_get_ctx();
  if (cpu->cur_inst->reg_1 == RT_A) {
    cpu_set_reg(cpu->cur_inst->reg_1, bus_read(0xFF00 | cpu->fetched_data));
  } else {
    bus_write(cpu->mem_dest, cpu->regs.af.a);
  }

  emu_cycles(1);
}

void proc_pop(void) {
  u16 lo = stack_pop();
  emu_cycles(1);
  u16 hi = stack_pop();
  emu_cycles(1);

  u16 n = (hi << 8) | lo;

  cpu_set_reg(cpu_get_ctx()->cur_inst->reg_1, n);

  if (cpu_get_ctx()->cur_inst->reg_1 == RT_AF) {
    cpu_set_reg(cpu_get_ctx()->cur_inst->reg_1, n & 0xFFF0);
  }
}

void proc_push(void) {
  cpu_ctx *cpu = cpu_get_ctx();
  u16 hi = (cpu_read_reg(cpu->cur_inst->reg_1) >> 8) & 0xFF;
  emu_cycles(1);
  stack_push(hi);

  u16 lo = cpu_read_reg(cpu->cur_inst->reg_1) & 0xFF;
  emu_cycles(1);
  stack_push(lo);

  emu_cycles(1);
}
