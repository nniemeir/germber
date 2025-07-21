#include <core/bus.h>
#include <core/dma.h>
#include <ppu/ppu.h>

typedef struct {
  bool active;
  u8 byte;
  u8 value;
  u8 start_delay;
} dma_context;

static dma_context dma;

void dma_start(u8 start) {
  dma.active = true;
  dma.byte = 0;
  dma.start_delay = 2;
  dma.value = start;
}

void dma_tick(void) {
  if (!dma.active) {
    return;
  }

  if (dma.start_delay) {
    dma.start_delay--;
    return;
  }

  ppu_oam_write(dma.byte, bus_read((dma.value * 0x100) + dma.byte));

  dma.byte++;

  dma.active = dma.byte < 0xA0;
}

bool dma_transferring(void) { return dma.active; }
