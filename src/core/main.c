#include <cart/cart.h>
#include <common.h>
#include <core/args.h>
#include <core/emu.h>
#include <core/signals.h>
#include <ppu/lcd.h>
#include <ppu/ppu.h>
#include <pthread.h>
#include <time.h>
#include <ui.h>

int main(int argc, char **argv) {
  init_sig_handler();

  emu_get_ctx()->debug_mode = false;

  palettes_init();
  cart_memory_init();

  process_args(argc, argv);

  ui_init();

  pthread_t emu_thread;

  if (pthread_create(&emu_thread, NULL, emu_runtime_loop, NULL)) {
    fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
    cleanup();
    exit(EXIT_FAILURE);
  }

  u32 prev_frame = 0;

  while (!emu_get_ctx()->die) {
    usleep(1000);
    ui_handle_events();

    if (prev_frame != ppu_get_ctx()->current_frame) {
      ui_update();
    }

    prev_frame = ppu_get_ctx()->current_frame;
  }

  cleanup();

  exit(EXIT_SUCCESS);
}
