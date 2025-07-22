
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <cpu/cpu.h>

typedef enum {
  IT_VBLANK = 1,
  IT_LCD_STAT = 2,
  IT_TIMER = 4,
  IT_SERIAL = 8,
  IT_JOYPAD = 16
} interrupt_type;

u8 cpu_get_ie_register(void);
void cpu_set_ie_register(u8 n);

u8 cpu_get_int_flags(void);
void cpu_set_int_flags(u8 value);

void cpu_request_interrupt(interrupt_type t);

void cpu_handle_interrupts(cpu_ctx *ctx);

#endif
