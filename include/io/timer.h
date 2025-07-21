#ifndef TIMER_H
#define TIMER_H

#include <common.h>

typedef struct {
    u16 div;
    u8 tima;
    u8 tma;
    u8 tac;
} timer_context;

void timer_init(void);
void timer_tick(void);

void timer_write(u16 address, u8 value);
u8 timer_read(u16 address);

timer_context *timer_get_context(void);

#endif
