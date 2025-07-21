#include <core/bus.h>
#include <ui.h>
#include <ppu/lcd.h>

void debug_window_init(ui_ctx *ui, int scale) {
  SDL_CreateWindowAndRenderer(16 * 8 * scale, 32 * 8 * scale, 0,
                              &ui->sdlDebugWindow, &ui->sdlDebugRenderer);

  ui->debugScreen = SDL_CreateRGBSurface(
      0, (16 * 8 * scale) + (16 * scale), (32 * 8 * scale) + (64 * scale), 32,
      0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

  ui->sdlDebugTexture = SDL_CreateTexture(
      ui->sdlDebugRenderer, SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING, (16 * 8 * scale) + (16 * scale),
      (32 * 8 * scale) + (64 * scale));

  int x, y;
  SDL_GetWindowPosition(ui->sdlWindow, &x, &y);
  SDL_SetWindowPosition(ui->sdlDebugWindow, x + SCREEN_WIDTH + 10, y);
}

void display_tile(SDL_Surface *surface, u16 startLocation, u16 tileNum, int x,
                  int y, int scale) {
  SDL_Rect rc;

  for (int tileY = 0; tileY < 16; tileY += 2) {
    u8 b1 = bus_read(startLocation + (tileNum * 16) + tileY);
    u8 b2 = bus_read(startLocation + (tileNum * 16) + tileY + 1);

    for (int bit = 7; bit >= 0; bit--) {
      u8 hi = !!(b1 & (1 << bit)) << 1;
      u8 lo = !!(b2 & (1 << bit));

      u8 color = hi | lo;

      rc.x = x + ((7 - bit) * scale);
      rc.y = y + (tileY / 2 * scale);
      rc.w = scale;
      rc.h = scale;

      SDL_FillRect(surface, &rc, lcd_get_context()->available_palettes[0][color]);
    }
  }
}

void dbg_window_update(ui_ctx *ui, int scale) {
  int xDraw = 0;
  int yDraw = 0;
  int tileNum = 0;

  SDL_Rect rc;
  rc.x = 0;
  rc.y = 0;
  rc.w = ui->debugScreen->w;
  rc.h = ui->debugScreen->h;
  SDL_FillRect(ui->debugScreen, &rc, 0xFF111111);

  u16 addr = 0x8000;

  // 384 tiles, 24 x 16
  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < 16; x++) {
      display_tile(ui->debugScreen, addr, tileNum, xDraw + (x * scale),
                   yDraw + (y * scale), scale);
      xDraw += (8 * scale);
      tileNum++;
    }

    yDraw += (8 * scale);
    xDraw = 0;
  }

  SDL_UpdateTexture(ui->sdlDebugTexture, NULL, ui->debugScreen->pixels,
                    ui->debugScreen->pitch);
  SDL_RenderClear(ui->sdlDebugRenderer);
  SDL_RenderCopy(ui->sdlDebugRenderer, ui->sdlDebugTexture, NULL, NULL);
  SDL_RenderPresent(ui->sdlDebugRenderer);
}
