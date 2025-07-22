#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <common.h>

typedef struct {
  bool start;
  bool select;
  bool a;
  bool b;
  bool up;
  bool down;
  bool left;
  bool right;
} gamepad_state;

void gamepad_init(void);
bool gamepad_button_sel(void);
bool gamepad_dir_sel(void);
void gamepad_set_sel(u8 value);

gamepad_state *gamepad_get_state(void);
u8 gamepad_get_output(void);

#endif
