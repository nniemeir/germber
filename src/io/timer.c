#include <cpu/interrupts.h>
#include <io/timer.h>

static timer_context timer = {0};

timer_context *timer_get_context(void) { return &timer; }

void timer_init(void) { timer.div = 0xAC00; }

void timer_tick(void) {
  u16 prev_div = timer.div;

  timer.div++;

  bool timer_update = false;

  switch (timer.tac & (0b11)) {
  case 0b00:
    timer_update = (prev_div & (1 << 9)) && (!(timer.div & (1 << 9)));
    break;
  case 0b01:
    timer_update = (prev_div & (1 << 3)) && (!(timer.div & (1 << 3)));
    break;
  case 0b10:
    timer_update = (prev_div & (1 << 5)) && (!(timer.div & (1 << 5)));
    break;
  case 0b11:
    timer_update = (prev_div & (1 << 7)) && (!(timer.div & (1 << 7)));
    break;
  }

  if (timer_update && timer.tac & (1 << 2)) {
    timer.tima++;

    if (timer.tima == 0xFF) {
      timer.tima = timer.tma;

      cpu_request_interrupt(IT_TIMER);
    }
  }
}

void timer_write(u16 address, u8 value) {
  switch (address) {
  case 0xFF04:
    // DIV
    timer.div = 0;
    break;

  case 0xFF05:
    // TIMA
    timer.tima = value;
    break;

  case 0xFF06:
    // TMA
    timer.tma = value;
    break;

  case 0xFF07:
    // TAC
    timer.tac = value;
    break;
  }
}

u8 timer_read(u16 address) {
  switch (address) {
  case 0xFF04:
    return timer.div >> 8;
  case 0xFF05:
    return timer.tima;
  case 0xFF06:
    return timer.tma;
  case 0xFF07:
    return timer.tac;
  }
  return 0;
}
