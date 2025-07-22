#include <common.h>
#include <core/emu.h>
#include <ppu/lcd.h>

struct string_num {
  char *name;
  int num;
};

struct string_num palettes_map[NUM_OF_PALETTES] = {{"grayscale", 0},
                                                   {"beige", 1},
                                                   {"forest", 2},
                                                   {"vaporwave", 3},
                                                   {"blue", 4}};

bool set_palette(char *optarg) {
  for (unsigned int i = 0; i < NUM_OF_PALETTES; i++) {
    if (strcmp(optarg, palettes_map[i].name) == 0) {
      lcd_get_context()->current_colors =
          lcd_get_context()->available_palettes[palettes_map[i].num];
      return true;
    }
  }
  return false;
}

void list_palettes(void) {
  printf("\nAvailable Palettes:\n");
  for (unsigned int i = 0; i < NUM_OF_PALETTES; i++) {
    printf("%s\n", palettes_map[i].name);
  }
}

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
      printf("  -r               Specify ROM Path\n");
      list_palettes();
      exit(EXIT_SUCCESS);
    case 'p':
      if (!set_palette(optarg)) {
        printf("Unknown palette.\n");
      }
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
