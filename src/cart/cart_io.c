#include "common.h"
#include <cart/cart.h>

bool cart_examine(void) {
  cart_ctx *cart = cart_get_ctx();

  if (!is_valid_rom()) {
    fprintf(stderr, "Missing Nintendo Logo at 0x104-0x133: ROM may be "
                    "incompatible with original hardware.\n");
  }

  cart->battery = cart_has_battery();
  cart->need_save = false;

  print_rom_info();

  if (!cart_setup_banking()) {
    return false;
  }

  if (!cart_verify_checksum()) {
    return false;
  }

  if (cart->battery) {
    if (!cart_battery_init() || !cart_battery_load()) {
      cleanup();
      exit(EXIT_FAILURE);
    }
    cart_battery_init();
    cart_battery_load();
  }
  return true;
}

bool cart_load(const char *filename) {
  cart_ctx *cart = cart_get_ctx();

  cart->filename = malloc(PATH_MAX);
  snprintf(cart->filename, PATH_MAX, "%s", filename);

  FILE *fp = fopen(filename, "rb");

  if (!fp) {
    fprintf(stderr, "Failed to open: %s: %s\n", filename, strerror(errno));
    return false;
  }

  if (fseek(fp, 0, SEEK_END) == -1) {
    fprintf(stderr, "Failed to set file position indicator: %s\n",
            strerror(errno));
    fclose(fp);
    return false;
  }

  const long check_rom_size = ftell(fp);

  if (check_rom_size == -1) {
    fprintf(stderr, "Failed to get file position indicator: %s\n",
            strerror(errno));
    fclose(fp);
    return false;
  }

  cart->rom_size = check_rom_size;

  rewind(fp);

  cart->rom_data = malloc(cart->rom_size);
  if (!cart->rom_data) {
    fprintf(stderr, "Failed to allocate memory for rom_data: %s\n",
            strerror(errno));
    if (fclose(fp) == EOF) {
      fprintf(stderr, "Failed to close file: %s\n", strerror(errno));
    }
    return false;
  }

  const size_t bytes_read = fread(cart->rom_data, cart->rom_size, 1, fp);

  if (fclose(fp) == EOF) {
    fprintf(stderr, "Failed to close file: %s\n", strerror(errno));
    free(cart->rom_data);
    return false;
  }

  if (bytes_read != 1) {
    fprintf(stderr, "Failed to read file from stream: %s\n", strerror(errno));
    free(cart->rom_data);
    return false;
  }
  return cart_examine();
}

u8 cart_read(u16 address) {
  cart_ctx *cart = cart_get_ctx();

  if (!cart_mbc1() || address < 0x4000) {
    return cart->rom_data[address];
  }

  if ((address & 0xE000) == 0xA000) {
    if (!cart->ram_enabled) {
      return 0xFF;
    }

    if (!cart->ram_bank) {
      return 0xFF;
    }

    return cart->ram_bank[address - 0xA000];
  }

  return cart->rom_bank_x[address - 0x4000];
}

void cart_write(u16 address, u8 value) {
  cart_ctx *cart = cart_get_ctx();

  if (!cart_mbc1()) {
    return;
  }

  if (address < 0x2000) {
    cart->ram_enabled = ((value & 0xF) == 0xA);
  }

  if ((address & 0xE000) == 0x2000) {
    // rom bank number
    if (value == 0) {
      value = 1;
    }

    value &= 0b11111;

    cart->rom_bank_value = value;
    cart->rom_bank_x = cart->rom_data + (0x4000 * cart->rom_bank_value);
  }

  if ((address & 0xE000) == 0x4000) {
    // ram bank number
    cart->ram_bank_value = value & 0b11;

    if (cart->ram_banking) {
      if (cart->need_save) {
        if (!cart_battery_save()) {
          cleanup();
          exit(EXIT_FAILURE);
        }
      }

      cart->ram_bank = cart->ram_banks[cart->ram_bank_value];
    }
  }

  if ((address & 0xE000) == 0x6000) {
    // banking mode select
    cart->banking_mode = value & 1;

    cart->ram_banking = cart->banking_mode;

    if (cart->ram_banking) {
      if (cart->need_save) {
        cart_battery_save();
      }

      cart->ram_bank = cart->ram_banks[cart->ram_bank_value];
    }
  }

  if ((address & 0xE000) == 0xA000) {
    if (!cart->ram_enabled) {
      return;
    }

    if (!cart->ram_bank) {
      return;
    }

    cart->ram_bank[address - 0xA000] = value;

    if (cart->battery) {
      cart->need_save = true;
    }
  }
}
