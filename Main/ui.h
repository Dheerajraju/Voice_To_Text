/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * UI Header
 * Compatible with LVGL v9.5
 * ============================================================
 */

#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include "lvgl.h"

/*-------------------------------------------------------------
 * Global UI Objects
 *------------------------------------------------------------*/

/* Main Screen */
extern lv_obj_t *ui_screen;

/* Title */
extern lv_obj_t *ui_title;

/* Microphone Button */
extern lv_obj_t *ui_mic_btn;
extern lv_obj_t *ui_mic_label;

/* Status Label */
extern lv_obj_t *ui_status;

/* Speech Labels */
extern lv_obj_t *ui_speech_title;
extern lv_obj_t *ui_speech_text;

/*-------------------------------------------------------------
 * Public Functions
 *------------------------------------------------------------*/

/**
 * @brief Initialize the complete UI
 */
void ui_init(void);

/*-------------------------------------------------------------
 * UI Update Functions
 *------------------------------------------------------------*/

void ui_set_status(const char *text);

void ui_set_speech(const char *text);

/*-------------------------------------------------------------
 * Button Control Functions
 *------------------------------------------------------------*/

void ui_enable_button(void);

void ui_disable_button(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_H */
