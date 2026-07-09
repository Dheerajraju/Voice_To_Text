#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void ui_create(void);
void ui_set_status(const char *text);
void ui_set_indicator_color(lv_color_t color);

#ifdef __cplusplus
}
#endif
