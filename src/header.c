#include "header.h"
#include "licensees.h"
#include "rom.h"

bool is_valid_rom(void) {
  static const unsigned char nintendo_logo[NINTENDO_LOGO_LINE_LENGTH] = {
      0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
      0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D};

  for (unsigned int i = 0; i < NINTENDO_LOGO_LINE_LENGTH; i++) {
    if (rom.data[0x104 + i] != nintendo_logo[i]) {
      return false;
    }
  }
  return true;
}

void print_title(void) {
  printf("Game title: ");

  for (unsigned int i = 0; i < TITLE_LENGTH; i++) {
    if (rom.data[0x134 + i] != 0) {
      printf("%c", rom.data[0x134 + i]);
    }
  }

  printf("\n");
}

void print_rom_size(void) {
  int data_size = 32 * (1 << rom.data[0x148]);
  printf("ROM size: %d KiB\n", data_size);
}

void print_cart_ram(void) {
  static const struct key_value ram_size_codes[NUM_OF_RAM_SIZE_CODES] = {
      {0x00, 0}, {0x02, 8}, {0x03, 32}, {0x04, 128}, {0x05, 64}};

  for (unsigned int i = 0; i < NUM_OF_RAM_SIZE_CODES; i++) {
    if (ram_size_codes[i].key == rom.data[0x149]) {
      printf("Cartridge RAM size: %d KiB\n", ram_size_codes[i].value);
      return;
    }
  }
}

void print_publisher(void) {
  for (unsigned int i = 0; i < NUM_OF_OLD_LICENSEE; i++) {
    if (rom.data[0x014B] == old_licensee_codes[i].hex) {
      printf("Publisher: %s\n", old_licensee_codes[i].string);
      return;
    }
  }

  char new_licensee[3] = {rom.data[0x144], rom.data[0x145], '\0'};

  for (unsigned int i = 0; i < NUM_OF_NEW_LICENSEE; i++) {
    if (strcmp(new_licensee_codes[i].key, new_licensee) == 0) {
      printf("Publisher: %s\n", new_licensee_codes[i].string);
      return;
    }
  }
}
