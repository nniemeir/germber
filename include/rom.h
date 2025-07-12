#ifndef ROM_H
#define ROM_H

#include "common.h"

struct rom_ctx {
    unsigned char *data;
};

extern struct rom_ctx rom;

void init_rom_ctx(void);
void cleanup_rom(void);
void print_rom_info(void);

#endif
