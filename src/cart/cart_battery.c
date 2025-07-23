#include <cart/cart.h>
#include <sys/types.h>

bool cart_has_battery(void) {
  // mbc1 only for now...
  return cart_get_ctx()->rom_data[0x0147] == 3;
}

bool cart_battery_init(void) {
  cart_ctx *cart = cart_get_ctx();
  const unsigned int path_size =
      strlen(cart->filename) + strlen(".battery") + NULL_TERMINATOR_LENGTH;
  if (path_size > PATH_MAX) {
    fprintf(stderr, "PATH_MAX exceeded for battery filename.\n");
    return false;
  }

  cart->battery_filename = malloc(path_size);
  if (!cart->battery_filename) {
    fprintf(stderr, "Memory allocation failed for battery filename.\n");
    return false;
  }

  snprintf(cart->battery_filename, path_size, "%s.battery", cart->filename);

  return true;
}

bool cart_battery_load(void) {
  cart_ctx *cart = cart_get_ctx();
  if (!cart->ram_bank) {
    return true;
  }

  FILE *fp = fopen(cart->battery_filename, "rb");

  if (!fp) {
    fprintf(stderr, "FAILED TO OPEN: %s\n", cart->battery_filename);
    return false;
  }

  fread(cart->ram_bank, 0x2000, 1, fp);
  fclose(fp);

  return true;
}

bool cart_battery_save(void) {
  cart_ctx *cart = cart_get_ctx();
  if (!cart->ram_bank) {
    return true;
  }

  FILE *fp = fopen(cart->battery_filename, "wb");

  if (!fp) {
    fprintf(stderr, "Failed to open: %s: %s\n", cart->battery_filename,
            strerror(errno));
    return false;
  }

  if (fwrite(cart->ram_bank, 0x2000, 1, fp) != 1) {
    fprintf(stderr, "Failed to write to %s: %s\n", cart->battery_filename,
            strerror(errno));
    return false;
  }

  if (fclose(fp) == EOF) {
    fprintf(stderr, "Failed to close file: %s\n", strerror(errno));
    return false;
  }

  return true;
}
