#include <io/gamepad.h>
#include <string.h>

typedef struct {
  bool button_sel;
  bool dir_sel;
  gamepad_state controller;
} gamepad_ctx;

static gamepad_ctx gamepad = {0};

bool gamepad_button_sel(void) { return gamepad.button_sel; }

bool gamepad_dir_sel(void) { return gamepad.dir_sel; }

void gamepad_set_sel(u8 value) {
  gamepad.button_sel = value & 0x20;
  gamepad.dir_sel = value & 0x10;
}

gamepad_state *gamepad_get_state(void) { return &gamepad.controller; }

u8 gamepad_get_output(void) {
  u8 output = 0xCF;

  if (!gamepad_button_sel()) {
    if (gamepad_get_state()->start) {
      output &= ~(1 << 3);
    }

    if (gamepad_get_state()->select) {
      output &= ~(1 << 2);
    }

    if (gamepad_get_state()->a) {
      output &= ~(1 << 0);
    }

    if (gamepad_get_state()->b) {
      output &= ~(1 << 1);
    }
  }

  if (!gamepad_dir_sel()) {
    if (gamepad_get_state()->left) {
      output &= ~(1 << 1);
    }

    if (gamepad_get_state()->right) {
      output &= ~(1 << 0);
    }

    if (gamepad_get_state()->up) {
      output &= ~(1 << 2);
    }

    if (gamepad_get_state()->down) {
      output &= ~(1 << 3);
    }
  }

  return output;
}
