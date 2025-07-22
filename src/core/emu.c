#include <cart/cart.h>
#include <core/bus.h>
#include <core/dma.h>
#include <core/emu.h>
#include <cpu/cpu.h>
#include <io/timer.h>
#include <ppu/lcd.h>
#include <ppu/ppu.h>
#include <stdio.h>
#include <ui.h>

static emu_ctx emu;

emu_ctx *emu_get_ctx(void) { return &emu; }

void cleanup(void) {
  for (unsigned int i = 0; i < 16; i++) {
    if (cart_get_ctx()->ram_banks[i]) {
      free(cart_get_ctx()->ram_banks[i]);
    }
  }

  if (cart_get_ctx()->rom_data) {
    free(cart_get_ctx()->rom_data);
  }

  pipeline_fifo_reset();
}

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
