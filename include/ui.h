#ifndef UI_H
#define UI_H

#include <common.h>

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

void ui_init(int scale);
void ui_handle_events(void);
void ui_update(int scale);

#endif
