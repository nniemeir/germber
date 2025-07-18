#include <cpu.h>
#include <emu.h>
#include <bus.h>
#include <stack.h>

void proc_cb(void) {
    u8 op = get_cpu_ctx()->fetched_data;
    reg_type reg = decode_reg(op & 0b111);
    u8 bit = (op >> 3) & 0b111;
    u8 bit_op = (op >> 6) & 0b11;
    u8 reg_val = cpu_read_reg8(reg);

    emu_cycles(1);

    if (reg == RT_HL) {
        emu_cycles(2);
    }

    switch(bit_op) {
        case 1:
            //BIT
            cpu_set_flags( !(reg_val & (1 << bit)), 0, 1, -1);
            return;

        case 2:
            //RST
            reg_val &= ~(1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;

        case 3:
            //SET
            reg_val |= (1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;
    }

    bool flagC = CPU_FLAG_C;

    switch(bit) {
        case 0: {
            //RLC
            bool setC = false;
            u8 result = (reg_val << 1) & 0xFF;

            if ((reg_val & (1 << 7)) != 0) {
                result |= 1;
                setC = true;
            }

            cpu_set_reg8(reg, result);
            cpu_set_flags( result == 0, false, false, setC);
        } return;

        case 1: {
            //RRC
            u8 old = reg_val;
            reg_val >>= 1;
            reg_val |= (old << 7);

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( !reg_val, false, false, old & 1);
        } return;

        case 2: {
            //RL
            u8 old = reg_val;
            reg_val <<= 1;
            reg_val |= flagC;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( !reg_val, false, false, !!(old & 0x80));
        } return;

        case 3: {
            //RR
            u8 old = reg_val;
            reg_val >>= 1;

            reg_val |= (flagC << 7);

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( !reg_val, false, false, old & 1);
        } return;

        case 4: {
            //SLA
            u8 old = reg_val;
            reg_val <<= 1;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( !reg_val, false, false, !!(old & 0x80));
        } return;

        case 5: {
            //SRA
            u8 u = (int8_t)reg_val >> 1;
            cpu_set_reg8(reg, u);
            cpu_set_flags( !u, 0, 0, reg_val & 1);
        } return;

        case 6: {
            //SWAP
            reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( reg_val == 0, false, false, false);
        } return;

        case 7: {
            //SRL
            u8 u = reg_val >> 1;
            cpu_set_reg8(reg, u);
            cpu_set_flags( !u, 0, 0, reg_val & 1);
        } return;
    }

    fprintf(stderr, "ERROR: INVALID CB: %02X", op);
    NO_IMPL
}

void proc_rlca(void) {
    u8 u = get_cpu_ctx()->regs.a;
    bool c = (u >> 7) & 1;
    u = (u << 1) | c;
    get_cpu_ctx()->regs.a = u;

    cpu_set_flags( 0, 0, 0, c);
}

void proc_rrca(void) {
    u8 b = get_cpu_ctx()->regs.a & 1;
    get_cpu_ctx()->regs.a >>= 1;
    get_cpu_ctx()->regs.a |= (b << 7);

    cpu_set_flags( 0, 0, 0, b);
}


void proc_rla(void) {
    u8 u = get_cpu_ctx()->regs.a;
    u8 cf = CPU_FLAG_C;
    u8 c = (u >> 7) & 1;

    get_cpu_ctx()->regs.a = (u << 1) | cf;
    cpu_set_flags( 0, 0, 0, c);
}


void proc_rra(void) {
    u8 carry = CPU_FLAG_C;
    u8 new_c = get_cpu_ctx()->regs.a & 1;

    get_cpu_ctx()->regs.a >>= 1;
    get_cpu_ctx()->regs.a |= (carry << 7);

    cpu_set_flags( 0, 0, 0, new_c);
}