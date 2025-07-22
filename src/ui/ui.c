#include <core/bus.h>
#include <core/emu.h>
#include <ppu/lcd.h>
#include <ppu/ppu.h>
#include <ui.h>

static ui_ctx ui;

ui_ctx *ui_get_ctx(void) { return &ui; }

void delay(u32 ms) { SDL_Delay(ms); }

u32 get_ticks(void) { return SDL_GetTicks(); }

void ui_init(void) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_CreateWindowAndRenderer(XRES * ui.scale, YRES * ui.scale, 0,
                              &ui.sdlWindow, &ui.sdlRenderer);

  ui.screen =
      SDL_CreateRGBSurface(0, XRES * ui.scale, YRES * ui.scale, 32, 0x00FF0000,
                           0x0000FF00, 0x000000FF, 0xFF000000);
  ui.sdlTexture = SDL_CreateTexture(ui.sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    XRES * ui.scale, YRES * ui.scale);

  if (emu_get_ctx()->debug_mode) {
    debug_window_init(&ui);
  }
}

void ui_update(void) {
  SDL_Rect rc;
  rc.x = rc.y = 0;
  rc.w = rc.h = 2048;

  u32 *video_buffer = ppu_get_ctx()->video_buffer;

  for (unsigned int line_num = 0; line_num < YRES; line_num++) {
    for (unsigned int x = 0; x < XRES; x++) {
      rc.x = x * ui.scale;
      rc.y = line_num * ui.scale;
      rc.w = ui.scale;
      rc.h = ui.scale;

      SDL_FillRect(ui.screen, &rc, video_buffer[x + (line_num * XRES)]);
    }
  }

  SDL_UpdateTexture(ui.sdlTexture, NULL, ui.screen->pixels, ui.screen->pitch);
  SDL_RenderClear(ui.sdlRenderer);
  SDL_RenderCopy(ui.sdlRenderer, ui.sdlTexture, NULL, NULL);
  SDL_RenderPresent(ui.sdlRenderer);

  if (emu_get_ctx()->debug_mode) {
    dbg_window_update(&ui);
  }
}
