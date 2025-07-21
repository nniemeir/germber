#ifndef UI_H
#define UI_H

#include <common.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "SDL_keycode.h"
#include "SDL_video.h"

typedef struct {
SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

SDL_Window *sdlDebugWindow;
SDL_Renderer *sdlDebugRenderer;
SDL_Texture *sdlDebugTexture;
SDL_Surface *debugScreen;
} ui_ctx;

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

void ui_init(int scale);
void ui_handle_events(void);
void ui_update(int scale);

void dbg_window_update(ui_ctx *ui, int scale);
void debug_window_init(ui_ctx *ui, int scale);

#endif
