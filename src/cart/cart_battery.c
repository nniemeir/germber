#include <cart/cart.h>
#include <linux/limits.h>

bool cart_has_battery(void) {
  // mbc1 only for now...
  return cart_get_ctx()->rom_data[0x0147] == 3;
}

void cart_battery_load(void) {
  if (!cart_get_ctx()->ram_bank) {
    return;
  }

  char filename[PATH_MAX];
  snprintf(filename, PATH_MAX, "%s.battery", cart_get_ctx()->filename);
  FILE *fp = fopen(filename, "rb");

  if (!fp) {
    fprintf(stderr, "FAILED TO OPEN: %s\n", filename);
    return;
  }

  fread(cart_get_ctx()->ram_bank, 0x2000, 1, fp);
  fclose(fp);
}

void cart_battery_save(void) {
  if (!cart_get_ctx()->ram_bank) {
    return;
  }

  char filename[PATH_MAX];
  snprintf(filename, PATH_MAX, "%s.battery", cart_get_ctx()->filename);
  FILE *fp = fopen(filename, "wb");

  if (!fp) {
    fprintf(stderr, "Failed to open: %s: %s\n", filename, strerror(errno));
    return;
  }

  if (fwrite(cart_get_ctx()->ram_bank, 0x2000, 1, fp) != 1) {
    fprintf(stderr, "Failed to write to %s: %s\n", filename, strerror(errno));
  }

  if (fclose(fp) == EOF) {
    fprintf(stderr, "Failed to close file: %s\n", strerror(errno));
  }
}
