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

void palettes_init(void) {
  lcd_ctx *lcd = lcd_get_ctx();
  static unsigned long colors_grayscale[4] = {0xFFFFFFFF, 0xFFAAAAAA,
                                              0xFF555555, 0xFF000000};
  static unsigned long colors_beige[4] = {0xFFFDF6E3, 0xFFDECBA4, 0xFF8B6E4B,
                                          0xFF3B2F2F};
  static unsigned long colors_forest[4] = {0xFFE8F5E9, 0xFF81C784, 0xFF388E3C,
                                           0xFF1B5E20};

  static unsigned long colors_vaporwave[4] = {0xFFFFB7C5, 0xFFB0E0E6,
                                              0xFF9B59B6, 0xFF2C3E50};

  static unsigned long colors_blue[4] = {0xFFE0FFFF, 0xFF00CED1, 0xFF4682B4,
                                         0xFF2F4F4F};

  lcd->available_palettes[0] = colors_grayscale;
  lcd->available_palettes[1] = colors_beige;
  lcd->available_palettes[2] = colors_forest;
  lcd->available_palettes[3] = colors_vaporwave;
  lcd->available_palettes[4] = colors_blue;

  lcd->current_colors = lcd->available_palettes[0];
}

bool set_palette(char *optarg) {
  lcd_ctx *lcd = lcd_get_ctx();
  for (unsigned int i = 0; i < NUM_OF_PALETTES; i++) {
    if (strcmp(optarg, palettes_map[i].name) == 0) {
      lcd->current_colors = lcd->available_palettes[palettes_map[i].num];
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
