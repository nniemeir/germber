#include <core/bus.h>
#include <core/dbg.h>

static char dbg_msg[DBG_MSG_MAX] = {0};
static int msg_size = 0;

void dbg_update(void) {
  if (bus_read(0xFF02) == 0x81) {
    const char c = bus_read(0xFF01);

    dbg_msg[msg_size++] = c;

    bus_write(0xFF02, 0);
  }
}

void dbg_print(void) {
  if (dbg_msg[0]) {
    // printf("DBG: %s\n", dbg_msg);
  }
}
