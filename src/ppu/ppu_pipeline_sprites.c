#include <ppu/ppu.h>
#include <core/bus.h>
#include <ppu/lcd.h>

u32 fetch_sprite_pixels(int bit, u32 color, u8 bg_color) {
  for (int i = 0; i < ppu_get_context()->fetched_entry_count; i++) {
    int sp_x = (ppu_get_context()->fetched_entries[i].x - 8) +
               ((lcd_get_context()->scroll_x % 8));

    if (sp_x + 8 < ppu_get_context()->pfc.fifo_x) {
      // past pixel point already...
      continue;
    }

    int offset = ppu_get_context()->pfc.fifo_x - sp_x;

    if (offset < 0 || offset > 7) {
      // out of bounds..
      continue;
    }

    bit = (7 - offset);

    if (ppu_get_context()->fetched_entries[i].f_x_flip) {
      bit = offset;
    }

    u8 hi = !!(ppu_get_context()->pfc.fetch_entry_data[i * 2] & (1 << bit));
    u8 lo =
        !!(ppu_get_context()->pfc.fetch_entry_data[(i * 2) + 1] & (1 << bit))
        << 1;

    bool bg_priority = ppu_get_context()->fetched_entries[i].f_bgp;

    if (!(hi | lo)) {
      // transparent
      continue;
    }

    if (!bg_priority || bg_color == 0) {
      color = (ppu_get_context()->fetched_entries[i].f_pn)
                  ? lcd_get_context()->sp2_colors[hi | lo]
                  : lcd_get_context()->sp1_colors[hi | lo];

      if (hi | lo) {
        break;
      }
    }
  }

  return color;
}

void pipeline_load_sprite_tile(void) {
  oam_line_entry *le = ppu_get_context()->line_sprites;

  while (le) {
    int sp_x = (le->entry.x - 8) + (lcd_get_context()->scroll_x % 8);

    if ((sp_x >= ppu_get_context()->pfc.fetch_x &&
         sp_x < ppu_get_context()->pfc.fetch_x + 8) ||
        ((sp_x + 8) >= ppu_get_context()->pfc.fetch_x &&
         (sp_x + 8) < ppu_get_context()->pfc.fetch_x + 8)) {
      // need to add entry
      ppu_get_context()
          ->fetched_entries[ppu_get_context()->fetched_entry_count++] =
          le->entry;
    }

    le = le->next;

    if (!le || ppu_get_context()->fetched_entry_count >= 3) {
      // max checking 3 sprites on pixels
      break;
    }
  }
}

void pipeline_load_sprite_data(u8 offset) {
  int cur_y = lcd_get_context()->ly;
  u8 sprite_height = LCDC_OBJ_HEIGHT;

  for (int i = 0; i < ppu_get_context()->fetched_entry_count; i++) {
    u8 ty = ((cur_y + 16) - ppu_get_context()->fetched_entries[i].y) * 2;

    if (ppu_get_context()->fetched_entries[i].f_y_flip) {
      // flipped upside down...
      ty = ((sprite_height * 2) - 2) - ty;
    }

    u8 tile_index = ppu_get_context()->fetched_entries[i].tile;

    if (sprite_height == 16) {
      tile_index &= ~(1); // remove last bit...
    }

    ppu_get_context()->pfc.fetch_entry_data[(i * 2) + offset] =
        bus_read(0x8000 + (tile_index * 16) + ty + offset);
  }
}
