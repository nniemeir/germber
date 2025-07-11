#include "main.h"
#include "file.h"
#include "header.h"
#include "rom.h"
#include "cart.h"
#include "emu.h"
#include "cpu.h"

struct emu_context emu;

int main(void) {
  init_rom_ctx();

  if (!is_valid_rom()) {
    fprintf(stderr, "Invalid ROM\n");
    free(rom.data);
    return EXIT_FAILURE;
  }
  
  print_rom_info();

  emu_init();

  cpu_init();

  while (emu.running) {
    if (!cpu_step()) {
      printf("CPU Stopped\n");
      return -3;
    }
    emu.ticks++;
  }

  free(rom.data);
  
  return EXIT_SUCCESS;
}
