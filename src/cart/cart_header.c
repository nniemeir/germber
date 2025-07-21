#include "cart/cart.h"
#include "cart/licensees.h"

extern cart_context cart;

const char *ROM_TYPES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

bool is_valid_rom(void) {
  static const unsigned char nintendo_logo[NINTENDO_LOGO_LINE_LENGTH] = {
      0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
      0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D};

  for (unsigned int i = 0; i < NINTENDO_LOGO_LINE_LENGTH; i++) {
    if (cart.rom_data[0x104 + i] != nintendo_logo[i]) {
      return false;
    }
  }
  return true;
}

bool cart_verify_checksum(void) {
  u16 checksum = 0;
  for (u16 i = 0x0134; i <= 0x014C; i++) {
    checksum = checksum - cart.rom_data[i] - 1;
  }

  if (checksum & 0xFF) {
    printf("Checksum : %2.2X (%s)\n", cart.rom_data[0x134], "PASSED");
    return true;
  } else {
    printf("Checksum : %2.2X (%s)\n", cart.rom_data[0x134], "FAILED");
    return false;
  }
}

char *get_rom_region(void) {
u8 region_code = cart.rom_data[0x014A];
if (region_code == 0) {
  return "Japan";
}
return "Overseas only";
}

void print_rom_info(void) {
  char *title = get_title();

  printf("Title: %s\n", get_title());
  printf("Publisher: %s\n", get_publisher());
  printf("Region : %s\n", get_rom_region());
  printf("Type: %2.2X (%s)\n", cart.rom_data[0x0147], cart_type_name());
  printf("ROM Size: %d KB\n", get_rom_size());
  printf("RAM Size: %d KB\n", get_cart_ram());

  free(title);
}

char *get_publisher(void) {
  // The New Licensee code is only checked if the Old Licensee code is 0x33
  if (cart.rom_data[0x014B] == 0x33) {
    char new_licensee[3] = {cart.rom_data[0x144], cart.rom_data[0x145], '\0'};
    for (unsigned int i = 0; i < NUM_OF_NEW_LICENSEE; i++) {
      if (strcmp(new_licensee_codes[i].key, new_licensee) == 0) {
        return new_licensee_codes[i].string;
      }
    }
  }

  for (unsigned int i = 0; i < NUM_OF_OLD_LICENSEE; i++) {
    if (cart.rom_data[0x014B] == old_licensee_codes[i].hex) {
      return old_licensee_codes[i].string;
    }
  }
  return "UNKNOWN";
}

const char *cart_type_name(void) {
  if (cart.rom_data[0x0147] <= 0x22) {
    return ROM_TYPES[cart.rom_data[0x0147]];
  }

  return "UNKNOWN";
}

char *get_title(void) {
  char *title = malloc(TITLE_LENGTH + 1);
  memset(title, 0, TITLE_LENGTH + 1);
  for (unsigned int i = 0; i < TITLE_LENGTH; i++) {
    if (cart.rom_data[0x134 + i] != 0) {
      title[i] = cart.rom_data[0x134 + i];
    }
  }
  if (title[0] == '\0') {
    strcpy(title, "UNKNOWN");
  }
  title[TITLE_LENGTH] = '\0';
  return title;
}

int get_rom_size(void) {
  return 32 * (1 << cart.rom_data[0x148]);
}

int get_cart_ram(void) {
  static const struct key_value ram_size_codes[NUM_OF_RAM_SIZE_CODES] = {
      {0x00, 0}, {0x02, 8}, {0x03, 32}, {0x04, 128}, {0x05, 64}};

  for (unsigned int i = 0; i < NUM_OF_RAM_SIZE_CODES; i++) {
    if (ram_size_codes[i].key == cart.rom_data[0x149]) {
      return ram_size_codes[i].value;
    }
  }
  return 0;
}
