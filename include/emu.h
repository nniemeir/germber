#ifndef EMU_H
#define EMU_H

#include <common.h>

typedef struct {
    bool paused;
    bool running;
    bool die;
    u64 ticks;
} emu_context;


int emu_run(int argc, char **argv);

emu_context *emu_get_context(void);

void emu_cycles(int cpu_cycles);

#endif
