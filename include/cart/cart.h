#pragma once

#include <common.h>

#define NINTENDO_LOGO_LINE_LENGTH 0x10
#define NUM_OF_OLD_LICENSEE 147
#define NUM_OF_NEW_LICENSEE 62
#define NUM_OF_RAM_SIZE_CODES 5
#define TITLE_LENGTH 0x10

typedef struct {
  char filename[1024];
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
  bool battery;   // has battery
  bool need_save; // should save battery backup.
} cart_context;

cart_context *cart_get_context(void);

bool cart_load(char *filename);

u8 cart_read(u16 address);
void cart_write(u16 address, u8 value);

bool cart_has_battery(void);
void cart_battery_load(void);
void cart_battery_save(void);


char *get_publisher(void);
const char *cart_type_name(void);
char *get_title(void);
int get_rom_size(void);
int get_cart_ram(void);
void print_rom_info(void);

bool cart_mbc1(void);
void cart_setup_banking(void);

bool cart_verify_checksum(void);
bool is_valid_rom(void);
