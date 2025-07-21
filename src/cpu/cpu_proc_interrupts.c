#include <core/bus.h>
#include <cpu/cpu.h>
#include <core/emu.h>
#include <core/stack.h>

void proc_di(void) { get_cpu_ctx()->int_master_enabled = false; }

void proc_ei(void) { get_cpu_ctx()->enabling_ime = true; }

void proc_stop(void) { fprintf(stderr, "STOPPING!\n"); }
