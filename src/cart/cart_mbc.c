#include <cart/cart.h>

bool cart_mbc1(void) { return BETWEEN(cart_get_ctx()->rom_data[0x0147], 1, 3); }

void cart_setup_banking(void) {
  u8 ram_size = cart_get_ctx()->rom_data[0x0149];
  for (unsigned int i = 0; i < 16; i++) {
    cart_get_ctx()->ram_banks[i] = 0;

    if ((ram_size == 2 && i == 0) || (ram_size == 3 && i < 4) ||
        (ram_size == 4 && i < 16) || (ram_size == 5 && i < 8)) {
      cart_get_ctx()->ram_banks[i] = malloc(0x2000);
      if (!cart_get_ctx()->ram_banks[i]) {
        fprintf(stderr, "Failed to allocate memory for ram bank: %s",
                strerror(errno));
        return;
      }
      memset(cart_get_ctx()->ram_banks[i], 0, 0x2000);
    }
  }

  cart_get_ctx()->ram_bank = cart_get_ctx()->ram_banks[0];
  cart_get_ctx()->rom_bank_x = cart_get_ctx()->rom_data + 0x4000; // rom bank 1
}
