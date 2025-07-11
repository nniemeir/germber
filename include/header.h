#ifndef HEADER_H
#define HEADER_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define NINTENDO_LOGO_LINE_LENGTH 0x10
#define NUM_OF_OLD_LICENSEE 147
#define NUM_OF_NEW_LICENSEE 62
#define NUM_OF_RAM_SIZE_CODES 5
#define TITLE_LENGTH 0x10

bool is_valid_rom(void);
void print_title(void);
void print_rom_size(void);
void print_cart_ram(void);
void print_publisher(void);

#endif
