/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * UI Header
 * Compatible with LVGL v9
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

/* Status */
extern lv_obj_t *ui_status;

/* Speech */
extern lv_obj_t *ui_speech_title;
extern lv_obj_t *ui_speech_text;

/*-------------------------------------------------------------
 * Public Functions
 *------------------------------------------------------------*/

/**
 * @brief Initialize complete UI
 */
void ui_init(void);

/**
 * @brief Create Home Screen
 */
void ui_home_create(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_H */
