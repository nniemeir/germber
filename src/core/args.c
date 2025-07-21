#include <common.h>
#include <core/emu.h>
#include <ppu/lcd.h>

void process_args(int argc, char *argv[], char **rom_path, int *scale) {
  int c;
  while ((c = getopt(argc, argv, "dhp:r:s:")) != -1) {
    switch (c) {
    case 'd':
      emu_get_context()->debug_mode = true;
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
      lcd_get_context()->current_colors =
          lcd_get_context()->available_palettes[atoi(optarg)];
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