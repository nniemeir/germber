#ifndef CART_H
#define CART_H

#include <common.h>

#define NINTENDO_LOGO_LINE_LENGTH 0x10
#define NUM_OF_OLD_LICENSEE 147
#define NUM_OF_NEW_LICENSEE 62
#define NUM_OF_RAM_SIZE_CODES 5
#define TITLE_LENGTH 0x10

typedef struct {
  char *filename;
  u32 rom_size;
  u8 *rom_data;

  // mbc1 related data
  bool ram_enabled;
  bool ram_banking;

  u8 *rom_bank_x;
  u8 banking_mode;

  u8 rom_bank_value;
  u8 ram_bank_value;

  u8 *ram_bank;      // current selected ram bank
  u8 *ram_banks[16]; // all ram banks

  // for battery
  char *battery_filename;
  bool battery;   // has battery
  bool need_save; // should save battery backup.
} cart_ctx;

cart_ctx *cart_get_ctx(void);

void cart_memory_init(void);

// Battery
bool cart_has_battery(void);
bool cart_battery_init(void);
bool cart_battery_load(void);
bool cart_battery_save(void);

// Header
bool is_valid_rom(void);
bool cart_verify_checksum(void);
void print_rom_info(void);

// IO
bool cart_load(const char *filename);
u8 cart_read(u16 address);
void cart_write(u16 address, u8 value);

// MBC
bool cart_mbc1(void);
bool cart_mbc3(void);
bool cart_setup_banking(void);

#endif
