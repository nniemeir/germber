#include <core/bus.h>
#include <core/emu.h>
#include <cpu/cpu.h>

void fetch_data(void) {
  cpu_ctx *cpu = cpu_get_ctx();
  cpu->mem_dest = 0;
  cpu->dest_is_mem = false;

  if (cpu->cur_inst == NULL) {
    return;
  }

  switch (cpu->cur_inst->mode) {
  case AM_IMP:
    return;

  case AM_R:
    cpu->fetched_data = cpu_read_reg(cpu->cur_inst->reg_1);
    return;

  case AM_R_R:
    cpu->fetched_data = cpu_read_reg(cpu->cur_inst->reg_2);
    return;

  case AM_R_D8:
    cpu->fetched_data = bus_read(cpu->regs.pc);
    emu_cycles(1);
    cpu->regs.pc++;
    return;

  case AM_R_D16:
  case AM_D16: {
    u16 lo = bus_read(cpu->regs.pc);
    emu_cycles(1);

    u16 hi = bus_read(cpu->regs.pc + 1);
    emu_cycles(1);

    cpu->fetched_data = lo | (hi << 8);

    cpu->regs.pc += 2;

    return;
  }

  case AM_MR_R:
    cpu->fetched_data = cpu_read_reg(cpu->cur_inst->reg_2);
    cpu->mem_dest = cpu_read_reg(cpu->cur_inst->reg_1);
    cpu->dest_is_mem = true;

    if (cpu->cur_inst->reg_1 == RT_C) {
      cpu->mem_dest |= 0xFF00;
    }

    return;

  case AM_R_MR: {
    u16 addr = cpu_read_reg(cpu->cur_inst->reg_2);

    if (cpu->cur_inst->reg_2 == RT_C) {
      addr |= 0xFF00;
    }

    cpu->fetched_data = bus_read(addr);
    emu_cycles(1);
  }
    return;

  case AM_R_HLI:
    cpu->fetched_data = bus_read(cpu_read_reg(cpu->cur_inst->reg_2));
    emu_cycles(1);
    cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
    return;

  case AM_R_HLD:
    cpu->fetched_data = bus_read(cpu_read_reg(cpu->cur_inst->reg_2));
    emu_cycles(1);
    cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
    return;

  case AM_HLI_R:
    cpu->fetched_data = cpu_read_reg(cpu->cur_inst->reg_2);
    cpu->mem_dest = cpu_read_reg(cpu->cur_inst->reg_1);
    cpu->dest_is_mem = true;
    cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
    return;

  case AM_HLD_R:
    cpu->fetched_data = cpu_read_reg(cpu->cur_inst->reg_2);
    cpu->mem_dest = cpu_read_reg(cpu->cur_inst->reg_1);
    cpu->dest_is_mem = true;
    cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
    return;

  case AM_R_A8:
    cpu->fetched_data = bus_read(cpu->regs.pc);
    emu_cycles(1);
    cpu->regs.pc++;
    return;

  case AM_A8_R:
    cpu->mem_dest = bus_read(cpu->regs.pc) | 0xFF00;
    cpu->dest_is_mem = true;
    emu_cycles(1);
    cpu->regs.pc++;
    return;

  case AM_HL_SPR:
    cpu->fetched_data = bus_read(cpu->regs.pc);
    emu_cycles(1);
    cpu->regs.pc++;
    return;

  case AM_D8:
    cpu->fetched_data = bus_read(cpu->regs.pc);
    emu_cycles(1);
    cpu->regs.pc++;
    return;

  case AM_A16_R:
  case AM_D16_R: {
    u16 lo = bus_read(cpu->regs.pc);
    emu_cycles(1);

    u16 hi = bus_read(cpu->regs.pc + 1);
    emu_cycles(1);

    cpu->mem_dest = lo | (hi << 8);
    cpu->dest_is_mem = true;

    cpu->regs.pc += 2;
    cpu->fetched_data = cpu_read_reg(cpu->cur_inst->reg_2);
  }
    return;

  case AM_MR_D8:
    cpu->fetched_data = bus_read(cpu->regs.pc);
    emu_cycles(1);
    cpu->regs.pc++;
    cpu->mem_dest = cpu_read_reg(cpu->cur_inst->reg_1);
    cpu->dest_is_mem = true;
    return;

  case AM_MR:
    cpu->mem_dest = cpu_read_reg(cpu->cur_inst->reg_1);
    cpu->dest_is_mem = true;
    cpu->fetched_data = bus_read(cpu_read_reg(cpu->cur_inst->reg_1));
    emu_cycles(1);
    return;

  case AM_R_A16: {
    u16 lo = bus_read(cpu->regs.pc);
    emu_cycles(1);

    u16 hi = bus_read(cpu->regs.pc + 1);
    emu_cycles(1);

    u16 addr = lo | (hi << 8);

    cpu->regs.pc += 2;
    cpu->fetched_data = bus_read(addr);
    emu_cycles(1);

    return;
  }

  default:
    printf("Unknown Addressing Mode! %d (%02X)\n", cpu->cur_inst->mode,
           cpu->cur_opcode);
    cleanup();
    exit(EXIT_FAILURE);
  }
}
