#include <cart/cart.h>

bool cart_has_battery(void) {
  // mbc1 only for now...
  return cart_get_context()->rom_data[0x0147] == 3;
}

void cart_battery_load(void) {
  if (!cart_get_context()->ram_bank) {
    return;
  }

  char fn[1048];
  snprintf(fn, 1048, "%s.battery", cart_get_context()->filename);
  FILE *fp = fopen(fn, "rb");

  if (!fp) {
    fprintf(stderr, "FAILED TO OPEN: %s\n", fn);
    return;
  }

  fread(cart_get_context()->ram_bank, 0x2000, 1, fp);
  fclose(fp);
}

void cart_battery_save(void) {
  if (!cart_get_context()->ram_bank) {
    return;
  }

  char fn[1048];
  snprintf(fn, 1048, "%s.battery", cart_get_context()->filename);
  FILE *fp = fopen(fn, "wb");

  if (!fp) {
    fprintf(stderr, "Failed to open: %s: %s\n", fn, strerror(errno));
    return;
  }

  if (fwrite(cart_get_context()->ram_bank, 0x2000, 1, fp) != 1) {
    fprintf(stderr, "Failed to write to %s: %s\n", fn, strerror(errno));
  }

  if (fclose(fp) == EOF) {
    fprintf(stderr, "Failed to close file: %s\n", strerror(errno));
  }
}
