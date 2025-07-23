#include <ppu/lcd.h>
#include <ppu/ppu.h>
#include <ppu/ppu_sm.h>
#include <string.h>

static ppu_ctx ppu;

ppu_ctx *ppu_get_ctx(void) { return &ppu; }

void ppu_init(void) {
  ppu.current_frame = 0;
  ppu.line_ticks = 0;
  ppu.video_buffer = malloc(YRES * XRES * sizeof(32));

  ppu.pfc.line_x = 0;
  ppu.pfc.pushed_x = 0;
  ppu.pfc.fetch_x = 0;
  ppu.pfc.pixel_fifo.size = 0;
  ppu.pfc.pixel_fifo.head = ppu.pfc.pixel_fifo.tail = NULL;
  ppu.pfc.cur_fetch_state = FS_TILE;

  ppu.line_sprites = 0;
  ppu.fetched_entry_count = 0;
  ppu.window_line = 0;

  lcd_init();
  LCDS_MODE_SET(MODE_OAM);

  memset(ppu.oam_ram, 0, sizeof(ppu.oam_ram));
  memset(ppu.video_buffer, 0, YRES * XRES * sizeof(u32));
}

void ppu_tick(void) {
  ppu.line_ticks++;

  switch (LCDS_MODE) {
  case MODE_OAM:
    ppu_mode_oam();
    break;
  case MODE_XFER:
    ppu_mode_xfer();
    break;
  case MODE_VBLANK:
    ppu_mode_vblank();
    break;
  case MODE_HBLANK:
    ppu_mode_hblank();
    break;
  }
}

void ppu_oam_write(u16 address, u8 value) {
  if (address >= 0xFE00) {
    address -= 0xFE00;
  }

  u8 *p = (u8 *)ppu.oam_ram;
  p[address] = value;
}

u8 ppu_oam_read(u16 address) {
  if (address >= 0xFE00) {
    address -= 0xFE00;
  }

  u8 *p = (u8 *)ppu.oam_ram;
  return p[address];
}

void ppu_vram_write(u16 address, u8 value) {
  ppu.vram[address - 0x8000] = value;
}

u8 ppu_vram_read(u16 address) { return ppu.vram[address - 0x8000]; }
