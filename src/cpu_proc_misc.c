#include <cpu.h>
#include <emu.h>
#include <bus.h>
#include <stack.h>

void proc_none(void) {
    printf("INVALID INSTRUCTION!\n");
    exit(-7);
}

void proc_nop(void) {

}

void proc_cpl(void) {
    get_cpu_ctx()->regs.a = ~get_cpu_ctx()->regs.a;
    cpu_set_flags( -1, 1, 1, -1);
}

void proc_scf(void) {
    cpu_set_flags( -1, 0, 0, 1);
}

void proc_ccf(void) {
    cpu_set_flags( -1, 0, 0, CPU_FLAG_C ^ 1);
}

