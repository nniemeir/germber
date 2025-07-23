#include <cart/cart.h>

static cart_ctx cart;

cart_ctx *cart_get_ctx(void) { return &cart; }

void cart_memory_init(void) {
  memset(&cart, 0, sizeof(cart_ctx));
  cart.rom_data = NULL;
  cart.battery_filename = NULL;
  cart.filename = NULL;

  for (unsigned int i = 0; i < 16; i++) {
    cart.ram_banks[i] = NULL;
  }
}
