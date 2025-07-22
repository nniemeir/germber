#include <core/bus.h>
#include <core/emu.h>
#include <core/stack.h>
#include <cpu/cpu.h>

void proc_di(void) { cpu_get_ctx()->int_master_enabled = false; }

void proc_ei(void) { cpu_get_ctx()->enabling_ime = true; }

void proc_stop(void) { fprintf(stderr, "STOPPING!\n"); }
