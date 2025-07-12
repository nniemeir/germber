#include "cart.h"
#include "rom.h"
#include <stdio.h>

uint8_t cart_read(uint16_t address) { return rom.data[address]; }

void cart_write(void) {
  printf("Writing to ROM not supported.\n");
  return;
}
