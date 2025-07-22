#include <core/dma.h>
#include <ppu/lcd.h>
#include <ppu/ppu.h>

static lcd_context lcd;

void palettes_init(void) {
  static unsigned long colors_grayscale[4] = {0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555,
                                         0xFF000000};
  static unsigned long colors_beige[4] = {0xFFFDF6E3, 0xFFDECBA4, 0xFF8B6E4B,
                                          0xFF3B2F2F};
  static unsigned long colors_forest[4] = {0xFFE8F5E9, 0xFF81C784, 0xFF388E3C,
                                           0xFF1B5E20};
  static unsigned long colors_vaporwave[4] = {
      0xFFFFB7C5,
      0xFFB0E0E6, 
      0xFF9B59B6, 
      0xFF2C3E50  
  };
  static unsigned long colors_blue[4] = {0xFFE0FFFF, 0xFF00CED1,
                                              0xFF4682B4, 0xFF2F4F4F};
  lcd.available_palettes[0] = colors_grayscale;
  lcd.available_palettes[1] = colors_beige;
  lcd.available_palettes[2] = colors_forest;
  lcd.available_palettes[3] = colors_vaporwave;
  lcd.available_palettes[4] = colors_blue;
  lcd.current_colors = lcd.available_palettes[0];
}

void lcd_init(void) {
  lcd.lcdc = 0x91;
  lcd.scroll_x = 0;
  lcd.scroll_y = 0;
  lcd.ly = 0;
  lcd.ly_compare = 0;
  lcd.bg_palette = 0xFC;
  lcd.obj_palette[0] = 0xFF;
  lcd.obj_palette[1] = 0xFF;
  lcd.win_y = 0;
  lcd.win_x = 0;

  for (int i = 0; i < 4; i++) {
    lcd.bg_colors[i] = lcd.current_colors[i];
    lcd.sp1_colors[i] = lcd.current_colors[i];
    lcd.sp2_colors[i] = lcd.current_colors[i];
  }
}

lcd_context *lcd_get_context(void) { return &lcd; }

u8 lcd_read(u16 address) {
  u8 offset = (address - 0xFF40);
  u8 *p = (u8 *)&lcd;

  return p[offset];
}

void update_palette(u8 palette_data, u8 pal) {
  u32 *p_colors = lcd.bg_colors;

  switch (pal) {
  case 1:
    p_colors = lcd.sp1_colors;
    break;
  case 2:
    p_colors = lcd.sp2_colors;
    break;
  }

  p_colors[0] = lcd.current_colors[palette_data & 0b11];
  p_colors[1] = lcd.current_colors[(palette_data >> 2) & 0b11];
  p_colors[2] = lcd.current_colors[(palette_data >> 4) & 0b11];
  p_colors[3] = lcd.current_colors[(palette_data >> 6) & 0b11];
}

void lcd_write(u16 address, u8 value) {

  u8 offset = (address - 0xFF40);
  u8 *p = (u8 *)&lcd;
  p[offset] = value;

  if (offset == 6) {
    // 0xFF46 = DMA
    dma_start(value);
  }

  if (address == 0xFF47) {
    update_palette(value, 0);
  } else if (address == 0xFF48) {
    update_palette(value & 0b11111100, 1);
  } else if (address == 0xFF49) {
    update_palette(value & 0b11111100, 2);
  }
}
