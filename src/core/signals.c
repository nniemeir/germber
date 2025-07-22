#include <common.h>
#include <core/emu.h>
#include <signal.h>
#include <string.h>

void handler(int signal_num) {
  (void)signal_num;
  cleanup();
}

int init_sig_handler(void) {

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_handler = handler;

  if (sigaction(SIGTERM, &sa, NULL) == -1) {
    fprintf(stderr, "Failed to configure signal handling: %s\n",
            strerror(errno));
    return 1;
  }

  return 0;
}
