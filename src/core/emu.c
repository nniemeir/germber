#include <core/bus.h>
#include <cart/cart.h>
#include <cpu/cpu.h>
#include <core/dma.h>
#include <core/emu.h>
#include <ppu/lcd.h>
#include <ppu/ppu.h>
#include <stdio.h>
#include <io/timer.h>
#include <ui.h>

static emu_context emu;

emu_context *emu_get_context(void) { return &emu; }

void *emu_runtime_loop(void *p) {
  (void)p;

  timer_init();
  cpu_init();
  ppu_init();
  mem_map_init();

  emu.running = true;
  emu.paused = false;
  emu.ticks = 0;

  while (emu.running) {
    if (emu.paused) {
      delay(10);
      continue;
    }

    if (!cpu_step()) {
      printf("CPU Stopped\n");
      return 0;
    }
  }

  return 0;
}

void emu_cycles(int cpu_cycles) {

  for (int i = 0; i < cpu_cycles; i++) {
    for (int n = 0; n < 4; n++) {
      emu.ticks++;
      timer_tick();
      ppu_tick();
    }

    dma_tick();
  }
}
