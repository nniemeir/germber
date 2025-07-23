#ifndef EMU_H
#define EMU_H

#include <common.h>

typedef struct {
  bool paused;
  bool running;
  bool die;
  bool debug_mode;
  u64 ticks;
} emu_ctx;

void *emu_runtime_loop(void *p);

emu_ctx *emu_get_ctx(void);

void emu_cycles(unsigned int cpu_cycles);

#endif
