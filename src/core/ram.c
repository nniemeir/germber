#include <core/ram.h>

typedef struct {
  u8 wram[0x2000];
  u8 hram[0x80];
} ram_ctx;

static ram_ctx ram;

u8 wram_read(u16 address) {
  address -= 0xC000;

  if (address >= 0x2000) {
    printf("INVALID WRAM ADDR %08X\n", address + 0xC000);
    cleanup();
    exit(EXIT_FAILURE);
  }

  return ram.wram[address];
}

void wram_write(u16 address, u8 value) {
  address -= 0xC000;

  if (address >= 0x2000) {
    printf("INVALID WRAM ADDR %08X\n", address + 0xC000);
    cleanup();
    exit(EXIT_FAILURE);
  }

  ram.wram[address] = value;
}

u8 hram_read(u16 address) {
  address -= 0xFF80;

  return ram.hram[address];
}

void hram_write(u16 address, u8 value) {
  address -= 0xFF80;

  ram.hram[address] = value;
}
