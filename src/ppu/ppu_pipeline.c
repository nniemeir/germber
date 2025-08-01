#include <core/bus.h>
#include <ppu/lcd.h>
#include <ppu/ppu.h>

bool window_visible(void) {
  return LCDC_WIN_ENABLE && lcd_get_ctx()->win_x <= 166 &&
         lcd_get_ctx()->win_y < YRES;
}

void pipeline_load_window_tile(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  if (!window_visible()) {
    return;
  }

  u8 window_y = lcd_get_ctx()->win_y;

  if (ppu->pfc.fetch_x + 7 >= lcd_get_ctx()->win_x &&
      ppu->pfc.fetch_x + 7U < lcd_get_ctx()->win_x + YRES + 14U) {
    if (lcd_get_ctx()->ly >= window_y && lcd_get_ctx()->ly < window_y + XRES) {
      u8 w_tile_y = ppu->window_line / 8;

      ppu->pfc.bgw_fetch_data[0] =
          bus_read(LCDC_WIN_MAP_AREA +
                   ((ppu->pfc.fetch_x + 7 - lcd_get_ctx()->win_x) / 8) +
                   (w_tile_y * 32));

      if (LCDC_BGW_DATA_AREA == 0x8800) {
        ppu->pfc.bgw_fetch_data[0] += 128;
      }
    }
  }
}

void pipeline_fetch(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  switch (ppu->pfc.cur_fetch_state) {
  case FS_TILE: {
    ppu->fetched_entry_count = 0;

    if (LCDC_BGW_ENABLE) {
      ppu->pfc.bgw_fetch_data[0] =
          bus_read(LCDC_BG_MAP_AREA + (ppu->pfc.map_x / 8) +
                   (((ppu->pfc.map_y / 8)) * 32));

      if (LCDC_BGW_DATA_AREA == 0x8800) {
        ppu->pfc.bgw_fetch_data[0] += 128;
      }

      pipeline_load_window_tile();
    }

    if (LCDC_OBJ_ENABLE && ppu->line_sprites) {
      pipeline_load_sprite_tile();
    }

    ppu->pfc.cur_fetch_state = FS_DATA0;
    ppu->pfc.fetch_x += 8;
  } break;

  case FS_DATA0: {
    ppu->pfc.bgw_fetch_data[1] =
        bus_read(LCDC_BGW_DATA_AREA + (ppu->pfc.bgw_fetch_data[0] * 16) +
                 ppu->pfc.tile_y);

    pipeline_load_sprite_data(0);

    ppu->pfc.cur_fetch_state = FS_DATA1;
  } break;

  case FS_DATA1: {
    ppu->pfc.bgw_fetch_data[2] =
        bus_read(LCDC_BGW_DATA_AREA + (ppu->pfc.bgw_fetch_data[0] * 16) +
                 ppu->pfc.tile_y + 1);

    pipeline_load_sprite_data(1);

    ppu->pfc.cur_fetch_state = FS_IDLE;

  } break;

  case FS_IDLE: {
    ppu->pfc.cur_fetch_state = FS_PUSH;
  } break;

  case FS_PUSH: {
    if (pipeline_fifo_add()) {
      ppu->pfc.cur_fetch_state = FS_TILE;
    }

  } break;
  }
}

void pipeline_push_pixel(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  if (ppu->pfc.pixel_fifo.size > 8) {
    u32 pixel_data = pixel_fifo_pop();
    u8 ly = lcd_get_ctx()->ly;
    u8 wx = lcd_get_ctx()->win_x;
    u8 wy = lcd_get_ctx()->win_y;

    bool use_window =
        window_visible() && (ppu->pfc.line_x + 7 >= wx) && (ly >= wy);

    if (use_window) {
      // Window starts at wx, not affected by scroll_x
      if (ppu->pfc.line_x + 7 >= wx) {
        int x = ppu->pfc.pushed_x;
        ppu->video_buffer[x + (ly * XRES)] = pixel_data;
        ppu->pfc.pushed_x++;
      }
    } else {
      // Background uses scroll_x
      if (ppu->pfc.line_x >= (lcd_get_ctx()->scroll_x % 8)) {
        int x = ppu->pfc.pushed_x;
        ppu->video_buffer[x + (ly * XRES)] = pixel_data;
        ppu->pfc.pushed_x++;
      }
    }

    ppu->pfc.line_x++;
  }
}

void pipeline_process(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  u8 ly = lcd_get_ctx()->ly;
  u8 wx = lcd_get_ctx()->win_x;
  u8 wy = lcd_get_ctx()->win_y;

  bool use_window =
      window_visible() && (ppu->pfc.fetch_x + 7 >= wx) && (ly >= wy);

  if (use_window) {
    // Horizontal position relative to window start
    int win_relative_x = ppu->pfc.fetch_x + 7 - wx;

    // Clamp to prevent negative indexing
    if (win_relative_x < 0) {
      win_relative_x = 0;
    }

    ppu->pfc.map_x = win_relative_x;
    ppu->pfc.map_y = ppu->window_line;
    ppu->pfc.tile_y = (ppu->window_line % 8) * 2;
  } else {
    ppu->pfc.map_y = ly + lcd_get_ctx()->scroll_y;
    ppu->pfc.map_x = ppu->pfc.fetch_x + lcd_get_ctx()->scroll_x;
    ppu->pfc.tile_y = ((ly + lcd_get_ctx()->scroll_y) % 8) * 2;
  }

  if (!(ppu->line_ticks & 1)) {
    pipeline_fetch();
  }

  pipeline_push_pixel();
}
