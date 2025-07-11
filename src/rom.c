#include "rom.h"
#include "file.h"
#include "header.h"

void print_rom_info(void) {
  print_title();
  print_publisher();
  print_cart_ram();
  print_rom_size();
  printf("\n");
}

struct rom_ctx rom;

void init_rom_ctx(void) {
  size_t file_size;
  rom.data = read_file("/home/noodle/Downloads/mm2.gb", &file_size);
}
