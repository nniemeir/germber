#ifndef EMU_H
#define EMU_H

#include "stdint.h"
#include "stdbool.h"

struct emu_context {
    bool paused;
    bool running;
    uint64_t ticks;
};

int emu_run(int argc, char **argv);
struct emu_context *emu_get_context();
void emu_cycles(int cpu_cycles);
void emu_init(void);

extern struct emu_context emu;

#endif
