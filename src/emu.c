#include <bus.h>
#include <cart.h>
#include <cpu.h>
#include <dma.h>
#include <emu.h>
#include <lcd.h>
#include <ppu.h>
#include <stdio.h>
#include <timer.h>
#include <ui.h>

// TODO Add Windows Alternative...
#include <pthread.h>
#include <unistd.h>

/*
  Emu components:

  |Cart|
  |CPU|
  |Address Bus|
  |PPU|
  |Timer|

*/

bool debug_mode = false;

static emu_context ctx;

emu_context *emu_get_context(void) { return &ctx; }

void *cpu_run(void *p) {
  (void)p;

  timer_init();
  cpu_init();
  ppu_init();
  mem_map_init();

  ctx.running = true;
  ctx.paused = false;
  ctx.ticks = 0;

  while (ctx.running) {
    if (ctx.paused) {
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

void process_args(int argc, char *argv[], char **rom_path, int *scale) {
  int c;
  while ((c = getopt(argc, argv, "dhp:r:s:")) != -1) {
    switch (c) {
    case 'd':
      debug_mode = true;
      break;
    case 'h':
      printf("Usage: clownish [options]\n");
      printf("Options:\n");
      printf("  -d               Enable debug mode\n");
      printf("  -h               Show this help message\n");
      printf("  -v               Specify ROM Path\n");
      exit(EXIT_SUCCESS);
    case 'p':
      if (atoi(optarg) > NUM_OF_PALETTES || atoi(optarg) < 0) {
        fprintf(stderr, "Invalid palette.\n");
        exit(EXIT_FAILURE);
      }
      lcd_get_context()->current_colors = lcd_get_context()->available_palettes[atoi(optarg)];
      break;
    case 'r':
      *rom_path = optarg;
      break;
    case 's':
      *scale = atoi(optarg);
      if (*scale == 0) {
        fprintf(stderr, "Failed to convert scale argument to int.\n");
        exit(EXIT_FAILURE);
      }
      break;
    case '?':
      fprintf(stderr, "Unknown option '-%c'. Run with -h for options.\n",
              optopt);
      exit(EXIT_FAILURE);
    }
  }
}

int emu_run(int argc, char **argv) {
  char *rom_path = NULL;
  int scale = 4;
  palettes_init();
  process_args(argc, argv, &rom_path, &scale);

  if (!rom_path) {
    fprintf(stderr, "ROM path not specified.\n");
    return -1;
  }

  if (!cart_load(rom_path)) {
    printf("Failed to load ROM file: %s\n", argv[1]);
    return -2;
  }

  printf("Cart loaded..\n");

  ui_init(scale);

  pthread_t t1;

  if (pthread_create(&t1, NULL, cpu_run, NULL)) {
    fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
    return -1;
  }

  u32 prev_frame = 0;

  while (!ctx.die) {
    usleep(1000);
    ui_handle_events();

    if (prev_frame != ppu_get_context()->current_frame) {
      ui_update(scale);
    }

    prev_frame = ppu_get_context()->current_frame;
  }

  return 0;
}

void emu_cycles(int cpu_cycles) {

  for (int i = 0; i < cpu_cycles; i++) {
    for (int n = 0; n < 4; n++) {
      ctx.ticks++;
      timer_tick();
      ppu_tick();
    }

    dma_tick();
  }
}
