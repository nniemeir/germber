#include <cart/cart.h>
#include <core/emu.h>
#include <cpu/cpu.h>
#include <cpu/interrupts.h>
#include <ppu/lcd.h>
#include <ppu/ppu.h>
#include <string.h>

void increment_ly(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  if (window_visible() && lcd_get_ctx()->ly >= lcd_get_ctx()->win_y &&
      lcd_get_ctx()->ly < lcd_get_ctx()->win_y + YRES) {
    ppu->window_line++;
  }

  lcd_get_ctx()->ly++;

  if (lcd_get_ctx()->ly == lcd_get_ctx()->ly_compare) {
    LCDS_LYC_SET(1);

    if (LCDS_STAT_INT(SS_LYC)) {
      cpu_request_interrupt(IT_LCD_STAT);
    }
  } else {
    LCDS_LYC_SET(0);
  }
}

void load_line_sprites(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  int cur_y = lcd_get_ctx()->ly;

  u8 sprite_height = LCDC_OBJ_HEIGHT;
  memset(ppu->line_entry_array, 0, sizeof(ppu->line_entry_array));

  for (unsigned int i = 0; i < 40; i++) {
    oam_entry e = ppu->oam_ram[i];

    if (!e.x) {
      // x = 0 means not visible...
      continue;
    }

    if (ppu->line_sprite_count >= 10) {
      // max 10 sprites per line...
      break;
    }

    if (e.y <= cur_y + 16 && e.y + sprite_height > cur_y + 16) {
      // this sprite is on the current line.

      oam_line_entry *entry = &ppu->line_entry_array[ppu->line_sprite_count++];

      entry->entry = e;
      entry->next = NULL;

      if (!ppu->line_sprites || ppu->line_sprites->entry.x > e.x) {
        entry->next = ppu->line_sprites;
        ppu->line_sprites = entry;
        continue;
      }

      // do some sorting...

      oam_line_entry *le = ppu->line_sprites;
      oam_line_entry *prev = le;

      while (le) {
        if (le->entry.x > e.x) {
          prev->next = entry;
          entry->next = le;
          break;
        }

        if (!le->next) {
          le->next = entry;
          break;
        }

        prev = le;
        le = le->next;
      }
    }
  }
}

void ppu_mode_oam(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  if (ppu->line_ticks >= 80) {
    LCDS_MODE_SET(MODE_XFER);

    ppu->pfc.cur_fetch_state = FS_TILE;
    ppu->pfc.line_x = 0;
    ppu->pfc.fetch_x = 0;
    ppu->pfc.pushed_x = 0;
    ppu->pfc.fifo_x = 0;
  }

  if (ppu->line_ticks == 1) {
    // read oam on the first tick only...
    ppu->line_sprites = 0;
    ppu->line_sprite_count = 0;

    load_line_sprites();
  }
}

void ppu_mode_xfer(void) {
  pipeline_process();

  if (ppu_get_ctx()->pfc.pushed_x >= XRES) {
    pipeline_fifo_reset();

    LCDS_MODE_SET(MODE_HBLANK);

    if (LCDS_STAT_INT(SS_HBLANK)) {
      cpu_request_interrupt(IT_LCD_STAT);
    }
  }
}

void ppu_mode_vblank(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  if (ppu->line_ticks >= TICKS_PER_LINE) {
    increment_ly();

    if (lcd_get_ctx()->ly >= LINES_PER_FRAME) {
      LCDS_MODE_SET(MODE_OAM);
      lcd_get_ctx()->ly = 0;
      ppu->window_line = 0;
    }

    ppu->line_ticks = 0;
  }
}

static u32 target_frame_time = 1000 / 60;
static long prev_frame_time = 0;
static long start_timer = 0;
static long frame_count = 0;

void ppu_mode_hblank(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  if (ppu->line_ticks >= TICKS_PER_LINE) {
    increment_ly();

    if (lcd_get_ctx()->ly >= YRES) {
      LCDS_MODE_SET(MODE_VBLANK);

      cpu_request_interrupt(IT_VBLANK);

      if (LCDS_STAT_INT(SS_VBLANK)) {
        cpu_request_interrupt(IT_LCD_STAT);
      }

      ppu->current_frame++;

      // calc FPS...
      u32 end = get_ticks();
      u32 frame_time = end - prev_frame_time;

      if (frame_time < target_frame_time) {
        delay((target_frame_time - frame_time));
      }

      if (end - start_timer >= 1000) {
        u32 fps = frame_count;
        start_timer = end;
        frame_count = 0;

        if (emu_get_ctx()->debug_mode) {
          printf("FPS: %d\n", fps);
        }

        if (cart_get_ctx()->need_save) {
          cart_battery_save();
        }
      }

      frame_count++;
      prev_frame_time = get_ticks();

    } else {
      LCDS_MODE_SET(MODE_OAM);
    }

    ppu->line_ticks = 0;
  }
}
