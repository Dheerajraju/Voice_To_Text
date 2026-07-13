/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Stage 4.3
 * Home Screen Header
 * Compatible with LVGL v9
 * ============================================================
 */

#ifndef UI_HOME_H
#define UI_HOME_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include "ui.h"

/*-------------------------------------------------------------
 * Public Functions
 *------------------------------------------------------------*/

/**
 * @brief Create the Home Screen
 *
 * Creates all UI objects on the main screen.
 */
void ui_home_create(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_HOME_H */
