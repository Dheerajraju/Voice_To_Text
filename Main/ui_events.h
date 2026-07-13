/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Stage 4.3
 * UI Events Header
 * Compatible with LVGL v9
 * ============================================================
 */

#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include "lvgl.h"

/*-------------------------------------------------------------
 * Event Callbacks
 *------------------------------------------------------------*/

/**
 * @brief Microphone button event callback
 *
 * Called whenever the "Press to Talk" button is clicked.
 *
 * @param e LVGL event pointer
 */
void mic_btn_event_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* UI_EVENTS_H */
