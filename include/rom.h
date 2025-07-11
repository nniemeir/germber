#ifndef ROM_H
#define ROM_H

struct rom_ctx {
    unsigned char *data;
};

extern struct rom_ctx rom;

void init_rom_ctx(void);
void print_rom_info(void);

#endif
