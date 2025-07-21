#include <core/bus.h>
#include <cpu/cpu.h>
#include <core/emu.h>
#include <core/stack.h>

void proc_and(void) {
  get_cpu_ctx()->regs.a &= get_cpu_ctx()->fetched_data;
  cpu_set_flags(get_cpu_ctx()->regs.a == 0, 0, 1, 0);
}

void proc_xor(void) {
  get_cpu_ctx()->regs.a ^= get_cpu_ctx()->fetched_data & 0xFF;
  cpu_set_flags(get_cpu_ctx()->regs.a == 0, 0, 0, 0);
}

void proc_or(void) {
  get_cpu_ctx()->regs.a |= get_cpu_ctx()->fetched_data & 0xFF;
  cpu_set_flags(get_cpu_ctx()->regs.a == 0, 0, 0, 0);
}

void proc_cp(void) {
  int n = (int)get_cpu_ctx()->regs.a - (int)get_cpu_ctx()->fetched_data;

  cpu_set_flags(n == 0, 1,
                ((int)get_cpu_ctx()->regs.a & 0x0F) -
                        ((int)get_cpu_ctx()->fetched_data & 0x0F) <
                    0,
                n < 0);
}

void proc_daa(void) {
  u8 u = 0;
  int fc = 0;

  if (CPU_FLAG_H || (!CPU_FLAG_N && (get_cpu_ctx()->regs.a & 0xF) > 9)) {
    u = 6;
  }

  if (CPU_FLAG_C || (!CPU_FLAG_N && get_cpu_ctx()->regs.a > 0x99)) {
    u |= 0x60;
    fc = 1;
  }

  get_cpu_ctx()->regs.a += CPU_FLAG_N ? -u : u;

  cpu_set_flags(get_cpu_ctx()->regs.a == 0, -1, 0, fc);
}

void proc_inc(void) {
  u16 val = cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) + 1;

  if (is_16_bit(get_cpu_ctx()->cur_inst->reg_1)) {
    emu_cycles(1);
  }

  if (get_cpu_ctx()->cur_inst->reg_1 == RT_HL &&
      get_cpu_ctx()->cur_inst->mode == AM_MR) {
    val = bus_read(cpu_read_reg(RT_HL)) + 1;
    val &= 0xFF;
    bus_write(cpu_read_reg(RT_HL), val);
  } else {
    cpu_set_reg(get_cpu_ctx()->cur_inst->reg_1, val);
    val = cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1);
  }

  if ((get_cpu_ctx()->cur_opcode & 0x03) == 0x03) {
    return;
  }

  cpu_set_flags(val == 0, 0, (val & 0x0F) == 0, -1);
}

void proc_dec(void) {
  u16 val = cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) - 1;

  if (is_16_bit(get_cpu_ctx()->cur_inst->reg_1)) {
    emu_cycles(1);
  }

  if (get_cpu_ctx()->cur_inst->reg_1 == RT_HL &&
      get_cpu_ctx()->cur_inst->mode == AM_MR) {
    val = bus_read(cpu_read_reg(RT_HL)) - 1;
    bus_write(cpu_read_reg(RT_HL), val);
  } else {
    cpu_set_reg(get_cpu_ctx()->cur_inst->reg_1, val);
    val = cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1);
  }

  if ((get_cpu_ctx()->cur_opcode & 0x0B) == 0x0B) {
    return;
  }

  cpu_set_flags(val == 0, 1, (val & 0x0F) == 0x0F, -1);
}

void proc_sub(void) {
  u16 val = cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) -
            get_cpu_ctx()->fetched_data;

  int z = val == 0;
  int h = ((int)cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) & 0xF) -
              ((int)get_cpu_ctx()->fetched_data & 0xF) <
          0;
  int c = ((int)cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1)) -
              ((int)get_cpu_ctx()->fetched_data) <
          0;

  cpu_set_reg(get_cpu_ctx()->cur_inst->reg_1, val);
  cpu_set_flags(z, 1, h, c);
}

void proc_sbc(void) {
  u8 val = get_cpu_ctx()->fetched_data + CPU_FLAG_C;

  int z = cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) - val == 0;

  int h = ((int)cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) & 0xF) -
              ((int)get_cpu_ctx()->fetched_data & 0xF) - ((int)CPU_FLAG_C) <
          0;
  int c = ((int)cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1)) -
              ((int)get_cpu_ctx()->fetched_data) - ((int)CPU_FLAG_C) <
          0;

  cpu_set_reg(get_cpu_ctx()->cur_inst->reg_1,
              cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) - val);
  cpu_set_flags(z, 1, h, c);
}

void proc_adc(void) {
  u16 u = get_cpu_ctx()->fetched_data;
  u16 a = get_cpu_ctx()->regs.a;
  u16 c = CPU_FLAG_C;

  get_cpu_ctx()->regs.a = (a + u + c) & 0xFF;

  cpu_set_flags(get_cpu_ctx()->regs.a == 0, 0, (a & 0xF) + (u & 0xF) + c > 0xF,
                a + u + c > 0xFF);
}

void proc_add(void) {
  u32 val = cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) +
            get_cpu_ctx()->fetched_data;

  bool is_16bit = is_16_bit(get_cpu_ctx()->cur_inst->reg_1);

  if (is_16bit) {
    emu_cycles(1);
  }

  if (get_cpu_ctx()->cur_inst->reg_1 == RT_SP) {
    val = cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) +
          (int8_t)get_cpu_ctx()->fetched_data;
  }

  int z = (val & 0xFF) == 0;
  int h = (cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) & 0xF) +
              (get_cpu_ctx()->fetched_data & 0xF) >=
          0x10;
  int c = (int)(cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) & 0xFF) +
              (int)(get_cpu_ctx()->fetched_data & 0xFF) >=
          0x100;

  if (is_16bit) {
    z = -1;
    h = (cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) & 0xFFF) +
            (get_cpu_ctx()->fetched_data & 0xFFF) >=
        0x1000;
    u32 n = ((u32)cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1)) +
            ((u32)get_cpu_ctx()->fetched_data);
    c = n >= 0x10000;
  }

  if (get_cpu_ctx()->cur_inst->reg_1 == RT_SP) {
    z = 0;
    h = (cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) & 0xF) +
            (get_cpu_ctx()->fetched_data & 0xF) >=
        0x10;
    c = (int)(cpu_read_reg(get_cpu_ctx()->cur_inst->reg_1) & 0xFF) +
            (int)(get_cpu_ctx()->fetched_data & 0xFF) >=
        0x100;
  }

  cpu_set_reg(get_cpu_ctx()->cur_inst->reg_1, val & 0xFFFF);
  cpu_set_flags(z, 0, h, c);
}
