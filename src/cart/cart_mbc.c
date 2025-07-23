#include <cart/cart.h>

bool cart_mbc1(void) { return BETWEEN(cart_get_ctx()->rom_data[0x0147], 1, 3); }

bool cart_setup_banking(void) {
  cart_ctx *cart = cart_get_ctx();

  const u8 ram_size = cart->rom_data[0x0149];
  for (unsigned int i = 0; i < 16; i++) {
    cart->ram_banks[i] = 0;

    if ((ram_size == 2 && i == 0) || (ram_size == 3 && i < 4) ||
        (ram_size == 4 && i < 16) || (ram_size == 5 && i < 8)) {
      cart->ram_banks[i] = malloc(0x2000);
      if (!cart->ram_banks[i]) {
        fprintf(stderr, "Failed to allocate memory for ram bank: %s",
                strerror(errno));
        return false;
      }
      memset(cart->ram_banks[i], 0, 0x2000);
    }
  }

  cart->ram_bank = cart->ram_banks[0];
  cart->rom_bank_x = cart->rom_data + 0x4000; // rom bank 1

  return true;
}
