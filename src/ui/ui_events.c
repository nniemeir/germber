#include <ui.h>
#include <core/emu.h>
#include <io/gamepad.h>

void ui_on_key(bool down, u32 key_code) {
  switch (key_code) {
  case SDLK_ESCAPE:
    emu_get_context()->die = true;
    break;
  case SDLK_LSHIFT:
  case SDLK_z:
    gamepad_get_state()->b = down;
    break;
  case SDLK_x:
  case SDLK_SPACE:
    gamepad_get_state()->a = down;
    break;
  case SDLK_RETURN:
    gamepad_get_state()->start = down;
    break;
  case SDLK_BACKSPACE:
    gamepad_get_state()->select = down;
    break;
  case SDLK_w:
  case SDLK_UP:
    gamepad_get_state()->up = down;
    break;
  case SDLK_s:
  case SDLK_DOWN:
    gamepad_get_state()->down = down;
    break;
  case SDLK_a:
  case SDLK_LEFT:
    gamepad_get_state()->left = down;
    break;
  case SDLK_d:
  case SDLK_RIGHT:
    gamepad_get_state()->right = down;
    break;
  }
}

void ui_handle_events(void) {
  SDL_Event e;
  while (SDL_PollEvent(&e) > 0) {
    if (e.type == SDL_KEYDOWN) {
      ui_on_key(true, e.key.keysym.sym);
    }

    if (e.type == SDL_KEYUP) {
      ui_on_key(false, e.key.keysym.sym);
    }

    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
      emu_get_context()->die = true;
    }
  }
}
