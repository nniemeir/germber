
#include "cart.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>

// The Game Boy's 16-bit address bus is used to address ROM, RAM, and I/O.

// 0000	3FFF	16 KiB ROM bank 00	From cartridge, usually a fixed bank
// 4000	7FFF	16 KiB ROM Bank 01–NN	From cartridge, switchable bank via
// mapper (if any) 8000	9FFF	8 KiB Video RAM (VRAM)	In CGB mode, switchable
// bank 0/1 A000	BFFF	8 KiB External RAM	From cartridge,
// switchable bank if any C000	CFFF	4 KiB Work RAM (WRAM) D000	DFFF
// 4 KiB Work RAM (WRAM)	In CGB mode, switchable bank 1–7 E000	FDFF
// Echo RAM (mirror of C000–DDFF)	Nintendo says use of this area is
// prohibited. FE00	FE9F	Object attribute memory (OAM) FEA0	FEFF
// Not Usable	Nintendo says use of this area is prohibited. FF00	FF7F
// I/O Registers FF80	FFFE	High RAM (HRAM) FFFF	FFFF	Interrupt Enable
// register (IE)

// I/O RANGES
// $FF00		DMG	Joypad input
// $FF01	$FF02	DMG	Serial transfer
// $FF04	$FF07	DMG	Timer and divider
// $FF0F		DMG	Interrupts
// $FF10	$FF26	DMG	Audio
// $FF30	$FF3F	DMG	Wave pattern
// $FF40	$FF4B	DMG	LCD Control, Status, Position, Scrolling, and
// Palettes $FF4F		CGB	VRAM Bank Select $FF50		DMG
// Boot ROM mapping control $FF51	$FF55	CGB	VRAM DMA $FF68	$FF6B
// CGB	BG / OBJ Palettes $FF70		CGB	WRAM Bank Select

uint8_t bus_read(uint16_t address) {
  if (address < 0x8000) {
    return cart_read(address);
  }

  fprintf(stderr, "Not implemented");

  return 0;
}

void bus_write(uint16_t address, uint8_t value) {
  if (address < 0x8000) {
    cart_write(address, value);
  }
  
  fprintf(stderr, "Not implemented");
}
