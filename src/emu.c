#include "emu.h"
#include <stdio.h>

void emu_init(void) {
  emu.running = true;
  emu.paused = false;
  emu.ticks = 0;
}

void emu_cycles(int cpu_cycles) {
printf("Advancing hardware clock by %d\n", cpu_cycles);
return;
}
