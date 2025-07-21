#include <cart/cart.h>

bool cart_load(char *filename) {
  snprintf(cart_get_context()->filename, sizeof(cart_get_context()->filename), "%s", filename);

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

  long check_rom_size = ftell(fp);

  if (check_rom_size == -1) {
    fprintf(stderr, "Failed to get file position indicator: %s\n", errno);
    fclose(fp);
    return false;
  }

  cart_get_context()->rom_size = check_rom_size;

  rewind(fp);

  cart_get_context()->rom_data = malloc(cart_get_context()->rom_size);
  if (!cart_get_context()->rom_data) {
    fprintf(stderr, "Failed to allocate memory for rom_data: %s\n",
            strerror(errno));
    if (fclose(fp) == EOF) {
      fprintf(stderr, "Failed to close file: %s\n", strerror(errno));
    }
    return false;
  }

  const size_t bytes_read = fread(cart_get_context()->rom_data, cart_get_context()->rom_size, 1, fp);

  if (fclose(fp) == EOF) {
    fprintf(stderr, "Failed to close file: %s\n", strerror(errno));
    free(cart_get_context()->rom_data);
    return false;
  }

  if (bytes_read != 1) {
    fprintf(stderr, "Failed to read file from stream: %s\n", strerror(errno));
    free(cart_get_context()->rom_data);
    return false;
  }

  if (!is_valid_rom()) {
    fprintf(stderr, "Missing Nintendo Logo at 0x104-0x133: ROM may be "
                    "incompatible with original hardware.\n");
  }

  // cart_get_context()->header = (rom_header *)(cart_get_context()->rom_data + 0x100);
  // cart_get_context()->header->title[15] = 0;
  cart_get_context()->battery = cart_has_battery();
  cart_get_context()->need_save = false;

  print_rom_info();

  cart_setup_banking();

  if (!cart_verify_checksum()) {
    return false;
  }

  if (cart_get_context()->battery) {
    cart_battery_load();
  }

  return true;
}

u8 cart_read(u16 address) {
  if (!cart_mbc1() || address < 0x4000) {
    return cart_get_context()->rom_data[address];
  }

  if ((address & 0xE000) == 0xA000) {
    if (!cart_get_context()->ram_enabled) {
      return 0xFF;
    }

    if (!cart_get_context()->ram_bank) {
      return 0xFF;
    }

    return cart_get_context()->ram_bank[address - 0xA000];
  }

  return cart_get_context()->rom_bank_x[address - 0x4000];
}

void cart_write(u16 address, u8 value) {
  if (!cart_mbc1()) {
    return;
  }

  if (address < 0x2000) {
    cart_get_context()->ram_enabled = ((value & 0xF) == 0xA);
  }

  if ((address & 0xE000) == 0x2000) {
    // rom bank number
    if (value == 0) {
      value = 1;
    }

    value &= 0b11111;

    cart_get_context()->rom_bank_value = value;
    cart_get_context()->rom_bank_x = cart_get_context()->rom_data + (0x4000 * cart_get_context()->rom_bank_value);
  }

  if ((address & 0xE000) == 0x4000) {
    // ram bank number
    cart_get_context()->ram_bank_value = value & 0b11;

    if (cart_get_context()->ram_banking) {
      if (cart_get_context()->need_save) {
        cart_battery_save();
      }

      cart_get_context()->ram_bank = cart_get_context()->ram_banks[cart_get_context()->ram_bank_value];
    }
  }

  if ((address & 0xE000) == 0x6000) {
    // banking mode select
    cart_get_context()->banking_mode = value & 1;

    cart_get_context()->ram_banking = cart_get_context()->banking_mode;

    if (cart_get_context()->ram_banking) {
      if (cart_get_context()->need_save) {
        cart_battery_save();
      }

      cart_get_context()->ram_bank = cart_get_context()->ram_banks[cart_get_context()->ram_bank_value];
    }
  }

  if ((address & 0xE000) == 0xA000) {
    if (!cart_get_context()->ram_enabled) {
      return;
    }

    if (!cart_get_context()->ram_bank) {
      return;
    }

    cart_get_context()->ram_bank[address - 0xA000] = value;

    if (cart_get_context()->battery) {
      cart_get_context()->need_save = true;
    }
  }
}
