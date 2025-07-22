#include "cpu/interrupts.h"
#include <cart/cart.h>
#include <core/bus.h>
#include <core/dma.h>
#include <core/ram.h>
#include <cpu/cpu.h>
#include <cpu/instructions.h>
#include <io/io.h>
#include <ppu/ppu.h>

// 0x0000 - 0x3FFF : ROM Bank 0
// 0x4000 - 0x7FFF : ROM Bank 1 - Switchable
// 0x8000 - 0x97FF : CHR RAM
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : Cartridge RAM
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page

typedef enum {
  ROM_BANK_0,
  ROM_BANK_1,
  CHR_RAM,
  BG_MAP_1,
  BG_MAP_2,
  CART_RAM,
  WRAM_BANK_0,
  WRAM_BANK_1_7,
  ECHO_RAM,
  OAM,
  UNUSABLE,
  IO,
  IE,
  HRAM
} MemRegion;

MemRegion mem_map[0x10000];

void mem_map_init(void) {
  for (u16 i = 0x0000; i <= 0x3FFF; ++i)
    mem_map[i] = ROM_BANK_0;
  for (u16 i = 0x4000; i <= 0x7FFF; ++i)
    mem_map[i] = ROM_BANK_1;
  for (u16 i = 0x8000; i <= 0x97FF; ++i)
    mem_map[i] = CHR_RAM;
  for (u16 i = 0x9800; i <= 0x9BFF; ++i)
    mem_map[i] = BG_MAP_1;
  for (u16 i = 0x9C00; i <= 0x9FFF; ++i)
    mem_map[i] = BG_MAP_2;
  for (u16 i = 0xA000; i <= 0xBFFF; ++i)
    mem_map[i] = CART_RAM;
  for (u16 i = 0xC000; i <= 0xCFFF; ++i)
    mem_map[i] = WRAM_BANK_0;
  for (u16 i = 0xD000; i <= 0xDFFF; ++i)
    mem_map[i] = WRAM_BANK_1_7;
  for (u16 i = 0xE000; i <= 0xFDFF; ++i)
    mem_map[i] = ECHO_RAM;
  for (u16 i = 0xFE00; i <= 0xFE9F; ++i)
    mem_map[i] = OAM;
  for (u16 i = 0xFEA0; i <= 0xFEFF; ++i)
    mem_map[i] = UNUSABLE;
  for (u16 i = 0xFF00; i <= 0xFF7F; ++i)
    mem_map[i] = IO;
  for (u16 i = 0xFF80; i <= 0xFFFE; ++i)
    mem_map[i] = HRAM;

  mem_map[0xFFFF] = IE;
}

u8 bus_read(u16 address) {
  switch (mem_map[address]) {
  case ROM_BANK_0:
  case ROM_BANK_1:
    return cart_read(address);
  case CHR_RAM:
  case BG_MAP_1:
  case BG_MAP_2:
    return ppu_vram_read(address);
  case CART_RAM:
    return cart_read(address);
  case WRAM_BANK_0:
  case WRAM_BANK_1_7:
    return wram_read(address);
  case OAM: {
    if (dma_transferring()) {
      return 0xFF;
    }

    return ppu_oam_read(address);
  }
  case ECHO_RAM:
  case UNUSABLE:
    return 0;
  case IO:
    return io_read(address);
  case IE:
    return cpu_get_ie_register();
  case HRAM:
    return hram_read(address);
  }
  return 0;
}

void bus_write(u16 address, u8 value) {
  switch (mem_map[address]) {
  case ROM_BANK_0:
  case ROM_BANK_1:
    cart_write(address, value);
    break;
  case CHR_RAM:
  case BG_MAP_1:
  case BG_MAP_2:
    ppu_vram_write(address, value);
    break;
  case CART_RAM:
    cart_write(address, value);
    break;
  case WRAM_BANK_0:
  case WRAM_BANK_1_7:
    wram_write(address, value);
    break;
  case OAM: {
    if (dma_transferring()) {
      break;
    }

    ppu_oam_write(address, value);
    break;
  }
  case ECHO_RAM:
  case UNUSABLE:
    break;
  case IO:
    io_write(address, value);
    break;
  case IE:
    cpu_set_ie_register(value);
    break;
  case HRAM:
    hram_write(address, value);
    break;
  }
}

u16 bus_read16(u16 address) {
  u16 lo = bus_read(address);
  u16 hi = bus_read(address + 1);

  return lo | (hi << 8); // the 16 bit value is organized by shifting the hi
                         // value 8 bits to the left
}

void bus_write16(u16 address, u16 value) {
  bus_write(address + 1, (value >> 8) & 0xFF); // Write the upper 8 bits
  bus_write(address, value & 0xFF);            // Then the lower 8 bits
}
