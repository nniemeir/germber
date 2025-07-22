#include <ppu/lcd.h>
#include <ppu/ppu.h>

void pixel_fifo_push(u32 value) {
  fifo_entry *next = malloc(sizeof(fifo_entry));
  next->next = NULL;
  next->value = value;

  if (!ppu_get_ctx()->pfc.pixel_fifo.head) {
    // first entry...
    ppu_get_ctx()->pfc.pixel_fifo.head = ppu_get_ctx()->pfc.pixel_fifo.tail =
        next;
  } else {
    ppu_get_ctx()->pfc.pixel_fifo.tail->next = next;
    ppu_get_ctx()->pfc.pixel_fifo.tail = next;
  }

  ppu_get_ctx()->pfc.pixel_fifo.size++;
}

u32 pixel_fifo_pop(void) {
  if (ppu_get_ctx()->pfc.pixel_fifo.size <= 0) {
    fprintf(stderr, "ERR IN PIXEL FIFO!\n");
    exit(EXIT_FAILURE);
  }

  fifo_entry *popped = ppu_get_ctx()->pfc.pixel_fifo.head;
  ppu_get_ctx()->pfc.pixel_fifo.head = popped->next;
  ppu_get_ctx()->pfc.pixel_fifo.size--;

  u32 val = popped->value;
  free(popped);

  return val;
}

bool pipeline_fifo_add(void) {
  if (ppu_get_ctx()->pfc.pixel_fifo.size > 8) {
    // fifo is full!
    return false;
  }

  int x = ppu_get_ctx()->pfc.fetch_x - (8 - (lcd_get_ctx()->scroll_x % 8));

  for (int i = 0; i < 8; i++) {
    int bit = 7 - i;
    u8 hi = !!(ppu_get_ctx()->pfc.bgw_fetch_data[1] & (1 << bit));
    u8 lo = !!(ppu_get_ctx()->pfc.bgw_fetch_data[2] & (1 << bit)) << 1;
    u32 color = lcd_get_ctx()->bg_colors[hi | lo];

    if (!LCDC_BGW_ENABLE) {
      color = lcd_get_ctx()->bg_colors[0];
    }

    if (LCDC_OBJ_ENABLE) {
      color = fetch_sprite_pixels(bit, color, hi | lo);
    }

    if (x >= 0) {
      pixel_fifo_push(color);
      ppu_get_ctx()->pfc.fifo_x++;
    }
  }

  return true;
}

void pipeline_fifo_reset(void) {
  while (ppu_get_ctx()->pfc.pixel_fifo.size) {
    pixel_fifo_pop();
  }

  ppu_get_ctx()->pfc.pixel_fifo.head = 0;
}
