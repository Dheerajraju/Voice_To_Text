/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4.1
 * UI Events Header
 * ============================================================
 */

#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Event Callbacks
 *------------------------------------------------------------*/

void mic_btn_event_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* UI_EVENTS_H */
