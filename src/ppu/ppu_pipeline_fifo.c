#include "common.h"
#include <ppu/lcd.h>
#include <ppu/ppu.h>

void pixel_fifo_push(u32 value) {
  ppu_ctx *ppu = ppu_get_ctx();
  fifo_entry *next = malloc(sizeof(fifo_entry));
  next->next = NULL;
  next->value = value;

  if (!ppu->pfc.pixel_fifo.head) {
    // first entry...
    ppu->pfc.pixel_fifo.head = ppu->pfc.pixel_fifo.tail = next;
  } else {
    ppu->pfc.pixel_fifo.tail->next = next;
    ppu->pfc.pixel_fifo.tail = next;
  }

  ppu->pfc.pixel_fifo.size++;
}

u32 pixel_fifo_pop(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  if (ppu->pfc.pixel_fifo.size <= 0) {
    fprintf(stderr, "ERR IN PIXEL FIFO!\n");
    cleanup();
    exit(EXIT_FAILURE);
  }

  fifo_entry *popped = ppu->pfc.pixel_fifo.head;
  ppu->pfc.pixel_fifo.head = popped->next;
  ppu->pfc.pixel_fifo.size--;

  u32 val = popped->value;
  free(popped);

  return val;
}

bool pipeline_fifo_add(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  if (ppu->pfc.pixel_fifo.size > 8) {
    // fifo is full!
    return false;
  }

  int x = ppu->pfc.fetch_x - (8 - (lcd_get_ctx()->scroll_x % 8));

  for (unsigned int i = 0; i < 8; i++) {
    unsigned int bit = 7 - i;
    u8 hi = !!(ppu->pfc.bgw_fetch_data[1] & (1 << bit));
    u8 lo = !!(ppu->pfc.bgw_fetch_data[2] & (1 << bit)) << 1;
    u32 color = lcd_get_ctx()->bg_colors[hi | lo];

    if (!LCDC_BGW_ENABLE) {
      color = lcd_get_ctx()->bg_colors[0];
    }

    if (LCDC_OBJ_ENABLE) {
      color = fetch_sprite_pixels(bit, color, hi | lo);
    }

    if (x >= 0) {
      pixel_fifo_push(color);
      ppu->pfc.fifo_x++;
    }
  }

  return true;
}

void pipeline_fifo_reset(void) {
  ppu_ctx *ppu = ppu_get_ctx();

  while (ppu->pfc.pixel_fifo.size) {
    pixel_fifo_pop();
  }

  ppu->pfc.pixel_fifo.head = 0;
}
