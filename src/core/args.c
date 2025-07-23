#include <cart/cart.h>
#include <common.h>
#include <core/emu.h>
#include <ppu/lcd.h>
#include <ui.h>

void process_args(int argc, char *argv[]) {
  char *rom_path = NULL;
  ui_ctx *ui = ui_get_ctx();
  ui->scale = 4;

  int c;
  while ((c = getopt(argc, argv, "dhp:r:s:")) != -1) {
    switch (c) {
    case 'd':
      emu_get_ctx()->debug_mode = true;
      break;
    case 'h':
      printf("Usage: clownish [options]\n");
      printf("Options:\n");
      printf("  -d               Enable debug mode\n");
      printf("  -h               Show this help message\n");
      printf("  -r               Specify ROM Path\n");
      list_palettes();
      cleanup();
      exit(EXIT_SUCCESS);
    case 'p':
      if (!set_palette(optarg)) {
        printf("Unknown palette.\n");
      }
      break;
    case 'r':
      rom_path = optarg;
      break;
    case 's':
      ui->scale = atoi(optarg);
      if (ui->scale < 1) {
        fprintf(stderr, "Invalid scale provided.\n");
        cleanup();
        exit(EXIT_FAILURE);
      }
      break;
    case '?':
      fprintf(stderr, "Unknown option '-%c'. Run with -h for options.\n",
              optopt);
      cleanup();
      exit(EXIT_FAILURE);
    }
  }

  if (!rom_path) {
    fprintf(stderr, "ROM path not specified.\n");
    cleanup();
    exit(EXIT_FAILURE);
  }

  if (!cart_load(rom_path)) {
    cleanup();
    exit(EXIT_FAILURE);
  }
}
