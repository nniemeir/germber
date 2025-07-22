#ifndef UI_H
#define UI_H

#include "SDL_keycode.h"
#include "SDL_video.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <common.h>

#define DEBUG_WINDOW_OFFSET 1034

typedef struct {
  SDL_Window *sdlWindow;
  SDL_Renderer *sdlRenderer;
  SDL_Texture *sdlTexture;
  SDL_Surface *screen;

  SDL_Window *sdlDebugWindow;
  SDL_Renderer *sdlDebugRenderer;
  SDL_Texture *sdlDebugTexture;
  SDL_Surface *debugScreen;

  int scale;
} ui_ctx;

ui_ctx *ui_get_ctx(void);

void ui_init(void);
void ui_handle_events(void);
void ui_update(void);

void dbg_window_update(ui_ctx *ui);
void debug_window_init(ui_ctx *ui);

#endif
