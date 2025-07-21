#ifndef EMU_H
#define EMU_H

#include <common.h>

typedef struct {
    bool paused;
    bool running;
    bool die;
    bool debug_mode;
    u64 ticks;
} emu_context;

void cleanup(void);

void *emu_runtime_loop(void *p);

emu_context *emu_get_context(void);

void emu_cycles(int cpu_cycles);

#endif
