#include <core/args.h>
#include <cart/cart.h>
#include <common.h>
#include <core/emu.h>
#include <ppu/lcd.h>
#include <ppu/ppu.h>
#include <pthread.h>
#include <core/signals.h>
#include <ui.h>

void cleanup(void) {
  for (unsigned int i = 0; i < 16; i++) {
    if (cart_get_context()->ram_banks[i]) {
      free(cart_get_context()->ram_banks[i]);
    }
  }
  if (cart_get_context()->rom_data) {
    free(cart_get_context()->rom_data);
  }
  pipeline_fifo_reset();
}

int main(int argc, char **argv) {
  init_sig_handler();
  emu_get_context()->debug_mode = false;
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

  ui_init(scale);

  pthread_t t1;

  if (pthread_create(&t1, NULL, emu_runtime_loop, NULL)) {
    fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
    return -1;
  }

  u32 prev_frame = 0;

  while (!emu_get_context()->die) {
    usleep(1000);
    ui_handle_events();

    if (prev_frame != ppu_get_context()->current_frame) {
      ui_update(scale);
    }

    prev_frame = ppu_get_context()->current_frame;
  }

  cleanup();

  return 0;
}
