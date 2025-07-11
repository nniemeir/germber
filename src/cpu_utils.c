#include "cpu.h"

uint16_t cpu_read_reg(reg_type rt) {
    switch(rt) {
        case RT_A: return cpu.regs.AF.A;
        case RT_F: return cpu.regs.AF.F;
        case RT_B: return cpu.regs.BC.B;
        case RT_C: return cpu.regs.BC.C;
        case RT_D: return cpu.regs.DE.D;
        case RT_E: return cpu.regs.DE.E;
        case RT_H: return cpu.regs.HL.H;
        case RT_L: return cpu.regs.HL.L;

        case RT_AF: return cpu.regs.AF.AF;
        case RT_BC: return cpu.regs.BC.BC;
        case RT_DE: return cpu.regs.DE.DE;
        case RT_HL: return cpu.regs.HL.HL;

        case RT_PC: return cpu.regs.PC;
        case RT_SP: return cpu.regs.SP;

        default: return 0;
    }
}
