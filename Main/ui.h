/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4.1
 * UI Header
 * ============================================================
 */

#ifndef UI_H
#define UI_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Global UI Objects
 *------------------------------------------------------------*/

extern lv_obj_t *ui_screen;

extern lv_obj_t *ui_title;

extern lv_obj_t *ui_mic_btn;
extern lv_obj_t *ui_mic_label;

extern lv_obj_t *ui_status;

extern lv_obj_t *ui_speech;

extern lv_obj_t *ui_response;

/*-------------------------------------------------------------
 * UI Initialization
 *------------------------------------------------------------*/

void ui_init(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_H */
